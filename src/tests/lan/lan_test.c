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

void lantest_ajout_machine()
{
    Reseau* lan = lan_init(0, 0);
    assert(lan != NULL);

    appareil* machine1 = appareil_init();
    appareil* machine2 = appareil_init();

    size_t nbMachines = 0;
    assert(lan_nombre_machine(lan, &nbMachines) == OK);
    assert(nbMachines == 0);

    assert(lan_ajout_machine(lan, machine1) == OK);
    assert(lan_nombre_machine(lan, &nbMachines) == OK);
    assert(nbMachines == 1);

    assert(lan_ajout_machine(lan, machine2) == OK);
    assert(lan_nombre_machine(lan, &nbMachines) == OK);
    assert(nbMachines == 2);

    appareil_deinit(&machine1);
    appareil_deinit(&machine2);
    lan_deinit(&lan);
}

void lantest_ajout_connexion()
{
    Reseau* lan = lan_init(0, 0);
    assert(lan != NULL);

    appareil* machine1 = appareil_init();
    appareil* machine2 = appareil_init();

    Lien lien;
    lien.inter1 = machine1;
    lien.inter2 = machine2;

    size_t nbConnexions = 0;
    assert(lan_nombre_connexion(lan, &nbConnexions) == OK);
    assert(nbConnexions == 0);

    assert(lan_ajout_connexion(lan, lien) == OK);

    assert(lan_nombre_connexion(lan, &nbConnexions) == OK);
    assert(nbConnexions == 1);

    appareil_deinit(&machine1);
    appareil_deinit(&machine2);
    lan_deinit(&lan);
}

void lantest_get_machines_adjacentes()
{
    Reseau* lan = lan_init(0, 0);
    assert(lan != NULL);

    appareil* machine1 = appareil_init();
    appareil* machine2 = appareil_init();
    appareil* machine3 = appareil_init();

    assert(lan_ajout_machine(lan, machine1) == OK);
    assert(lan_ajout_machine(lan, machine2) == OK);
    assert(lan_ajout_machine(lan, machine3) == OK);

    Lien lien1 = { machine1, machine2 };
    Lien lien2 = { machine1, machine3 };

    assert(lan_ajout_connexion(lan, lien1) == OK);
    assert(lan_ajout_connexion(lan, lien2) == OK);

    appareil* adjacentes[2] = { NULL, NULL };

    int nbAdj = lan_get_machines_adjacentes(lan, adjacentes, machine1);

    assert(nbAdj == 2);
    assert(
       (adjacentes[0] == machine2 || adjacentes[1] == machine2) &&
       (adjacentes[0] == machine3 || adjacentes[1] == machine3)
    );

    nbAdj = lan_get_machines_adjacentes(NULL, adjacentes, machine1);
    assert(nbAdj == -1);

    nbAdj = lan_get_machines_adjacentes(lan, NULL, machine1);
    assert(nbAdj == -1);

    nbAdj = lan_get_machines_adjacentes(lan, adjacentes, NULL);
    assert(nbAdj == -1);

    appareil_deinit(&machine1);
    appareil_deinit(&machine2);
    appareil_deinit(&machine3);
    lan_deinit(&lan);
}
