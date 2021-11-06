#include "../include/utils.h"

int main(int argc, char const *argv[])
{
	//Checking usage
	if(argc != 4)
	{
		fprintf(stdout, "usage: ./destination <IP_dist> <local_port> <dist_port>\n");
		exit(EXIT_FAILURE);
	}

	//Getting command args
	const char*	ipdist = argv[1];
	int	localport =	atoi(argv[2]);
	int	distport =	atoi(argv[3]);

	int sok;
	struct sockaddr_in dist;
	socklen_t dist_size = sizeof(struct sockaddr_in);
	int domain = AF_INET;

	CHECK(sok = socket(domain, SOCK_DGRAM, 0));

	if(memset(&dist, '\0', sizeof(dist)) == NULL)
		raler(1, "memset error");

	dist.sin_family = domain;
	dist.sin_port = htons(distport);
	if(inet_aton(ipdist, &dist.sin_addr) == 0)
		raler(1, "inet_aton: adress not valid");

	CHECK(bind(sok, (struct sockaddr*) &dist, sizeof(struct sockaddr_in)));

	while(1)
	{
		Packet p;
		int received;
		CHECK((received = recvfrom(sok, &p, sizeof(Packet), 0, (struct sockaddr*) &dist, &dist_size)));
		if(received > 0)
		{
			switch(p.type)
			{
				case DATA:
					fprintf(stdout, "Packet received\n");
					display(p);
					p.type += ACK;
					p.ack_num = p.seq_num + 1;
					p.message = "Acquittement";
					CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr*) &dist, dist_size));
					fprintf(stdout, "Ack sent\n");
					break;
									
				case ACK:

				case FIN:

				case SYN:

				case ACK + FIN:
					break;

				case RST:
					break;
			}
		}
	}
   
	exit(EXIT_SUCCESS);
}