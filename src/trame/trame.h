#ifndef TRAME_H
#define TRAME_H

#include "../mac-ip/mac.h"

typedef struct Trame Trame;

Trame* trame_init(mac* adresse_source, mac* adresse_destination, char* data);
void trame_deinit(Trame** trame);
ERREUR_CODE trame_print(Trame* trame);
ERREUR_CODE trame_print_hex(Trame* trame);
ERREUR_CODE trame_get_source(Trame* trame, mac** source);
ERREUR_CODE trame_get_destination(Trame* trame, mac** destination);





#endif