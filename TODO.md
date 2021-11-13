# TODO

- Gestion d'erreur à la réception des paquets (erreurs globales, tout tester)


## Test

Pour tester il faut faire dans l'ordre:

- Ouvrir 2 terminaux
- 		make
	dans n'importe lequel
-		./bin/destination 127.0.0.1 6666 5555
	dans un premier terminal
-		./bin/source stopandwait 127.0.0.1 5555 6666
	dans le deuxieme.

L'ordre est important. Il faudra quand meme tester dans le desordre pour la gestion d'erreur.