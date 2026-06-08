#ifndef APPAREIL_H
#define APPAREIL_H

#include "../error/error.h"
#include "../station/station.h"
#include "../switch/switch.h"
#include "../trame/trame.h"
#include <error.h>

typedef struct appareil appareil;

typedef enum TYPE_APPAREIL {
    SWITCH,
    STATION,
    INDEFINI
} TYPE_APPAREIL;

appareil* appareil_init();
ERREUR_CODE appareil_deinit(appareil** ap);

ERREUR_CODE appareil_set_station(appareil* ap, station* st);
ERREUR_CODE appareil_set_switch(appareil *ap, Switch *sw);
ERREUR_CODE appareil_get_type(appareil* ap, TYPE_APPAREIL* type);
ERREUR_CODE appareil_recieve_trame(Trame* trame, appareil* recepteur);
ERREUR_CODE appareil_get_ordnanceur_size(appareil* ap, size_t* size);



ERREUR_CODE appareil_get_station(appareil* ap, station** st);
ERREUR_CODE appareil_get_switch(appareil* ap, Switch** sw);
ERREUR_CODE appareil_get_mac(appareil* ap, mac* mac);

#endif
