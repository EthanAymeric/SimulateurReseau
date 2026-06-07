#include "parser.h"


FILE* file_init(char* path)
{
    FILE* fptr;
    fptr = fopen(path, "r");
    if (fptr == NULL)
    {
        return NULL;
    }

    return fptr;
}

void file_deinit(FILE* fptr)
{
    fclose(fptr);
}


ERREUR_CODE file_lan_create(FILE* fptr)
{
    char lan_config[20];
    fgets(lan_config, 20, fptr);
    size_t nbMachine = (size_t)strtok(lan_config, " ");
    size_t nbConnexion = (size_t)strtok(lan_config, " ");

    // Init lan avec le bon nombre de machine et de connexions

    return OK;
}


ERREUR_CODE file_equipements_create(FILE* fptr)
{
    char ligne[255];
    while(fgets(ligne,255,fptr))
    {
        char* strToken = strtok(ligne, ";");
        if (strstr(strToken,"2"))
        {
            // Ajouter un Switch
            mac* adresseMac;
            mac_set_string(adresseMac,strtok(NULL, ";"));
            size_t nbPorts = (size_t)strtok(NULL, ";");
            uint32_t priority = (uint32_t)strtok(NULL, ";");
        }

        if (strstr(strToken, "1"))
        {
            // Ajouter une Station

            mac* adresseMac;
            mac_set_string(adresseMac,strtok(NULL, ";"));
            uint8_t ip = (uint8_t)strtok(NULL, ";");
        }
    }

    return OK;
}

