#include "mac-ip/mac.h"
#include "switch/switch.h"
#include "parser/parser.h"

#include <stdlib.h>
#include <stdio.h>

int main() {
    FILE* fptr = file_init("config_files/config1.txt");
    file_lan_create(fptr);
    file_equipements_create(fptr, 3);
    file_connexions_create(fptr, 2);
    file_deinit(fptr);
}
