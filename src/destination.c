#include "utils.h"

void disconnection(int sok, struct sockaddr_in* dist, uint16_t saved_seq)
{
	int timeoutcounter = 0;

	Packet scout;
		scout.id_flux = 0;
		scout.type = FIN;
		scout.seq_num = saved_seq;
		scout.ack_num = 0;
		scout.ecn = 0;
		scout.ewnd = 1;
		strcpy(scout.message, "Ready to disconnect");

	CHECK(sendto(sok, &scout, sizeof(Packet), 0, (struct sockaddr *) dist, sizeof(struct sockaddr_in)));
	printf("FIN packet sent.\nWaiting for ack...\n\n");

	while(!timeout(sok, 5))
	{
		timeoutcounter++;
		if(timeoutcounter < NmaxT)
		{
			printf("Timed out (%d)\nResending packet...\n\n", timeoutcounter);
			CHECK(sendto(sok, &scout, sizeof(Packet), 0, (struct sockaddr *) dist, sizeof(struct sockaddr_in)));
		}
		else
		{
			printf("Could not disconnect normally!\nExiting program...\n\n");
			exit(EXIT_FAILURE);
		}
	}
}

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
	int checkFIN = 0;
	
	while (1)
	{
		Packet p;
		int received;

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

			case ACK:

				if (p.ack_num != seq + 1)
				{
					fprintf(stdout, "Wrong ack received.\n");
					break;
				}
				else
				{
					display(p);
					if(checkFIN == 1)
					{
						fprintf(stdout, "Received ack!\nExiting.\n\n");
						CHECK(close(sok));
						exit(EXIT_SUCCESS);
					}
					else
					{
						saved_sequence = seq;
						fprintf(stdout, "Received ack!\nConnexion established.\n\n\n");
					}	
				}

				break;

			case FIN:

				printf("FIN received!\nSending ack...\n\n");
				display(p);

				p.type = ACK;
				p.ack_num = p.seq_num + 1;
				strcpy(p.message, "ACK of the FIN packet");

				CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)&dist, addr_size));
				printf("Ack sent.\n\n");

				checkFIN = 1;
				disconnection(sok, &dist, saved_sequence);

				break;

			case SYN:

				printf("SYN received!\nSending ack...\n\n");
				display(p);

				p.type = SYN + ACK;
				p.ack_num = p.seq_num + 1;
				p.seq_num = seq;
				strcpy(p.message, "Ready to connect");

				CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)&dist, addr_size));
				printf("Ack sent.\n\n");

				break;

			case RST:
				break;
			}
		}
	}

	CHECK(close(sok));
	exit(EXIT_SUCCESS);
}