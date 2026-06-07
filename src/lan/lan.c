#include "lan.h"
#include "../error/error.h"
#include "../appareil/appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union Equipements {
    Switch* sw;
    station* st;
};

typedef struct Lien{
    union Equipements inter1;
    union Equipements inter2;
    
} Lien;

typedef struct Reseau {
    union Equipements* machine;
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
    if (r == NULL)
    {
        return NULL;
    }
    r->nbMachines = nbMachines;
    r->nbConnexion = nbConnexion;
    r->machine = malloc(sizeof(union Equipements)*nbMachines);
    if (r->machine == NULL)
    {
        free(r);
        return NULL;
    }
    r->connexion = malloc(sizeof(Lien)*nbConnexion);
    if (r->connexion == NULL)
    {
        free(r->machine);
        free(r);
        return NULL;
    }
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

size_t lan_get_machine_adjacente(Reseau* lan, union Equipements* machines_adjacentes, union Equipements machine)
{
    size_t temp = 0;
    //for (size_t i = 0; i < lan_nombre_connexion(lan); i++)
    //{
    //    if (lan->connexion[i].inter1 == machine)
    //    {
    //        machines_adjacentes[i] = lan->connexion[i].inter2;
    //        temp++;
    //    }
    //    else if (lan->connexion[i].inter2 == machine)
    //    {
    //        machines_adjacentes[i] = lan->connexion[i].inter1;
    //        temp++;
    //    }
    //}

    return temp;
}

ERREUR_CODE lan_ajout_machine(Reseau* lan, union Equipements machine)
{
    if (lan_check_pointeur_null(lan) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }

    lan->machine = realloc(lan->machine, sizeof(union Equipements) * lan->nbMachines+1);
    
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