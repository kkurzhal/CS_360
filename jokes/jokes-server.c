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

#define SERVER_PORT 2983
#define MAX_PENDING 5
#define MAX_MSG 1004
#define DEBUG 1

int killsig = 0;

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

int main()
{
	struct sockaddr_in sin;
	unsigned int len, code_size = 3, message_size = 64, joke_state = 0, joke_num = 4, finished;
	int sock, new_sock, code_found, code_num = 7;
	char mesg[code_size + message_size];
	char codes[code_num][code_size], response1[joke_num][message_size], response2[joke_num][message_size];

//	codes = {"000", "100", "200", "300", "400", "500", "600"};

	strcpy(codes[0], "000");
	strcpy(codes[1], "100");
	strcpy(codes[2], "200");
	strcpy(codes[3], "300");
	strcpy(codes[4], "400");
	strcpy(codes[5], "500");
	strcpy(codes[6], "600");

	//initialize the response messages
/*	for(unsigned int i = 0; i < joke_num; ++i)
	{
		bzero(response1[i], message_size);
		bzero(response2[i], message_size);
	}
*/
	strncpy(response1[0],"doughnut", message_size);
	strncpy(response1[1],"little old lady", message_size);
	strncpy(response1[2], "who", message_size);
	strncpy(response1[3], "Dewey", message_size);

	strncpy(response2[0], "Doughnut ask, it is a secret.", message_size);
	strncpy(response2[1], "I didn't know you could yodel!", message_size);
	strncpy(response2[2], "Are you an owl?", message_size);
	strncpy(response2[3], "Dewey have to keep telling silly jokes?", message_size);

//	response1 = {"doughnut", "little old lady", "who", "Dewey"};
//	response2 = {"Doughnut ask, it is a secret.", "I didn't know you could yodel!", "Are you an owl?", "Dewey have to keep telling silly jokes?"};

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
	len = sizeof(sin);

	while(killsig == 0)
	{
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
				bzero(mesg, code_size + message_size);
				len = recv(new_sock, mesg, code_size + message_size, 0);
				
				//check to make sure that the message length is correct
//				len = code_size + message_size; //temporary for testing
//				if(len != code_size + message_size)
				if(len > code_size + message_size || len < 1)
				{
					finished = 1;
					killsig = 1;					

					if(DEBUG == 1)
					{
						printf("Message size does not match expected size.\n");
						printf("Message was: %s\n", mesg);
						printf("Message length: %d\n", len);
					}

					//bzero(mesg, code_size + message_size);					
					strncpy(mesg, "600Errors aren't funny!", code_size + message_size);
					send(new_sock, mesg, code_size + message_size, 0);
				}
				else
				{
					//get the code and decode it
					code_found = -1;
					for(int i = 0; i < code_num && code_found == -1; ++i)
					{
						if(strncmp(mesg, codes[i], code_size) == 0)
							code_found = i;
					}

					if(code_found != -1)
					{

						//handle the appropriate code
						switch(code_found)
						{
							//keep in mind, there might be a problem rewriting to mesg!  Maybe need to allocate new memory?

							//think of the case representing the code divided by 100
							case 0:
								strncpy(mesg, "100knock knock", code_size + message_size);
								break;
/*							case 1:
								printf("Something went wrong!  Received code 100!\n");
								break;
*/							case 2:
								strncpy(mesg, "300", code_size);
								strcat(mesg, response1[joke_state]);
								break;
/*							case 3:
								printf("Somethings went wrong!  Received code 300!\n");
								break;
*/							case 4:
								strncpy(mesg, "500", code_size);									
								strcat(mesg, response2[joke_state]);
								finished = 1;
								break;
/*							case 5:
								printf("Something went wrong!  Received code 500!\n");
								break;
							case 6:
								*mesg = "600Errors aren't funny!";
								break;
*/							default:
								//the code needs to be revamped so that there is no need for a default
								if(DEBUG == 1)
								{
									printf("Message matched a code, but not one of the valid ones.\n");
									printf("Message was: %s\n", mesg);
								}

								//bzero(mesg, code_size + message_size);
								strncpy(mesg, "600Errors aren't funny!", code_size + message_size);
						}

						send(new_sock, mesg, code_size + message_size, 0);
					}
					else
					{
						if(DEBUG == 1)
						{
							printf("Message length matched, but no code was found.\n");
							printf("Message was: %s\n", mesg);
						}

						bzero(mesg, code_size + message_size);
						strncpy(mesg, "600Errors aren't funny!", code_size + message_size);
						send(new_sock, mesg, code_size + message_size, 0);
					}
				}
			}
			while(finished == 0);

			close(new_sock);
			joke_state = (joke_state + 1) % joke_num;
		}
	}

	close(sock);

	return 0;
}

