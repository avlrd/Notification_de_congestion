#include "utils.h"

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
	p.message = "Test";

	// Send, sendto, et sendmsg permettent de transmettre un mes­sage à destination d'une autre socket.

	CHECK(sendto(sok, &p, sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
	printf("Sent.\n");

	int received;
	socklen_t tmp = sizeof(struct sockaddr_in);
	CHECK((received = recvfrom(sok, &p, sizeof(Packet), 0, (struct sockaddr *)dist, &tmp)));
}
/*
void gobackn()
{

}
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

	//Function needed variables
	//int* ack_rcvd; *ack_rcvd = 1;

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