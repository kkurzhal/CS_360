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

	new_cstr = malloc(counter + 1);

	for(; index < counter; ++index)
	{
		new_cstr[index] = toupper(cstr[index]);		
	}

	//Add a null byte on the end
	new_cstr[index] = '\0';

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
	in_file = NULL;
	return file_length;
}

//need to dynamically calculate the size of the string based on the length of the message, the code text, and the content type text.
char* make_header(char code_mesg[], char content_type[], int length)
{
	char length_str[2];

	char *header = malloc(122);

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

char* make_code(int code_flag)
{
	char* code = NULL;
	code_flag = 0;

	switch(code_flag)
	{
		case 1:
			strcpy(code, "200 OK");
			break;
		case 2:
			strcpy(code, "400 Bad Request");
			break;
		case 3:
			strcpy(code, "404 Not Found");
			break;
		case 4:
			strcpy(code, "414 Request-URI Too Long");
			break;
		case 5:
			strcpy(code, "501 Not Implemented");
			break;
		default:
			strcpy(code, "NONE");
	}

	return code;
}

int check_request(char request[], char root[])
{
	char expected_request[] = "GET /", *p_file = NULL;
	int index = 0, request_len = strlen(request), file_found_flag = 0;
	char tmp_request[request_len];
	FILE *file_found;
	
	//add copy of request to tmp_request
	strcpy(tmp_request, request);

	//first check to make sure that there is a line feed at the end of the request
	//if there is no newline, return a 400 code flag
	if(tmp_request[request_len - 1] != '\n')
		return 2;
	else
	{
		//continue checking the request to see if it is properly formatted
		for(; index < request_len; ++index)
		{
			if(index < 5 && tmp_request[index] != expected_request[index])
			{
				//if it is the request type that is wrong, then return the 501 code flag
				if(index < 3)
					return 5;
				//otherwise, return the incorrect format, 400, code flag
				else
					return 2;
			}
			else if(index > 5)
			{
				//break and the index can be reused when checking if a file exists
				if(tmp_request[index] == ' ')
					break;
			}
		}
	}

	//check if the request is too large for the buffer
	//if so, return 414 code flag
	if(request_len > MAX_REQUEST)
		return 4;

	//check if the file exists
	//fill out the rest of the request with null bytes to account for the concatenating function
	for(; index < request_len; ++index)
		tmp_request[index] = '\0';

	//concatenate the file with the root to be sure that the file exists
	p_file = &tmp_request[4];
	p_file = concat_file_path(tmp_request, root);

	//attempt to open the file, and set the flag if it is found
	file_found = fopen(p_file, "r");
	if(file_found != NULL);
	{
		file_found_flag = 1;
		fclose(file_found);
		file_found = NULL;
	}

	//if the file is not found, then return the flag for code 404
	if(file_found_flag == 0)
		return 3;
	//otherwise, return a 200 code flag
	else
		return 1;
}

char* concat_file_path(char file_requested[], char root[])
{
	//get the lengths of the strings
	int root_len = strlen(file_requested), file_r_len = strlen(file_requested);
	//create a pointer for the file path
	char* file_finder = malloc(root_len + file_r_len), *slash_found = NULL;
	strcpy(file_finder, root);

	//only allow one forward slash
	if(root[root_len] == '/' && file_requested[0] == '/')
		slash_found = &file_requested[1];
	else
		slash_found = &file_requested[0];

	strcat(file_finder, slash_found);

	return file_finder;
}

char* get_MIME_type(char file_name[])
{
	//create a new c-string
	char *mime;
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
	{
		mime = malloc(strlen(types[1][index]) + 1);
		strcpy(mime, types[1][index]);
	}
	else
	{
		mime = malloc(5);
		strcpy(mime, "NONE\0");
	}

//	free(cap_file_name);

	return mime;
}

int check_root_exists(char root[])
{
	int flag = 0;
	//attempt to open the directory
	DIR *tmp_dir = opendir(root);

	//check if the directory was opened
	if(tmp_dir != NULL)
		flag = 1;
	
	closedir(tmp_dir);
	tmp_dir = NULL;
	//return a true/false value determined by an existing directory
	return flag;
}

char* get_root()
{
	char *root = malloc(MAX_IN);
	printf("\nPlease input a valid root folder, or specify \"EXIT\" to exit: ");
	fflush(stdout);
	//get the root folder
	scanf("%s", root);

	return root;
}
