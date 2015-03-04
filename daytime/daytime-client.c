/********************************
Author:		Kyle Kurzhal
Course:		CS 360
Professor:	Dr. Jerkins
Date:		2/6/2015
********************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#define SERVER_PORT 1113
#define MAX_LINE 256

/*******************************************************
Input: cstr - The string that needs capitalizing.

Output: No return; changes *cstr in memory.

Description: Capitalize a string.
*******************************************************/
void capitalize(char *cstr)
{
	for(int i = 0; cstr[i] != '\0'; ++i)
	{
		cstr[i] = toupper(cstr[i]);
	}
}

int main(int argc, char* argv[])
{
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host = NULL, buf[MAX_LINE] = {0}, exit_str[] = "EXIT";
	int sock;

	if(argc == 2)
	{
		host = argv[1];	//get the hostname if passed
	}

	while( host == NULL || (strncmp(host, exit_str, 4) != 0 && !(hp = gethostbyname(host)) ) )	//empty hostnames are not allowed, and check for an exit
	{
		if(host == NULL)
			host = (char*) malloc(MAX_LINE);

		printf("Please input a valid hostname, or specify \"EXIT\" to exit: ");
		fflush(stdout);
		scanf("%s", host);

		capitalize(host);
	}

	if(argc == 2 && host != argv[1])
		free(host);

	if( strncmp(host, exit_str, 4) == 0 )
	{
		printf("Now exiting...\n");
		exit(1);
	}

	// build address data structure
	bzero( (char*) &sin, sizeof(sin));
	sin.sin_family = AF_INET;
	bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);

	//open the socket
	if( (sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Error: Could not properly open the socket .\n");
		exit(1);
	}

	if( connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0)
	{
		printf("Error: Could not properly connect to the socket.\n");
		close(sock);
		exit(1);
	}

	//actually get the datetime
	send(sock, buf, 0, 0);
	recv(sock, buf, 30, 0);
	printf("%s\n", buf);

	return 0;
}
