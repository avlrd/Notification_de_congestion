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
	printf("Synchronizing packet sent.\nWaiting for syn-ack...\n\n");

	while (!timeout(sok, 5))
	{
		timeoutCounter++;
		if (timeoutCounter < NmaxT)
		{
			printf("Timed out (%d).\nResending packet...\n\n", timeoutCounter);
			CHECK(sendto(sok, &scout, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
		}
		else
		{
			printf("Connexion couldn't be established!\nExiting...\n\n");
			exit(EXIT_FAILURE);
		}
	}

	CHECK(received = recvfrom(sok, &scout, sizeof(Packet), 0, NULL, NULL));
	if (scout.type == (SYN + ACK) && scout.ack_num == (seq + 1))
	{
		printf("Ack + Syn received !\nSending ack aswell...\n\n");			display(scout);

		scout.type = ACK;			
		scout.ack_num = scout.seq_num + 1;

		CHECK(sendto(sok, &scout, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));

		printf("Ack packet sent.\n\n");
	}
	else
	{
		if(scout.type != (SYN + ACK))
			printf("Received wrong type packet!\nExiting...\n\n");
		if(scout.ack_num != (seq + 1))
			printf("Received ack of the wrong packet!\nExiting...\n\n");
		exit(EXIT_FAILURE);
	}
	printf("Communication established!\n\n\n");
	return seq; //seq est le numéro de sequence utilisé dans l'envoi du pack SYN
}

////////////////////////////////////////////////////////////////////////////////

void stopandwait(int sok, struct sockaddr_in *dist)
{
	int received;
	int nb_msg = 3;
	int timeoutcounter = 0;
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

		CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
		printf("Packet n°%d sent, waiting for ack...\n", p.seq_num);

		while(!timeout(sok, 5))
		{
			timeoutcounter++;
			if(timeoutcounter < NmaxT)
			{
				printf("Timed out (%d).\nResending packet...\n\n", timeoutcounter);
				CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
			}
			else
			{
				printf("Ack not received...\nExiting...\n\n");
			}
			CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
		}

		CHECK(received = recvfrom(sok, &p, sizeof(Packet), 0, NULL, NULL));
		if(p.type == ACK && p.ack_num == i + 1)
		{
			printf("Ack received!\n");
			display(p);
		}
		else
			printf("Error!!!\n");
	}
	printf("Finished sending messages.\n\n\n");
}

int quitter(char *message)
{
	return (strncmp(message, "exit(q)", 7) == 0) || (strncmp(message, "\0", 1) == 0);
}

void disconnection(int sok, struct sockaddr_in *dist, uint16_t savedseq)
{
	int received, timeoutcounter = 0;

	Packet scout;
		scout.id_flux = 0;
		scout.type = FIN;
		scout.seq_num = savedseq;
		scout.ack_num = 0;
		scout.ecn = 0;
		scout.ewnd = 1;
	
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

	CHECK(received = recvfrom(sok, &scout, sizeof(Packet), 0, NULL, NULL));
	if((scout.type == ACK) && (scout.ack_num == (savedseq + 1)))
	{
		printf("Ack received !\nCannot send message anymore.\nIf you desire to send more messages, please restart the program.\nWaiting for FIN from server to exit the program...\n\n");
		display(scout);
	}
	else
	{
		if(scout.type != (FIN + ACK))
			printf("Received wrong type packet!\nForcing quit...\n\n");
		if(scout.ack_num != (savedseq + 1))
			printf("Received ack of the wrong packet!\nForcing quit...\n\n");
		exit(EXIT_FAILURE);
	}

	printf("If this program does not receive any FIN packet from server in 30 seconds, it will force quit.\n\n");
	timeoutcounter = 0;
	while(!timeout(sok, 1))
	{
		timeoutcounter++;
		if(timeoutcounter < 30)
			printf(".\n");
		else
		{
			printf("Timed out.\nForcing quit.\n\n");
			exit(EXIT_FAILURE);
		}
	}

	CHECK(received = recvfrom(sok, &scout, sizeof(Packet), 0, NULL, NULL));
	if(scout.type == FIN)
	{
		printf("FIN received!\nSending ack and exiting.\n\n");
		display(scout);

		scout.type = ACK;
		scout.ack_num = scout.seq_num + 1;

		CHECK(sendto(sok, &scout, sizeof(Packet), 0, (struct sockaddr *) dist, sizeof(struct sockaddr_in)));

		printf("Ack packet sent.\n\n");
	}
	else
	{
		printf("Received wrong type packet!\nForcing quit.\n\n");
		exit(EXIT_FAILURE);
	}
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
	int domain = AF_INET;

	//Socket init and bind
	int sok;
	struct sockaddr_in dist;

	CHECK(sok = socket(domain, SOCK_DGRAM, IPPROTO_UDP));

	if (memset(&dist, '\0', sizeof(dist)) == NULL)
		raler(1, "memset error");

	dist.sin_family = domain;		  // distant sera IPv4
	dist.sin_port = htons(localport); // port local

	if (inet_aton(ipdist, &dist.sin_addr) == 0)
		raler(1, "inet_aton: adress not valid");

	CHECK(bind(sok, (struct sockaddr *)&dist, sizeof(struct sockaddr_in)));
	dist.sin_port = htons(distport);

	// Three-way handshake
	uint16_t saved_sequence = threewayhandshake(sok, &dist);
	//printf("\n\nSaved sequence: %d\n", saved_sequence);

	//Mode calls
	if (mode == 0)
	{
		stopandwait(sok, &dist);
	}
	else
	{
		//gobackn();
	}

	disconnection(sok, &dist, saved_sequence);
	printf("Disconnected.\n\n");
	CHECK(close(sok));
	exit(EXIT_SUCCESS);
}