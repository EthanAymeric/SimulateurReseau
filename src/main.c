#include "mac-ip/mac.h"
#include "switch/switch.h"

#include <stdlib.h>
#include <stdio.h>

int main() {
    mac* mac = mac_init();
    mac_set_uint64(mac, 255766639995918);
    Switch* s = switch_init_with_parameter(mac, 8, 4096);
    switch_set_commutation_table(s, 5, mac);
    char* str = malloc(sizeof(char) * 255);
    switch_show_commutation_table(s,str,19);
    printf("%s\n", str);

    switch_deinit(s);
    free(str);
}
