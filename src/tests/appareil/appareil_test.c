#include "appareil_test.h"
#include "../../appareil/appareil.h"
#include <assert.h>

void appareiltest_set_station(){
    appareil* ap = NULL;
    station* st = NULL;
    Switch* sw = switch_init();

    assert(appareil_set_station(ap, st) == POINTEUR_NULL);

    ap = appareil_init();
    assert(appareil_set_station(ap, st) == POINTEUR_NULL);

    st = station_init(ip_init(), mac_init());
    assert(appareil_set_station(ap, st) == OK);

    appareil_set_switch(ap, sw);
    assert(appareil_set_station(ap, st) == OK);
    assert(appareil_set_station(ap, st) == OK);

    switch_deinit(sw);
    appareil_deinit(ap);
}

void appareiltest_set_switch(){
    appareil* ap = NULL;
    Switch* sw = NULL;
    station* st = station_init(ip_init(), mac_init());

    assert(appareil_set_switch(ap, sw) == POINTEUR_NULL);

    ap = appareil_init();
    assert(appareil_set_switch(ap, sw) == POINTEUR_NULL);

    sw = switch_init();
    assert(appareil_set_switch(ap, sw) == OK);

    appareil_set_station(ap, st);
    assert(appareil_set_switch(ap, sw) == OK);
    assert(appareil_set_switch(ap, sw) == OK);

    station_deinit(st);
    appareil_deinit(ap);
}

void appareiltest_get_type(){}
void appareiltest_get_station(){}
void appareiltest_get_switch(){}
