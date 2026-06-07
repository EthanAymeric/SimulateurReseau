#ifndef LAN_H
#define LAN_H

#include "../switch/switch.h"
#include "../station/station.h"
#include "../appareil/appareil.h"

typedef size_t Interface;
typedef struct Lien Lien;
typedef struct Reseau Reseau;

Reseau* lan_init(size_t nbMachines, size_t nbConnexion);
ERREUR_CODE lan_deinit(Reseau** lan);
ERREUR_CODE lan_nombre_machine(Reseau* lan, size_t* nbMachines);
ERREUR_CODE lan_nombre_connexion(Reseau* lan, size_t *nbConnexions);

int lan_get_machines_adjacentes(Reseau* lan, appareil** machines_adjacentes, appareil* machine);
ERREUR_CODE lan_ajout_machine(Reseau* lan, appareil* machine);
ERREUR_CODE lan_ajout_connexion(Reseau* lan, Lien lien);

#endif
