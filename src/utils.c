#include "utils.h"

noreturn void raler(int syserr, char *msg, ...)
{
    va_list ap;

    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    fprintf(stderr, "\n");
    va_end(ap);

    if (syserr == 1)
        perror("");

    exit(EXIT_FAILURE);
}


void display(Packet p)
{
	printf("----- Message reçu -----\n\n");
	printf("Flux: %d\n", p.id_flux);
	switch(p.type)
	{
		case DATA:
			printf("Type: DATA\n");
			break;
		
		case SYN:
			printf("Type: SYN\n");
			break;

		case FIN:
			printf("Type: FIN\n");
			break;
		
		case RST:
			printf("Type: RST\n");
			break;

		case ACK:
			printf("Type: ACK\n");
			break;
		
		default:
			break;
	}
	printf("Num. Seq.: %d\nNum. Ack.: %d\nECN: %d\nFenêtre d'émission: %d\n\nMessage: %s\n\n------------------------\n\n", p.seq_num, p.ack_num, p.ecn, p.ewnd, p.message);
}

int timeout(int sok,int delai)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	// Surveiller socket en attente d'entrées
	FD_ZERO(&rfds);		// efface un ensemble.
	FD_SET(sok, &rfds); // ajoute un descripteur dans un ensemble (la socket)

	// Délai avant timeout : delai = 5 secondes maxi par exemple
	tv.tv_sec = delai;
	tv.tv_usec = 0;

	// nfds : numéro du plus grand descripteur valide +1
	CHECK(retval = select(sok + 1, &rfds, NULL, NULL, &tv)); // -1
	// Considérer tv comme indéfini maintenant !
	return retval;
}