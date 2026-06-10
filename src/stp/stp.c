#include "stp.h"
#include "../appareil/appareil.h"
#include "../switch/switch.h"
#include "../interface/interface.h"
#include "../mac-ip/mac.h"
#include <stdio.h>
#include <string.h>

/* Trouve quel switch du réseau possède l'interface 'iface' dans ses connexions.
   Retourne OK et renseigne *sw_out et *port_out si trouvé. */
static ERREUR_CODE find_switch_owner(Reseau* lan, Interface* iface,
                                      Switch** sw_out, size_t* port_out)
{
    size_t nb = 0;
    lan_nombre_machine(lan, &nb);
    for (size_t i = 0; i < nb; i++) {
        appareil* ap = NULL;
        lan_get_machine(lan, i, &ap);
        TYPE_APPAREIL type;
        appareil_get_type(ap, &type);
        if (type != SWITCH) continue;
        Switch* sw = NULL;
        appareil_get_switch(ap, &sw);
        size_t nb_ports = 0;
        switch_get_nb_connexions(sw, &nb_ports);
        for (size_t p = 0; p < nb_ports; p++) {
            Interface* inter = NULL;
            switch_get_connexion(sw, p, &inter);
            if (inter == iface) {  /* comparaison de pointeurs : même adresse = même objet */
                *sw_out = sw;
                *port_out = p;
                return OK;
            }
        }
    }
    return INVALID_ARGUMENT;
}

void stp_run(Reseau* lan)
{
    if (lan == NULL) return;

    size_t nb = 0;
    lan_nombre_machine(lan, &nb);

    /* Phase 1 : chaque switch s'initialise comme racine */
    for (size_t i = 0; i < nb; i++) {
        appareil* ap = NULL;
        lan_get_machine(lan, i, &ap);
        TYPE_APPAREIL type;
        appareil_get_type(ap, &type);
        if (type != SWITCH) continue;
        Switch* sw = NULL;
        appareil_get_switch(ap, &sw);
        switch_stp_init(sw);
    }

    /* Phase 2 : propagation des BPDU jusqu'à convergence */
    /* Démarre à 1 pour forcer au moins un passage; repart si un voisin a été mis à jour */
    int global_changed = 1;
    while (global_changed) {
        global_changed = 0;
        for (size_t i = 0; i < nb; i++) {
            appareil* ap = NULL;
            lan_get_machine(lan, i, &ap);
            TYPE_APPAREIL type;
            appareil_get_type(ap, &type);
            if (type != SWITCH) continue;
            Switch* sw = NULL;
            appareil_get_switch(ap, &sw);

            uint32_t root_prio = 0, root_cost = 0;
            char root_mac[18] = "";
            int root_port = -1;
            switch_stp_get_root_info(sw, &root_prio, root_mac, sizeof(root_mac),
                                      &root_cost, &root_port);

            size_t nb_ports = 0;
            switch_get_nb_connexions(sw, &nb_ports);
            for (size_t p = 0; p < nb_ports; p++) {
                Interface* inter = NULL;
                switch_get_connexion(sw, p, &inter);
                /* autre = l'Interface* côté voisin (autreAppareil du lien) */
                Interface* autre = NULL;
                interface_get_autre_appareil(inter, &autre);
                if (autre == NULL) continue;

                /* si autre n'appartient à aucun switch, c'est une station : on skip */
                Switch* voisin = NULL;
                size_t voisin_port = 0;
                if (find_switch_owner(lan, autre, &voisin, &voisin_port) != OK) continue;

                size_t poids = 0;
                interface_get_poids(inter, &poids);

                int chg = 0;
                /* on propose au voisin notre racine + coût du lien qui nous sépare */
                switch_stp_try_update(voisin, root_prio, root_mac,
                                      root_cost + (uint32_t)poids,
                                      voisin_port, &chg);
                if (chg) global_changed = 1;
            }
        }
    }

    /* Phase 3 : assignation des états de ports */
    for (size_t i = 0; i < nb; i++) {
        appareil* ap = NULL;
        lan_get_machine(lan, i, &ap);
        TYPE_APPAREIL type;
        appareil_get_type(ap, &type);
        if (type != SWITCH) continue;
        Switch* sw = NULL;
        appareil_get_switch(ap, &sw);

        uint32_t root_prio = 0, root_cost = 0;
        char root_mac[18] = "";
        int root_port = -1;
        switch_stp_get_root_info(sw, &root_prio, root_mac, sizeof(root_mac),
                                  &root_cost, &root_port);

        size_t nb_ports = 0;
        switch_get_nb_connexions(sw, &nb_ports);
        for (size_t p = 0; p < nb_ports; p++) {
            Interface* inter = NULL;
            switch_get_connexion(sw, p, &inter);
            Interface* autre = NULL;
            interface_get_autre_appareil(inter, &autre);
            if (autre == NULL) continue;

            Switch* voisin = NULL;
            size_t voisin_port = 0;
            int lien_switch = (find_switch_owner(lan, autre, &voisin, &voisin_port) == OK);

            if (!lien_switch) {
                /* Lien vers une station → toujours désigné */
                switch_stp_set_port_etat(sw, p, PORT_DESIGNE);
                continue;
            }

            if (root_port == -1) {
                /* root_port == -1 signifie "je suis la racine", pas de port vers un parent */
                switch_stp_set_port_etat(sw, p, PORT_DESIGNE);
                continue;
            }

            if ((int)p == root_port) {
                switch_stp_set_port_etat(sw, p, PORT_RACINE);
                continue;
            }

            /* Lien switch-switch non-racine : comparer les coûts vers la racine */
            uint32_t v_root_prio = 0, v_root_cost = 0;
            char v_root_mac[18] = "";
            int v_root_port = -1;
            switch_stp_get_root_info(voisin, &v_root_prio, v_root_mac, sizeof(v_root_mac),
                                      &v_root_cost, &v_root_port);

            if (root_cost < v_root_cost) {
                switch_stp_set_port_etat(sw, p, PORT_DESIGNE);
            } else if (root_cost > v_root_cost) {
                switch_stp_set_port_etat(sw, p, PORT_BLOQUE);
            } else {
                /* Même coût : on départage par Bridge ID (priorité puis MAC en lexicographique, 
                   ça marche car MAC est en FULL MAJ ou FULL Min donc l'ordre numérique marche).
                   Le plus petit Bridge ID gagne DESIGNE, l'autre est BLOQUE. */
                uint32_t our_prio = 0;
                switch_get_priority(sw, &our_prio);
                uint32_t their_prio = 0;
                switch_get_priority(voisin, &their_prio);

                int decide;
                if (our_prio != their_prio) {
                    decide = (our_prio < their_prio) ? -1 : 1;
                } else {
                    mac* our_mac_ptr = NULL;
                    switch_get_mac(sw, &our_mac_ptr);
                    char our_mac_str[18] = "";
                    mac_get_string(our_mac_ptr, ':', our_mac_str, sizeof(our_mac_str));
                    mac* their_mac_ptr = NULL;
                    switch_get_mac(voisin, &their_mac_ptr);
                    char their_mac_str[18] = "";
                    mac_get_string(their_mac_ptr, ':', their_mac_str, sizeof(their_mac_str));
                    decide = strcmp(our_mac_str, their_mac_str);
                }

                if (decide < 0)
                    switch_stp_set_port_etat(sw, p, PORT_DESIGNE);
                else
                    switch_stp_set_port_etat(sw, p, PORT_BLOQUE);
            }
        }
    }
}

