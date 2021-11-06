#include "../include/utils.h"

void stopandwait(int sok, struct sockaddr_in* dist)
{
	/*
		Le principe est d'envoyer un packet dans un while infini, de bloquer l'envoi de packet en attendant l'ack grace au recvfrom
	*/
	Packet p;
	p.id_flux = 1; //MODIFY WHEN TESTS ARE OK FOR BASIC IMPLEMENTATION
	p.type = DATA;
	p.seq_num = 0;
	p.ack_num = 0;
	p.ecn = 0;
	p.ewnd = 1;
	p.message = "Test";

	CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr*) dist, sizeof(struct sockaddr_in)));
	printf("Sent.\n");

	int received;
	socklen_t tmp = sizeof(struct sockaddr_in);
	CHECK((received = recvfrom(sok, &p, sizeof(Packet), 0, (struct sockaddr*) dist, &tmp)));
}
/*
void gobackn()
{

}
*/
int main(int argc, char const *argv[])
{
//Checking usage
	if(argc != 5)
	{
		fprintf(stderr, "usage: ./source <mode> <IP_dist> <local_port> <dist_port>\n");
		exit(EXIT_FAILURE);
	}

//Gettings command args
	const char*	arg1 = argv[1];
	int mode;
	if(strcmp(arg1, "stopandwait") == 0)
		mode = 0;
	else if(strcmp(arg1, "gobackn") == 0)
		mode = 1;
	else
	{
		fprintf(stderr, "Possible values for mode: <stopandwait> or <gobackn>");
		exit(EXIT_FAILURE);
	}
	const char*	ipdist = argv[2];
	int	localport =	atoi(argv[3]);
	int	distport =	atoi(argv[4]);

//Ergonomy
	int domain = AF_INET;

//Socket init and bind
	int sok;
	struct sockaddr_in dist;

	CHECK(sok = socket(domain, SOCK_DGRAM, 0));

	if(memset(&dist, '\0', sizeof(dist)) == NULL)
		raler(1, "memset error");

	dist.sin_family = domain;
	dist.sin_port = htons(distport);
	if(inet_aton(ipdist, &dist.sin_addr) == 0)
		raler(1, "inet_aton: adress not valid");

	CHECK(bind(sok, (struct sockaddr*) &dist, sizeof(struct sockaddr_in)));

//Function needed variables
	//int* ack_rcvd; *ack_rcvd = 1;

//Mode calls

	if(mode == 0)
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