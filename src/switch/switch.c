#include "switch.h"
#include "../mac-ip/mac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct EntreeCommutation {
    mac* adresse;
    size_t port;
} EntreeCommutation;

struct commutateur {
    EntreeCommutation* commutationTable;
    size_t nbEntrees;
    size_t tailleCommutationTable;
    Interface** connexions;
    size_t nbConnexions;
    mac* macAddress;
    uint32_t prio;
    size_t nbPorts;
    /* STP */
    PORT_ETAT* etatsPorts;
    uint32_t stp_root_prio;
    char stp_root_mac[18];
    uint32_t stp_root_cost;
    int stp_root_port;    /* -1 = ce switch est la racine */
};

ERREUR_CODE switch_check_pointeur_null(void* ptr)
{
    return (ptr == NULL) ? POINTEUR_NULL : OK;
}

Switch* switch_init()
{
    Switch* s = malloc(sizeof(Switch));
    if (switch_check_pointeur_null(s) != OK) return NULL;

    s->macAddress = mac_init();
    if (switch_check_pointeur_null(s->macAddress) != OK) {
        free(s);
        return NULL;
    }

    s->nbPorts = 8;
    s->connexions = malloc(sizeof(Interface*) * s->nbPorts);
    if (switch_check_pointeur_null(s->connexions) != OK) {
        mac_deinit(&s->macAddress);
        free(s);
        return NULL;
    }
    s->nbConnexions = 0;
    s->prio = 32768;

    s->tailleCommutationTable = 8;
    s->nbEntrees = 0;
    s->commutationTable = malloc(sizeof(EntreeCommutation) * s->tailleCommutationTable);
    if (switch_check_pointeur_null(s->commutationTable) != OK) {
        free(s->connexions);
        mac_deinit(&s->macAddress);
        free(s);
        return NULL;
    }

    s->etatsPorts = NULL;
    s->stp_root_prio = 0;
    s->stp_root_mac[0] = '\0';
    s->stp_root_cost = 0;
    s->stp_root_port = -1;

    return s;
}

Switch* switch_init_with_parameter(mac* mac, size_t nbPorts, uint32_t priority)
{
    Switch* s = malloc(sizeof(Switch));
    if (switch_check_pointeur_null(s) != OK ||
        switch_check_pointeur_null(mac) != OK) return NULL;

    s->macAddress = mac;
    s->connexions = malloc(sizeof(Interface*) * nbPorts);
    if (switch_check_pointeur_null(s->connexions) != OK) {
        free(s);
        return NULL;
    }
    s->nbConnexions = 0;
    s->nbPorts = nbPorts;
    s->prio = priority;

    s->tailleCommutationTable = nbPorts > 0 ? nbPorts : 8;
    s->nbEntrees = 0;
    s->commutationTable = malloc(sizeof(EntreeCommutation) * s->tailleCommutationTable);
    if (switch_check_pointeur_null(s->commutationTable) != OK) {
        free(s->connexions);
        free(s);
        return NULL;
    }

    s->etatsPorts = NULL;
    s->stp_root_prio = 0;
    s->stp_root_mac[0] = '\0';
    s->stp_root_cost = 0;
    s->stp_root_port = -1;

    return s;
}

ERREUR_CODE switch_deinit(Switch** s)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(*s)) != OK) return err;

    for (size_t i = 0; i < (*s)->nbEntrees; i++) {
        mac_deinit(&(*s)->commutationTable[i].adresse);
    }
    free((*s)->commutationTable);
    (*s)->commutationTable = NULL;

    for (size_t j = 0; j < (*s)->nbConnexions; j++) {
        interface_deinit(&(*s)->connexions[j]);
    }
    free((*s)->connexions);

    free((*s)->etatsPorts);

    mac_deinit(&(*s)->macAddress);

    free(*s);
    *s = NULL;

    return OK;
}

ERREUR_CODE switch_show_mac_hexa(Switch* s, char* str, size_t taille_str)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK ||
        (err = mac_get_string(s->macAddress, ':', str, taille_str)) != OK) {
        return err;
    }
    return OK;
}

ERREUR_CODE switch_show_commutation_table(Switch* s, char* str, size_t taille_str)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK) return err;

    str[0] = '\0';
    char buffer[255];
    char adresse[18];
    size_t nbCharEcris = 0;

    for (size_t i = 0; i < s->nbEntrees; i++) {
        mac_get_string(s->commutationTable[i].adresse, ':', adresse, sizeof(adresse));
        int n = snprintf(buffer, sizeof(buffer), "Port %zu : %s\n",
                         s->commutationTable[i].port + 1, adresse);
        if (n < 0) return INVALID_ARGUMENT;
        nbCharEcris += (size_t)n;
        if (nbCharEcris >= taille_str) return TAILLE_STRING;
        strcat(str, buffer);
    }
    return OK;
}

ERREUR_CODE switch_set_priority(Switch* s, uint32_t priority)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK) return err;

    if (priority % 4096 == 0) {
        s->prio = priority;
        return OK;
    }
    return INVALID_ARGUMENT;
}

