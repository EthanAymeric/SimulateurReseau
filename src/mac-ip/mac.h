#ifndef MAC_H
#define MAC_H

#include <stdint.h>

typedef enum ERREUR_MAC {
    ALLOCATION,
    POINTEUR_NULL,
    VALEUR,
    FORMAT_STRING,
    NULLE
} ERREUR_MAC;

typedef struct mac mac;

mac* init();
ERREUR_MAC deinit(mac* mac);
ERREUR_MAC set_uint64(mac* mac, uint64_t valeur);
ERREUR_MAC set_string(mac* mac, char* valeur);
ERREUR_MAC set_octets(mac* mac, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4, uint8_t octet5, uint8_t octet6);
ERREUR_MAC get_string(mac* mac, char separateur, char* str);

#endif 
