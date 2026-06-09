#ifndef INTERFACE_H
#define INTERFACE_H

#include "../mac-ip/mac.h"
#include "../trame/trame.h"
#include "../error/error.h"

#include <stdlib.h>

typedef struct Interface Interface;

ERREUR_CODE send_trame(Trame* trame,Interface* emetteur);

#endif