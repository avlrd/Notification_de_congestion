# Notification de congestion - Application de gestion de traffic UDP

Projet informatique réalisé dans le cadre de l'UE Algorithme des Réseaux de la 3ème année de Licence Informatique à l'Université de Strasbourg.  
Auteurs :
> Cédric Geissert  
> Arthur Villard

## Utilisation

> Deux modes d'utilisation sont disponibles.

### Compilation et execution simultanées
<br>

- Placez vous dans le répertoire principal du projet et ouvrez trois terminaux.
- Dans le premier terminal, executez le fichier `medium.py`. La documentation de ce medium est disponible ici : [lien du medium](https://git.unistra.fr/alfroy/projet_algo_reseau2021)
- Dans le deuxième, entrez :

		make destination
- Dans le troisième, entrez :

		make source MARQ=<mode> //mode = S pour stopandwait ou G pour gobackn

### Compilation et execution séparées
<br>

- Placez vous dans le répertoire principal du projet et ouvrez trois terminaux.
- Dans le premier terminal, entrez :

		make
- Dans ce même terminal, executez le fichier `medium.py`. La documentation de ce medium est disponible ici : [lien du medium](https://git.unistra.fr/alfroy/projet_algo_reseau2021)
- Dans le deuxième, entrez :

		./bin/destination <IP_distante> <port_local> <port_distant>
- Dans le troisième, entrez :

		./bin/source <mode> <IP_distante> <port_local> <port_distant>
