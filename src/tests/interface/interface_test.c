#include "interface_test.h"


void interface_broadcast()
{

    Switch* sw1 = switch_init();
    Switch* sw2 = switch_init();
    Switch* sw3 = switch_init();
    Interface* inter1 = interface_init();
    Interface* inter4 = interface_init();
    Interface* inter2 = interface_init();
    Interface* inter3 = interface_init();

    interface_set_interface(inter1, &inter2);
    interface_set_interface(inter2, &inter1);

    interface_set_interface(inter3, &inter4);
    interface_set_interface(inter4, &inter3);


    switch_set_interface(sw1,inter1);
    switch_set_interface(sw1, inter4);
    switch_set_interface(sw2, inter2);
    switch_set_interface(sw3, inter3);
    
    Trame* t = trame_init(NULL,NULL,"data");
    assert(switch_broadcast_trame(sw1,t) == OK);


    interface_traite_trame(inter2);
    interface_traite_trame(inter3);
    trame_deinit(&t);
    switch_deinit(&sw1);
    switch_deinit(&sw2);
    switch_deinit(&sw3);
}