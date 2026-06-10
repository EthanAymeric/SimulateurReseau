#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>
#include "../error/error.h"
#include "../mac-ip/mac.h"
#include "../interface/interface.h"

typedef struct commutateur Switch;

typedef enum PORT_ETAT {
    PORT_INCONNU,
    PORT_RACINE,
    PORT_DESIGNE,
    PORT_BLOQUE
} PORT_ETAT;

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

/* Getters utiles pour le STP */
ERREUR_CODE switch_get_priority(Switch* s, uint32_t* prio);
ERREUR_CODE switch_get_nb_connexions(Switch* s, size_t* nb);
ERREUR_CODE switch_get_connexion(Switch* s, size_t port, Interface** inter);

/* STP */
ERREUR_CODE switch_stp_init(Switch* s);
ERREUR_CODE switch_stp_get_root_info(Switch* s, uint32_t* root_prio, char* root_mac,
                                      size_t mac_len, uint32_t* root_cost, int* root_port);
ERREUR_CODE switch_stp_try_update(Switch* s, uint32_t root_prio, const char* root_mac,
                                   uint32_t cost, size_t via_port, int* changed);
ERREUR_CODE switch_stp_get_port_etat(Switch* s, size_t port, PORT_ETAT* etat);
ERREUR_CODE switch_stp_set_port_etat(Switch* s, size_t port, PORT_ETAT etat);


#endif
