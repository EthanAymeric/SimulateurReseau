# SAE2.3 — Simulation d'un réseau local

Projet réalisé dans le cadre de la **SAE2.3 — Réseaux** du BUT Informatique à l'IUT Robert Schuman.

L'objectif de ce projet est de développer en **C** une simulation d'un réseau local permettant de représenter des équipements réseau, leurs connexions et les échanges de trames Ethernet.

Le projet vise notamment à mettre en pratique les structures de données, la représentation d'un graphe, le fonctionnement d'Ethernet et le protocole **STP (Spanning Tree Protocol)**.

---

## Objectifs du projet

Le projet consiste à :

* représenter des adresses **MAC** et **IPv4** ;
* modéliser des **stations** et des **switches** ;
* représenter un **réseau local** sous la forme d'un graphe ;
* permettre l'affichage des différentes informations du réseau ;
* charger une architecture réseau depuis un fichier de configuration ;
* simuler la **commutation de trames Ethernet** ;
* implémenter le protocole **STP** et l'échange de BPDU entre les switches.

---

# Organisation du projet

Le développement est organisé à l'aide de branches Git dédiées aux différentes fonctionnalités :

```text
feature-<nom-de-la-feature>
```

La branche `main` contient la version finale et fonctionnelle du projet.

Exemple :

```text
main
│
├── feature-Lan
├── feature-parseur
├── feature-ordonnanceur
└── ...
```

---

# Structures de données

## IP

Une adresse IPv4 est représentée par **4 octets (`uint8_t`)**.

La structure permet notamment :

* de stocker une adresse IP ;
* de stocker son masque ;
* d'afficher l'adresse en notation décimale pointée.

Les attributs de la structure sont cachés afin de respecter l'encapsulation.

Exemple :

```text
130.79.80.21
```

La fonction `get_string` permet d'obtenir une représentation textuelle de l'adresse.

> L'appelant doit vérifier que l'allocation réalisée lors de `init` a réussi.

---

## MAC

Une adresse MAC est représentée par un **`uint64_t`**.

Une adresse MAC ne nécessitant que 48 bits, seuls les **48 bits de poids faible** sont utilisés.

Exemple :

```text
01:45:23:a6:f7:ab
```

La MAC peut être affichée sous forme hexadécimale.

---

## Interface réseau

Une interface réseau permet de représenter la connexion d'un équipement au réseau.

Elle est utilisée pour représenter les connexions entre les différents appareils du réseau local.

---

## Station

Une station représente un équipement terminal du réseau.

Elle possède notamment :

* une adresse MAC ;
* une adresse IP ;
* une ou plusieurs interfaces réseau selon l'évolution du projet.

Exemple :

```text
Station
├── Adresse MAC
└── Adresse IP
```

---

## Switch

Un switch représente un équipement permettant de relier plusieurs appareils dans le réseau local.

Il possède notamment :

* une adresse MAC ;
* un nombre de ports ;
* une priorité utilisée par **STP** ;
* une table de commutation.

Exemple :

```text
Switch
├── Adresse MAC
├── Nombre de ports
├── Priorité STP
└── Table de commutation
```

La priorité du switch est utilisée lors de l'élection du switch racine dans le protocole STP.

---

## Appareil

Un `appareil` représente un équipement du réseau pouvant être :

* une **station** ;
* un **switch**.

Son type peut notamment être :

```text
INDEFINI
STATION
SWITCH
```

Les fonctions `appareil_set_switch` et `appareil_set_station` permettent de définir le type de l'appareil.

---

## LAN

Un LAN représente le **réseau local**.

Il est modélisé comme un **graphe** dont :

* les nœuds sont des appareils ;
* les arêtes représentent les connexions entre les appareils.

Une arête relie deux interfaces réseau.

Le sujet demande ainsi de pouvoir représenter une architecture réseau interconnectant plusieurs équipements.

---

# Fichier de configuration

Une architecture réseau peut être décrite dans un fichier texte afin de pouvoir être chargée automatiquement.

La première ligne contient :

```text
nombre_d'équipements nombre_de_liens
```

Les équipements sont ensuite décrits un par ligne.

Le type de l'équipement est indiqué au début de la ligne :

```text
1 → Station
2 → Switch
```

Les switches sont placés en premier, puis les stations.

Les dernières lignes décrivent les connexions entre les équipements ainsi que leur coût.

### Exemple

```text
4 3
2;01:45:23:a6:f7:ab;8;1024
1;54:d6:a6:82:c5:23;130.79.80.21
1;c8:69:72:5e:43:af;130.79.80.27
1;77:ac:d6:82:12:23;130.79.80.42
0;1;4
0;2;19
0;3;4
```

