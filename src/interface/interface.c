#include "interface.h"

typedef struct Interface{
    Interface* autreAppareil;
    size_t poids;
    mac* mac;
    Trame* buffer;
} Interface;