static const char* etat_str(PORT_ETAT e)
{
    switch (e) {
        case PORT_RACINE:   return "RACINE   ";
        case PORT_DESIGNE:  return "DESIGNE  ";
        case PORT_BLOQUE:   return "BLOQUE   ";
        default:            return "INCONNU  ";
    }
}

void stp_print(Reseau* lan)
{
    if (lan == NULL) return;

    size_t nb = 0;
    lan_nombre_machine(lan, &nb);

    printf("========================================\n");
    printf("  Etat STP apres convergence\n");
    printf("========================================\n");

    for (size_t i = 0; i < nb; i++) {
        appareil* ap = NULL;
        lan_get_machine(lan, i, &ap);
        TYPE_APPAREIL type;
        appareil_get_type(ap, &type);
        if (type != SWITCH) continue;

        Switch* sw = NULL;
        appareil_get_switch(ap, &sw);

        char sw_mac[18] = "";
        switch_show_mac_hexa(sw, sw_mac, sizeof(sw_mac));

        uint32_t root_prio = 0, root_cost = 0;
        char root_mac[18] = "";
        int root_port = -1;
        switch_stp_get_root_info(sw, &root_prio, root_mac, sizeof(root_mac),
                                  &root_cost, &root_port);

        printf("\nSwitch [%s]", sw_mac);
        if (root_port == -1)
            printf("  << PONT RACINE >>");
        printf("\n");
        printf("  Racine connue : prio=%u  mac=%s  cout=%u\n",
               root_prio, root_mac, root_cost);

        size_t nb_ports = 0;
        switch_get_nb_connexions(sw, &nb_ports);
        for (size_t p = 0; p < nb_ports; p++) {
            PORT_ETAT etat;
            switch_stp_get_port_etat(sw, p, &etat);

            Interface* inter = NULL;
            switch_get_connexion(sw, p, &inter);
            Interface* autre = NULL;
            interface_get_autre_appareil(inter, &autre);

            char voisin_info[32] = "?";
            if (autre != NULL) {
                Switch* voisin_sw = NULL;
                size_t voisin_port = 0;
                if (find_switch_owner(lan, autre, &voisin_sw, &voisin_port) == OK) {
                    char v_mac[18] = "";
                    switch_show_mac_hexa(voisin_sw, v_mac, sizeof(v_mac));
                    snprintf(voisin_info, sizeof(voisin_info), "SW %s", v_mac + 12); /* +12 : 3 derniers octets seulement */
                } else {
                    snprintf(voisin_info, sizeof(voisin_info), "station");
                }
            }
            printf("  Port %zu -> %-12s  [%s]\n", p + 1, voisin_info, etat_str(etat));
        }
    }
    printf("\n");
}
