#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trame.h"

typedef struct Trame
{
    mac* adresse_source;
    mac* adresse_destination;
    char* data;

    uint8_t type[2];
    uint8_t SFD;
    uint8_t preambule[7];
    uint8_t FCS[4];
} Trame;

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

void trame_deinit(Trame** trame)
{
    free(*trame);
    *trame = NULL;
}

ERREUR_CODE trame_print(Trame* trame)
{
    if (trame == NULL) return POINTEUR_NULL;

    if (trame->adresse_source != NULL) {
        char source[18] = "";
        mac_get_string(trame->adresse_source, ':', source, sizeof(source));
        printf("Adresse source: %s\n", source);
    } else {
        printf("Adresse source: (null)\n");
    }

    if (trame->adresse_destination != NULL) {
        char destination[18] = "";
        mac_get_string(trame->adresse_destination, ':', destination, sizeof(destination));
        printf("Adresse destination: %s\n", destination);
    } else {
        printf("Adresse destination: (null)\n");
    }

    printf("Data: %s\n", trame->data != NULL ? trame->data : "(null)");

    return OK;
}

ERREUR_CODE trame_print_hex(Trame* trame)
{
    if (trame == NULL) return POINTEUR_NULL;

    // Préambule (7 octets)
    for (size_t i = 0; i < sizeof(trame->preambule); i++)
        printf("%02X ", trame->preambule[i]);
    printf("| ");

    // SFD (1 octet)
    printf("%02X | ", trame->SFD);

    // @destination (6 octets)
    if (trame->adresse_destination != NULL) {
        char dst[18] = "";
        mac_get_string(trame->adresse_destination, ' ', dst, sizeof(dst));
        printf("%s | ", dst);
    }

    // @source (6 octets)
    if (trame->adresse_source != NULL) {
        char src[18] = "";
        mac_get_string(trame->adresse_source, ' ', src, sizeof(src));
        printf("%s | ", src);
    }

    // Type (2 octets)
    printf("%02X %02X | ", trame->type[0], trame->type[1]);

    // Données
    if (trame->data != NULL) {
        for (size_t i = 0; i < strlen(trame->data); i++)
            printf("%02X ", (unsigned char)trame->data[i]);
    }
    printf("| ");

    // FCS (4 octets)
    for (size_t i = 0; i < sizeof(trame->FCS); i++)
        printf("%02X ", trame->FCS[i]);
    printf("\n");

    return OK;
}

ERREUR_CODE trame_get_source(Trame* trame, mac** source)
{
    if (trame == NULL) return POINTEUR_NULL;

    *source = trame->adresse_source;
    return OK;
}

ERREUR_CODE trame_get_destination(Trame* trame, mac** destination)
{
    if (trame == NULL) return POINTEUR_NULL;

    *destination = trame->adresse_destination;
    return OK;
}
