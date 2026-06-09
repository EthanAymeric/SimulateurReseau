#include "station.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct station {
    ip* ip;
    mac* mac;
    Interface* connexion;
};

ERREUR_CODE station_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : OK;
}

station* station_init(){
    station* a = NULL;
    a = malloc(sizeof(station));

    if (station_check_pointeur_null(a) != OK) return NULL;

    a->ip = ip_init();
    if (station_check_pointeur_null(a->ip) != OK){
        free(a);
        return NULL;
    }
    
    a->mac = mac_init();
    if (station_check_pointeur_null(a->mac) != OK){
        free(a->ip);
        free(a);
        return NULL;
    }

    a->connexion = interface_init();
    if (station_check_pointeur_null(a->connexion) != OK)
    {
        free(a->mac);
        free(a->ip);
        free(a);
        return NULL;
    }

    return a;
}

ERREUR_CODE station_deinit(station** station){
    ERREUR_CODE err;
    if ((err = station_check_pointeur_null(*station)) != OK){
        return err;
    }

    ip_deinit(&(*station)->ip);
    mac_deinit(&(*station)->mac);

    free(*station);
    *station = NULL;

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
    if ((err = station_check_pointeur_null(station)) != OK ||
        (err = station_check_pointeur_null(ip)) != OK ||
        (err = station_check_pointeur_null(mac)) != OK){
        return err;
    }
    station_set_ip(station, ip);
    station_set_mac(station, mac);

    return OK;
}

ERREUR_CODE station_set_ip(station* station, ip* ip){
    ERREUR_CODE err;
    if ((err = station_check_pointeur_null(station)) != OK ||
        (err = station_check_pointeur_null(ip)) != OK){
        return err;
    }

    ip_deinit(&station->ip);
    station->ip = ip;

    return OK;
}

ERREUR_CODE station_set_mac(station* station, mac* mac){
    ERREUR_CODE err;
    if ((err = station_check_pointeur_null(station)) != OK ||
        (err = station_check_pointeur_null(mac)) != OK){
        return err;
    }

    mac_deinit(&station->mac);
    station->mac = mac;

    return OK;
}

ERREUR_CODE station_get_mac(station* station, mac** mac){
    ERREUR_CODE err;
    if ((err = station_check_pointeur_null(station)) != OK ||
        (err = station_check_pointeur_null(station->mac)) != OK){
        return err;
    }

    *mac = station->mac;

    return OK;
}
