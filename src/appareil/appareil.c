#include <stdlib.h>
#include <string.h>

#include "appareil.h"
#include "../switch/switch.h"
#include "../station/station.h"

const size_t ORD_SIZE = 8;

struct appareil {
    TYPE_APPAREIL type;
    Trame** ordnanceur;
    size_t tailleOrdnanceur;
    size_t nbTrame;

    union {
        station* st;
        Switch* sw;
    } appareil;
};

ERREUR_CODE appareil_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : OK;
}

appareil* appareil_init(){
    appareil* ap = NULL;

    ap = malloc(sizeof(appareil));
    if (appareil_check_pointeur_null(ap) != OK) return NULL;
    ap->tailleOrdnanceur = ORD_SIZE;
    ap->ordnanceur = malloc(sizeof(Trame*) * ap->tailleOrdnanceur);
    ap->nbTrame = 0;
    ap->type = INDEFINI;
    ap->appareil.st = NULL;

    return ap;
}

ERREUR_CODE appareil_deinit(appareil** ap){
    ERREUR_CODE err;
    if ((err = appareil_check_pointeur_null(*ap)) != OK){
        return err;
    }

    if ((*ap)->type == SWITCH && appareil_check_pointeur_null((*ap)->appareil.sw) == OK){
        switch_deinit(&(*ap)->appareil.sw);
        (*ap)->appareil.sw = NULL;
    }
    else if ((*ap)->type == STATION && appareil_check_pointeur_null((*ap)->appareil.st) == OK){
        station_deinit(&(*ap)->appareil.st);
        (*ap)->appareil.st = NULL;
    }

    for (size_t i = 0; i < (*ap)->nbTrame; i++)
    {
        if (appareil_check_pointeur_null((*ap)->ordnanceur[i]) != POINTEUR_NULL)
        {
            trame_deinit((*ap)->ordnanceur[i]);
        }
    }
    free((*ap)->ordnanceur);

    free(*ap);
    *ap= NULL;

    return OK;
}

ERREUR_CODE appareil_set_station(appareil *ap, station *st){
    ERREUR_CODE err; 
    if ((err = appareil_check_pointeur_null(ap)) != OK ||
        (err = appareil_check_pointeur_null(st)) != OK){
        return err;
    }

    if (ap->type == STATION){
        station_deinit(&ap->appareil.st);
    }
    else if (ap->type == SWITCH){
        switch_deinit(&ap->appareil.sw);
    }

    ap->type = STATION;
    ap->appareil.st = st;

    return OK;
}

ERREUR_CODE appareil_set_switch(appareil *ap, Switch *sw){
    ERREUR_CODE err; 
    if ((err = appareil_check_pointeur_null(ap)) != OK ||
        (err = appareil_check_pointeur_null(sw)) != OK){
        return err;
    }

    if (ap->type == STATION){
        station_deinit(&ap->appareil.st);
    }
    else if (ap->type == SWITCH){
        switch_deinit(&ap->appareil.sw);
    }

    ap->type = SWITCH;
    ap->appareil.sw = sw;

    return OK;
}

ERREUR_CODE appareil_get_type(appareil *ap, TYPE_APPAREIL *type){
    ERREUR_CODE err;
    if ((err = appareil_check_pointeur_null(ap)) != OK){
        return err;
    }

    // pour éviter de pouvoir changer le type en dehors de la structure
    memcpy(type, &ap->type, sizeof(TYPE_APPAREIL)); 

    return OK;
}

ERREUR_CODE appareil_get_station(appareil *ap, station **st){
    ERREUR_CODE err; 
    if ((err = appareil_check_pointeur_null(ap)) != OK ||
        (err = appareil_check_pointeur_null(ap->appareil.st)) != OK){
        return err;
    }
    if (ap->type != STATION){
        return TYPE;
    }

    // pour pouvoir modifier les informations de la station en dehors de appareil
    *st = ap->appareil.st;

    return OK;
}

ERREUR_CODE appareil_get_switch(appareil *ap, Switch **sw){
    ERREUR_CODE err; 
    if ((err = appareil_check_pointeur_null(ap)) != OK ||
        (err = appareil_check_pointeur_null(ap->appareil.sw)) != OK){
        return err;
    }
    if (ap->type != SWITCH){
        return TYPE;
    }

    // pour pouvoir modifier les informations du switch en dehors de appareil
    *sw = ap->appareil.sw;

    return OK;
}

ERREUR_CODE appareil_get_mac(appareil* ap, mac* mac){
    TYPE_APPAREIL type;
    appareil_get_type(ap, &type);
    station* st = NULL;
    Switch* sw = NULL;

    switch (type){
        case SWITCH:
            appareil_get_switch(ap, &sw);
            break;

        case STATION:
            appareil_get_station(ap, &st);
            station_get_mac(st, &mac);
            break;

        case INDEFINI:
            return INVALID_ARGUMENT;
            break;
    }

    return OK;
}

ERREUR_CODE appareil_recieve_trame(Trame* trame, appareil* recepteur)
{
    if(appareil_check_pointeur_null(trame) == POINTEUR_NULL || appareil_check_pointeur_null(recepteur) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }
    size_t index = recepteur->nbTrame;

    if (index == recepteur->tailleOrdnanceur)
    {
        recepteur->tailleOrdnanceur *= 2;
        recepteur->ordnanceur = realloc(recepteur->ordnanceur, sizeof(Trame*) * recepteur->tailleOrdnanceur);

    }

    recepteur->ordnanceur[index] = trame;
    recepteur->nbTrame++;

    return OK;
}

ERREUR_CODE appareil_get_ordnanceur_size(appareil* ap, size_t* size)
{
    ERREUR_CODE err;
    if ((err = appareil_check_pointeur_null(ap)) != OK){
        return err;
    }

    *size = ap->nbTrame;
    return OK;
}
