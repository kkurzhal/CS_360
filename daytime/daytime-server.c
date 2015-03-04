/********************************
Author:		Kyle Kurzhal
Course:		CS 360
Professor:	Dr. Jerkins
Date:		2/6/2015
********************************/
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

#define SERVER_PORT 1113
#define MAX_PENDING 5
#define MAX_MSG 1004

int sock, new_sock;  //global variable for the sockets; done for closing in the signal handler

/*******************************************************
Input: sig - The signal received

Output: None

Description: Gracefully closes all sockets before exiting.
*******************************************************/
void softkill(int sig)
{
	printf("Gracefully closing socket and exiting...\n");

	if(close(new_sock))
		printf("Failed to close the last allocated socket.\n");

	if(close(sock))
		printf("Failed to close the original socket.\n");

	exit(1);
}

int main()
{
	struct sockaddr_in sin;
	unsigned int len;
	time_t tmptime;
	char* tmpmesg = NULL;

	signal(SIGINT, softkill);
	
	//build adddress data structure
	bzero((char *) &sin, sizeof(sin));	//zero out the address in terms of bytes
	sin.sin_family = AF_INET; //AF_INET - address family inet
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);	//order

	//passive open
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Error: The socket was not able to be opened.\n");
		exit(1);
	}

	if(bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0)
	{
		printf("Error: Unable to bind the socket to the address.\n");
		exit(1);
	}

	listen(sock, MAX_PENDING);

	while(1)
	{
		len = sizeof(sin);
		if((new_sock = accept(sock, (struct sockaddr *) &sin, &len)) < 0)
		{
			printf("Error: The connection was not accepted by the server.\n");
			exit(1);
		}

		len = 0;

		//get the current time
		tmptime = time(NULL);
		tmpmesg = ctime(&tmptime);

		//get the length of the string to send
		for(int i = 0; tmpmesg[i] != '\n'; i++)
		{
			len = len + 1;
		}

		//send datetime to client and print helpful message
		send(new_sock, tmpmesg, len, 0);
		printf("Sending: %s\n", tmpmesg);
		tmpmesg = NULL;

		close(new_sock);
	}

	close(sock);

	return 0;
}

