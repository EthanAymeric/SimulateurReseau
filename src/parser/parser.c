// #include "parser.h"


// FILE* file_init(char* path)
// {
//     FILE* fptr;
//     fptr = fopen(path, "r");
//     if (fptr == NULL)
//     {
//         return NULL;
//     }

//     return fptr;
// }

// void file_deinit(FILE* fptr)
// {
//     fclose(fptr);
// }


// ERREUR_CODE file_lan_create(FILE* fptr, Reseau* lan)
// {
//     char lan_config[20];
//     fgets(lan_config, 20, fptr);

//     char* nbMachine_str = strtok(lan_config, " ");
//     char* nbConnexion_str = strtok(NULL, " ");

//     size_t nbMachine = (size_t)strtoul(nbMachine_str, NULL, 10);
//     size_t nbConnexion = (size_t)strtoul(nbConnexion_str, NULL, 10);
    
//     // Init lan avec le bon nombre de machine et de connexions
    

//     return OK;
// }


// ERREUR_CODE file_equipements_create(FILE* fptr, size_t nbMachine, Reseau* lan)
// {
//     char ligne[255];
//     size_t i;

//     for (i = 0; i < nbMachine; i++)
//     {
//         fgets(ligne, 255, fptr);
//         char* strToken = strtok(ligne, ";");

//         if (strstr(strToken,"2"))
//         {
//             // Ajouter un Switch
//             char* adresseMac_str = strtok(NULL, ";");
//             char* nbPorts_str = strtok(NULL, ";");
//             char* priority_str = strtok(NULL, ";");

//             size_t nbPorts = atoi(nbPorts_str);
//             uint32_t priority_num = atol(priority_str);

//             // Ajouter le switch avec adresseMac_str, nbPorts et priority_num
//         }
//         if (strstr(strToken, "1"))
//         {
//             // Ajouter une Station
//             char* adresseMac_str = strtok(NULL, ";");
//             char* ip_str = strtok(NULL, ";");

//             // Ajouter la station avec adresseMac_str et ip_str

//         }
//     }
//     return OK;
// }

// ERREUR_CODE file_connexions_create(FILE* fptr, size_t nbConnexion, Reseau* lan)
// {
//     char ligne[255];
//     size_t i;

//     for (i = 0; i < nbConnexion; i++)
//     {
//         fgets(ligne, 255, fptr);

//         char* interface1_str= strtok(ligne, ";");
//         char* interface2_str = strtok(NULL, ";");
//         char* poids_str = strtok(NULL, ";");

//         // Ajouter la connexion entre les deux interfaces
//     }
//     return OK;
// }

// ERREUR_CODE file_parse(char* path, Reseau* lan)
// {
//     FILE* fptr = file_init(path);
//     if (fptr == NULL)
//     {
//         return ERREUR;
//     }

//     file_lan_create(fptr, lan);
//     file_equipements_create(fptr, lan->nbMachine, lan);
//     file_connexions_create(fptr, lan->nbConnexion, lan);

//     file_deinit(fptr);
//     return OK;
// }

