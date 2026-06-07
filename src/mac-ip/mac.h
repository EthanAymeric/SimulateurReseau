#ifndef MAC_H
#define MAC_H

#include <stddef.h>
#include <stdint.h>
#include "../error/error.h"


typedef struct mac mac;

mac* mac_init();
ERREUR_CODE mac_deinit(mac** mac);
ERREUR_CODE mac_set_uint64(mac* mac, uint64_t valeur);
ERREUR_CODE mac_set_string(mac* mac, char* valeur);
ERREUR_CODE mac_set_octets(mac* mac, int octet1, int octet2, int octet3, int octet4, int octet5, int octet6);
ERREUR_CODE mac_set_octet(mac* mac, int valeur, size_t index);
ERREUR_CODE mac_get_string(mac* mac, char separateur, char* str, size_t taille_str);

#endif 
