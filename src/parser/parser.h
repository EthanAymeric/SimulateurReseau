#ifndef PARSER_H
#define PARSER_H

#include "../switch/switch.h"
#include "../station/station.h"
#include "../lan/lan.h"
#include "../mac-ip/mac.h"
#include "../mac-ip/ip.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE* file_init(char* path);
void file_deinit(FILE* fptr);
ERREUR_CODE file_lan_create(FILE* fptr, Reseau** lan);
ERREUR_CODE file_equipements_create(FILE* fptr, size_t nbMachine, Reseau** lan);
ERREUR_CODE file_connexions_create(FILE* fptr, Reseau** lan);
ERREUR_CODE file_parse(char* path, Reseau** lan);

#endif
