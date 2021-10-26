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
    if (argc != 5)
        return EXIT_FAILURE;

    int m = atoi(argv[1]), ip = atoi(argv[2]), pl = atoi(argv[3]), pe = atoi(argv[4]);
   
    return EXIT_SUCCESS;
}