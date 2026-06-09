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
    inter->autreAppareil = interface_init();
    inter->poids = 4;
    inter->mac = mac_init();
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

Interface* interface_init_with_parameters(Interface* other, size_t valuation, mac* macAddress)
{
    Interface* inter = malloc(sizeof(Interface));
    if (inter == NULL)
    {
        return NULL;
    }
    inter->autreAppareil = other;
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

void interface_deinit(Interface* inter)
{
    for (size_t i = 0; i < inter->tailleBuffer; i++)
    {
        free(inter->buffer[i]);
        inter->buffer[i] = NULL; 
    }
    free(inter->buffer);
    free(inter);
}

ERREUR_CODE interface_set_interface(Interface* inter, Interface* other)
{
    if (inter == NULL || other == NULL) return POINTEUR_NULL;

    inter->autreAppareil = other;
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

    interface_recieve_trame(trame, emetteur->autreAppareil);
}

