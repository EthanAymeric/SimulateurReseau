#ifndef STATION_H
#define STATION_H

#include "../error/error.h"
#include "../mac-ip/ip.h"
#include "../mac-ip/mac.h"

#include <stddef.h>
#include <stdint.h>

typedef struct station station;

station* init();
ERREUR_CODE deinit(station* station);

ERREUR_CODE get_string(station* station, char* str);
ERREUR_CODE set_station(station* station, ip ip, mac mac);
ERREUR_CODE station_ajoute_relation_station(station* st1, station* st2);
ERREUR_CODE station_suppr_relation(station* st1, station* st2);
// ERREUR_CODE station_ajoute_relation_switch(station* st, switch* sw);

#endif
