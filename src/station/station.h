#ifndef STATION_H
#define STATION_H

#include <stddef.h>
#include <stdint.h>

typedef enum ERREUR_STATION {
    ALLOCATION,
    POINTEUR_NULL,
    NULLE
}   ERREUR_STATION;

typedef struct station station;

station* init();
ERREUR_STATION deinit(station* station);

ERREUR_STATION get_string(station* station, char* str);
ERREUR_STATION set_station(station* station, ip ip, mac mac);