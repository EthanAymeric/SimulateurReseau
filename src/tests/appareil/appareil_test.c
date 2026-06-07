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
    st = station_init(ip_init(), mac_init()); // parce que set_switch deinit l'ancien station
    assert(appareil_set_station(ap, st) == OK);

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
    sw = switch_init();
    assert(appareil_set_switch(ap, sw) == OK);

    appareil_deinit(ap);
}

void appareiltest_get_type(){
    appareil* ap = NULL;
    station* st = station_init(ip_init(), mac_init());
    Switch* sw = switch_init();
    TYPE_APPAREIL t, t1;

    assert(appareil_get_type(ap, &t) == POINTEUR_NULL);

    ap = appareil_init();
    assert(appareil_get_type(ap, &t) == OK);
    assert(t == INDEFINI);

    appareil_set_station(ap, st);
    assert(appareil_get_type(ap, &t) == OK);
    assert(t == STATION);

    // prouve que le type renvoyé n'est qu'une copie de appareil->type
    t = INDEFINI;
    assert(appareil_get_type(ap, &t1) == OK);
    assert(t1 == STATION);

    appareil_set_switch(ap, sw);
    assert(appareil_get_type(ap, &t) == OK);
    assert(t == SWITCH);
    
    appareil_deinit(ap);
}

void appareiltest_get_station(){
    appareil* ap = NULL;
    station* st = station_init(ip_init(), mac_init());
    station* st1 = NULL;
    Switch* sw = switch_init();

    assert(appareil_get_station(ap, st) == POINTEUR_NULL);

    ap = appareil_init();
    appareil_set_switch(ap, sw);
    assert(appareil_get_station(ap, st1) == TYPE);

    appareil_set_station(ap, st);
    assert(appareil_get_station(ap, st1) == OK);

    // pas besoin de deinit st1 car c'est la même zone mémoire que st, qui est deinit dans appareil_deinit
    appareil_deinit(ap);
}

void appareiltest_get_switch(){
    appareil* ap = NULL;
    station* st = station_init(ip_init(), mac_init());
    Switch* sw = switch_init();
    Switch* sw1 = NULL;

    assert(appareil_get_switch(ap, sw) == POINTEUR_NULL);

    ap = appareil_init();
    appareil_set_station(ap, st);
    assert(appareil_get_switch(ap, sw1) == TYPE);

    appareil_set_switch(ap, sw);
    assert(appareil_get_switch(ap, sw1) == OK);

    // pas besoin de deinit sw1 car c'est la même zone mémoire que sw, qui est deinit dans appareil_deinit
    appareil_deinit(ap);
}
