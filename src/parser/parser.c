#include "parser.h"
#include <string.h>

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


ERREUR_CODE file_lan_create(FILE* fptr, Reseau** lan)
{
    char lan_config[20];
    fgets(lan_config, 20, fptr);

    char* nbMachine_str = strtok(lan_config, " ");
    char* nbConnexion_str = strtok(NULL, " ");

    size_t nbMachine = (size_t)strtoul(nbMachine_str, NULL, 10);
    size_t nbConnexion = (size_t)strtoul(nbConnexion_str, NULL, 10);
    
    // Init lan avec le bon nombre de machine et de connexions
    *lan = lan_init(nbMachine, nbConnexion);

    return OK;
}


ERREUR_CODE file_equipements_create(FILE* fptr, size_t nbMachine, Reseau** lan)
{
    char ligne[255];
    size_t i;

    for (i = 0; i < nbMachine; i++)
    {
        fgets(ligne, 255, fptr);
        char* strToken = strtok(ligne, ";");

        if (strstr(strToken,"2"))
        {
            // Ajouter un Switch
            char* adresseMac_str = strtok(NULL, ";");
            char* nbPorts_str = strtok(NULL, ";");
            char* priority_str = strtok(NULL, ";");

            size_t nbPorts = atoi(nbPorts_str);
            uint32_t priority_num = atol(priority_str);

            // ajout du switch avec adresseMac_str, nbPorts et priority_num
            mac* macSwitch = mac_init();
            if (macSwitch == NULL) return ALLOCATION;
            mac_set_string(macSwitch, adresseMac_str);

            Switch* sw = switch_init_with_parameter(macSwitch, nbPorts, priority_num);
            if (sw == NULL) return ALLOCATION;

            appareil* ap = appareil_init();
            if (ap == NULL) return ALLOCATION;

            appareil_set_switch(ap, sw);
            lan_ajout_machine(*lan, ap);
        }

        if (strstr(strToken, "1"))
        {
            // Ajouter une Station
            char* adresseMac_str = strtok(NULL, ";");
            char* ip_str = strtok(NULL, ";");

            // Ajouter la station avec adresseMac_str et ip_str
            mac* macStation = mac_init();
            if (macStation == NULL) return ALLOCATION;
            mac_set_string(macStation, adresseMac_str);

            ip* ipStation = ip_init();
            char* octet = strtok(ip_str, ".");
            if (ipStation == NULL) return ALLOCATION;
            for (size_t i = 0; i < 4; i++){
                ip_set_octet_adresse(ipStation, atoi(octet), i);
                octet = strtok(NULL, ".");
            }
            ip_set_cidr(ipStation, 24);

            station* st = station_init();
            if (st== NULL) return ALLOCATION;
            station_set_ip_mac(st, ipStation, macStation);

            appareil* ap = appareil_init();
            if (ap == NULL) return ALLOCATION;

            appareil_set_station(ap, st);
            lan_ajout_machine(*lan, ap);
        }
    }
    return OK;
}

ERREUR_CODE file_connexions_create(FILE* fptr, size_t nbConnexion, Reseau** lan)
{
    char ligne[255];
    size_t i, nbMachines;
    appareil* current;
    mac* macCurrent;
    char macStr[100];
    lan_nombre_machine(*lan, &nbMachines);

    Lien l;

    for (i = 0; i < nbConnexion; i++)
    {
        fgets(ligne, 255, fptr);

        char* interface1_str = strtok(ligne, ";");
        char* interface2_str = strtok(NULL, ";");
        char* poids_str = strtok(NULL, ";");

        // Ajouter la connexion entre les deux interfaces
        for (size_t j = 0; j < nbMachines; j++){
            lan_get_machine(*lan, j, &current);
            appareil_get_mac(current, &macCurrent);
            mac_get_string(macCurrent, ':', macStr, 100);

            if (strcmp(macStr, interface1_str) == 0){
                l.inter1 = current;
            }
            else if (strcmp(macStr, interface2_str) == 0){
                l.inter2 = current;
            }
        }
        l.poids = atoi(poids_str);

        lan_ajout_connexion(*lan, l);
    }
    return OK;
}

ERREUR_CODE file_parse(char* path, Reseau** lan)
{
    FILE* fptr = file_init(path);
    if (fptr == NULL)
    {
        return POINTEUR_NULL;
    }
    size_t nbConnexions, nbMachines;
    file_lan_create(fptr, lan);
    ERREUR_CODE err;
    if ((err = lan_nombre_connexion(*lan, &nbConnexions) != OK) ||
    (err = lan_nombre_machine(*lan, &nbMachines) != OK)){
        return err;
    }
    
    if ((err =file_equipements_create(fptr, nbMachines, lan) != OK)) return err;
    if ((err = file_connexions_create(fptr, nbConnexions, lan) != OK)) return err;

    file_deinit(fptr);
    return OK;
}

