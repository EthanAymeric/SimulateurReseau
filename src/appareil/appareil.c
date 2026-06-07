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

ERREUR_CODE appareil_deinit(appareil *ap){
    ERREUR_CODE err;
    if ((err = appareil_check_pointeur_null(ap)) != OK){
        return err;
    }

    if (ap->type == SWITCH){
        switch_deinit(ap->appareil.sw);
        ap->appareil.sw = NULL;
    }
    else if (ap->type == STATION){
        station_deinit(ap->appareil.st);
        ap->appareil.st = NULL;
    }

    free(ap);
    ap= NULL;

    return OK;
}

ERREUR_CODE appareil_set_station(appareil *ap, station *st){
    ERREUR_CODE err; 
    if ((err = appareil_check_pointeur_null(ap)) != OK ||
        (err = appareil_check_pointeur_null(st)) != OK){
        return err;
    }

    if (ap->type == SWITCH){
        switch_deinit(ap->appareil.sw);
    }
    else if (ap->type == STATION){
        station_deinit(ap->appareil.st);
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

    if (ap->type == SWITCH){
        switch_deinit(ap->appareil.sw);
    }
    else if (ap->type == STATION){
        station_deinit(ap->appareil.st);
    }

    appareil->type = SWITCH;
    appareil->appareil.sw = sw;

    return OK;
}
