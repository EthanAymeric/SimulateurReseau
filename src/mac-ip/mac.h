#ifndef MAC_H
#define MAC_H

#include <stdint.h>

typedef enum ERREUR_MAC {
    ALLOCATION,
    POINTEUR_NULL,
    VALEUR,
    FORMAT_STRING,
    OK
} ERREUR_MAC;

typedef struct mac mac;

mac* mac_init();
ERREUR_MAC mac_deinit(mac* mac);
ERREUR_MAC mac_set_uint64(mac* mac, uint64_t valeur);
ERREUR_MAC mac_set_string(mac* mac, char* valeur);
ERREUR_MAC mac_set_octets(mac* mac, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4, uint8_t octet5, uint8_t octet6);
ERREUR_MAC mac_get_string(mac* mac, char separateur, char* str);

#endif 
