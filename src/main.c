#include "switch/switch.h"
#include "station/station.h"
#include "appareil/appareil.h"
#include "interface/interface.h"
#include "parser/parser.h"
#include "trame/trame.h"
#include "lan/lan.h"
#include "stp/stp.h"
#include <stdlib.h>
#include <stdio.h>

static void afficher_table(Switch* sw, const char* nom)
{
    char table[1024];
    switch_show_commutation_table(sw, table, sizeof(table));
    if (table[0] == '\0')
        printf("Table de commutation %s : (vide)\n", nom);
    else
        printf("Table de commutation %s :\n%s", nom, table);
}

static void afficher_buffer(Interface* iface, const char* nom)
{
    size_t nb = 0;
    interface_get_buffer_size(iface, &nb);
    if (nb == 0) {
        printf("  %s : aucune trame recue\n", nom);
        return;
    }
    printf("  %s : %zu trame(s) recue(s)\n", nom, nb);
    for (size_t i = 0; i < nb; i++) {
        Trame* t = NULL;
        interface_get_trame(iface, i, &t);
        printf("    [Trame %zu]\n", i + 1);
        trame_print(t);
    }
    interface_vider_buffer(iface);
}

/* ────────────────────────────────────────────────────────────
 * Cas 1 : démonstration de la commutation Ethernet
 *
 * config5.txt :  1 switch (sw0) + 3 stations (st1, st2, st3)
 *
 *   st1 ---[port 0]---+
 *   st2 ---[port 1]--- sw0
 *   st3 ---[port 2]---+
 *
 * Envoi 1 : st1 -> st2  destination inconnue => flood (st2 ET st3 recoivent)
 * Envoi 2 : st2 -> st1  destination connue  => unicast (st3 ne recoit rien)
 * ──────────────────────────────────────────────────────────── */
static void demo_commutation(void)
{
    Reseau* lan = NULL;
    file_parse("config_files/config5.txt", &lan);

    appareil* ap_sw  = NULL;
    appareil* ap_st1 = NULL;
    appareil* ap_st2 = NULL;
    appareil* ap_st3 = NULL;
    lan_get_machine(lan, 0, &ap_sw);
    lan_get_machine(lan, 1, &ap_st1);
    lan_get_machine(lan, 2, &ap_st2);
    lan_get_machine(lan, 3, &ap_st3);

    Switch*  sw  = NULL;
    station* st1 = NULL;
    station* st2 = NULL;
    station* st3 = NULL;
    appareil_get_switch(ap_sw,   &sw);
    appareil_get_station(ap_st1, &st1);
    appareil_get_station(ap_st2, &st2);
    appareil_get_station(ap_st3, &st3);

    Interface* iface_st1 = NULL;
    Interface* iface_st2 = NULL;
    Interface* iface_st3 = NULL;
    station_get_connexion(st1, &iface_st1);
    station_get_connexion(st2, &iface_st2);
    station_get_connexion(st3, &iface_st3);

    mac* mac_st1 = NULL;
    mac* mac_st2 = NULL;
    station_get_mac(st1, &mac_st1);
    station_get_mac(st2, &mac_st2);

    printf("========================================\n");
    printf("  Demo 1 : commutation Ethernet\n");
    printf("  Reseau : config5.txt\n");
    printf("  Topologie : st1 -- sw0 -- st2\n");
    printf("                      |-- st3\n");
    printf("========================================\n\n");

    afficher_table(sw, "sw0");
    printf("\n");

    printf("--- Envoi 1 : st1 -> st2 ---\n");
    printf("    sw0 ne connait pas st2 => flood sur tous les ports sauf port 0\n\n");

    Trame* t1 = trame_init(mac_st1, mac_st2, "Bonjour st2 !");
    interface_send_trame(t1, iface_st1);
    switch_traiter_trame(sw);

    printf("Recepteurs apres le flood :\n");
    afficher_buffer(iface_st2, "st2");
    afficher_buffer(iface_st3, "st3 (recu par flood, pas la destination)");
    printf("\n");
    afficher_table(sw, "sw0");
    printf("\n");

    printf("--- Envoi 2 : st2 -> st1 ---\n");
    printf("    sw0 connait st1 (port 1) => unicast, st3 ne recoit rien\n\n");

    Trame* t2 = trame_init(mac_st2, mac_st1, "Bonjour st1 !");
    interface_send_trame(t2, iface_st2);
    switch_traiter_trame(sw);

    printf("Recepteurs apres l'unicast :\n");
    afficher_buffer(iface_st1, "st1");
    afficher_buffer(iface_st3, "st3 (ne doit rien recevoir)");
    printf("\n");
    afficher_table(sw, "sw0");

    trame_deinit(&t1);
    trame_deinit(&t2);
    lan_deinit(&lan);
}

