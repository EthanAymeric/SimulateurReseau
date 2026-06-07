#ifndef LAN_H
#define LAN_H

#include "../switch/switch.h"
#include "../station/station.h"

typedef size_t Interface;

union Equipements;

typedef struct Lien Lien;

typedef struct Reseau Reseau;

ERREUR_CODE lan_check_pointeur_null(void* ptr);
Reseau* lan_init(size_t nbMachines, size_t nbConnexion);
void lan_deinit(Reseau* lan);
size_t lan_nombre_machine(Reseau* lan);
size_t lan_nombre_connexion(Reseau* lan);

size_t lan_get_machine_adjacente(Reseau* lan, union Equipements* machines_adjacentes, union Equipements machine);
ERREUR_CODE lan_ajout_machine(Reseau* lan, union Equipements machine);
ERREUR_CODE lan_ajout_connexion(Reseau* lan, Lien lien);

#endif
