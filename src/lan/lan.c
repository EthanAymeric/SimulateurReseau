#include "lan.h"
#include "../error/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union equipements {
    Switch* sw;
    station* st;
} Equipements;

typedef struct Lien{
    union equipements inter1;
    union equipements inter2;
    
} Lien;

typedef struct Reseau {
    union equipements* machine;
    Lien* connexion;
    size_t nbConnexion;
    size_t nbMachines;
} Reseau;

ERREUR_CODE lan_check_pointeur_null(void* ptr)
{
    return (ptr == NULL) ? POINTEUR_NULL : OK;
}

Reseau* lan_init(size_t nbMachines, size_t nbConnexion)
{
    Reseau* r = malloc(sizeof(Reseau));
    r->nbMachines = nbMachines;
    r->nbConnexion = nbConnexion;
    r->machine = malloc(sizeof(Equipements)*nbMachines);
    r->connexion = malloc(sizeof(Lien)*nbConnexion);
    return r;
}

void lan_deinit(Reseau* lan)
{
    free(lan->machine);
    free(lan->connexion);
    free(lan);
}

size_t lan_nombre_machine(Reseau* lan)
{
    return lan->nbMachines;
}

size_t lan_nombre_connexion(Reseau* lan)
{
    return lan->nbConnexion;
}

ERREUR_CODE lan_get_connexion(Reseau* lan, char* str, union equipements machine)
{
    if (lan_check_pointeur_null(lan) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }
    return OK;
}

ERREUR_CODE lan_ajout_machine(Reseau* lan, union equipements machine)
{
    if (lan_check_pointeur_null(lan) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }

    lan->machine = realloc(lan->machine, sizeof(Equipements) * lan->nbMachines+1);
    
    if (lan_check_pointeur_null(lan->machine) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }

    lan->machine[lan_nombre_machine(lan)] = machine;
    lan->nbMachines++;
    return OK;
}

ERREUR_CODE lan_ajout_connexion(Reseau* lan, Lien lien)
{
    if (lan_check_pointeur_null(lan) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }
    lan->connexion = realloc(lan->connexion, sizeof(Lien) * lan->nbConnexion+1);
    
    if (lan_check_pointeur_null(lan->connexion) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }

    lan->connexion[lan_nombre_connexion(lan)] = lien;
    lan->nbConnexion++;
    return OK;
}