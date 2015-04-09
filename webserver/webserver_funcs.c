#include "webserver_funcs.h"

/*******************************************************
Input: cstr - The string that needs capitalizing.

Output: Returns a c-string to a new capitalized string
		of the input.

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

	new_cstr = (char*) malloc(counter + 1);

	for(; index < counter; ++index)
	{
		new_cstr[index] = toupper(cstr[index]);		
	}

	//Add a null byte on the end
	new_cstr[index] = '\0';

	return new_cstr;
}


/*******************************************************
Input: request - The request c-string that needs to be
				 validated.

	   root - The c-string representing the root folder
			  that files may be accessed from in the system.

Output: Returns an integer flag representing a specific
		code message for the HTTP header.

Description: Checks the request given in order to
			 determine what type of response must be
			 given to the HTTP header.
*******************************************************/
int check_request(char request[], char root[])
{
	char expected_request[] = "GET /", *p_file = NULL;
	int index = 0, request_len = strlen(request), file_found_flag = 0;
	char tmp_request[request_len];
	FILE *file_found = NULL;
	
	//add copy of request to tmp_request
	strcpy(tmp_request, request);

	//check if the request is too large for the buffer
	//if so, return 414 code flag
	if(request_len > MAX_REQUEST)
		return 4;

	//check to make sure that there is a line feed at the end of the request
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
				if(tmp_request[index] == ' ' || tmp_request[index] == '\n')
					break;
			}
		}
	}

	//check if the file exists
	p_file = get_file_path(request, root);

	//attempt to open the file, and set the flag if it is found
	file_found = fopen(p_file, "r");

	if(file_found != NULL)
	{
		file_found_flag = 1;
		fclose(file_found);
		file_found = NULL;
	}

	free(p_file);
	p_file = NULL;

	//if the file is not found, then return the flag for code 404
	if(file_found_flag == 0)
		return 3;
	//otherwise, return a 200 code flag
	else
		return 1;
}


/*******************************************************
Input: root - The c-string representing the root folder
			  that will be checked

Output: Returns 1 for True, and 0 for False.

Description: Checks if the specified root folder does
			 actually exist.
*******************************************************/
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


/*******************************************************
Input: file_requested - The c-string file that needs to be
						concatenated to the root folder.

	   root - The c-string representing the root folder
			  to which a file will be concatenated.

Output: Returns a c-string of the concatenated root folder
		and the specified file name.

Description: Concatenates the specified root folder and
			 file name as a single path.
*******************************************************/
char* concat_file_path(char file_requested[], char root[])
{
	//get the lengths of the strings
	int root_len = strlen(root), file_r_len = strlen(file_requested);
	//create a pointer for the file path
	char* file_finder = (char*) malloc(root_len + file_r_len), *slash_found = NULL;
	strcpy(file_finder, root);

	//only allow one forward slash
	if(root[root_len - 1] == '/' && file_requested[0] == '/')
		slash_found = &file_requested[1];
	else
		slash_found = &file_requested[0];

	strcat(file_finder, slash_found);

	return file_finder;
}


/*******************************************************
Input: request - The request c-string received from the
				 HTTP request.

	   root - The root folder c-string from which files
			  may be accessed in the system.

Output: Returns the c-string of the file path.

Description: Strips out the name of the file from the
			 request, and formats it as a complete file
			 path.
*******************************************************/
char* get_file_path(char request[], char root[])
{
	int request_len = strlen(request), index = 0;
	char *file_path = NULL, *tmp_slash = NULL, tmp_request[request_len];

	//make a copy of the request for manipulation
	strcpy(tmp_request, request);
	tmp_slash = strstr(tmp_request, "/");

	//if the no "/" is found, then exit and return "NONE"
	if(tmp_slash == NULL)
	{
		file_path = (char*) malloc(5);
		strcpy(file_path, "NONE");
	}
	//otherwise, fill out with null bytes in spaces following the destination address
	else
	{
		//first find the "/"
		for(; tmp_request[index] != '/' && index < request_len; ++index)
			continue;

		//then find a space, newline, or in extreme cases, the end of the string
		for(; tmp_request[index] != ' ' && tmp_request[index] != '\n' && index < request_len; ++index)
			continue;

		//fill out the remaining spots with null bytes
		for(; index < request_len; ++index)
			tmp_request[index] = '\0';
	}

	file_path = concat_file_path(tmp_slash, root);

	tmp_slash = NULL;

	return file_path;
}


/*******************************************************
Input: file_d - The name of the file that will be
				evaluated.

Output: Returns a long type representing the number of
		bytes within the file.

Description: Measures the size of the file in terms of
			 bytes.
*******************************************************/
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


/*******************************************************
Input: file_name - The c-string representing the name of
				   the file that must be evaluated.

Output: Returns a c-string of the MIME type, representing
		the file, to be used in the HTTP header.

Description: Gets the MIME type of the specified file.
*******************************************************/
char* get_MIME_type(char file_name[])
{
	//create a new c-string
	char *mime;
	char extensions[7][6] = {".html", ".css", ".js", ".jpeg", ".png", ".gif"};

	char types[6][15] = {"text/html", "text/css", "application/js", "image/jpeg", "image/png", "image/gif"};

	int index = 0;

	//find which file type matches, if any, according to the substring
	for(; index < 6; ++index)
	{
		if(strstr(file_name, extensions[index]))
			break;
	}

	//copy the appropriate value into the "mime" variable
	if(index < 6)
	{
		mime = (char*) malloc(strlen(types[index]) + 1);
		strcpy(mime, types[index]);
	}
	else
	{
		mime = (char*) malloc(5);
		strcpy(mime, "NONE");
	}

	return mime;
}


/*******************************************************
Input: None

Output: Returns a c-string of the specified file.

Description: Gets the root folder from the command line
			 by prompting the user.
*******************************************************/
char* get_root()
{
	char *root = (char*) malloc(MAX_ROOT);
	printf("\nPlease input a valid root folder, or specify \"EXIT\" to exit: ");
	fflush(stdout);
	//get the root folder
	scanf("%s", root);

	return root;
}


/*******************************************************
Input: code_mesg - The code message that will be added
				   to the header.

	   content_type - The type of content that will be
					  added to the header.

	   length - The length of the file that will be sent;
				this length will be added to the header.

Output: Returns the formatted header as a c-string.

Description: Makes the header needed to properly send
			 back an HTTP response.
*******************************************************/
char* make_header(char code_mesg[], char content_type[], int length)
{
	char length_str[2];

	char *header = (char*) malloc(122);

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

/*******************************************************
Input: code_flag - The integer flag that determines which
				   code message will be returned.

Output: Returns the code message as a c-string.

Description: Makes the code message needed for the HTTP
			 header.
*******************************************************/
char* make_code(int code_flag)
{
	char* code = (char*) malloc(25);

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
