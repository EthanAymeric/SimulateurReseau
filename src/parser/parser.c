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

    size_t nbMachine = (size_t)strtoul(nbMachine_str, NULL, 10);

    *lan = lan_init(nbMachine);

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

ERREUR_CODE file_connexions_create(FILE* fptr, Reseau** lan)
{
    char ligne[255];

    while(fgets(ligne, 255, fptr))
    {
        char* interface1_str = strtok(ligne, ";");
        char* interface2_str = strtok(NULL, ";");
        char* poids_str = strtok(NULL, ";");       

        size_t indexInter1 = atoi(interface1_str);
        size_t indexInter2 = atoi(interface2_str);
        size_t poids = atoi(poids_str);

        appareil* app1 = NULL;
        appareil* app2 = NULL;
        lan_get_machine(*lan, indexInter1,&app1);
        lan_get_machine(*lan, indexInter2,&app2);
        
        mac* mac1 = NULL;
        mac* mac2 = NULL;
        appareil_get_mac(app1, &mac1);
        appareil_get_mac(app2, &mac2);

        Interface* inter1 = interface_init_with_parameters(poids, mac1);
        Interface* inter2 = interface_init_with_parameters(poids, mac2);

        TYPE_APPAREIL type1;
        TYPE_APPAREIL type2;
        appareil_get_type(app1, &type1);
        appareil_get_type(app2, &type2);

        if (type1 == SWITCH)
        {
            Switch* sw1 = NULL;
            appareil_get_switch(app1, &sw1);
            if (switch_set_interface(sw1, inter1) != OK)
            {
                fprintf(stderr, "Erreur config : le switch %zu n'a pas assez de ports "
                        "declares pour accueillir toutes ses connexions.\n", indexInter1);
                interface_deinit(&inter1);
                interface_deinit(&inter2);
                return INVALID_ARGUMENT;
            }
        }
        else if (type1 == STATION)
        {
            station* st1 = NULL;
            appareil_get_station(app1, &st1);
            station_set_connexion(st1, inter1);
        }

        if (type2 == SWITCH)
        {
            Switch* sw2 = NULL;
            appareil_get_switch(app2, &sw2);
            if (switch_set_interface(sw2, inter2) != OK)
            {
                fprintf(stderr, "Erreur config : le switch %zu n'a pas assez de ports "
                        "declares pour accueillir toutes ses connexions.\n", indexInter2);
                interface_deinit(&inter2);
                return INVALID_ARGUMENT;
            }
        }
        else if (type2 == STATION)
        {
            station* st2 = NULL;
            appareil_get_station(app2, &st2);
            station_set_connexion(st2, inter2);
        }

        interface_set_interface(inter1, inter2);
        interface_set_interface(inter2, inter1);
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
    ERREUR_CODE err;
    if ((err = file_lan_create(fptr, lan)) != OK) return err;

    size_t nbMachines;
    if ((err = lan_nombre_machine(*lan, &nbMachines)) != OK) return err;

    if ((err = file_equipements_create(fptr, nbMachines, lan)) != OK) return err;
    if ((err = file_connexions_create(fptr, lan)) != OK)
    {
        file_deinit(fptr);
        lan_deinit(lan);
        return err;
    }

    file_deinit(fptr);
    return OK;
}

