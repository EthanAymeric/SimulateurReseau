#include "ip_test.h"
#include "../../mac-ip/ip.h"
#include <assert.h>

void iptest_set_adresse(){
    ip* a = NULL;

    assert(ip_set_adresse(a, 0, 0, 0, 0) == POINTEUR_NULL);
    
    a = ip_init();
    assert(ip_set_adresse(a, -1, 0, 0, 0) == VALEUR_OCTET);
    assert(ip_set_adresse(a, 0, 256, 0, 0) == VALEUR_OCTET);
    assert(ip_set_adresse(a, 192, 168, 1, 1) == OK);

    ip_deinit(a);
}

void iptest_set_octet_adresse(){
    ip* a = NULL;

    assert(ip_set_octet_adresse(a, 0, 0) == POINTEUR_NULL);

    a = ip_init();
    assert(ip_set_octet_adresse(a, -1, 0) == VALEUR_OCTET);
    assert(ip_set_octet_adresse(a, 256, 0) == VALEUR_OCTET);
    assert(ip_set_octet_adresse(a, 1, -1) == INDEX_OCTET);
    assert(ip_set_octet_adresse(a, 1, 4) == INDEX_OCTET);
    assert(ip_set_octet_adresse(a, 192, 0) == OK);

    ip_deinit(a);
}


void iptest_set_cidr(){
    ip* a = NULL;

    assert(ip_set_cidr(a, 0) == POINTEUR_NULL);
    
    a = ip_init();
    assert(ip_set_cidr(a, -1) == VALEUR_CIDR);
    assert(ip_set_cidr(a, 33) == VALEUR_CIDR);
    assert(ip_set_cidr(a, 24) == OK);

    ip_deinit(a);
}
