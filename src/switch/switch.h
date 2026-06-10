#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>
#include "../error/error.h"
#include "../mac-ip/mac.h"
#include "../interface/interface.h"

typedef struct commutateur Switch;

Switch* switch_init();
Switch* switch_init_with_parameter(mac* mac, size_t nbPorts, uint32_t priority);
ERREUR_CODE switch_deinit(Switch** s);
ERREUR_CODE switch_show_mac_hexa(Switch* s, char* str, size_t taille_str);
ERREUR_CODE switch_show_commutation_table(Switch* s, char* str, size_t taille_str);
ERREUR_CODE switch_set_priority(Switch* s, uint32_t prio);
ERREUR_CODE switch_set_commutation_table(Switch* s, size_t port, mac* macAddress);
ERREUR_CODE switch_learn_mac(Switch* s, mac* adresse, size_t port);
ERREUR_CODE switch_find_port(Switch* s, mac* adresse, size_t* port);
ERREUR_CODE switch_get_mac(Switch* s, mac** m);
ERREUR_CODE switch_set_interface(Switch* s, Interface* inter);
ERREUR_CODE switch_broadcast_trame(Switch* s, Trame* trame);
ERREUR_CODE switch_commuter_trame(Switch* s, Trame* trame, size_t port_entree);
ERREUR_CODE switch_traiter_trame(Switch* s);


#endif
