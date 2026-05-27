#include "ip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ip {
    char adresse[4];
    uint8_t cidr;
};

ERREUR_IP check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : NULLE;
}

ip* init(){
    ip* a = NULL;
    a = malloc(sizeof(ip));

    return a;
}

ERREUR_IP check_index_octet(size_t index){
    return (index >= 0 && index < 4) ? NULLE : INDEX_OCTET;
}

ERREUR_IP check_cidr(uint8_t cidr){
    return (cidr >= 0 && cidr <= 32) ? NULLE : VALEUR_CIDR;
}

ERREUR_IP deinit(ip* ip){
    if (check_pointeur_null(ip) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    free(ip);
    ip = NULL;

    return NULLE;
}

ERREUR_IP get_string(ip* ip, char* str){
    if (check_pointeur_null(ip) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    sprintf(str, "%d.%d.%d.%d/%d", ip->adresse[0], ip->adresse[1], ip->adresse[2], ip->adresse[3], ip->cidr);

    return NULLE;
}

ERREUR_IP set_adresse(ip* ip, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4){
    if (check_pointeur_null(ip) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    ip->adresse[0] = octet1;
    ip->adresse[1] = octet2;
    ip->adresse[2] = octet3;
    ip->adresse[3] = octet4;

    return NULLE;
}

ERREUR_IP set_octet_adresse(ip* ip, uint8_t octet, size_t index){
    if (check_pointeur_null(ip) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    if (check_index_octet(index) == INDEX_OCTET){
        return INDEX_OCTET;
    }

    ip->adresse[index] = octet;

    return NULLE;
}

ERREUR_IP set_cidr(ip* ip , uint8_t cidr){
    if (check_pointeur_null(ip) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    if (check_cidr(cidr) == VALEUR_CIDR){
        return VALEUR_CIDR;
    }

    ip->cidr = cidr;

    return NULLE;
}
