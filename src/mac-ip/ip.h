#ifndef IP_H
#define IP_H

#include <stddef.h>
#include <stdint.h>

typedef enum ERREUR_IP {
    ALLOCATION,
    POINTEUR_NULL,
    INDEX_OCTET,
    VALEUR_OCTET,
    NULLE
} ERREUR_IP;

typedef struct ip ip;

ip* init();
void deinit(ip*);

char* get(ip*);
ERREUR_IP set_adresse(ip*, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4);
ERREUR_IP set_octet_adresse(ip*, uint8_t octet, size_t numero);
ERREUR_IP set_masque(ip*, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4);
ERREUR_IP set_octet_masque(ip*, uint8_t octet, size_t numero);
ERREUR_IP get_string(ip*, char* str);

#endif 
