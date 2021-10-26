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
    if (argc != 4)
        return EXIT_FAILURE;

    int ip = atoi(argv[1]), pl = atoi(argv[2]), pe = atoi(argv[3]);
   
    return EXIT_SUCCESS;
}