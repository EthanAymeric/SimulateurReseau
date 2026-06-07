#include "mac.h"
#include <ctype.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

const uint8_t NB_BITS = 48;
const uint8_t OCTET = 8;
const uint8_t NB_OCTETS = NB_BITS / OCTET;

struct mac {
    uint64_t adresse;
};

ERREUR_CODE mac_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : OK;
}

mac* mac_init(){
    mac* a = NULL;
    a = malloc(sizeof(mac));

    if (mac_check_pointeur_null(a) != OK) return NULL;

    mac_set_octets(a, 0, 0, 0, 0, 0, 0);

    return a;
}

ERREUR_CODE mac_check_valeur(uint64_t valeur){
    return valeur < 0xFFFFFFFFFFFF ? OK : VALEUR;
}

ERREUR_CODE mac_check_valeur_octet(int valeur){
    return (valeur >= 0 && valeur <= 255) ? OK : VALEUR_OCTET;
}

ERREUR_CODE mac_check_index_octet(int index){
    return (index >= 0 && index < NB_OCTETS) ? OK : INDEX_OCTET;
}

ERREUR_CODE mac_deinit(mac* mac){
    ERREUR_CODE err;
    if ((err = mac_check_pointeur_null(mac)) != OK){
        return err;
    }

    free(mac);
    mac = NULL;

    return OK;
}

ERREUR_CODE mac_set_uint64(mac* mac, uint64_t valeur){
    ERREUR_CODE err;
    if ((err = mac_check_pointeur_null(mac)) != OK ||
        (err = mac_check_valeur(valeur)) != OK){
        return err;
    }

    mac->adresse = valeur;

    return OK;
}

ERREUR_CODE mac_set_octets(mac* mac, int octet1, int octet2, int octet3, int octet4, int octet5, int octet6){
    ERREUR_CODE err;
    if ((err = mac_check_pointeur_null(mac)) != OK ||
        (err = mac_check_valeur_octet(octet1)) != OK ||
        (err = mac_check_valeur_octet(octet2)) != OK ||
        (err = mac_check_valeur_octet(octet3)) != OK ||
        (err = mac_check_valeur_octet(octet4)) != OK ||
        (err = mac_check_valeur_octet(octet5)) != OK ||
        (err = mac_check_valeur_octet(octet6)) != OK){
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

ERREUR_CODE mac_set_string(mac* mac, char* valeur){
    ERREUR_CODE err;
    if ((err = mac_check_pointeur_null(mac)) != OK){
        return err;
    }

    uint octets[6];
    int caracteres_lus = 0;
    if (sscanf(valeur, "%x%*c%x%*c%x%*c%x%*c%x%*c%x%n",
               &octets[0], &octets[1], &octets[2],
               &octets[3], &octets[4], &octets[5], &caracteres_lus) != 6){
        return FORMAT_STRING;
    }
    // vérifie qu'il n'y ait pas de préfixe 
    if (!isxdigit((unsigned char)valeur[0])){
        return FORMAT_STRING;
    }
    // vérifie qu'il n'y ait pas de suffixe: valeur doit s'arrêter où sscanf s'est arrêté  
    if (valeur[caracteres_lus] != '\0'){
        return FORMAT_STRING;
    }

    err = mac_set_octets(mac, octets[0], octets[1], octets[2], octets[3], octets[4], octets[5]);
    if (err == VALEUR_OCTET){
        return VALEUR;
    }
    else if (err != OK){
        return err;
    }

    return OK;
}

ERREUR_CODE mac_set_octet(mac* mac, int valeur, size_t index){
    ERREUR_CODE err;
    if ((err = mac_check_pointeur_null(mac)) != OK ||
        (err = mac_check_index_octet(index)) != OK ||
        (err = mac_check_valeur_octet(valeur)) != OK){ 
        return err;
    }

    uint8_t offset = (NB_BITS - (OCTET * index));
    uint8_t masque = 0xFF << offset; // on masque le n-ième octet
    mac->adresse &= ~masque; // on met cet octet à 0: adresse = adresse ET NOT(masque)
    mac->adresse |= valeur << offset; // on remplace l'octet à 0 par la nouvelle valeur: adresse OU valeur

    return OK;
}

ERREUR_CODE mac_get_string(mac* mac, char separateur, char* str, size_t taille_str){
    ERREUR_CODE err;
    if ((err = mac_check_pointeur_null(mac)) != OK){
        return err;
    }

    size_t compteur_octets = 0;
    char octet[8]; 
    memset(str, '\0', sizeof(char));

    // pour chaque octet
    uint64_t masque = (uint64_t)0xFF << (NB_BITS - OCTET);
    for (size_t i = 0; i < (size_t)NB_OCTETS - 1; i++){
        uint8_t byte = (mac->adresse & masque) >> (NB_BITS - (i + 1) * OCTET);
        compteur_octets += snprintf(octet, taille_str, "%02X%c", byte, separateur);

        if (compteur_octets >= taille_str){
            return TAILLE_STRING;
        }

        strcat(str, octet);
        masque >>= OCTET;
    }

    compteur_octets += snprintf(octet, taille_str, "%02X", (uint)(mac->adresse & masque));
    if (compteur_octets >= taille_str){
        return TAILLE_STRING;
    }

    strcat(str, octet);

    return OK;
}
