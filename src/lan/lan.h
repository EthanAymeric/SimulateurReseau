#ifndef LAN_H
#define LAN_H

#include "../switch/switch.h"
#include "../station/station.h"
#include "../appareil/appareil.h"

typedef struct Reseau Reseau;

Reseau* lan_init(size_t nbMachines);
ERREUR_CODE lan_deinit(Reseau** lan);
ERREUR_CODE lan_nombre_machine(Reseau* lan, size_t* nbMachines);
ERREUR_CODE lan_get_machine(Reseau* lan, size_t index, appareil** ap);
ERREUR_CODE lan_ajout_machine(Reseau* lan, appareil* machine);

#endif
