/********************************
Author:		Kyle Kurzhal
Course:		CS 360
Professor:	Dr. Jerkins
Date:		3/4/2015
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

#define SERVER_PORT 2983
#define MAX_PENDING 5
#define MAX_MSG 1004
#define DEBUG 1

int killsig = 0, finished = 0;

/*******************************************************
Input: sig - The signal received

Output: None

Description: Sets killsig, a global boolean variable which  represents
			 a recognized kill signal, to true.
*******************************************************/
void softkill(int sig)
{
	killsig = 1;
	finished = 1;
}


int main()
{
	struct sockaddr_in sin;
	unsigned int len, code_size = 3, message_size = 64, packet_size = code_size + message_size, joke_round = 0, joke_state = 0, joke_num = 4;
	int sock, new_sock, code_found, code_num = 7;
	char mesg[packet_size];
	char codes[code_num][code_size], response1[joke_num][message_size], response2[joke_num][message_size];

	//fill the codes array with codes
	strcpy(codes[0], "000");
	strcpy(codes[1], "100");
	strcpy(codes[2], "200");
	strcpy(codes[3], "300");
	strcpy(codes[4], "400");
	strcpy(codes[5], "500");
	strcpy(codes[6], "600");

	//fill the first response joke array
	strncpy(response1[0],"doughnut", message_size);
	strncpy(response1[1],"little old lady", message_size);
	strncpy(response1[2], "who", message_size);
	strncpy(response1[3], "Dewey", message_size);

	//file the second response joke array
	strncpy(response2[0], "Doughnut ask, it is a secret.", message_size);
	strncpy(response2[1], "I didn't know you could yodel!", message_size);
	strncpy(response2[2], "Are you an owl?", message_size);
	strncpy(response2[3], "Dewey have to keep telling silly jokes?", message_size);

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

	listen(sock, MAX_PENDING);
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
			// assume that the joke is not finished
			finished = 0;

			do
			{
				//get the message and its length
				bzero(mesg, packet_size);
				len = recv(new_sock, mesg, packet_size, 0);
				printf("%s\n", mesg);
				
				//check to make sure that the message length is greater than 0
				if(len < 1)
				{
					finished = 1;
					killsig = 1;
				
					strncpy(mesg, "600Errors aren't funny!", packet_size);
					send(new_sock, mesg, packet_size, 0);
				}
				else
				{
					//get the code and decode it

					if(strncmp(mesg, codes[joke_state], code_size) == 0)
					{					
						code_found = joke_state;
					}
					else
					{
						code_found = -1;
					}

					bzero(mesg, packet_size);

					//handle the appropriate code
					switch(code_found)
					{
						//think of the case representing the code divided by 100
						case 0:
							joke_state = 2;
							strncpy(mesg, "100knock knock", packet_size);
							break;
						case 2:
							joke_state = 4;
							strncpy(mesg, codes[3], code_size);
							strcat(mesg, response1[joke_round]);
							break;
						case 4:
							strncpy(mesg, codes[5], code_size);									
							strcat(mesg, response2[joke_round]);
							finished = 1;
							break;
						default:
							finished = 1;
							strncpy(mesg, "600Errors aren't funny!", packet_size);
					}

					send(new_sock, mesg, packet_size, 0);
					printf("%s\n", mesg);
				}
			}
			while(finished == 0);

			if(close(new_sock) == 0)
			{
				printf("New socket closed...\n");
			}

			joke_round = (joke_round + 1) % joke_num;
			joke_state = 0;
		}
	}

	if(close(sock) == 0)
	{
		printf("Main socket closed...\n");
	}

	return 0;
}

