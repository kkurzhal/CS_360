#include "webserver_funcs.h"

int get_file()
{
	return 1;
}

//need to dynamically calculate the size of the string based on the length of the message, the code text, and the content type text.
char* make_header(char code_mesg[], char content_type[], int length)
{
	char length_str[2];

	char *header = malloc(122 * sizeof(char));

	strcpy(header, "HTTP/1.0 ");

	//get the code message
	strcat(header, code_mesg);

	strcat(header, "\nConnection: close\nContent-Type: ");

	//get the content type
	strcat(header, content_type);

	strcat(header, "\nContent-Length: ");
	
	//get the length as a c-string
	sprintf(length_str, "%d", length);
	strcat(header, length_str);

	strcat(header, "\nServer: cs360httpd/1.0 (Unix)\n\n");
	return header;
}

char* get_MIME_type(char file_name[])
{
	return "";
}
