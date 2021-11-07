#include "utils.h"

int main(int argc, char const *argv[])
{
	//Checking usage
	if (argc != 4)
	{
		fprintf(stdout, "usage: ./destination <IP_dist> <local_port> <dest_port>\n");
		exit(EXIT_FAILURE);
	}

	////////////////////////////////////////////////////////////////////////////////
	// idem que source
	////////////////////////////////////////////////////////////////////////////////

	//Getting command args
	const char *ipdist = argv[1];
	int localport = atoi(argv[2]);
	int destport = atoi(argv[3]);

	int sok;
	struct sockaddr_in dest; // serveur
	socklen_t dest_size = sizeof(struct sockaddr_in);
	struct sockaddr_in source; // client
	socklen_t source_size = sizeof(struct sockaddr_in);
	int domain = AF_INET;

	CHECK(sok = socket(domain, SOCK_DGRAM, IPPROTO_UDP));

	if (memset(&dest, '\0', sizeof(dest)) == NULL)
		raler(1, "memset error");

	dest.sin_family = domain;
	dest.sin_port = htons(localport);
	if (inet_aton(ipdist, &dest.sin_addr) == 0)
		raler(1, "inet_aton: adress not valid");

	CHECK(bind(sok, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)));

	////////////////////////////////////////////////////////////////////////////////

	/* recv(),  recvfrom()  et  recvmsg()  sont  utilisés pour recevoir des messages depuis 
   une socket, et peuvent servir sur une socket orientée  connexion  ou  non.

    ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, 
					struct sockaddr *src_addr, socklen_t *addrlen)  enregistre  le  message reçu  
	dans  le tampon buf. Le processus appelant doit préciser la taille de ce tampon dans len.
*/

	while (1)
	{
		Packet p; // paquet
		long received;

		CHECK((received = recvfrom(sok, &p, sizeof(Packet), 0, (struct sockaddr *)&source, &source_size)));
		if (received > 0)
		{
			switch (p.type)
			{
			case DATA:
				fprintf(stdout, "Packet received\n");
				display(p);
				p.type += ACK;
				p.ack_num = p.seq_num + 1;
				p.message = "Acquittement";
				dest.sin_port = htons(destport);
				CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)&dest, dest_size));
				fprintf(stdout, "Ack sent\n");
				break;

			case ACK: // Three way handshake

				// Manque gestion erreurs et timeout !!
				fprintf(stdout, "Packet received\n");
				display(p);
				uint16_t A = p.seq_num;
				p.type = ACK;
				p.ack_num = A + 1; // Le numéro du ACK est égal au numéro de séquence du paquet précédent (SYN) incrémenté de un (A + 1)
				p.message = "Ack tWH";
				CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)&source, sizeof(struct sockaddr_in)));
				break;

			case FIN:
				break;

			case SYN: // Three way handshake

				// Manque gestion erreurs et timeout !!
				fprintf(stdout, "Packet received\n");
				display(p);
				uint16_t B = rand() % 65535; // max uint16_t = 65535, aleatoire pour la securite
				p.type = SYN;				 // Le serveur va répondre au client à l'aide d'un paquet SYN-ACK (synchronize, acknowledge).
				p.seq_num = B;				 // Le numéro de séquence du paquet SYN-ACK est un nombre aléatoire B.
				p.message = "Syn tWH";
				break;

			case ACK + FIN:
				break;

			case RST:
				break;
			}
		}
	}

	exit(EXIT_SUCCESS);
}