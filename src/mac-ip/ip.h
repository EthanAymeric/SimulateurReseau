#ifndef IP_H
#define IP_H

#include <stddef.h>
#include <stdint.h>

typedef enum ERREUR_IP {
    ALLOCATION,
    POINTEUR_NULL,
    INDEX_OCTET,
    VALEUR_OCTET,
    VALEUR_CIDR,
    NULLE
} ERREUR_IP;

typedef struct ip ip;

ip* ip_init();
ERREUR_IP ip_deinit(ip* ip);

ERREUR_IP ip_set_adresse(ip* ip, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4);
ERREUR_IP ip_set_octet_adresse(ip* ip, uint8_t octet, size_t index);
ERREUR_IP ip_set_cidr(ip* ip , uint8_t cidr);
ERREUR_IP ip_get_string(ip* ip, char* str);

#endif 
