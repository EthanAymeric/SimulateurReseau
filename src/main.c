#include "switch/switch.h"
#include "parser/parser.h"
#include "trame/trame.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Reseau* lan = NULL;
    file_parse("config_files/config2.txt", &lan);

    size_t nbMachine = 0;
    lan_nombre_machine(lan, &nbMachine);
    printf("%zu", nbMachine);

    appareil* app1 = NULL;
    lan_get_machine(lan,0,&app1);

    Switch* sw1 = NULL;
    appareil_get_switch(app1, &sw1);

    Trame* t = trame_init(NULL,NULL,"data");
    switch_broadcast_trame(sw1,t);

    appareil* app2 = NULL;
    lan_get_machine(lan,1,&app2);

    Switch* sw2 = NULL;
    appareil_get_switch(app2, &sw2);

    switch_traiter_trame(sw1);

    lan_deinit(&lan);
}
