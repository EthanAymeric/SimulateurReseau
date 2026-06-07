#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>
#include "../error/error.h"
#include "../mac-ip/mac.h"

typedef struct commutateur Switch;

Switch* switch_init();
Switch* switch_init_with_parameter(mac* mac, size_t nbPorts, uint32_t priority);
ERREUR_CODE switch_deinit(Switch** s);
ERREUR_CODE switch_show_mac_hexa(Switch* s, char* str, size_t taille_str);
ERREUR_CODE switch_show_commutation_table(Switch* s, char* str, size_t taille_str);
ERREUR_CODE switch_set_priority(Switch* s, uint32_t prio);
ERREUR_CODE switch_set_commutation_table(Switch* s, size_t port, mac* macAddress);

#endif
