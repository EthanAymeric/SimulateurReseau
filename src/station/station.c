#include "station.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct station {
    ip* ip;
    mac* mac;
    // Première case pour l'entrée, Deuxième case pour la sortie...?
    char interface[2];
};

ERREUR_CODE station_check_pointeur_null(void* ptr){
    return ptr == NULL ? POINTEUR_NULL : OK;
}

station* station_init(){
    station* a = NULL;
    a = malloc(sizeof(station));
    a->ip = ip_init();
    a->mac = mac_init();

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

ERREUR_CODE station_get_string(station* station, char* str){
    if (check_pointeur_null(station) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    sprintf(str, "%d;%d", mac_get_string(station->mac, str), ip_get_string(station->ip, str));

    return OK;
}

ERREUR_CODE station_set_station(station* station, ip ip, mac mac){
    if (check_pointeur_null(station) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    station->ip = ip;
    station->mac = mac;

    // Station en relation avec aucune autre station
    station->interface[0] = NULL;
    station->interface[1] = NULL;

    return OK;
}

ERREUR_CODE station_ajoute_relation(station* st1, station* st2){
    /*
    Fonction qui permet de relier deux stations entre elles (pas du tout sûr so c'est la bonne manière de s'y prendre)
    Corriger si besoin svp (ça vaut pour tous mes fichiers comme le dit le READ.me lol)
    */
    if (check_pointeur_null(station) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    st1->interface[0] = st2->ip;
    st2->interface[1] = st1->ip;

    return OK;
}

ERREUR_CODE station_suppr_relation_station(station* st1, station* st2){
    /*
    Fonction qui permet de suppprimer une relation entre 2 stations
    Corriger si besoin svp (ça vaut pour tous mes fichiers comme le dit le READ.me lol)
    */
    if (check_pointeur_null(station) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }

    // on vide la table de ses relations (pas du tout sûr encore une fois, help me)
    if (st1->interface[0] == st2->ip){
        st1->interface[0] = NULL;
    }

    if (st1->interface[1] == st2->ip){
        st1->interface[1] = NULL;
    }

    if (st2->interface[0] == st1->ip){
        st2->interface[0] = NULL;
    }

    if (st2->interface[1] == st1->ip){
        st2->interface[1] = NULL;
    }

    return OK;
}

ERREUR_CODE station_ajoute_relation_switch(station* st, switch* sw){
    /*
    Fonction qui permet de relier une station à un switch
    Il faudrait donc que la table de commutation du switch soit mis à jour : comment faire ?!
    Corriger si besoin svp (ça vaut pour tous mes fichiers comme le dit le READ.me lol)
    */
    if (check_pointeur_null(station) == POINTEUR_NULL){
        return POINTEUR_NULL;
    }



    return OK;
}
