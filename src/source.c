#include "../include/check.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 1024


void stopandwait()
{
	/*
		Le principe est d'envoyer un packet dans un while infini, de bloquer l'envoi de packet en attendant l'ack grace au recvfrom
	*/
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
	int mode =		(argv[1] == "gobackn") ? 1 : 0, //0 -> stopandwait (default)
		ipdist =	atoi(argv[2]),
		localport =	atoi(argv[3]),
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
	if(inet_pton(domain, argv[2], &dist.sin_addr) != 1)
		raler(1, "inet_pton error");

	if(mode == 0)
	{
		stopandwait();
	}
	else
	{
		gobackn();
	}

	fprintf(stdout, "Done.\n");
	CHECK(close(sok));
	exit(EXIT_SUCCESS);
}