ERREUR_CODE switch_get_priority(Switch* s, uint32_t* prio)
{
    if (switch_check_pointeur_null(s) != OK || prio == NULL) return POINTEUR_NULL;
    *prio = s->prio;
    return OK;
}

ERREUR_CODE switch_get_nb_connexions(Switch* s, size_t* nb)
{
    if (switch_check_pointeur_null(s) != OK || nb == NULL) return POINTEUR_NULL;
    *nb = s->nbConnexions;
    return OK;
}

ERREUR_CODE switch_get_connexion(Switch* s, size_t port, Interface** inter)
{
    if (switch_check_pointeur_null(s) != OK || inter == NULL) return POINTEUR_NULL;
    if (port >= s->nbConnexions) return INVALID_ARGUMENT;
    *inter = s->connexions[port];
    return OK;
}

ERREUR_CODE switch_learn_mac(Switch* s, mac* adresse, size_t port)
{
    if (switch_check_pointeur_null(s) != OK ||
        switch_check_pointeur_null(adresse) != OK) return POINTEUR_NULL;

    char str_new[18] = "", str_existing[18] = "";
    mac_get_string(adresse, ':', str_new, sizeof(str_new));

    for (size_t i = 0; i < s->nbEntrees; i++) {
        mac_get_string(s->commutationTable[i].adresse, ':', str_existing, sizeof(str_existing));
        if (strcmp(str_new, str_existing) == 0) return OK;
    }

    if (s->nbEntrees == s->tailleCommutationTable) {
        s->tailleCommutationTable *= 2;
        s->commutationTable = realloc(s->commutationTable,
            sizeof(EntreeCommutation) * s->tailleCommutationTable);
        if (s->commutationTable == NULL) return ALLOCATION;
    }

    mac* copy = mac_init();
    if (copy == NULL) return ALLOCATION;
    mac_set_string(copy, str_new);

    s->commutationTable[s->nbEntrees].adresse = copy;
    s->commutationTable[s->nbEntrees].port = port;
    s->nbEntrees++;

    return OK;
}

ERREUR_CODE switch_find_port(Switch* s, mac* adresse, size_t* port)
{
    if (switch_check_pointeur_null(s) != OK ||
        switch_check_pointeur_null(adresse) != OK) return POINTEUR_NULL;

    char str_target[18] = "", str_entry[18] = "";
    mac_get_string(adresse, ':', str_target, sizeof(str_target));

    for (size_t i = 0; i < s->nbEntrees; i++) {
        mac_get_string(s->commutationTable[i].adresse, ':', str_entry, sizeof(str_entry));
        if (strcmp(str_target, str_entry) == 0) {
            *port = s->commutationTable[i].port;
            return OK;
        }
    }

    return INVALID_ARGUMENT;
}

ERREUR_CODE switch_set_commutation_table(Switch* s, size_t port, mac* macAddress)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK ||
        (err = switch_check_pointeur_null(macAddress)) != OK) return err;
    if (port >= s->nbPorts) return INVALID_ARGUMENT;

    if (s->nbEntrees == s->tailleCommutationTable) {
        s->tailleCommutationTable *= 2;
        s->commutationTable = realloc(s->commutationTable,
            sizeof(EntreeCommutation) * s->tailleCommutationTable);
        if (s->commutationTable == NULL) return ALLOCATION;
    }

    s->commutationTable[s->nbEntrees].adresse = macAddress;
    s->commutationTable[s->nbEntrees].port = port;
    s->nbEntrees++;

    return OK;
}

ERREUR_CODE switch_get_mac(Switch* s, mac** m)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK ||
        (err = switch_check_pointeur_null(s->macAddress)) != OK) return err;
    *m = s->macAddress;
    return OK;
}

ERREUR_CODE switch_set_interface(Switch* s, Interface* inter)
{
    if (switch_check_pointeur_null(s) != OK || switch_check_pointeur_null(inter) != OK)
        return POINTEUR_NULL;
    if (s->nbConnexions >= s->nbPorts) return INVALID_ARGUMENT;
    s->connexions[s->nbConnexions] = inter;
    s->nbConnexions++;
    return OK;
}

ERREUR_CODE switch_broadcast_trame(Switch* s, Trame* trame)
{
    if (switch_check_pointeur_null(s) == POINTEUR_NULL ||
        switch_check_pointeur_null(trame) == POINTEUR_NULL) return POINTEUR_NULL;

    for (size_t i = 0; i < s->nbConnexions; i++) {
        if (s->etatsPorts != NULL && s->etatsPorts[i] == PORT_BLOQUE) continue;
        interface_send_trame(trame, s->connexions[i]);
    }
    return OK;
}

