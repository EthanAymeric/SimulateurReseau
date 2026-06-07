#ifndef APPAREIL_H
#define APPAREIL_H

#include "../error/error.h"
#include "../station/station.h"
#include "../switch/switch.h"
#include <error.h>

typedef struct appareil appareil;

typedef enum TYPE_APPAREIL {
    SWITCH,
    STATION,
    INDEFINI
} TYPE_APPAREIL;

appareil* appareil_init();
ERREUR_CODE appareil_deinit(appareil* appareil);

ERREUR_CODE appareil_set_station(appareil* appareil, station* station);
ERREUR_CODE appareil_set_switch(appareil* appareil, Switch* sw);
ERREUR_CODE appareil_get_type(appareil* appareil, TYPE_APPAREIL* type);
ERREUR_CODE appareil_get_station(appareil* appareil, station* station);
ERREUR_CODE appareil_get_switch(appareil* appareil, Switch* sw);

#endif
