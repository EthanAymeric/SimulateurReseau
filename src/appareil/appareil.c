#include "appareil.h"
#include "../switch/switch.h"
#include "../station/station.h"

struct appareil {
    TYPE_APPAREIL type;

    union {
        station* st;
        Switch* sw;
    } appareil;
};
