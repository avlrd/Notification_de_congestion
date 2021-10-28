#include "check.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char const *argv[])
{
	//Checking usage
	if(argc != 4)
	{
		fprintf(stdout, "usage: ./destination <IP_dist> <local_port> <dist_port>\n");
		return EXIT_FAILURE;
	}

	//Getting command args
	int ipdist =	atoi(argv[1]),
		localport =	atoi(argv[2]),
		distport =	atoi(argv[3]);

	//Accept dans un while (while(accept...)) pour ecouter en continu
   
	return EXIT_SUCCESS;
}