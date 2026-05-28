#include "mac-ip/ip_test.h"
#include "mac-ip/mac_test.h"
#include <stdlib.h>

int main(int argc, char** argv){
    iptest_set_adresse();
    iptest_set_octet_adresse();
    iptest_set_cidr();
    iptest_get_string();

    mactest_set_uint64();
    mactest_set_string();
    mactest_set_octets();
    mactest_get_string();

    return EXIT_SUCCESS;
}
