#include "ip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ip {
    char adresse[4];
    uint8_t cidr;
};

ERREUR_IP ip_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : OK;
}

ip* ip_init(){
    ip* a = NULL;
    a = malloc(sizeof(ip));

    return a;
}

ERREUR_IP ip_check_index_octet(size_t index){
    // index > 0 toujours vrai
    return index < 4 ? OK : INDEX_OCTET;
}

ERREUR_IP ip_check_cidr(uint8_t cidr){
    // cidr > 0 toujours vrai
    return cidr <= 32 ? OK : VALEUR_CIDR;
}

ERREUR_IP ip_deinit(ip* ip){
    ERREUR_IP err;
    if ((err = ip_check_pointeur_null(ip)) != OK){
        return err;
    }

    free(ip);
    ip = NULL;

    return OK;
}

ERREUR_IP ip_get_string(ip* ip, char* str){
    ERREUR_IP err;
    if ((err = ip_check_pointeur_null(ip)) != OK){
        return err;
    }

    sprintf(str, "%d.%d.%d.%d/%d", ip->adresse[0], ip->adresse[1], ip->adresse[2], ip->adresse[3], ip->cidr);

    return OK;
}

ERREUR_IP ip_set_adresse(ip* ip, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4){
    ERREUR_IP err;
    if ((err = ip_check_pointeur_null(ip)) != OK){
        return err;
    }

    ip->adresse[0] = octet1;
    ip->adresse[1] = octet2;
    ip->adresse[2] = octet3;
    ip->adresse[3] = octet4;

    return OK;
}

ERREUR_IP ip_set_octet_adresse(ip* ip, uint8_t octet, size_t index){
    ERREUR_IP err;
    if ((err = ip_check_pointeur_null(ip)) != OK){
        return err;
    }

    if ((err = ip_check_index_octet(index)) != OK){
        return err;
    }

    ip->adresse[index] = octet;

    return OK;
}

ERREUR_IP ip_set_cidr(ip* ip , uint8_t cidr){
    ERREUR_IP err;
    if ((err = ip_check_pointeur_null(ip)) != OK){
        return err;
    }

    if ((err = ip_check_cidr(cidr)) != OK){
        return err;
    }

    ip->cidr = cidr;

    return OK;
}