ERREUR_CODE switch_commuter_trame(Switch* s, Trame* trame, size_t port_entree)
{
    if (switch_check_pointeur_null(s) != OK ||
        switch_check_pointeur_null(trame) != OK) return POINTEUR_NULL;

    mac* source = NULL;
    trame_get_source(trame, &source);
    if (source != NULL) {
        switch_learn_mac(s, source, port_entree);
    }

    mac* destination = NULL;
    trame_get_destination(trame, &destination);

    size_t port_dest;
    if (destination != NULL && switch_find_port(s, destination, &port_dest) == OK) {
        if (s->etatsPorts == NULL || s->etatsPorts[port_dest] != PORT_BLOQUE)
            interface_send_trame(trame, s->connexions[port_dest]);
    } else {
        for (size_t i = 0; i < s->nbConnexions; i++) {
            if (i == port_entree) continue;
            if (s->etatsPorts != NULL && s->etatsPorts[i] == PORT_BLOQUE) continue;
            interface_send_trame(trame, s->connexions[i]);
        }
    }

    return OK;
}

ERREUR_CODE switch_traiter_trame(Switch* s)
{
    if (switch_check_pointeur_null(s) == POINTEUR_NULL) return POINTEUR_NULL;

    for (size_t port = 0; port < s->nbConnexions; port++) {
        if (s->etatsPorts != NULL && s->etatsPorts[port] == PORT_BLOQUE) {
            interface_vider_buffer(s->connexions[port]);
            continue;
        }
        size_t nbTrames = 0;
        interface_get_buffer_size(s->connexions[port], &nbTrames);

        for (size_t j = 0; j < nbTrames; j++) {
            Trame* trame = NULL;
            interface_get_trame(s->connexions[port], j, &trame);
            switch_commuter_trame(s, trame, port);
        }

        interface_vider_buffer(s->connexions[port]);
    }
    return OK;
}

/* ── STP ─────────────────────────────────────────── */

ERREUR_CODE switch_stp_init(Switch* s)
{
    if (switch_check_pointeur_null(s) != OK) return POINTEUR_NULL;

    if (s->etatsPorts == NULL) {
        s->etatsPorts = malloc(sizeof(PORT_ETAT) * s->nbPorts);
        if (s->etatsPorts == NULL) return ALLOCATION;
    }
    for (size_t i = 0; i < s->nbPorts; i++)
        s->etatsPorts[i] = PORT_INCONNU;

    /* Chaque switch suppose qu'il est la racine */
    mac* m;
    switch_get_mac(s, &m);
    mac_get_string(m, ':', s->stp_root_mac, sizeof(s->stp_root_mac));
    s->stp_root_prio = s->prio;
    s->stp_root_cost = 0;
    s->stp_root_port = -1;

    return OK;
}

ERREUR_CODE switch_stp_get_root_info(Switch* s, uint32_t* root_prio, char* root_mac,
                                      size_t mac_len, uint32_t* root_cost, int* root_port)
{
    if (switch_check_pointeur_null(s) != OK) return POINTEUR_NULL;
    *root_prio = s->stp_root_prio;
    strncpy(root_mac, s->stp_root_mac, mac_len - 1);
    root_mac[mac_len - 1] = '\0';
    *root_cost = s->stp_root_cost;
    *root_port = s->stp_root_port;
    return OK;
}

ERREUR_CODE switch_stp_try_update(Switch* s, uint32_t root_prio, const char* root_mac,
                                   uint32_t cost, size_t via_port, int* changed)
{
    if (switch_check_pointeur_null(s) != OK) return POINTEUR_NULL;

    /* Compare (prio, mac) : plus petit = meilleure racine */
    int cmp_prio = (root_prio < s->stp_root_prio) ? -1
                 : (root_prio > s->stp_root_prio) ? 1 : 0;
    int cmp_mac  = strcmp(root_mac, s->stp_root_mac);
    int better_root = (cmp_prio < 0) || (cmp_prio == 0 && cmp_mac < 0);
    int same_root   = (cmp_prio == 0 && cmp_mac == 0);

    if (better_root || (same_root && cost < s->stp_root_cost)) {
        s->stp_root_prio = root_prio;
        strncpy(s->stp_root_mac, root_mac, sizeof(s->stp_root_mac) - 1);
        s->stp_root_mac[sizeof(s->stp_root_mac) - 1] = '\0';
        s->stp_root_cost = cost;
        s->stp_root_port = (int)via_port;
        *changed = 1;
    }
    return OK;
}

ERREUR_CODE switch_stp_get_port_etat(Switch* s, size_t port, PORT_ETAT* etat)
{
    if (switch_check_pointeur_null(s) != OK || etat == NULL) return POINTEUR_NULL;
    if (s->etatsPorts == NULL || port >= s->nbPorts) return INVALID_ARGUMENT;
    *etat = s->etatsPorts[port];
    return OK;
}

ERREUR_CODE switch_stp_set_port_etat(Switch* s, size_t port, PORT_ETAT etat)
{
    if (switch_check_pointeur_null(s) != OK) return POINTEUR_NULL;
    if (s->etatsPorts == NULL || port >= s->nbPorts) return INVALID_ARGUMENT;
    s->etatsPorts[port] = etat;
    return OK;
}
