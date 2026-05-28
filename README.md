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

# Station (commencé par Ethan)
# Recheckez mon code et modifier le si nécessaire, parce que je ne suis pas sûr de ce que je fais! 
Merci beaucoup!