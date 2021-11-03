#include "../include/check.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>			//avoid implicit declaration inet_aton

#define FORMATSIZE 52
#define SIZE 1024

typedef struct packet
{
	int* id_flux;				//numero de flux du paquet
	int* type;					//type: 16(ACK) 4(RST) 2(FIN) 1(SYN)
	int* seq_num;				//Num Sequence
	int* ack_num;				//Num Acquittement
	int* ecn;					//notif de congestion
	int* ewnd;					//Fenetre d'emission
	void* data;					//Donnée, peut etre de n'importe quel type
}Packet;

void stopandwait(int* ack_check)
{
	/*
		Le principe est d'envoyer un packet dans un while infini, de bloquer l'envoi de packet en attendant l'ack grace au recvfrom
	*/
	Packet* p = malloc(FORMATSIZE);
	p->id_flux = 1; //MODIFY WHEN TESTS ARE OK FOR BASIC IMPLEMENTATION
	p->type = 0;	//DATA type
	p->ack_num = 0;
	p->ecn = 1;
	p->ewnd = 1;
	p->data = "Test.\n"; //NOT SURE

	while(1)
	{
		if(ack_check == 1)
		{
			
			
		}
					
	}
}

void gobackn()
{

}


int main(int argc, char const *argv[])
{
//Checking usage
	if(argc != 5)
	{
		fprintf(stderr, "usage: ./source <mode> <IP_dist> <local_port> <dist_port>\n");
		exit(EXIT_FAILURE);
	}

//Gettings command args
	const char*	mode =		argv[1];
	const char*	ipdist =	argv[2];

	int			localport =	atoi(argv[3]),
				distport =	atoi(argv[4]);

//Ergonomy
	int domain = AF_INET;

//Socket init
	int sok;
	struct sockaddr_in dist;

	CHECK(sok = socket(domain, SOCK_DGRAM, 0));

	if(memset(&dist, '\0', sizeof(dist)) == NULL)
		raler(1, "memset error");

	dist.sin_family = domain;
	dist.sin_port = htons(distport);
	if(inet_aton(ipdist, &dist.sin_addr) == 0)
		raler(1, "inet_aton: adress not valid");

//Function needed variables
	int* ack_rcvd; *ack_rcvd = 1;

//Mode calls
	if(mode == 0)
	{
		stopandwait(ack_rcvd);
	}
	else
	{
		gobackn();
	}

	fprintf(stdout, "Done.\n");
	CHECK(close(sok));
	exit(EXIT_SUCCESS);
}

/*
	//Sending each packet
	for (i = 0; i < NB_PACKETS; i++)
	{
		printf("Send the packet number %d\n", i);
		sprintf(buf, "Packet content %d", i);
		/ We need (struct sockaddr *) because we are passing it a 
		memory layout-compatible (struct sockadd_in *) /
		CHECK(sendto(s, buf, BUFLEN, 0, (struct sockaddr *)&saddr_dest,
					 sizeof(saddr_dest)));
	}
*/