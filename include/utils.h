/**
 * \file utils.h
 * \brief Utils header
 * \authors Cedric GEISSERT, Arthur VILLARD
 * \version 0.1
 * \date 17 november 2021
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>		 // For fprintf, perror
#include <stdlib.h>		 // For exit
#include <stdnoreturn.h> // For noreturn
#include <errno.h>		 // For perror
#include <stdarg.h>		 // For va_start/va_end

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Avoid implicit declaration inet_aton

/*
 * Macro: Size  
 * --------------------  
 */
#define FORMATSIZE 52
#define SIZE 1024

/*
 * Macro: Segments
 * -------------------- 
 */
#define ACK 16
#define RST 4
#define FIN 2
#define SYN 1
#define DATA 0

/** \def CHECK
* \brief Check the return values (system calls)
* \param op return value
* CHECK((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)));
* CHECK((pid = fork()));
* CHECK(wait(NULL));
*/
#define CHECK(op)                                           \
	do                                                      \
	{                                                       \
		if ((op) == -1)                                     \
		{                                                   \
			fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
			raler(1, #op);                                  \
		}                                                   \
	} while (0)

/** \def CHECKT
* \brief Check the return values (thread functions)
* \param opt return value
* CHECKT(errno = pthread_join(thread_id[i], NULL));
*/
#define CHECKT(opt)                                         \
	do                                                      \
	{                                                       \
		if ((opt) > 0)                                      \
		{                                                   \
			fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
			raler(1, #opt);                                 \
		}                                                   \
	} while (0)

/**
 * \fn noreturn void raler(int syserr, char *msg, ...)
 * \brief Complain with a message if syserr == 1.
 * \param syserr system error
 * \param msg    message
 * \param dots   various use
 */
noreturn void raler(int syserr, char *msg, ...);

/** \struct packet
* \brief Each layer of a packet  
*/
typedef struct packet
{
	uint8_t id_flux;  /**!< numero de flux du paquet           */
	uint8_t type;	  /**!< type: 16(ACK) 4(RST) 2(FIN) 1(SYN) */
	uint16_t seq_num; /**!< Num Sequence                       */
	uint16_t ack_num; /**!< Num Acquittement                   */
	uint8_t ecn;	  /**!< notif de congestion                */
	uint8_t ewnd;	  /**!< Fenetre d'emission                 */
	char *message;	  /**!< Contenu du message                 */
} Packet;

/**
 * \fn void display(Packet p)
 * \brief Display packet layers information
 * \param p Packet
 */
void display(Packet p);

/**
 * \fn void test()
 * \brief Test a packet
 */
void test();

#endif /* __UTILS_H__ */