#include "switch.h"
#include <stdio.h>
#include <stdlib.h>



struct commutateur {
    uint64_t* commutationTable; 
    uint64_t macAddress;
    uint32_t prio;
    size_t nbPorts;
};

SWITCH_ERROR check_pointeur_null(void* ptr)
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

void switch_show_mac_hexa(Switch* s)
{
    printf("%lx\n",s->macAddress);
}

void switch_show_commutation_table(Switch* s)
{
    for (size_t i = 0; i < s->nbPorts; i++)
    {
        printf("Port %zu : %lx\n", i+1, s->commutationTable[i]);
    }
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
