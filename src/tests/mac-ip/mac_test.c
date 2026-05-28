#include "mac_test.h"
#include "../../mac-ip/mac.h"
#include <assert.h>
#include <string.h>

void mactest_set_uint64(){
    mac* a = NULL;

    assert(mac_set_uint64(a, 0) == POINTEUR_NULL);

    a = mac_init();
    assert(mac_set_uint64(a, -1) == VALEUR);
    assert(mac_set_uint64(a, 0x1000000000000) == VALEUR);
    assert(mac_set_uint64(a, 0xffff) == OK);

    mac_deinit(a);
}

void mactest_set_string(){
    mac* a = NULL;

    assert(mac_set_string(a, "01:23:45:67:89:AB") == POINTEUR_NULL);
    
    a = mac_init();
    assert(mac_set_string(a, "00:00:00") == FORMAT_STRING);
    assert(mac_set_string(a, "Hello world !") == FORMAT_STRING);
    assert(mac_set_string(a, "Hello01:23:45:67:89:AB") == FORMAT_STRING);
    assert(mac_set_string(a, "01:23:45:67:89:ABworld") == FORMAT_STRING);
    assert(mac_set_string(a, "01:23:45:67:89:AB:CD") == FORMAT_STRING);
    assert(mac_set_string(a, "01:23:45:67:89:AB") == OK);
    assert(mac_set_string(a, "01-23-45-67-89-AB") == OK);
}

void mactest_set_octets(){
    mac* a = NULL;
    
    assert(mac_set_octets(a, 0, 0, 0, 0, 0, 0) == POINTEUR_NULL);

    a = mac_init();
    assert(mac_set_octets(a, -1, 0, 0, 0, 0, 0) == VALEUR_OCTET);
    assert(mac_set_octets(a, 0, 0, 0, 256, 0, 0) == VALEUR_OCTET);
    assert(mac_set_octets(a, 0, 0, 0, 255, 0, 0) == OK);

    mac_deinit(a);
}

void mactest_get_string(){
    mac* a = NULL;
    char str[1], str2[17];

    assert(mac_get_string(a, ':', str, 1) == POINTEUR_NULL);

    a = mac_init();
    mac_set_string(a, "01:23:45:67:89:AB");
    assert(mac_get_string(a, ':', str, 1) == TAILLE_STRING);
    assert(mac_get_string(a, ':', str2, 40) == OK);
    assert(strcmp(str2, "01:23:45:67:89:AB") == 0);
}
