#include "station.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct station {
    ip* ip;
    mac* mac;
};

ERREUR_CODE station_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : OK;
}

station* station_init(ip* ip, mac* mac){
    station* a = NULL;
    a = malloc(sizeof(station));
    a->ip = ip;
    a->mac = mac;

    return a;
}

ERREUR_CODE station_deinit(station* station){
    ERREUR_CODE err;
    if ((err = station_check_pointeur_null(station)) != OK){
        return err;
    }

    ip_deinit(station->ip);
    mac_deinit(station->mac);

    free(station);
    station = NULL;

    return OK;
}

ERREUR_CODE station_get_string(station* station, char* str, size_t taille_str){
    ERREUR_CODE err;
    if ((err = station_check_pointeur_null(station)) != OK){
        return err;
    }

    memset(str, '\0', taille_str); // équivalent à str = ""

    char ip_str[20], mac_str[20];
    if ((err = ip_get_string(station->ip, ip_str, 20)) != OK ||
        (err = mac_get_string(station->mac, ':', mac_str, 20)) != OK){
        return err;
    }

    int nb_ecrit = snprintf(str, taille_str, "%s\t%s", mac_str, ip_str);
    if (nb_ecrit < 0 || (size_t)nb_ecrit >= taille_str){
        return TAILLE_STRING;
    }

    return OK;
}

ERREUR_CODE station_set_ip_mac(station* station, ip* ip, mac* mac){
    ERREUR_CODE err;
    if ((err = station_set_ip(station, ip)) != OK ||
        (err = station_set_mac(station, mac)) != OK){
        return err;
    }

    return OK;
}

ERREUR_CODE station_set_ip(station* station, ip* ip){
    ERREUR_CODE err;
    if ((err = station_check_pointeur_null(station)) != OK ||
        (err = station_check_pointeur_null(ip)) != OK){
        return err;
    }

    station->ip = ip;

    return OK;
}

ERREUR_CODE station_set_mac(station* station, mac* mac){
    ERREUR_CODE err;
    if ((err = station_check_pointeur_null(station)) != OK ||
        (err = station_check_pointeur_null(mac)) != OK){
        return err;
    }

    station->mac = mac;

    return OK;
}