/* ────────────────────────────────────────────────────────────
 * Cas 2 : démonstration du STP
 *
 * config2.txt : 3 switches en triangle + 1 station par switch
 *
 *   st3 --- sw0 --- sw1 --- st4
 *             \    /
 *              sw2
 *               |
 *              st5
 *
 * Le cycle sw0-sw1-sw2 crée une boucle. STP élit sw0 (plus petite
 * MAC) comme pont racine et bloque le port sw2->sw1 pour couper la
 * boucle. Ensuite on envoie une trame st3 -> st5.
 * ──────────────────────────────────────────────────────────── */
static void demo_stp(void)
{
    Reseau* lan = NULL;
    file_parse("config_files/config2.txt", &lan);

    appareil* ap_sw0 = NULL;
    appareil* ap_sw1 = NULL;
    appareil* ap_sw2 = NULL;
    appareil* ap_st3 = NULL;
    appareil* ap_st4 = NULL;
    appareil* ap_st5 = NULL;
    lan_get_machine(lan, 0, &ap_sw0);
    lan_get_machine(lan, 1, &ap_sw1);
    lan_get_machine(lan, 2, &ap_sw2);
    lan_get_machine(lan, 3, &ap_st3);
    lan_get_machine(lan, 4, &ap_st4);
    lan_get_machine(lan, 5, &ap_st5);

    Switch*  sw0 = NULL;
    Switch*  sw1 = NULL;
    Switch*  sw2 = NULL;
    station* st3 = NULL;
    station* st4 = NULL;
    station* st5 = NULL;
    appareil_get_switch(ap_sw0, &sw0);
    appareil_get_switch(ap_sw1, &sw1);
    appareil_get_switch(ap_sw2, &sw2);
    appareil_get_station(ap_st3, &st3);
    appareil_get_station(ap_st4, &st4);
    appareil_get_station(ap_st5, &st5);

    Interface* iface_st3 = NULL;
    Interface* iface_st4 = NULL;
    Interface* iface_st5 = NULL;
    station_get_connexion(st3, &iface_st3);
    station_get_connexion(st4, &iface_st4);
    station_get_connexion(st5, &iface_st5);

    mac* mac_st3 = NULL;
    mac* mac_st5 = NULL;
    station_get_mac(st3, &mac_st3);
    station_get_mac(st5, &mac_st5);

    printf("========================================\n");
    printf("  Demo 2 : STP (Spanning Tree Protocol)\n");
    printf("  Reseau : config2.txt\n");
    printf("  Topologie : triangle sw0 -- sw1 -- sw2 -- sw0\n");
    printf("========================================\n\n");

    printf("Avant STP : le reseau contient une boucle sw0-sw1-sw2.\n\n");

    stp_run(lan);
    stp_print(lan);

    printf("========================================\n");
    printf("  Envoi st3 -> st5 apres STP\n");
    printf("========================================\n\n");
    printf("Chemin attendu : st3 -> sw0 -> sw2 -> st5\n");
    printf("(le port bloque sur sw2 coupe le cycle sw1-sw2)\n\n");

    Trame* t = trame_init(mac_st3, mac_st5, "Bonjour st5 depuis st3 !");
    interface_send_trame(t, iface_st3);

    switch_traiter_trame(sw0);
    switch_traiter_trame(sw1);
    switch_traiter_trame(sw2);

    printf("Recepteurs :\n");
    afficher_buffer(iface_st4, "st4 (flood depuis sw1)");
    afficher_buffer(iface_st5, "st5 (destination)");
    printf("\n");

    printf("Tables de commutation apres l'envoi :\n");
    afficher_table(sw0, "sw0");
    afficher_table(sw1, "sw1");
    afficher_table(sw2, "sw2");

    trame_deinit(&t);
    lan_deinit(&lan);
}

