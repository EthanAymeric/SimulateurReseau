# Informations importantes

Rendu : 

Soutenance : Jeudi 11 Juin

## Structure du projet

Organiser le projet en branche ``feature-<nom de la feature>``

Le rendu final et fonctionnel se trouve sur le ``main``

Pour rendre les attributs "private" on déclare juste le typedef dans le header et les fonctions liées. On définit la structure dans le .c .

## Etape 1

typeDef MAC
typeDef IP

structure InterfaceRéseau

structure Station
- Adresse Mac
- Adresse IP

structure Switch
- Adresse MAC
- Nombre de ports
- Priorité (STP)
- Table de commutation 

structure ReseauLocal
- Station
- Switch
- Aretes (Deux interfacesRéseau reliée entre eux)

Affichage IP décimal pointé
Affichage MAC hexadécimal
Affiche Table de commutation du switch
etc.

Ordnanceur d'event, boucler sur toute les machines et traiter la queue de chaque machine

### IP 

Chaque octet de l'adresse et le masque sont représentés sous forme de byte/uint8_t (donc un tableau de 4 bytes pour l'adresse entière). Cette structure sert principalement à de l'affichage, d'où sa fonction `get_string`. Ses attributs sont cachés. Chaque méthode (sauf `init`) renvoie une énumération d'erreurs spécifique à **ip**. 

**C'est à l'appelant d'IP de vérifier que l'allocation lors de `init` est réussie.** 

### MAC 

Une adresse MAC est représentée par un `uint64_t` (entier sur 64 bits). Celle-ci faisant uniquement 48 bits, seuls les 48 bits de poids faible sont utilisés. 

### Appareil 

Un appareil est soit un `Switch`, soit une `station`. En appelant `appareil_set_switch` ou `appareil_set_station`, l'**ancienne valeur est déinit**, il est donc impossible de set 2 fois le même `Switch` ou la même `station` pour un appareil sans l'init à nouveau. Lorsque aucune valeur n'est encore set dans un appareil, le type de celui-ci est `INDEFINI`.

**C'est à l'appelant de MAC de vérifier que l'allocation lors de `init` est réussie.** 

### LAN 

Un lan est un graphe ayant pour nœuds des `appareil` pouvant être soit un `Switch` soit une `station`.

# To-Do 

- [x] Ajouter tests pour `switch` 
- [x] Ajouter vérifications de pointeur null dans `switch` 
- [x] blinder les `init()` de toutes les structures pour éviter le crash quand `malloc` fail (condition `if (ptr == NULL) return NULL;` )
- [x] Vérifier (et tester) que les doubles `deinit`  ne font pas crash (partout)
- [x] Vérifier pourquoi `switch` peut init sans paramètres mais pas `station` 
- [x] Modifier les `appareil_set` pour ne plus avoir besoin de déinit après avoir changé de type
- [x] Ajouter structure `reseauLocal` (branche `feature-Lan`)
- [ ] Ajouter 'parseur' pour lire et décoder les fichiers de configuration 
- [ ] Ajouter ordonnanceur
- [x] Ajouter explications des structures dans le `README.md` à la manière de `IP` et `MAC` (même en mieux) pour faciliter les soutenances

- [ ] Gérer les interfaces
- [ ] Ajouter un send_trame qui prend en paramètre l'adresse destination (L'interface en face) et la trame
- [ ] Recv pour traiter les trames dans le buffer
- [ ] STP
- [ ] Corriger le parseur
- [ ] Ajouter des tests si on a pas la flemme


- [ ] supprimer mac et commutationTable de switch (car il y a interface et connexions)
- [ ] remplacer mac de station par interface et lui ajouter send_trame et receive_trame
- [ ] ajouter send et receive trame dans appareil 
- [ ] appareil_get_ordnanceur_size ??? (voir compilation)
