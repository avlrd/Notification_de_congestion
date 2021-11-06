#include "../include/utils.h"

void display(Packet p)
{
	printf("----- Message reçu -----\n\n");
	printf("Flux: %d\nType: %d\nNum. Seq.: %d\nNum. Ack.: %d\nECN: %d\nFenêtre d'émission: %d\n\nMessage: %s", p.id_flux, p.type, p.seq_num, p.ack_num, p.ecn, p.ewnd, p.message);
	printf("\n\n");
}

void test()
{
	Packet p;
	p.id_flux = 1;
	p.type = DATA;
	p.seq_num = 1;
	p.ack_num = 0;
	p.ecn = 0;
	p.ewnd = 1;
	p.message = "Message de test.";

	display(p);
}
