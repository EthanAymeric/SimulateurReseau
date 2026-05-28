#include "mac.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const uint8_t NB_BITS = 48;
const uint8_t OCTET = 8;
const uint8_t NB_OCTETS = NB_BITS / OCTET;

struct mac {
    uint64_t adresse;
};

ERREUR_MAC mac_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : OK;
}

mac* mac_init(){
    mac* a = NULL;
    a = malloc(sizeof(mac));

    return a;
}

ERREUR_MAC mac_check_valeur(uint64_t valeur){
    return valeur > 0xFFFFFFFFFFFF ? VALEUR : OK;
}

ERREUR_MAC mac_deinit(mac* mac){
    ERREUR_MAC err;
    if ((err = mac_check_pointeur_null(mac)) != OK){
        return err;
    }

    free(mac);
    mac = NULL;

    return OK;
}

ERREUR_MAC mac_set_uint64(mac* mac, uint64_t valeur){
    ERREUR_MAC err;
    if ((err = mac_check_pointeur_null(mac)) != OK){
        return err;
    }

    if ((err = mac_check_valeur(valeur)) != OK){
        return err;
    }

    mac->adresse = valeur;

    return OK;
}

ERREUR_MAC mac_set_octets(mac *mac, uint8_t octet1, uint8_t octet2, uint8_t octet3, uint8_t octet4, uint8_t octet5, uint8_t octet6){
    ERREUR_MAC err;
    if ((err = mac_check_pointeur_null(mac)) != OK){
        return err;
    }

    uint64_t valeur = octet1;
    valeur = (valeur << OCTET) + octet2;
    valeur = (valeur << OCTET) + octet3;
    valeur = (valeur << OCTET) + octet4;
    valeur = (valeur << OCTET) + octet5;
    valeur = (valeur << OCTET) + octet6;

    if ((err = mac_check_valeur(valeur)) != OK){
        return err;
    }

    mac->adresse = valeur;

    return OK;
}

ERREUR_MAC mac_set_string(mac* mac, char* valeur){
    ERREUR_MAC err;
    if ((err = mac_check_pointeur_null(mac)) != OK){
        return err;
    }

    uint octets[6];
    if (sscanf(valeur, "%x%*c%x%*c%x%*c%x%*c%x%*c%x", &octets[0], &octets[1], &octets[2], &octets[3], &octets[4], &octets[5]) != 6){
        return FORMAT_STRING;
    }

    if ((err = mac_set_octets(mac, octets[0], octets[1], octets[2], octets[3], octets[4], octets[5])) != OK){
        return err;
    }

    printf("valeur: %lu\n", mac->adresse);

    return OK;
}

ERREUR_MAC mac_get_string(mac* mac, char separateur, char* str){
    ERREUR_MAC err;
    if ((err = mac_check_pointeur_null(mac)) != OK){
        return err;
    }

    char octet[8]; 
    memset(str, '\0', sizeof(char));

    // pour chaque octet
    uint64_t masque = (uint64_t)0xFF << (NB_BITS - OCTET);
    for (size_t i = 0; i < (size_t)NB_OCTETS - 1; i++){
        uint8_t byte = (mac->adresse & masque) >> (NB_BITS - (i + 1) * OCTET);
        sprintf(octet, "%02X%c", byte, separateur);
        strcat(str, octet);
        masque >>= OCTET;
    }

    sprintf(octet, "%02X", (uint)(mac->adresse & masque));
    strcat(str, octet);

    return OK;
}
