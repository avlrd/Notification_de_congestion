#include "utils.h"

////////////////////////////////////////////////////////////////////////////////

char *text = "Away we went then, and we drove for at least an hour."
			 "Colonel Lysander Stark had said that it was only seven"
			 "miles, but I should think, from the rate that we seemed"
			 "to go, and from the time that we took, that it must have"
			 "been nearer twelve. He sat at my side in silence all the"
			 "time, and I was aware, more than once when I glanced in his"
			 "direction, that he was looking at me with great intensity.";

Packet MsgBuffer[MAX];
int nbMsgBuffer = 0;

////////////////////////////////////////////////////////////////////////////////
/*
	Renvoie un uint16_t (short int) car il faut sauvegarder le numéro de séquence utilisé pour communiquer dans le sens client -> serveur (source -> destination).
	Voir destination.c pour comprendre le fonctionnement.
*/
uint16_t threewayhandshake(int sok, struct sockaddr_in *dist)
{
	uint16_t seq = rand() % 65535;
	int received, timeoutCounter = 0;

	Packet scout;
	scout.id_flux = 0;
	scout.type = SYN;
	scout.seq_num = seq;
	scout.ack_num = 0;
	scout.ecn = 0;
	scout.ewnd = 1;

	CHECK(sendto(sok, &scout, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
	printf("Synchronizing packet sent.\nWaiting for ack...\n\n");

	while (!timeout(sok, 5)) // while there is a timeout (timeout == 0)
	{
		timeoutCounter++;
		if (timeoutCounter < NmaxT)
		{
			printf("Time out n°%d.\nResending packet...\n\n", timeoutCounter);
			CHECK(sendto(sok, &scout, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in))); //Renvoi du packet en cas de timeout
		}
		else
		{
			fprintf(stderr, "Connexion non établie");
			exit(1);
		}
	}

	CHECK(received = recvfrom(sok, &scout, sizeof(Packet), 0, NULL, NULL));
	if (scout.type == (SYN + ACK) && scout.ack_num == (seq + 1)) //verif du type
	{
		printf("Ack + Syn received !\nSending ack aswell...\n\n");
		display(scout);

		scout.type = ACK;
		scout.ack_num = scout.seq_num + 1; //voir wikipedia

		CHECK(sendto(sok, &scout, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));

		printf("Ack packet sent.\n\n");
	}
	printf("Communication established!\n");

	return seq; //seq est le numéro de sequence utilisé dans l'envoi du pack SYN
}

////////////////////////////////////////////////////////////////////////////////

void stopandwait(int sok, struct sockaddr_in *dist)
{
	int received;
	int nb_msg = 5;
	int i;

	Packet p;

	for(i = 0; i < nb_msg; i++)
	{
		p.seq_num = i;
		p.ack_num = 0;
		p.type = DATA;
		p.id_flux = 1;
		p.ecn = 0;
		p.ewnd = 1;
		strcpy(p.message, "Test");

		CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *) dist, sizeof(struct sockaddr_in)));
		printf("Packet n°%d sent, waiting for ack...\n", p.seq_num);

		while(timeout(sok, 5) == 0)
		{
			printf("Timed out.\nResending packet...\n\n");

			CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *) dist, sizeof(struct sockaddr_in)));
		}

		CHECK(received = recvfrom(sok, &p, sizeof(Packet), 0, NULL, NULL));
		if(p.type == ACK && p.ack_num == i+1)
		{
			printf("Ack received!\n");
			display(p);
		}
		else
			printf("Error!!!\n");
	}
	printf("Finished sending messages.\n");
}

int quitter(char *message)
{
	return (strncmp(message, "exit(q)", 7) == 0) || (strncmp(message, "\0", 1) == 0);
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

/*
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
		// A FAIRE : 
		if (timeout(sok)) // >1
		{ 
			printf("Des données sont disponibles maintenant\n");
			break; // pas de timeout, on sort de la boucle
		}
		// FD_ISSET(0, &rfds) est vrai
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
*/
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

/*
void disconnection(int sok, struct sockaddr_in *dist)
{
	// Manque gestion erreurs et timeout !!
	uint16_t A = rand() % 65535; // max uint16_t = 65535, aleatoire pour la securite
	long received;
	socklen_t tmp = sizeof(struct sockaddr_in);
	struct sockaddr_in distTmp; // structure d'@ pour @ distante temporaire


*/

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
	dist.sin_port = htons(distport);
	//Function needed variables
	//int* ack_rcvd; *ack_rcvd = 1;

	// Three-way handshake
	uint16_t saved_sequence = threewayhandshake(sok, &dist);
	printf("\n\nSaved sequence: %d\n", saved_sequence);

	//Mode calls

	if (mode == 0)
	{
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