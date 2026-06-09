#ifndef INTERFACE_H
#define INTERFACE_H

#include "../mac-ip/mac.h"
#include "../trame/trame.h"
#include "../error/error.h"

#include <stdlib.h>

typedef struct Interface Interface;

ERREUR_CODE interface_send_trame(Trame* trame,Interface* emetteur);
ERREUR_CODE interface_recieve_trame(Trame* trame, Interface* recepteur);
Interface* interface_init();
Interface* interface_init_with_parameters(Interface** other, size_t valuation, mac* macAddress);
void interface_deinit(Interface* inter);
ERREUR_CODE interface_set_interface(Interface* inter, Interface** other);
ERREUR_CODE interface_traite_trame(Interface* inter);


#endif