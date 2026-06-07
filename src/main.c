#include "mac-ip/mac.h"
#include "switch/switch.h"
#include "parser/parser.h"
#include "trame/trame.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
    mac* mac1 = mac_init();
    mac_set_string(mac1, "11:22:33:44:55:66");
    mac* mac2 = mac_init();
    mac_set_string(mac2, "66:77:88:99:AA:BB");
    Trame* trame = trame_init(mac1, mac2, "Hello, World!");
    trame_print(trame);
    trame_print_hex(trame);
    trame_deinit(trame);
}
