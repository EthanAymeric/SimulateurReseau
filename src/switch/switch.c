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
    s->macAddress = mac_init();
    s->nbPorts = 8;
    s->prio = 32768;
    s->commutationTable = malloc(sizeof(mac*) * s->nbPorts);
    for (size_t i = 0; i < s->nbPorts ; i++)
    {
        s->commutationTable[i] = mac_init();
        mac_set_octets(s->commutationTable[i], 0, 0, 0, 0, 0, 0);
    }
   
    return s;
}

Switch* switch_init_with_parameter(mac* mac, size_t nbPorts, uint32_t priority)
{
    Switch* s = malloc(sizeof(Switch));
    s->macAddress = mac;
    s->nbPorts = nbPorts;
    s->prio = priority;
    s->commutationTable = malloc(sizeof(uint64_t) * s->nbPorts);
    for (size_t i = 0; i < s->nbPorts ; i++)
    {
        s->commutationTable[i] = 0;
    }
    return s;
}

void switch_deinit(Switch* s)
{
    for (size_t i = 0; i < s->nbPorts; i++){
        mac_deinit(s->commutationTable[i]);
    }

    free(s->commutationTable);
    s->commutationTable = NULL;

    mac_deinit(s->macAddress);

    free(s);
    s = NULL;
}

ERREUR_CODE switch_show_mac_hexa(Switch* s, char* str)
{
    ERREUR_CODE err;
    if ((err = switch_check_pointeur_null(s)) != OK ||
        (err = mac_get_string(s->macAddress,':',str,19)) != OK){
        return err;
    }

    return OK;
}

ERREUR_CODE switch_show_commutation_table(Switch* s, char* str)
{
    if (switch_check_pointeur_null(s) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }
    char buffer[255];
    char adresse[40];
    for (size_t i = 0; i < s->nbPorts; i++)
    {
        mac_get_string(s->commutationTable[i], ':', adresse, 40);
        sprintf(buffer,"Port %zu : %s\n", i+1, adresse);
        strcat(str,buffer);
    }
    return OK;
}

ERREUR_CODE switch_set_priority(Switch* s, uint32_t priority)
{
    if (priority%4096 == 0)
    {
        s->prio = priority;
        return OK;
    }
    return INVALID_ARGUMENT;
}

ERREUR_CODE switch_set_commutation_table(Switch* s, size_t port, mac* macAddress)
{
    if (port >= s->nbPorts)
    {
        return INVALID_ARGUMENT;
    }

    s->commutationTable[port] = macAddress;
    return OK;
}
