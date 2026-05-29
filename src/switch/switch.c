#include "switch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct commutateur {
    uint64_t* commutationTable; 
    uint64_t macAddress;
    uint32_t prio;
    size_t nbPorts;
};

SWITCH_ERROR switch_check_pointeur_null(void* ptr)
{
    return (ptr == NULL) ? POINTEUR_NULL : NULLE;
}

Switch* switch_init() {
    Switch* s = malloc(sizeof(Switch));
    s->nbPorts = 8;
    s->prio = 32768;
    s->commutationTable = malloc(sizeof(uint64_t) * s->nbPorts);
    for (size_t i = 0; i < s->nbPorts ; i++)
    {
        s->commutationTable[i] = 0;
    }
   
    return s;
}

Switch* switch_init_with_parameter(uint64_t mac, size_t nbPorts, uint32_t priority)
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
    free(s->commutationTable);
    free(s);
}

SWITCH_ERROR switch_show_mac_hexa(Switch* s, char* str)
{
    if (switch_check_pointeur_null(s) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }
    sprintf(str,"%lx\n",s->macAddress);
    return OK;
}

SWITCH_ERROR switch_show_commutation_table(Switch* s, char* str)
{
    if (switch_check_pointeur_null(s) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }
    char buffer[255];
    for (size_t i = 0; i < s->nbPorts; i++)
    {
        sprintf(buffer,"Port %zu : %lx\n", i+1, s->commutationTable[i]);
        strcat(str,buffer);
    }
    return OK;
}

SWITCH_ERROR switch_set_priority(Switch* s, uint32_t priority)
{
    if (priority%4096 == 0)
    {
        s->prio = priority;
        return OK;
    }
    return INVALID_ARGUMENT;
}

SWITCH_ERROR switch_set_commutation_table(Switch* s, size_t port, uint64_t macAddress)
{
    if (port >= s->nbPorts)
    {
        return INVALID_ARGUMENT;
    }
    s->commutationTable[port] = macAddress;
    return OK;
}