Ce fichier représente :

```text
             Switch
               │
        ┌──────┼──────┐
        │      │      │
      ST1     ST2     ST3
```

Les coûts des liens correspondent au débit :

| Débit    | Coût STP |
| -------- | -------: |
| 10 Mb/s  |      100 |
| 100 Mb/s |       19 |
| 1 Gb/s   |        4 |

Ces valeurs sont celles données dans le sujet.

---

# Trames Ethernet

Le projet doit également permettre de représenter une **trame Ethernet**.

La structure doit rester proche de la structure réelle d'une trame Ethernet, même si seuls certains champs sont réellement utilisés dans la simulation.

La trame est composée de :

```text
Préambule
SFD
Adresse destination
Adresse source
Type
Données
Bourrage
FCS
```

Les champs principaux utilisés par le projet sont notamment :

* adresse MAC destination ;
* adresse MAC source ;
* type ;
* données.

Exemples de valeurs pour le champ `type` :

```text
0x0800 → IPv4
0x0806 → ARP
0x86DD → IPv6
```

Une fonction d'affichage doit permettre de présenter une trame :

1. dans un format lisible pour l'utilisateur ;
2. sous forme hexadécimale correspondant au contenu brut des octets.

---

# Commutation Ethernet

La simulation doit permettre de représenter l'échange de trames Ethernet dans le réseau local.

Le switch utilise une **table de commutation** afin de déterminer vers quel port transmettre une trame.

À terme, le projet doit notamment gérer :

* l'envoi d'une trame ;
* la réception d'une trame ;
* le traitement des trames dans un buffer ;
* les interfaces réseau ;
* la table de commutation.

---

# STP — Spanning Tree Protocol

Le projet doit implémenter le protocole **STP** afin d'éviter les boucles dans le réseau local.

Les switches échangent des **BPDU** afin de déterminer la topologie du réseau et de configurer leurs ports.

Au départ :

* les tables de commutation sont vides ;
* les ports sont dans un état inconnu.

Le protocole STP doit ensuite permettre une convergence du réseau.

Les différents rôles des ports sont notamment :

* **Root Port** : port permettant de rejoindre le switch racine ;
* **Designated Port** : port autorisé à transmettre ;
* **Port bloqué** : port bloqué afin d'éviter les boucles.

Ces mécanismes sont décrits dans l'étape 4 du sujet.

---

# Ordonnanceur d'événements

Un ordonnanceur est prévu afin de gérer les événements du réseau.

Son rôle sera notamment de :

1. parcourir les différentes machines ;
2. consulter leur file d'événements ;
3. traiter les événements en attente ;
4. permettre la simulation des échanges dans le réseau.

---

# Gestion des erreurs et de la mémoire

Une attention particulière est portée à la gestion de la mémoire.

Les fonctions `init()` doivent notamment vérifier que les allocations dynamiques ont réussi :

```c
if (ptr == NULL)
    return NULL;
```

Les fonctions `deinit()` doivent également pouvoir être appelées sans provoquer de crash en cas de double désallocation.

Les structures utilisent autant que possible des attributs privés en déclarant uniquement les types et fonctions nécessaires dans les fichiers `.h`, puis en définissant les structures dans les fichiers `.c`.

---

# Technologies utilisées

* **Langage :** C
* **Gestion de versions :** Git
* **Hébergement :** Gitlab, puis GitHub
* **Structures de données :** graphes, structures C, tableaux et files
* **Réseaux étudiés :** Ethernet, IPv4 et STP

---

# Compilation et exécution

Exemple :

```bash
git clone <URL_DU_DEPOT>
cd <NOM_DU_PROJET>
```

Puis compiler le projet avec :

```bash
make
```

Et lancer le programme avec :

```bash
./<nom_du_programme>
```

---

# Organisation Git

Chaque fonctionnalité importante doit être développée dans une branche dédiée :

```text
feature-<nom-de-la-feature>
```

Une fois la fonctionnalité terminée et testée, elle peut être intégrée à `main`.

```bash
git add .
git commit -m "Ajout de la structure LAN"
git push origin feature-Lan
```

La branche `main` correspond à la version finale fonctionnelle du projet.

---

# Auteurs

[Guillaume Niederlaender](https://github.com/Guyomeuh) (Chef de projet)
[eliott.wnr](https://github.com/eliottwnr)
[EthanAymeric](https://github.com/EthanAymeric)
