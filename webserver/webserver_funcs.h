#ifndef WEBSERVER_FUNCS
#define WEBSERVER_FUNCS

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

int get_file();
char* make_header(char[], char[], int);
char* get_MIME_type(char[]);

#endif
