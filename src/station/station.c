#include station.h
#include ip.c
#include mac.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct station {
    ip ip;
    mac mac;
    // Première case pour l'entrée, Deuxième case pour la sortie...?
    char interface[2];
};

ERREUR_STATION station_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : NULLE;
}

station* station_init(){
    station* a = NULL;
    a = malloc(sizeof(station));

    return a;
}

ERREUR_STATION station_deinit(station* station){
    if (check_pointeur_null(station) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    free(station);
    station = NULL;

    return NULLE;
}

ERREUR_STATION station_get_string(station* station, char* str){
    if (check_pointeur_null(station) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    sprintf(str, "%d;%d", mac_get_string(station->mac, str), ip_get_string(station->ip, str));

    return NULLE;
}

ERREUR_STATION station_set_station(station* station, ip ip, mac mac){
    if (check_pointeur_null(station) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    station->ip = ip;
    station->mac = mac;

    // Station en relation avec aucune autre station
    station->interface[0] = NULL;
    station->interface[1] = NULL;

    return NULLE;
}

ERREUR_STATION station_add_relation(station* st1, station* st2){
    /*
    Fonction qui permet de relier deux stations entre elles (pas du tout sûr so c'est la bonne manière de s'y prendre)
    Corriger si besoin
    */
}