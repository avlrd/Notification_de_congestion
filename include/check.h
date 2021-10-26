#ifndef CHECK_H
#define CHECK_H
/*
 * This file provides a cool macro to check the return values
 * without polluting your beautiful code. Few examples:
 *
 * CHECK((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);
 * CHECK((pid = fork()) != -1);
 * CHECK(wait(NULL) != -1);
 *
 * In your code, just add this to use it:
 * #include "check.h"
 * -Given to MI students by Cedric Bastoul and rearranged by Cedric Geissert
 */

#include <stdio.h> // For fprintf, perror
#include <stdlib.h> // For exit
#include <stdnoreturn.h> // For noreturn
#include <errno.h> // For perror
#include <stdarg.h> // For va_start/va_end

#define CHECK(op) \
  do { \
        if ((op) == -1){ \
          fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
          raler(1, #op); \
     } \
  } while (0)

#define CHECKT(opt) \
  do { \
        if ((opt) > 0){ \
          fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
          raler(1, #opt); \
     } \
  } while (0)

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

#endif /* __CHECK_H__ */
