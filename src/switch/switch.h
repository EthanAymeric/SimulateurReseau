#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>

typedef enum SWITCH_ERROR{
    ALLOCATION,
    POINTEUR_NULL,
    INVALID_ARGUMENT,
    NULLE,
    OK
} SWITCH_ERROR;

typedef struct commutateur Switch;

Switch* switch_init();
Switch* switch_init_with_parameter(uint64_t mac, size_t nbPorts, uint32_t priority);
void switch_deinit(Switch* s);
void switch_show_mac_hexa(Switch* s);
void switch_show_commutation_table(Switch* s);
SWITCH_ERROR switch_set_priority(Switch* s, uint32_t prio);
SWITCH_ERROR switch_set_commutation_table(Switch* s, size_t port, uint64_t macAddress);





#endif