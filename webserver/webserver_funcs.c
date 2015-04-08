#include "webserver_funcs.h"

/*******************************************************
Input: cstr - The string that needs capitalizing.

Output: No return; changes *cstr in memory.

Description: Capitalize a string.
*******************************************************/
char* capitalize(char cstr[])
{
	int index = 0, counter = 0;
	char *new_cstr;

	while(cstr[counter] != '\0')
	{
		++counter;
	}

	new_cstr = malloc(counter);

	for(; index < counter; ++index)
	{
		new_cstr[index] = toupper(cstr[index]);		
	}

	return new_cstr;
}

long get_file_size(char file_d[])
{
	long file_length = 0;	
	FILE *in_file = fopen(file_d, "r");
	
	//go to the end of the file
	fseek(in_file, 0L, SEEK_END);

	//get the position/length of the cursor
	file_length = ftell(in_file);

	fclose(in_file);
	return file_length;
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
	//create a new c-string
	char *mime = malloc(14);
	char *types[2][6] = 
	{
		{".html", ".css", ".js", ".jpeg", ".png", ".gif"},
		{"text/html", "text/css", "application/js", "image/jpeg", "image/png", "image/gif"}
	};

	int index = 0;

	//capitalize the file name for case insensitive substring searching
//	char *cap_file_name = capitalize(file_name);

	//find which file type matches, if any, according to the substring
	for(; index < 6; ++index)
	{
		if(strstr(file_name, types[0][index]))
			break;
	}

	//copy the appropriate value into the "mime" variable
	if(index < 6)
		strcpy(mime, types[1][index]);
	else
		strcpy(mime, "NONE");

//	free(cap_file_name);

	return mime;
}
