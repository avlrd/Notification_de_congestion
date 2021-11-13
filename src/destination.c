#include "utils.h"

int main(int argc, char const *argv[])
{
	//Checking usage
	if (argc != 4)
	{
		fprintf(stdout, "usage: ./destination <IP_dist> <local_port> <dist_port>\n");
		exit(EXIT_FAILURE);
	}

	//Getting command args
	const char *ipdist = argv[1];
	int localport = atoi(argv[2]);
	int distport = atoi(argv[3]);

	int sok;
	struct sockaddr_in dist;
	socklen_t addr_size = sizeof(struct sockaddr_in);
	int domain = AF_INET;

	CHECK(sok = socket(domain, SOCK_DGRAM, IPPROTO_UDP));

	if (memset(&dist, '\0', sizeof(dist)) == NULL)
		raler(1, "memset error");

	dist.sin_family = domain;
	dist.sin_port = htons(localport);
	if (inet_aton(ipdist, &dist.sin_addr) == 0)
		raler(1, "inet_aton: adress not valid");

	CHECK(bind(sok, (struct sockaddr *)&dist, sizeof(struct sockaddr_in)));
	dist.sin_port = htons(distport);

	/*
	 * max uint16_t = 65535, aleatoire pour la securite
	 * déclaration hors du while car rand()
	*/
	uint16_t seq = rand() % 65534;
	uint16_t saved_sequence; //sauvegarde du numéro de sequence comme src

	while (1)
	{
		Packet p;
		long received;

		CHECK((received = recvfrom(sok, &p, sizeof(Packet), 0, (struct sockaddr *)&dist, &addr_size)));

		if (received > 0) //FAIRE UN TEST GENERAL DES VALEURS DU PAQUET
		{
			fprintf(stdout, "Packet received\n");

			switch (p.type)
			{
			case DATA:

				display(p);

				p.type += ACK;
				p.ack_num = p.seq_num + 1;
				strcpy(p.message, "Ack");

				CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)&dist, addr_size));

				fprintf(stdout, "Ack sent\n");
				break;

			case ACK: //Cas uniquement utilisé par 3wayhandshake

				if(p.ack_num != seq + 1) //verif du numéro d'ack
				{
					fprintf(stdout, "Wrong ack received. Cannot establish connexion.\n");
					break;
				}
				else
				{
					display(p);
					saved_sequence = seq; //saved_sequence=seqAlea
					fprintf(stdout, "Connexion established!\n");
				}
				break;

			case FIN:
				break;

			case SYN:

				display(p);
				// Serveur repond client avec paquet synchronize-acknowledge.
				p.type = SYN + ACK;		   
				p.ack_num = p.seq_num + 1; // numeroACK=numPreviousSEQ (SYN) + 1
				p.seq_num = seq;		   // numSeq SYN-ACK nombre aléatoire.

				CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)&dist, addr_size));

				break;

			case ACK + FIN:
				break;

			case RST:
				break;
			}
		}
	}
	printf("%d", saved_sequence); //warning

	exit(EXIT_SUCCESS);
}