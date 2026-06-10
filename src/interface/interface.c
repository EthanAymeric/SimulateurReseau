#include "interface.h"

typedef struct Interface{
    Interface* autreAppareil;
    size_t poids;
    mac* mac;
    Trame** buffer;
    size_t tailleBuffer;
    size_t nbElementBuffer;
} Interface;

Interface* interface_init()
{
    Interface* inter = malloc(sizeof(Interface));
    if (inter == NULL)
    {
        return NULL;
    }
    inter->autreAppareil = NULL;
    inter->poids = 4;
    inter->mac = NULL;
    inter->buffer = malloc(sizeof(Trame*) * 8);
    if (inter->buffer == NULL)
    {
        free(inter);
        inter = NULL;
        return NULL;
    }
    inter->tailleBuffer = 8;
    inter->nbElementBuffer = 0;
    return inter;
}

Interface* interface_init_with_parameters(size_t valuation, mac* macAddress)
{
    Interface* inter = malloc(sizeof(Interface));
    if (inter == NULL)
    {
        return NULL;
    }
    inter->autreAppareil = NULL;
    inter->poids = valuation;
    inter->mac = macAddress;
    inter->buffer = malloc(sizeof(Trame*) * 8);
    if (inter->buffer == NULL)
    {
        free(inter);
        inter = NULL;
        return NULL;
    }
    inter->tailleBuffer = 8;
    inter->nbElementBuffer = 0;
    return inter;
}

void interface_deinit(Interface** inter)
{
    free((*inter)->buffer);
    free((*inter));
    *inter = NULL;
}

ERREUR_CODE interface_set_interface(Interface* inter, Interface* other)
{
    if (inter == NULL || other == NULL) return POINTEUR_NULL;

    inter->autreAppareil = other;
    return OK;
}



ERREUR_CODE interface_recieve_trame(Trame* trame, Interface* recepteur)
{
    if (trame == NULL || recepteur == NULL) return POINTEUR_NULL;

    if (recepteur->tailleBuffer == recepteur->nbElementBuffer)
    {
        recepteur->tailleBuffer *= 2;
        recepteur->buffer = realloc(recepteur->buffer, sizeof(Trame*) * recepteur->tailleBuffer);
    }
    recepteur->buffer[recepteur->nbElementBuffer] = trame;
    recepteur->nbElementBuffer++;

    return OK;
}

ERREUR_CODE interface_send_trame(Trame* trame, Interface* emetteur){
    if(trame == NULL || emetteur == NULL) return POINTEUR_NULL;
    if(emetteur->autreAppareil == NULL) return POINTEUR_NULL;

    interface_recieve_trame(trame, emetteur->autreAppareil);
    return OK;
}

ERREUR_CODE interface_traite_trame(Interface* inter)
{
    for (size_t i = 0; i < inter->nbElementBuffer; i++)
    {
        trame_print(inter->buffer[i]);
    }
    return OK;
}

ERREUR_CODE interface_get_buffer_size(Interface* inter, size_t* size)
{
    if (inter == NULL) return POINTEUR_NULL;

    *size = inter->nbElementBuffer;
    return OK;
}

ERREUR_CODE interface_get_trame(Interface* inter, size_t index, Trame** trame)
{
    if (inter == NULL || trame == NULL) return POINTEUR_NULL;
    if (index >= inter->nbElementBuffer) return INVALID_ARGUMENT;

    *trame = inter->buffer[index];
    return OK;
}

ERREUR_CODE interface_vider_buffer(Interface* inter)
{
    if (inter == NULL) return POINTEUR_NULL;

    inter->nbElementBuffer = 0;
    return OK;
}

ERREUR_CODE interface_get_autre_appareil(Interface* inter, Interface** autre)
{
    if (inter == NULL || autre == NULL) return POINTEUR_NULL;
    *autre = inter->autreAppareil;
    return OK;
}

ERREUR_CODE interface_get_poids(Interface* inter, size_t* poids)
{
    if (inter == NULL || poids == NULL) return POINTEUR_NULL;
    *poids = inter->poids;
    return OK;
}