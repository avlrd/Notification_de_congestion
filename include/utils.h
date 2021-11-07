#ifndef UTILS_H
#define UTILS_H

#include "check.h"
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
#define ACK 16
#define RST 4
#define FIN 2
#define SYN 1
#define DATA 0

typedef struct packet
{
	uint8_t id_flux;				//numero de flux du paquet
	uint8_t type;					//type: 16(ACK) 4(RST) 2(FIN) 1(SYN)
	uint16_t seq_num;				//Num Sequence
	uint16_t ack_num;				//Num Acquittement
	uint8_t ecn;					//notif de congestion
	uint8_t ewnd;					//Fenetre d'emission
	char* message;					//Contenu du message
}Packet;

void display(Packet p);
void test();

#endif /* __UTILS_H__ */