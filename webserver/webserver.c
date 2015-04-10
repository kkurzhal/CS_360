/********************************
Author:		Kyle Kurzhal
Course:		CS 360
Professor:	Dr. Jerkins
Date:		4/8/2015
********************************/
#include "webserver_funcs.h"

#define SERVER_PORT 8181
#define MAX_PENDING 5
#define MAX_MSG 1024
#define DEBUG 1

int killsig = 0;
void softkill(int);

int main(int argc, char* argv[])
{
	struct sockaddr_in sin;
	unsigned int len;
	int sock, new_sock;
	char mesg[MAX_RESPONSE];
	char *root = NULL, *file_name = NULL, *response_code = NULL, *content_type = NULL,  *header = NULL;
	int root_found = 0, code_flag = 0, file_size = 0;

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
			//get the message and its length
			bzero(mesg, MAX_RESPONSE);
			len = recv(new_sock, mesg, MAX_MSG, 0);
			printf("%s\n", mesg);
			
			//check to make sure that the message length is greater than 0
			if(len < 1)
				killsig = 1;
			else
			{
				//get the file path and the file size
				file_name = get_file_path(mesg, root);
				printf("Got file name: %s\n", file_name);
				file_size = get_file_size(file_name);
				printf("Got file size: %d\n", file_size);

				//check the request and get the code flag
				code_flag = check_request(mesg, root);
				printf("Got code flag...\n");
			
				//make the correct response code
				response_code = make_code(code_flag);
				printf("Got response code...\n");

				//get the MIME type
				content_type = get_MIME_type(file_name);
				printf("Got MIME type...\n");

				//make the header
				header = make_header(response_code, content_type, file_size);
				printf("Got header...\n");

				printf("----------------------HEADER START----------------------\n%s\n----------------------HEADER END----------------------\n", header);

				//need to send back the message here
			}

			//remember to close the new socket
			if(close(new_sock) == 0)
			{
				printf("New socket closed...\n");
			}

			//necessary to reset the ints?

			//free the memory for the response code
			free(file_name);
			file_name = NULL;

			free(response_code);
			response_code = NULL;

			free(content_type);
			content_type = NULL;

			free(header);
			header = NULL;

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
