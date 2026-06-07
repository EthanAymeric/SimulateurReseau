#include <stdlib.h>

#include "appareil.h"
#include "../switch/switch.h"
#include "../station/station.h"

struct appareil {
    TYPE_APPAREIL type;

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
    ap->type = INDEFINI;

    return ap;
}

ERREUR_CODE appareil_deinit(appareil *appareil){
    ERREUR_CODE err;
    if ((err = appareil_check_pointeur_null(appareil)) != OK){
        return err;
    }

    if (appareil->type == SWITCH){
        switch_deinit(appareil->appareil.sw);
        appareil->appareil.sw = NULL;
    }
    else if (appareil->type == STATION){
        station_deinit(appareil->appareil.st);
        appareil->appareil.st = NULL;
    }

    free(appareil);
    appareil = NULL;

    return OK;
}

ERREUR_CODE appareil_set_station(appareil *appareil, station *station){
    ERREUR_CODE err; 
    if ((err = appareil_check_pointeur_null(appareil)) != OK ||
        (err = appareil_check_pointeur_null(station)) != OK){
        return err;
    }

    if (appareil->type == SWITCH){
        switch_deinit(appareil->appareil.sw);
    }
    else if (appareil->type == STATION){
        station_deinit(appareil->appareil.st);
    }

    appareil->type = STATION;
    appareil->appareil.st = station;

    return OK;
}

ERREUR_CODE appareil_set_switch(appareil *appareil, Switch *sw){
    ERREUR_CODE err; 
    if ((err = appareil_check_pointeur_null(appareil)) != OK ||
        (err = appareil_check_pointeur_null(sw)) != OK){
        return err;
    }

    if (appareil->type == SWITCH){
        free(appareil->appareil.sw);
    }
    else if (appareil->type == STATION){
        free(appareil->appareil.st);
    }

    appareil->type = SWITCH;
    appareil->appareil.sw = sw;

    return OK;
}
