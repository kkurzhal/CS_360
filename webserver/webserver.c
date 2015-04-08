/********************************
Author:		Kyle Kurzhal
Course:		CS 360
Professor:	Dr. Jerkins
Date:		4/8/2015
********************************/
#include "webserver_funcs.h"

#define SERVER_PORT 8181
#define MAX_PENDING 5
#define MAX_MSG 1004
#define DEBUG 1

int killsig = 0;
void softkill(int);

int main(int argc, char* argv[])
{
	struct sockaddr_in sin;
	unsigned int len, packet_size = 100;
	int sock, new_sock;
	char mesg[packet_size];
	char *root = NULL, *header = NULL;
	int root_found = 0;

	//get the root folder for serving files
	while(root_found == 0)
	{
		root = get_root();
		if(check_root_exists(root) == 1)
		{
			//exit if the user specifies "EXIT"
			if(strncmp(root, "EXIT", 4) == 0)
			{
				printf("Now exiting...\n");
				free(root);
				root = NULL;
				exit(1);
			}
			else
				root_found = 1;
		}
	}

	//set up the signal detection
	signal(SIGINT, softkill);
	signal(SIGTERM, softkill);
	
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

	listen(sock, MAX_PENDING);mai
	len = sizeof(sin);

	//loop until the kill signal is activated
	while(killsig == 0)
	{
		//check if the new socket is created
		if((new_sock = accept(sock, (struct sockaddr *) &sin, &len)) < 0)
		{
			printf("Error: The connection was not accepted by the server.\n");
			killsig = 1;
		}
		else
		{
			//get the message and its length
			bzero(mesg, packet_size);
			len = recv(new_sock, mesg, packet_size, 0);
			printf("%s\n", mesg);
			
			//check to make sure that the message length is greater than 0
			if(len < 1)
				killsig = 1;
			else
			{
			}

			if(close(new_sock) == 0)
			{
				printf("New socket closed...\n");
			}

		}
	}

	if(close(sock) == 0)
	{
		printf("Main socket closed...\n");
	}

	free(root);
	root = NULL;

	return 0;
}

/*******************************************************
Input: sig - The signal received

Output: None

Description: Sets killsig, a global boolean variable which  represents
			 a recognized kill signal, to true.
*******************************************************/
void softkill(int sig)
{
	killsig = 1;
}
