#ifndef STATION_H
#define STATION_H

#include "../error/error.h"
#include "../mac-ip/ip.h"
#include "../mac-ip/mac.h"

#include <stddef.h>
#include <stdint.h>

typedef struct station station;

station* station_init();
ERREUR_CODE station_deinit(station** station);

ERREUR_CODE station_get_string(station* station, char* str, size_t taille_str);
ERREUR_CODE station_get_mac(station* station, mac** mac);
ERREUR_CODE station_set_ip_mac(station* station, ip* ip, mac* mac);
ERREUR_CODE station_set_ip(station* station, ip* ip);
ERREUR_CODE station_set_mac(station* station, mac* mac);

#endif
