/********************************
Author:		Kyle Kurzhal
Course:		CS 360
Professor:	Dr. Jerkins
Date:		3/6/2015
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

#define SERVER_PORT 2983
#define PACKET_SIZE 67

/*******************************************************
Input: cstr - The string that needs capitalizing.

Output: No return; changes *cstr in memory.

Description: Capitalize a string.
*******************************************************/
void capitalize(char *cstr)
{
	int i;

	for(i = 0; cstr[i] != '\0'; ++i)
	{
		cstr[i] = toupper(cstr[i]);
	}
}

void help(char *options[])
{
	printf("Option\tCode\tMessage\n");
	printf("%s\t%s\t%s\n", options[0], options[1], options[2]);
}

int main(int argc, char* argv[])
{
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host = NULL, mesg[PACKET_SIZE] = {0}, exit_str[] = "EXIT", response_mesg[PACKET_SIZE] = {0};
	int sock, finished = 0, valid_choice, i, j;
	const int opt_rows = 5, opt_columns = 3;
	char *options[5][3] = {
			{"0", "None", "Get help"},
			{"1", "000", "Tell me a joke"},
			{"2", "200", "Who's there?"},
			{"3", "400", " who?"},
			{"4", "600", "Errors aren't funny"}
		};
	char opt_choice[] = "0";
	strcpy(response_mesg, "NO RESPONSE");

	if(argc == 2)
	{
		host = argv[1];	//get the hostname if passed
	}

	while( host == NULL || (strncmp(host, exit_str, 4) != 0 && !(hp = gethostbyname(host)) ) )	//empty hostnames are not allowed, and check for an exit
	{
		if(host == NULL)
			host = (char*) malloc(PACKET_SIZE);

		printf("Please input a valid hostname, or specify \"EXIT\" to exit: ");
		fflush(stdout);
		scanf("%s", host);
		printf("\n");

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


	//print the help message
	printf("Option\tCode\tMessage\n------\t----\t-------\n");
	for(i = 0; i < opt_rows; ++i)
	{
		printf("%s\t%s\t%s\n", options[i][0], options[i][1], options[i][2]);

	}

	while(finished == 0)
	{
		valid_choice = -1;

		//prompt for message
		do
		{
			fflush(stdout);
			printf("Please choose an option: ");
//			fflush(stdout);
			scanf("%c", opt_choice);
			printf("\n");

			//check if choice is valid
			for(i = 0; i < opt_rows; ++i)
			{
				if(strncmp(opt_choice, options[i][0], 1) == 0)
				{
					valid_choice = i;
					break;
				}
			}
		}
		while(valid_choice == -1);

		//set up message
		switch(valid_choice)
		{
			case 0:
				//print the help message
				printf("Option\tCode\tMessage\n------\t----\t-------\n");
				for(i = 0; i < opt_rows; ++i)
				{
					printf("%s\t%s\t%s\n", options[i][0], options[i][1], options[i][2]);
				}
				break;
			case 3:
				strncpy(mesg, options[3][1], PACKET_SIZE);
				//tack on the previous response_mesg to the "(response_mesg) who?" message
				strcat(mesg, response_mesg);
				strcat(mesg, options[3][2]);
				break;
			default:
				//the default handles cases for messages 1,2,4
				strncpy(mesg, options[valid_choice][1], PACKET_SIZE);
				strcat(mesg, options[valid_choice][2]);
				break;
		}

		if(valid_choice > 0)
		{
			//send message
			send(sock, mesg, PACKET_SIZE, 0);
			printf("Sending: %s\n", &mesg[3]);

			//receive a response
			bzero(mesg, PACKET_SIZE);
			recv(sock, mesg, PACKET_SIZE, 0);
			strncpy(response_mesg, &mesg[3], PACKET_SIZE);
			printf("Received: %s\n", &mesg[3]);

			//check if the response is 600 or 500
			if(strncmp(options[4][1], mesg, 3) == 0 || strncmp("500", mesg, 3) == 0)
			{
				finished = 1;
			}
			else
			{
				bzero(mesg, PACKET_SIZE);
			}
		}
	}

	return 0;
}
