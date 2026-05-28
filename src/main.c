#include "switch/switch.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {

    Switch* s = switch_init_with_parameter(255771439995918, 8, 4096);
    switch_set_commutation_table(s,5,255766639995918);
    char* str = malloc(sizeof(char) * 255);
    switch_show_commutation_table(s,str);
    printf("%s\n", str);
    switch_deinit(s);
    free(str);
}