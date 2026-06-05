#include "station_test.h"
#include "../../station/station.h"
#include <assert.h>
#include <string.h>


void stationtest_set_ip(){
    station* s = NULL;
    ip* a = ip_init();
    ip_set_adresse(a, 192, 168, 1, 0);
    ip_set_cidr(a, 24);

    assert(station_set_ip(s, a) == POINTEUR_NULL);
    assert(station_set_ip(s, NULL) == POINTEUR_NULL);

    s = station_init(a, mac_init());
    assert(station_set_ip(s, a) == OK);

    station_deinit(s);
}

void stationtest_set_mac(){
    station* s = NULL;
    mac* a = mac_init();
    mac_set_string(a, "01:23:45:67:89:AB");

    assert(station_set_mac(s, a) == POINTEUR_NULL);
    assert(station_set_mac(s, NULL) == POINTEUR_NULL);

    s = station_init(ip_init(), a);
    assert(station_set_mac(s, a) == OK);

    station_deinit(s);
}

void stationtest_set_ip_mac(){
    station* s = NULL;
    ip* a = ip_init();
    mac* b = mac_init();

    assert(station_set_ip_mac(s, a, b) == POINTEUR_NULL);

    s = station_init(a, b);
    assert(station_set_ip_mac(s, NULL, NULL) == POINTEUR_NULL);
    assert(station_set_ip_mac(s, NULL, b) == POINTEUR_NULL);
    assert(station_set_ip_mac(s, a, NULL) == POINTEUR_NULL);

    assert(station_set_ip_mac(s, a, b) == OK);

    station_deinit(s);
}

void stationtest_get_string(){
    station* s = NULL;
    ip* a = ip_init();
    ip_set_adresse(a, 192, 168, 1, 0);
    ip_set_cidr(a, 24);

    mac* b = mac_init();
    mac_set_string(b, "01:23:45:67:89:AB");

    char str[1], str2[40];

    assert(station_get_string(s, str, 1) == POINTEUR_NULL);

    s = station_init(a, b);
    assert(station_get_string(s, str, 1) == TAILLE_STRING);

    station_set_ip_mac(s, a, b);
    assert(station_get_string(s, str2, 40) == OK);
    assert(strcmp(str2, "01:23:45:67:89:AB\t192.168.1.0/24") == 0);

    station_deinit(s);
}
