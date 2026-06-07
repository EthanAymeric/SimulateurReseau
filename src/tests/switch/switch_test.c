#include "switch_test.h"
#include "../../switch/switch.h"
#include "../../mac-ip/mac.h"
#include <assert.h>
#include <string.h>

void switchtest_show_mac_hexa(){
    Switch* s = NULL;
    char str[1];

    assert(switch_show_mac_hexa(s, str, 1) == POINTEUR_NULL);

    s = switch_init();
    assert(switch_show_mac_hexa(s, str, 1) == TAILLE_STRING);

    char str2[40];
    assert(switch_show_mac_hexa(s, str2, 40) == OK);

    switch_deinit(&s);
    switch_deinit(&s);
}

void switchtest_show_commutation_table(){
    Switch* s = NULL;
    char str[1];

    assert(switch_show_commutation_table(s, str, 1) == POINTEUR_NULL);

    s = switch_init();
    assert(switch_show_commutation_table(s, str, 1) == TAILLE_STRING);

    char str2[1024];
    assert(switch_show_commutation_table(s, str2, 1024) == OK);

    switch_deinit(&s);
}

void switchtest_set_priority(){
    Switch* s = NULL;

    assert(switch_set_priority(s, 0) == POINTEUR_NULL);

    s = switch_init();
    assert(switch_set_priority(s, 1) == INVALID_ARGUMENT);
    assert(switch_set_priority(s, 4096) == OK);
    assert(switch_set_priority(s, 8192) == OK);

    switch_deinit(&s);
}

void switchtest_set_commutation_table(){
    Switch* s = NULL;
    mac* m = NULL;

    assert(switch_set_commutation_table(s, 0, m) == POINTEUR_NULL);

    s = switch_init();
    assert(switch_set_commutation_table(s, 0, m) == POINTEUR_NULL);

    m = mac_init();
    mac_set_string(m, "01:23:45:67:89:0A");
    assert(switch_set_commutation_table(s, 0, m) == OK);

    switch_deinit(&s);
}
