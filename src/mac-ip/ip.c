#include "ip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ip {
    uint8_t adresse[4];
    uint8_t cidr;
};

ERREUR_CODE ip_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : OK;
}

ip* ip_init(){
    ip* a = NULL;
    a = malloc(sizeof(ip));

    if (ip_check_pointeur_null(a) != OK) return NULL;

    ip_set_adresse(a, 0, 0, 0, 0);
    ip_set_cidr(a, 0);

    return a;
}

ERREUR_CODE ip_check_index_octet(size_t index){
    // index > 0 toujours vrai
    return index < 4 ? OK : INDEX_OCTET;
}

ERREUR_CODE ip_check_cidr(uint8_t cidr){
    // cidr > 0 toujours vrai
    return cidr <= 32 ? OK : VALEUR_CIDR;
}

ERREUR_CODE ip_check_valeur_octet(int octet){
    // utilisation de int car 0 <= octet <= 255 et que pour uint8_t octet = -1 ; octet == 255 
    // On évite ainsi l'absence d'erreur pour octet = -1
    return (octet >= 0 && octet < 256) ? OK : VALEUR_OCTET;
}

ERREUR_CODE ip_deinit(ip* ip){
    ERREUR_CODE err;
    if ((err = ip_check_pointeur_null(ip)) != OK){
        return err;
    }

    free(ip);
    ip = NULL;

    return OK;
}

ERREUR_CODE ip_get_string(ip* ip, char* str, size_t taille_str){
    ERREUR_CODE err;
    if ((err = ip_check_pointeur_null(ip)) != OK){
        return err;
    }

    memset(str, '\0', sizeof(char));

    int nb_ecrit = snprintf(str, taille_str, "%d.%d.%d.%d/%d", ip->adresse[0], ip->adresse[1], ip->adresse[2], ip->adresse[3], ip->cidr);
    if (nb_ecrit < 0 || (size_t)nb_ecrit >= taille_str){
        return TAILLE_STRING;
    }

    return OK;
}

ERREUR_CODE ip_set_adresse(ip* ip, int octet1, int octet2, int octet3, int octet4){
    ERREUR_CODE err;
    if ((err = ip_check_pointeur_null(ip)) != OK ||
        (err = ip_check_valeur_octet(octet1)) != OK || 
        (err = ip_check_valeur_octet(octet2)) != OK || 
        (err = ip_check_valeur_octet(octet3)) != OK || 
        (err = ip_check_valeur_octet(octet4)) != OK){
        return err;
    } 

    ip->adresse[0] = octet1;
    ip->adresse[1] = octet2;
    ip->adresse[2] = octet3;
    ip->adresse[3] = octet4;

    return OK;
}

ERREUR_CODE ip_set_octet_adresse(ip* ip, int octet, size_t index){
    ERREUR_CODE err;
    if ((err = ip_check_pointeur_null(ip)) != OK ||
        (err = ip_check_index_octet(index)) != OK ||
        (err = ip_check_valeur_octet(octet)) != OK){
        return err;
    }

    ip->adresse[index] = octet;

    return OK;
}

ERREUR_CODE ip_set_cidr(ip* ip , uint8_t cidr){
    ERREUR_CODE err;
    if ((err = ip_check_pointeur_null(ip)) != OK ||
        (err = ip_check_cidr(cidr)) != OK){
        return err;
    }

    ip->cidr = cidr;

    return OK;
}
