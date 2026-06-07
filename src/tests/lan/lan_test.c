#include "lan_test.h"
#include "../../lan/lan.h"
#include <assert.h>
#include <stdlib.h>

void lantest_nombre_machine()
{
    Reseau* lan = NULL;
    lan = lan_init(2, 1);
    assert(lan_nombre_machine(lan) == 2);
    lan_deinit(lan);
}

void lantest_nombre_connexion()
{
    Reseau* lan = NULL;
    lan = lan_init(2, 1);
    assert(lan_nombre_connexion(lan) == 1);
    lan_deinit(lan);
}
