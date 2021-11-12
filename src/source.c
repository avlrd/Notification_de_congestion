#include "utils.h"

char *text = "Away we went then, and we drove for at least an hour."
			 "Colonel Lysander Stark had said that it was only seven"
			 "miles, but I should think, from the rate that we seemed"
			 "to go, and from the time that we took, that it must have"
			 "been nearer twelve. He sat at my side in silence all the"
			 "time, and I was aware, more than once when I glanced in his"
			 "direction, that he was looking at me with great intensity.";

Packet MsgBuffer[MAX];
int nbMsgBuffer = 0;

/*
 * Function: threeWayHandshake
 * ---------------------------
 * Ouverture de la connexion entre client et serveur.
 *
 *  sock : int 
 *  dist : struct sockaddr_in*
 *
 *  returns: nothing
 */
void threeWayHandshake(int sok, struct sockaddr_in *dist)
{
	// Manque gestion erreurs et timeout !!
	uint16_t A = rand() % 65535; // max uint16_t = 65535, aleatoire pour la securite
	long received;
	socklen_t tmp = sizeof(struct sockaddr_in);
	struct sockaddr_in distTmp; // structure d'@ pour @ distante temporaire

	Packet pSyn;
	pSyn.id_flux = 1;
	pSyn.type = SYN;  // Le client qui désire établir une connexion avec un serveur va envoyer un premier paquet SYN (synchronized) au serveur.
	pSyn.seq_num = A; // Le numéro de séquence de ce paquet est un nombre aléatoire A.
	pSyn.ack_num = 0;
	pSyn.ecn = 0;
	pSyn.ewnd = 1;
	pSyn.message[MSIZE] = "message tWH";

	CHECK(sendto(sok, &pSyn, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
	CHECK((received = recvfrom(sok, &pSyn, sizeof(Packet), 0, (struct sockaddr *)&distTmp, &tmp)));

	uint16_t B = pSyn.seq_num;

	Packet pAck;
	pAck.id_flux = 1;
	pAck.type = ACK;
	pAck.seq_num = B + 1; // Le numéro du ACK est égal au numéro de séquence du paquet précédent (SYN-ACK) incrémenté de un (B + 1).
	pAck.ack_num = A + 1; // Le numéro d'acquittement de ce paquet est défini selon le numéro de séquence reçu précédemment (par exemple : A + 1).
	pAck.ecn = 0;
	pAck.ewnd = 1;
	pAck.message[MSIZE] = "message tWH";
	CHECK(sendto(sok, &pAck, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
}

/*
 * Function: stopandwait
 * ---------------------------
 * Arrêt et attente d’acquittement.
 *
 *  sock : int 
 *  dist : struct sockaddr_in*
 *
 *  returns: nothing
 */
void stopandwait(int sok, struct sockaddr_in *dist)
{
	/* Le principe est d'envoyer un packet dans un while infini, 
	   de bloquer l'envoi de packet en attendant l'ack grace au recvfrom
	*/
	Packet p;
	p.id_flux = 1; //MODIFY WHEN TESTS ARE OK FOR BASIC IMPLEMENTATION
	p.type = DATA;
	p.seq_num = 0;
	p.ack_num = 0;
	p.ecn = 0;
	p.ewnd = 1;
	p.message[MSIZE] = "Test";

	// Send, sendto, et sendmsg permettent de transmettre un mes­sage à destination d'une autre socket.

	// timeout bloquant car surveille descripteur donc avant recvfrom
	while (1)
	{
		CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
		printf("Sent.\n");
		// Pas de message suivant si pas ack, donc possibilité d'un timeout infini
		if (timeout(sok)) // >1
		{ 
			printf("Des données sont disponibles maintenant\n");
			break; // pas de timeout, on sort de la boucle
		}
		/* FD_ISSET(0, &rfds) est vrai */
		else // 0 
		{ 
			printf("Aucun acquittement recu\n");
			printf("Aucune données durant les 5 secondes\n");
		}
	}
	long received;
	socklen_t tmp = sizeof(struct sockaddr_in);
	CHECK((received = recvfrom(sok, &p, sizeof(Packet), 0, (struct sockaddr *)dist, &tmp)));
}

/*
 * Function: timeout
 * --------------------
 * Terminer/quitter la saisie des messages sur l'entrée standard.
 *
 *  sock : int 
 *  dist : struct sockaddr_in*
 *
 *  returns: nothing
 */
int quitter(char *message)
{
	return (strncmp(message, "exit(q)", 7) == 0) || (strncmp(message, "\0", 1) == 0);
}

/*
 * Function: timeout
 * --------------------
 * Le timeout détermine le temps d'attente maximal de la réponse. Si le timeout est écoulé, 
 * l'échange se termine avec un compte-rendu d'erreur, de même, la réception d'une réponse 
 * après la fin du timeout est refusée.
 *
 *  sock : int 
 *
 *  returns: nothing
 */
int timeout(int sok)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	/* Surveiller stdin (fd 0) en attente d'entrées */
	FD_ZERO(&rfds);		// efface un ensemble.
	FD_SET(sok, &rfds); // ajoute un descripteur dans un ensemble (la socket)

	/* Délai avant timeout : 5 secondes maxi */
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	// nfds : numéro du plus grand descripteur valide +1
	CHECK(retval = select(sok + 1, &rfds, NULL, NULL, &tv)); // -1
	/* Considérer tv comme indéfini maintenant ! */
	return retval;
}

/*
 * Function:  gobackn
 * --------------------
 * Le Go-Back-N ARQ est un type de méthode Automatic Repeat-reQuest dans lequel l'émetteur envoie 
 * un certain nombre de trames, regroupées en une fenêtre, sans recevoir d'acquittement de la part 
 * du destinataire pour chaque trame.
 *
 *  sock : int 
 *  dist : struct sockaddr_in*
 *
 *  returns: nothing
 */
void gobackn(int sok, struct sockaddr_in *dist)
{
	int fenetre = 1; // taille de la fenetre emission
	int Msg2bSend = 0;
	// nbMsgBuffer tous les messages qu'on veut envoyer.

	Packet messageP;

	while (nbMsgBuffer < 255) // nb msg qu'on veut envoyer
	{						  // quitter saisie terminal des messages
		if (quitter(messageP.message))
			break;
		// lis les messages sur l'entree standard
		fgets(messageP.message, MSIZE, stdin);
		// copie les messages dans le buffer
		strcpy(&MsgBuffer[nbMsgBuffer].message, &messageP.message);
		nbMsgBuffer++;
	}

	while (Msg2bSend < nbMsgBuffer) // traiter donnees
	{
		for (size_t i = 0; i < fenetre; i++) // envoi n messages en fct de la fenetre
		{
			if (Msg2bSend + i == nbMsgBuffer)
				break;
			CHECK(sendto(sok, &MsgBuffer[i + Msg2bSend], sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
		}

		long received;
		socklen_t tmp = sizeof(struct sockaddr_in);
		// serveur a acquitté tous les messages envoyés
		/* A FAIRE : */
		if (timeout(sok)) // >1
		{ 
			printf("Des données sont disponibles maintenant\n");
			break; // pas de timeout, on sort de la boucle
		}
		/* FD_ISSET(0, &rfds) est vrai */
		else // 0 
		{ 
			printf("Aucun acquittement recu\n");
			printf("Aucune données durant les 5 secondes\n");
		}
		CHECK((received = recvfrom(sok, &MsgBuffer[Msg2bSend], sizeof(Packet), 0, (struct sockaddr *)dist, &tmp)));
		fenetre += 1; // quand réussi à acquitter les messages
		Msg2bSend += fenetre;
	}
}

/*
 * Function: disconnection
 * ------------------------
 * Fermeture de la connexion entre client et serveur
 *
 *  sock : int 
 *  dist : struct sockaddr_in*
 *
 *  returns: nothing
 */
void disconnection(int sok, struct sockaddr_in *dist)
{
	// Manque gestion erreurs et timeout !!
	uint16_t A = rand() % 65535; // max uint16_t = 65535, aleatoire pour la securite
	long received;
	socklen_t tmp = sizeof(struct sockaddr_in);
	struct sockaddr_in distTmp; // structure d'@ pour @ distante temporaire

	/*
     *  A CONTINUER !!
	 */
}

int main(int argc, char const *argv[])
{
	//Checking usage
	if (argc != 5)
	{
		fprintf(stderr, "usage: ./source <mode> <IP_dist> <local_port> <dist_port>\n");
		exit(EXIT_FAILURE);
	}

	//Gettings command args
	const char *arg1 = argv[1];
	int mode;
	if (strcmp(arg1, "stopandwait") == 0)
		mode = 0;
	else if (strcmp(arg1, "gobackn") == 0)
		mode = 1;
	else
	{
		fprintf(stderr, "Possible values for mode: <stopandwait> or <gobackn>");
		exit(EXIT_FAILURE);
	}
	const char *ipdist = argv[2];
	int localport = atoi(argv[3]);
	int distport = atoi(argv[4]);

	//Ergonomy
	int domain = AF_INET; // = Format d'adresse, Internet = Adresses IP  (famille IPv4)

	//Socket init and bind
	int sok;
	struct sockaddr_in dist; // structure d'@ pour @ distante

	// IPv4, datagram-based protocol, IPPROTO_UDP
	CHECK(sok = socket(domain, SOCK_DGRAM, IPPROTO_UDP)); // cree un descripteur/socket -> int socket(famille,type,protocole)

	if (memset(&dist, '\0', sizeof(dist)) == NULL) // reinitialise contenu de struct dist a O
		raler(1, "memset error");

	// htons() convertit un entier court non signé hostshort depuis l'ordre des octets de l'hôte vers celui du réseau.
	// -> ( big ou little endian géré automatiquement)

	dist.sin_family = domain;		  // distant sera IPv4
	dist.sin_port = htons(localport); // port local

	/* inet_aton(const char *cp, struct in_addr *inp) convertit l'adresse Internet de l'hôte cp 
	   depuis la notation IPv4 avec nombres et points vers une forme binaire (dans l'ordre d'octet
	   du réseau), et la  stocke  dans  la structure  pointée  par  inp.  inet_aton()  renvoie  
	   une valeur non nulle si l'adresse est valable, et zéro sinon. L'adresse fournie à cp peut 
	   avoir l'une des formes suivantes
	*/

	if (inet_aton(ipdist, &dist.sin_addr) == 0)
		raler(1, "inet_aton: adress not valid");

	/* bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) affecte l'adresse 
	  spécifiée dans addr à la socket référencée par le descripteur de fichier sockfd. */

	// bind l'@ local à la socket
	CHECK(bind(sok, (struct sockaddr *)&dist, sizeof(struct sockaddr_in)));

	//Function needed variables
	//int* ack_rcvd; *ack_rcvd = 1;

	// Three-way handshake
	threeWayHandshake(sok, &dist);

	//Mode calls

	if (mode == 0)
	{
		dist.sin_port = htons(distport); // port distant
		stopandwait(sok, &dist);
	}
	else
	{
		//gobackn();
	}

	//test();

	fprintf(stdout, "Done.\n");
	CHECK(close(sok));
	exit(EXIT_SUCCESS);
}