#include "utils.h"

/*
void gobackn(int sok, struct sockaddr_in *dist)
{
	int fenetre = 1; // taille de la fenetre emission
	int Msg2bSend = 0;
	// nbMsgBuffer tous les messages qu'on veut envoyer.

	Packet messageP;

	while (nbMsgBuffer < 255) // nb msg qu'on veut envoyer
	{						  // quitter saisie terminal des messages
		if (quitter(messageP.message))
			break;
		// lis les messages sur l'entree standard
		fgets(messageP.message, MSIZE, stdin);
		// copie les messages dans le buffer
		strcpy(&MsgBuffer[nbMsgBuffer].message, &messageP.message);
		nbMsgBuffer++;
	}

	while (Msg2bSend < nbMsgBuffer) // traiter donnees
	{
		for (size_t i = 0; i < fenetre; i++) // envoi n messages en fct de la fenetre
		{
			if (Msg2bSend + i == nbMsgBuffer)
				break;
			CHECK(sendto(sok, &MsgBuffer[i + Msg2bSend], sizeof(Packet), 0, (struct sockaddr *)dist, sizeof(struct sockaddr_in)));
		}

		long received;
		socklen_t tmp = sizeof(struct sockaddr_in);
		// serveur a acquitté tous les messages envoyés
		// A FAIRE : 
		if (timeout(sok)) // >1
		{ 
			printf("Des données sont disponibles maintenant\n");
			break; // pas de timeout, on sort de la boucle
		}
		// FD_ISSET(0, &rfds) est vrai
		else // 0 
		{ 
			printf("Aucun acquittement recu\n");
			printf("Aucune données durant les 5 secondes\n");
		}
		CHECK((received = recvfrom(sok, &MsgBuffer[Msg2bSend], sizeof(Packet), 0, (struct sockaddr *)dist, &tmp)));
		fenetre += 1; // quand réussi à acquitter les messages
		Msg2bSend += fenetre;
	}
}
*/