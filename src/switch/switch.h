#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>
#include "../error/error.h"

typedef struct commutateur Switch;

Switch* switch_init();
Switch* switch_init_with_parameter(uint64_t mac, size_t nbPorts, uint32_t priority);
void switch_deinit(Switch* s);
ERREUR_CODE switch_show_mac_hexa(Switch* s, char* str);
ERREUR_CODE switch_show_commutation_table(Switch* s, char* str);
ERREUR_CODE switch_set_priority(Switch* s, uint32_t prio);
ERREUR_CODE switch_set_commutation_table(Switch* s, size_t port, uint64_t macAddress);





#endif