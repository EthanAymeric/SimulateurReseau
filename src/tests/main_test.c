#include "mac-ip/ip_test.h"
#include "mac-ip/mac_test.h"
#include "station/station_test.h"
#include "appareil/appareil_test.h"
#include "lan/lan_test.h"
#include <stdlib.h>

int main(int argc, char** argv){
    iptest_set_adresse();
    iptest_set_octet_adresse();
    iptest_set_cidr();
    iptest_get_string();

    mactest_set_uint64();
    mactest_set_string();
    mactest_set_octets();
    mactest_set_octet();
    mactest_get_string();

    stationtest_get_string();
    stationtest_set_ip_mac();
    stationtest_set_ip();
    stationtest_set_mac();

    appareiltest_set_station();
    appareiltest_set_switch();
    appareiltest_get_type();
    appareiltest_get_station();
    appareiltest_get_switch();

    lantest_nombre_machine();
    lantest_nombre_connexion();
    return EXIT_SUCCESS;
}
