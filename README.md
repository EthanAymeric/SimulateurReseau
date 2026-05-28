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

**C'est à l'appelant de MAC de vérifier que l'allocation lors de `init` est réussie.** 

# To-Do 

- [ ] refactor les vérifications de paramètres (conditions)
- [x] écrire des tests pour ip
- [ ] écrire des tests pour mac
