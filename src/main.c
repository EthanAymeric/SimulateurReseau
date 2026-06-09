#include "mac-ip/mac.h"
#include "switch/switch.h"
#include "parser/parser.h"
#include "trame/trame.h"
#include "interface/interface.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Interface* inter1 = interface_init();
    Interface* inter2 = interface_init();

    interface_set_interface(inter1, &inter2);
    interface_set_interface(inter2, &inter1);
    interface_send_trame(trame_init(NULL,NULL,"data"), inter1);

    interface_traite_trame(inter2);

    interface_deinit(inter1);
    interface_deinit(inter2);
}
