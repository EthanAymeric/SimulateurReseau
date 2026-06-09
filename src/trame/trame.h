#ifndef TRAME_H
#define TRAME_H

#include "../mac-ip/mac.h"

typedef struct Trame Trame;

Trame* trame_init(mac* adresse_source, mac* adresse_destination, char* data);
void trame_deinit(Trame** trame);
void trame_print(Trame* trame);
void trame_print_hex(Trame* trame);






#endif