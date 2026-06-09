#include "mac-ip/mac.h"
#include "switch/switch.h"
#include "parser/parser.h"
#include "trame/trame.h"
#include "interface/interface.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    Reseau* lan = NULL;
    file_parse("config_files/config2.txt", &lan);

    size_t nbMachine = 0;
    lan_nombre_machine(lan, &nbMachine);
    printf("%zu", nbMachine);
    
}
