#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Sizes and max numbers used in some functions
#define FORMATSIZE 52
#define MSIZE 44
#define MAX 255
#define SIZE 1024
#define NmaxT 4

//Packet types
#define ACK 16
#define RST 4
#define FIN 2
#define SYN 1
#define DATA 0

//CHECK and CHECKT macros used for testing lib functions' return values
//CHECK		-> testing if == -1
//CHECKT	-> testing if > 0
#define CHECK(op)                                           \
	do                                                      \
	{                                                       \
		if ((op) == -1)                                     \
		{                                                   \
			fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
			raler(1, #op);                                  \
		}                                                   \
	} while (0)

#define CHECKT(opt)                                         \
	do                                                      \
	{                                                       \
		if ((opt) > 0)                                      \
		{                                                   \
			fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
			raler(1, #opt);                                 \
		}                                                   \
	} while (0)

noreturn void raler(int syserr, char *msg, ...);

//Struct as described in project topic, types are used to describe number of
//bytes for each value, this way, size of packet is always 52bytes
typedef struct packet
{
	uint8_t id_flux;
	uint8_t type;
	uint16_t seq_num;
	uint16_t ack_num;
	uint8_t ecn;
	uint8_t ewnd;
	char message[MSIZE];
} Packet;

void display(Packet p);

int timeout(int sok, int delai);

#endif /* __UTILS_H__ */