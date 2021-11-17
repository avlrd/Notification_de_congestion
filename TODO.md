# TODO

- gobackn
- Gestion d'erreur à la réception des paquets (erreurs globales, tout tester)
- script de création de figures illustrant l'impact du mécanisme ECN
- gestion de flux


## Test

Pour tester il faut faire dans l'ordre:

- Ouvrir 3 terminaux
	dans n'importe lequel
-		make medium
	dans un deuxieme terminal
-		make destination
	dans un troisieme terminal
-		make source MARQ=S


L'ordre est important. Il faudra quand meme tester dans le desordre pour la gestion d'erreur.