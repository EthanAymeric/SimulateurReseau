#include "../../trame/trame.h"

#include "trame_test.h"

void print_trame_test()
{
    mac* mac1 = mac_init();
    mac_set_string(mac1, "11:22:33:44:55:66");
    mac* mac2 = mac_init();
    mac_set_string(mac2, "66:77:88:99:AA:BB");
    Trame* trame = trame_init(mac1, mac2, "Hello, World!");
    trame_print(trame);
    trame_deinit(trame);
    mac_deinit(&mac1);
    mac_deinit(&mac2);
}

void print_trame_hex_test()
{
    mac* mac1 = mac_init();
    mac_set_string(mac1, "11:22:33:44:55:66");
    mac* mac2 = mac_init();
    mac_set_string(mac2, "66:77:88:99:AA:BB");
    Trame* trame = trame_init(mac1, mac2, "Hello, World!");
    trame_print_hex(trame);
    trame_deinit(trame);
    mac_deinit(&mac1);
    mac_deinit(&mac2);
}