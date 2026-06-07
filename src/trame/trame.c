#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trame.h"

Trame* trame_init(mac* adresse_source, mac* adresse_destination, char* data)
{
    Trame* trame = malloc(sizeof(Trame));
    if (trame == NULL)
    {
        return NULL;
    }

    trame->adresse_source = adresse_source;
    trame->adresse_destination = adresse_destination;
    trame->data = data;
    trame->type[0] = 0x08; // Type Ethernet II
    trame->type[1] = 0x00; // Type IPv4
    trame->SFD = 0xAB; // Start Frame Delimiter
    memset(trame->preambule, 0x55, sizeof(trame->preambule)); // Preambule
    memset(trame->FCS, 0x00, sizeof(trame->FCS)); // Frame Check Sequence (FCS)

    return trame;
}

void trame_deinit(Trame* trame)
{
    if (trame != NULL)
    {
        free(trame);
    }
}

void trame_print(Trame* trame)
{
    if (trame != NULL)
    {
        char source[18];
        char destination[18];
        mac_get_string(trame->adresse_source, ':', source, sizeof(source));
        mac_get_string(trame->adresse_destination, ':', destination, sizeof(destination));
        printf("Adresse source: %s\n", source);
        printf("Adresse destination: %s\n", destination);
        printf("Data: %s\n", trame->data);
    }
}

void trame_print_hex(Trame* trame)
{
    if (trame != NULL)
    {
        for (size_t i = 0; i < sizeof(trame->preambule); i++)
        {
            printf("%02X ", trame->preambule[i]);
        }
        printf("%02X ", trame->SFD);
        printf("%02X %02X ", trame->type[0], trame->type[1]);
        for (size_t i = 0; i < strlen(trame->data); i++)
        {
            printf("%02X ", (unsigned char)trame->data[i]);
        }
        for (size_t i = 0; i < sizeof(trame->FCS); i++)
        {
            printf("%02X ", trame->FCS[i]);
        }
        printf("\n");
    }
}