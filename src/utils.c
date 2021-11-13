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
	printf("Flux: %d\nType: %d\nNum. Seq.: %d\nNum. Ack.: %d\nECN: %d\nFenêtre d'émission: %d\n\nMessage: %s", p.id_flux, p.type, p.seq_num, p.ack_num, p.ecn, p.ewnd, p.message);
	printf("\n\n");
}


/*
 * Function: timeout
 * --------------------
 * Le timeout détermine le temps d'attente maximal de la réponse. Si le timeout est écoulé, 
 * l'échange se termine avec un compte-rendu d'erreur, de même, la réception d'une réponse 
 * après la fin du timeout est refusée.
 *
 *  sock : int 
 *
 *  returns: nothing
 */
int timeout(int sok)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	/* Surveiller stdin (fd 0) en attente d'entrées */
	FD_ZERO(&rfds);		// efface un ensemble.
	FD_SET(sok, &rfds); // ajoute un descripteur dans un ensemble (la socket)

	/* Délai avant timeout : 5 secondes maxi */
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	// nfds : numéro du plus grand descripteur valide +1
	CHECK(retval = select(sok + 1, &rfds, NULL, NULL, &tv)); // -1
	/* Considérer tv comme indéfini maintenant ! */
	return retval;
}