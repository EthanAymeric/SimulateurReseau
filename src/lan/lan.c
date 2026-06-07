#include "lan.h"
#include "../error/error.h"
#include "../appareil/appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Lien{
    appareil* inter1;
    appareil* inter2;
    
};

typedef struct Reseau {
    appareil** machines;
    Lien* connexions;
    size_t nbConnexions;
    size_t nbMachines;
} Reseau;

ERREUR_CODE lan_check_pointeur_null(void* ptr)
{
    return (ptr == NULL) ? POINTEUR_NULL : OK;
}

Reseau* lan_init(size_t nbMachines, size_t nbConnexions)
{
    Reseau* r = malloc(sizeof(Reseau));
    if (r == NULL) return NULL;

    r->nbMachines = nbMachines;
    r->nbConnexions = nbConnexions;
    r->machines = malloc(sizeof(appareil*) * nbMachines);
    if (r->machines == NULL)
    {
        free(r);
        return NULL;
    }

    r->connexions = malloc(sizeof(Lien) * nbConnexions);
    if (r->connexions == NULL)
    {
        free(r->machines);
        free(r);
        return NULL;
    }
    return r;
}

ERREUR_CODE lan_deinit(Reseau** lan)
{
    if (lan_check_pointeur_null(*lan) != OK){
        return POINTEUR_NULL;
    }

    free((*lan)->machines);
    free((*lan)->connexions);
    
    free(*lan);
    *lan = NULL;

    return OK;
}

size_t lan_nombre_machine(Reseau* lan)
{
    return lan->nbMachines;
}

size_t lan_nombre_connexion(Reseau* lan)
{
    return lan->nbConnexions;
}

size_t lan_get_machines_adjacentes(Reseau* lan, appareil** machines_adjacentes, appareil* machine)
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

ERREUR_CODE lan_ajout_machine(Reseau* lan, appareil* machine)
{
    if (lan_check_pointeur_null(lan) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }

    lan->machines = realloc(lan->machines, sizeof(appareil*) * lan->nbMachines+1);
    
    if (lan_check_pointeur_null(lan->machines) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }

    lan->machines[lan_nombre_machine(lan)] = machine;
    lan->nbMachines++;
    return OK;
}

ERREUR_CODE lan_ajout_connexion(Reseau* lan, Lien lien)
{
    if (lan_check_pointeur_null(lan) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }
    lan->connexions = realloc(lan->connexions, sizeof(Lien) * lan->nbConnexions + 1);
    
    if (lan_check_pointeur_null(lan->connexions) == POINTEUR_NULL)
    {
        return POINTEUR_NULL;
    }

    lan->connexions[lan_nombre_connexion(lan)] = lien;
    lan->nbConnexions++;
    return OK;
}
