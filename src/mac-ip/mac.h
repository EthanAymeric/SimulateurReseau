#ifndef MAC_H
#define MAC_H

#include <stddef.h>
#include <stdint.h>

typedef enum ERREUR_MAC {
    ALLOCATION,
    POINTEUR_NULL,
    VALEUR,
    VALEUR_OCTET,
    FORMAT_STRING,
    TAILLE_STRING,
    INDEX_OCTET,
    OK
} ERREUR_MAC;

typedef struct mac mac;

mac* mac_init();
ERREUR_MAC mac_deinit(mac* mac);
ERREUR_MAC mac_set_uint64(mac* mac, uint64_t valeur);
ERREUR_MAC mac_set_string(mac* mac, char* valeur);
ERREUR_MAC mac_set_octets(mac* mac, int octet1, int octet2, int octet3, int octet4, int octet5, int octet6);
ERREUR_MAC mac_set_octet(mac* mac, int valeur, size_t index);
ERREUR_MAC mac_get_string(mac* mac, char separateur, char* str, size_t taille_str);

#endif 
