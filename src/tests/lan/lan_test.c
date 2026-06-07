#include "lan_test.h"
#include "../../lan/lan.h"
#include <assert.h>
#include <stdlib.h>

void lantest_nombre_machine()
{
    Reseau* lan = NULL;
    size_t nb;
    assert(lan_nombre_machine(lan, &nb) == POINTEUR_NULL);

    lan = lan_init(2, 1);
    assert(lan_nombre_machine(lan, &nb) == OK);
    assert(nb == 2);

    lan_deinit(&lan);
    lan_deinit(&lan);
}

void lantest_nombre_connexion()
{
    Reseau* lan = NULL;
    size_t nb;
    assert(lan_nombre_connexion(lan, &nb) == POINTEUR_NULL);

    lan = lan_init(2, 1);
    assert(lan_nombre_connexion(lan, &nb) == OK);
    assert(nb == 1);

    lan_deinit(&lan);
}
