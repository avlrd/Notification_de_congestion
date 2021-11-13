[//]: # (IMPORTANT : Ctrl+Shift+V pour prévisualiser le fichier markdown sur Visual Studio Code)
# Notification de congestion

[![forthebadge](https://forthebadge.com/images/badges/made-with-markdown.svg)](https://forthebadge.com)

## Application de gestion de traffic UDP.

### 1. Compiler et tester le projet 

#### 1.1 Makefile

> Dans le terminal saisissez : 

    make source MARQ=S (OU MARQ=G)
    make destination
    make medium ET/OU MOPT=votreOption

> Cela va compiler : 

    make MODE=MF TFM='source utils'
    make MODE=MF TFM='destination utils'

#### 1.1 Tester

> Dans le terminal saisissez : 

    ./bin/source <mode> <IP_distante> <port_local> <port_ecoute_src_pertubateur>
    ./bin/destination <IP_distante> <port_local> <port_ecoute_dst_pertubateur>

### 2. Medium

> Lien du [medium](https://git.unistra.fr/alfroy/projet_algo_reseau2021).

> Dans le terminal saisissez : `python3 medium.py`

### Auteur   
    Cedric GEISSERT
    Arthur Villard

### Licence 
> Réalisée par Cédric Geissert et Arthur Villard, étudiants en L3 Informatique à l'Université de Strasbourg dans le cadre de l'UE Algorithmes des Réseaux.
&nbsp;

[![forthebadge](https://forthebadge.com/images/badges/cc-nc.svg)](https://forthebadge.com) 
[![forthebadge](https://forthebadge.com/images/badges/cc-by-nd.svg)](https://forthebadge.com)