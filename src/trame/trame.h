#ifndef TRAME_H
#define TRAME_H

#include "../mac-ip/mac.h"

typedef struct Trame
{
    mac* adresse_source;
    mac* adresse_destination;
    char* data;

    uint8_t type[2];
    uint8_t SFD;
    uint8_t preambule[7];
    uint8_t FCS[4];
} Trame;

Trame* trame_init(mac* adresse_source, mac* adresse_destination, char* data);
void trame_deinit(Trame* trame);
void trame_print(Trame* trame);
void trame_print_hex(Trame* trame);






#endif