/* ────────────────────────────────────────────────────────────
 * Cas 3 : envoi libre — l'utilisateur choisit le fichier de
 * config, les stations source/destination et le message.
 * Le STP est exécuté avant l'envoi pour garantir l'absence
 * de boucle, puis les tables et l'état STP sont affichés.
 * ──────────────────────────────────────────────────────────── */
static void demo_interactif(void)
{
    /* Saisie du fichier de configuration */
    char chemin[256];
    printf("Chemin du fichier de config : ");
    scanf("%255s", chemin);
    printf("\n");

    Reseau* lan = NULL;
    if (file_parse(chemin, &lan) != OK) {
        printf("Erreur : impossible de charger '%s'\n", chemin);
        return;
    }

    size_t nb = 0;
    lan_nombre_machine(lan, &nb);

    /* STP executé silencieusement avant tout envoi */
    stp_run(lan);

    /* Listage des stations disponibles */
    printf("Stations disponibles :\n");
    size_t nb_stations = 0;
    for (size_t i = 0; i < nb; i++) {
        appareil* ap = NULL;
        lan_get_machine(lan, i, &ap);
        TYPE_APPAREIL type;
        appareil_get_type(ap, &type);
        if (type != STATION) continue;
        station* st = NULL;
        appareil_get_station(ap, &st);
        mac* m = NULL;
        station_get_mac(st, &m);
        char mac_str[18] = "";
        mac_get_string(m, ':', mac_str, sizeof(mac_str));
        printf("  [%zu]  %s\n", i, mac_str);
        nb_stations++;
    }

    if (nb_stations < 2) {
        printf("Erreur : le reseau doit avoir au moins 2 stations.\n");
        lan_deinit(&lan);
        return;
    }

    int continuer = 1;
    while (continuer) {
        /* Saisie source / destination */
        size_t idx_src = 0, idx_dst = 0;
        printf("\nIndex station source      : ");
        scanf("%zu", &idx_src);
        printf("Index station destination : ");
        scanf("%zu", &idx_dst);

        if (idx_src >= nb || idx_dst >= nb || idx_src == idx_dst) {
            printf("Erreur : indices invalides.\n");
            goto fin;
        }

        appareil* ap_src = NULL;
        appareil* ap_dst = NULL;
        lan_get_machine(lan, idx_src, &ap_src);
        lan_get_machine(lan, idx_dst, &ap_dst);

        TYPE_APPAREIL type_src, type_dst;
        appareil_get_type(ap_src, &type_src);
        appareil_get_type(ap_dst, &type_dst);

        if (type_src != STATION || type_dst != STATION) {
            printf("Erreur : les deux indices doivent correspondre a des stations.\n");
            goto fin;
        }

        station* st_src = NULL;
        station* st_dst = NULL;
        appareil_get_station(ap_src, &st_src);
        appareil_get_station(ap_dst, &st_dst);

        mac* mac_src = NULL;
        mac* mac_dst = NULL;
        station_get_mac(st_src, &mac_src);
        station_get_mac(st_dst, &mac_dst);

        Interface* iface_src = NULL;
        station_get_connexion(st_src, &iface_src);

        /* Saisie du message (espace initial pour consommer le \n laissé par scanf) */
        char message[256];
        printf("Message : ");
        scanf(" %255[^\n]", message);
        printf("\n");

        /* Envoi de la trame */
        Trame* t = trame_init(mac_src, mac_dst, message);
        interface_send_trame(t, iface_src);

        /* Propagation à travers les switches.
           STP garantit un arbre sans cycle, donc nb itérations suffisent
           pour couvrir le chemin le plus long possible. */
        for (size_t iter = 0; iter < nb; iter++) {
            for (size_t i = 0; i < nb; i++) {
                appareil* ap = NULL;
                lan_get_machine(lan, i, &ap);
                TYPE_APPAREIL type;
                appareil_get_type(ap, &type);
                if (type != SWITCH) continue;
                Switch* sw = NULL;
                appareil_get_switch(ap, &sw);
                switch_traiter_trame(sw);
            }
        }

        /* Affichage des trames reçues par toutes les stations */
        printf("--- Trames recues ---\n");
        for (size_t i = 0; i < nb; i++) {
            appareil* ap = NULL;
            lan_get_machine(lan, i, &ap);
            TYPE_APPAREIL type;
            appareil_get_type(ap, &type);
            if (type != STATION) continue;
            station* st = NULL;
            appareil_get_station(ap, &st);
            mac* m = NULL;
            station_get_mac(st, &m);
            char mac_str[18] = "";
            mac_get_string(m, ':', mac_str, sizeof(mac_str));
            Interface* iface = NULL;
            station_get_connexion(st, &iface);
            char label[48];
            snprintf(label, sizeof(label), "[%zu] %s", i, mac_str);
            afficher_buffer(iface, label);
        }

        /* Tables de commutation */
        printf("\n--- Tables de commutation ---\n");
        for (size_t i = 0; i < nb; i++) {
            appareil* ap = NULL;
            lan_get_machine(lan, i, &ap);
            TYPE_APPAREIL type;
            appareil_get_type(ap, &type);
            if (type != SWITCH) continue;
            Switch* sw = NULL;
            appareil_get_switch(ap, &sw);
            char mac_str[18] = "";
            switch_show_mac_hexa(sw, mac_str, sizeof(mac_str));
            afficher_table(sw, mac_str);
        }

        /* Etat STP */
        stp_print(lan);

        trame_deinit(&t);

        /* Continuer ? */
        char rep[4];
        printf("Envoyer une autre trame ? (o/n) : ");
        scanf(" %3s", rep);
        printf("\n");
        if (rep[0] != 'o' && rep[0] != 'O') continuer = 0;

        if (continuer) {
            printf("Stations disponibles :\n");
            for (size_t i = 0; i < nb; i++) {
                appareil* ap = NULL;
                lan_get_machine(lan, i, &ap);
                TYPE_APPAREIL type;
                appareil_get_type(ap, &type);
                if (type != STATION) continue;
                station* st = NULL;
                appareil_get_station(ap, &st);
                mac* m = NULL;
                station_get_mac(st, &m);
                char mac_str[18] = "";
                mac_get_string(m, ':', mac_str, sizeof(mac_str));
                printf("  [%zu]  %s\n", i, mac_str);
            }
        }
    }

fin:
    lan_deinit(&lan);
}

int main()
{
    int choix = 0;

    printf("========================================\n");
    printf("  SAE2.3 - Simulation reseau Ethernet\n");
    printf("========================================\n\n");
    printf("  1. Commutation Ethernet (flood / unicast)\n");
    printf("  2. STP - Spanning Tree Protocol\n");
    printf("  3. Envoi libre (config + stations au choix)\n\n");
    printf("Votre choix : ");

    if (scanf("%d", &choix) != 1) choix = 0;

    printf("\n");

    switch (choix) {
        case 1:
            demo_commutation();
            break;
        case 2:
            demo_stp();
            break;
        case 3:
            demo_interactif();
            break;
        default:
            printf("Choix invalide. Entrez 1, 2 ou 3.\n");
            return 1;
    }

    return 0;
}
