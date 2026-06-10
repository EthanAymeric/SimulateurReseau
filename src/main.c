#include "switch/switch.h"
#include "station/station.h"
#include "appareil/appareil.h"
#include "interface/interface.h"
#include "parser/parser.h"
#include "trame/trame.h"
#include "lan/lan.h"
#include <stdlib.h>
#include <stdio.h>

static void afficher_table(Switch* sw, const char* nom)
{
    char table[1024];
    switch_show_commutation_table(sw, table, sizeof(table));
    if (table[0] == '\0')
        printf("Table de commutation %s : (vide)\n", nom);
    else
        printf("Table de commutation %s :\n%s", nom, table);
}

static void afficher_buffer(Interface* iface, const char* nom)
{
    size_t nb = 0;
    interface_get_buffer_size(iface, &nb);
    if (nb == 0) {
        printf("  %s : aucune trame recue\n", nom);
        return;
    }
    printf("  %s : %zu trame(s) recue(s)\n", nom, nb);
    for (size_t i = 0; i < nb; i++) {
        Trame* t = NULL;
        interface_get_trame(iface, i, &t);
        printf("    [Trame %zu - lisible]\n", i + 1);
        trame_print(t);
        printf("    [Trame %zu - hexadecimal]\n", i + 1);
        trame_print_hex(t);
    }
    interface_vider_buffer(iface);
}

int main()
{
    /*
     * config5.txt : 1 switch (sw0), 3 stations (st1, st2, st3)
     *
     *   st1 ---[port 0]---+
     *   st2 ---[port 1]--- sw0
     *   st3 ---[port 2]---+
     *
     * Objectif : montrer le flood quand la destination est inconnue.
     *
     *   Envoi 1 : st1 -> st2  (destination inconnue)
     *             sw0 apprend mac_st1 sur port 0
     *             sw0 floode sur ports 1 ET 2 => st2 ET st3 recoivent
     *
     *   Envoi 2 : st2 -> st1  (st1 connu => unicast uniquement sur port 0)
     *             sw0 apprend mac_st2 sur port 1
     *             st3 ne recoit rien cette fois
     */

    Reseau* lan = NULL;
    file_parse("config_files/config5.txt", &lan);

    appareil* ap_sw  = NULL;
    appareil* ap_st1 = NULL;
    appareil* ap_st2 = NULL;
    appareil* ap_st3 = NULL;
    lan_get_machine(lan, 0, &ap_sw);
    lan_get_machine(lan, 1, &ap_st1);
    lan_get_machine(lan, 2, &ap_st2);
    lan_get_machine(lan, 3, &ap_st3);

    Switch*  sw  = NULL;
    station* st1 = NULL;
    station* st2 = NULL;
    station* st3 = NULL;
    appareil_get_switch(ap_sw,   &sw);
    appareil_get_station(ap_st1, &st1);
    appareil_get_station(ap_st2, &st2);
    appareil_get_station(ap_st3, &st3);

    Interface* iface_st1 = NULL;
    Interface* iface_st2 = NULL;
    Interface* iface_st3 = NULL;
    station_get_connexion(st1, &iface_st1);
    station_get_connexion(st2, &iface_st2);
    station_get_connexion(st3, &iface_st3);

    mac* mac_st1 = NULL;
    mac* mac_st2 = NULL;
    station_get_mac(st1, &mac_st1);
    station_get_mac(st2, &mac_st2);

    printf("========================================\n");
    printf("  Test de commutation Ethernet\n");
    printf("  Reseau : config5.txt\n");
    printf("  Topologie : st1 -- sw0 -- st2\n");
    printf("                      |-- st3\n");
    printf("========================================\n\n");

    afficher_table(sw, "sw0");
    printf("\n");

    /* --- Envoi 1 : st1 -> st2 (flood car st2 inconnue) --- */
    printf("--- Envoi 1 : st1 -> st2 ---\n");
    printf("    sw0 ne connait pas st2 => flood sur tous les ports sauf port 0\n\n");

    Trame* t1 = trame_init(mac_st1, mac_st2, "Bonjour st2 !");
    interface_send_trame(t1, iface_st1);
    switch_traiter_trame(sw);

    printf("Recepteurs apres le flood :\n");
    afficher_buffer(iface_st2, "st2");
    afficher_buffer(iface_st3, "st3 (recu par flood, pas la destination)");
    printf("\n");
    afficher_table(sw, "sw0");
    printf("\n");

    /* --- Envoi 2 : st2 -> st1 (unicast car st1 connue) --- */
    printf("--- Envoi 2 : st2 -> st1 ---\n");
    printf("    sw0 connait st1 (port 1) => unicast, st3 ne recoit rien\n\n");

    Trame* t2 = trame_init(mac_st2, mac_st1, "Bonjour st1 !");
    interface_send_trame(t2, iface_st2);
    switch_traiter_trame(sw);

    printf("Recepteurs apres l'unicast :\n");
    afficher_buffer(iface_st1, "st1");
    afficher_buffer(iface_st3, "st3 (ne doit rien recevoir)");
    printf("\n");
    afficher_table(sw, "sw0");
    printf("\n");

    trame_deinit(&t1);
    trame_deinit(&t2);
    lan_deinit(&lan);
    return 0;
}
