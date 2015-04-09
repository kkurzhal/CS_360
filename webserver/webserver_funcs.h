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
#include <ctype.h>
#include <dirent.h>

#define MAX_IN 100
#define MAX_REQUEST 64
#define MAX_RESPONSE 1500

char* capitalize(char[]);
int check_request(char[], char[]);
int check_root_exists(char[]);
char* concat_file_path(char[], char[]);
char* get_file_path(char[], char[]);
long get_file_size(char[]);
char* get_MIME_type(char[]);
char* get_root();
char* make_header(char[], char[], int);
char* make_code(int);

#endif
