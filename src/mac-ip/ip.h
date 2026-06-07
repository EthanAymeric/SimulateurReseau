#ifndef IP_H
#define IP_H

#include <stddef.h>
#include <stdint.h>
#include "../error/error.h"

typedef struct ip ip;

ip* ip_init();
ERREUR_CODE ip_deinit(ip** ip);

ERREUR_CODE ip_set_adresse(ip* ip, int octet1, int octet2, int octet3, int octet4);
ERREUR_CODE ip_set_octet_adresse(ip* ip, int octet, size_t index);
ERREUR_CODE ip_set_cidr(ip* ip , uint8_t cidr);
ERREUR_CODE ip_get_string(ip* ip, char* str, size_t taille_str);

#endif 
