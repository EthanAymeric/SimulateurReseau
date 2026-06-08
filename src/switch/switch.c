#include "switch.h"
#include "../mac-ip/mac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct commutateur {
    mac** commutationTable; 
    mac* macAddress;
    uint32_t prio;
    size_t nbPorts;
};

ERREUR_CODE switch_check_pointeur_null(void* ptr)
{
    return (ptr == NULL) ? POINTEUR_NULL : OK;
}

Switch* switch_init() {
    Switch* s = malloc(sizeof(Switch));

    if (switch_check_pointeur_null(s) != OK) return NULL;

    s->macAddress = mac_init();
    if (switch_check_pointeur_null(s->macAddress) != OK){
        free(s);
        return NULL;
    }

    s->nbPorts = 8;
    s->prio = 32768;
    s->commutationTable = malloc(sizeof(mac*) * s->nbPorts);
    if (switch_check_pointeur_null(s->commutationTable) != OK){
        free(s->macAddress);
        free(s);
        return NULL;
    }

    for (size_t i = 0; i < s->nbPorts ; i++)
    {
        s->commutationTable[i] = mac_init();
        if (switch_check_pointeur_null(s->commutationTable[i]) != OK){
            free(s->macAddress);
            for (int j = i; j >= 0; j--){
                free(s->commutationTable[j]);
            }
            free(s->commutationTable);
            free(s);
            return NULL;
        }

        mac_set_octets(s->commutationTable[i], 0, 0, 0, 0, 0, 0);
    }
   
    return s;
}

Switch* switch_init_with_parameter(mac* mac, size_t nbPorts, uint32_t priority)
{
    Switch* s = malloc(sizeof(Switch));
    if (switch_check_pointeur_null(s) != OK ||
        switch_check_pointeur_null(mac)) return NULL;

    s->macAddress = mac;
    s->nbPorts = nbPorts;
    s->prio = priority;
    s->commutationTable = malloc(sizeof(uint64_t) * s->nbPorts);
    if (switch_check_pointeur_null(s->commutationTable) != OK){
        free(s->macAddress);
        free(s);
        return NULL;
    }

    for (size_t i = 0; i < s->nbPorts ; i++)
    {
        s->commutationTable[i] = mac_init();
        if (switch_check_pointeur_null(s->commutationTable[i]) != OK){
            free(s->macAddress);
            for (int j = i; j >= 0; j--){
                free(s->commutationTable[j]);
            }
            free(s->commutationTable);
            free(s);
            return NULL;
        }

        mac_set_octets(s->commutationTable[i], 0, 0, 0, 0, 0, 0);
    }
    return s;
}

ERREUR_CODE switch_deinit(Switch** s)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(*s)) != OK){
        return err;
    }

    for (size_t i = 0; i < (*s)->nbPorts; i++){
        mac_deinit(&(*s)->commutationTable[i]);
    }

    free((*s)->commutationTable);
    (*s)->commutationTable = NULL;

    mac_deinit(&(*s)->macAddress);

    free(*s);
    *s = NULL;

    return OK;
}

ERREUR_CODE switch_show_mac_hexa(Switch* s, char* str, size_t taille_str)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK ||
        (err = mac_get_string(s->macAddress,':', str, taille_str)) != OK){
        return err;
    }

    return OK;
}

ERREUR_CODE switch_show_commutation_table(Switch* s, char* str, size_t taille_str)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK)
    {
        return err;
    }

    char buffer[255];
    char adresse[40];
    size_t nbCharEcris = 0;
    for (size_t i = 0; i < s->nbPorts; i++)
    {
        mac_get_string(s->commutationTable[i], ':', adresse, 40);
        nbCharEcris += snprintf(buffer, taille_str, "Port %zu : %s\n", i+1, adresse);

        if (nbCharEcris >= taille_str){
            return TAILLE_STRING;
        }

        strcat(str,buffer);
    }
    return OK;
}

ERREUR_CODE switch_set_priority(Switch* s, uint32_t priority)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK){
        return err;
    }

    if (priority%4096 == 0) // d'après la doc
    {
        s->prio = priority;
        return OK;
    }
    return INVALID_ARGUMENT;
}

ERREUR_CODE switch_set_commutation_table(Switch* s, size_t port, mac* macAddress)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK || 
        (err = switch_check_pointeur_null(macAddress)) != OK){
        return err;
    }
    if (port >= s->nbPorts)
    {
        return INVALID_ARGUMENT;
    }

    mac_deinit(&s->commutationTable[port]);
    s->commutationTable[port] = macAddress;
    return OK;
}

ERREUR_CODE switch_get_mac(Switch* s, mac* m){
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK ||
        (err = switch_check_pointeur_null(s->macAddress)) != OK){
        return err;
    }

    m = s->macAddress;

    return OK;
}
