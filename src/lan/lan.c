#include "lan.h"
#include "../error/error.h"
#include "../appareil/appareil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Reseau {
    appareil** machines;
    size_t nbMachines;
    size_t tailleTabMachines;
} Reseau;

ERREUR_CODE lan_check_pointeur_null(void* ptr)
{
    return (ptr == NULL) ? POINTEUR_NULL : OK;
}

ERREUR_CODE lan_check_index_machine(Reseau* lan, size_t index){
    return (index < lan->nbMachines) ? OK : INVALID_ARGUMENT;
}

Reseau* lan_init(size_t nbMachines)
{
    Reseau* r = malloc(sizeof(Reseau));
    if (r == NULL) return NULL;

    r->nbMachines = 0;
    r->tailleTabMachines = nbMachines;
    r->machines = malloc(sizeof(appareil*) * nbMachines);
    if (r->machines == NULL)
    {
        free(r);
        return NULL;
    }

    return r;
}

ERREUR_CODE lan_deinit(Reseau** lan)
{
    ERREUR_CODE err;
    if ((err = lan_check_pointeur_null(*lan)) != OK){
        return err;
    }

    for (size_t i = 0; i < (*lan)->nbMachines; i++)
    {
       appareil_deinit(&(*lan)->machines[i]);
    }
    free((*lan)->machines);
    
    free(*lan);
    *lan = NULL;

    return OK;
}

ERREUR_CODE lan_nombre_machine(Reseau* lan, size_t* nbMachines)
{
    ERREUR_CODE err;
    if ((err = lan_check_pointeur_null(lan)) != OK){
        return err;
    }

    *nbMachines = lan->tailleTabMachines;

    return OK;
}


ERREUR_CODE lan_ajout_machine(Reseau* lan, appareil* machine)
{
    ERREUR_CODE err;
    if ((err = lan_check_pointeur_null(lan)) != OK)
    {
        return err;
    }
    if ((err = lan_check_pointeur_null(lan->machines)) != OK)
    {
        return err;
    }


    if (lan->nbMachines == lan->tailleTabMachines)
    {
        lan->tailleTabMachines *= 2;
        lan->machines = realloc(lan->machines, sizeof(appareil*) * lan->tailleTabMachines);
    }
    lan->machines[lan->nbMachines] = machine;
    lan->nbMachines++;
    return OK;
}


ERREUR_CODE lan_get_machine(Reseau* lan, size_t index, appareil** ap){
    ERREUR_CODE err;
    if ((err = lan_check_pointeur_null(lan)) != OK ||
        (err = lan_check_index_machine(lan, index)) != OK){
        return err;
    }

    *ap = lan->machines[index];

    return OK;
}
