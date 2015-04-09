#include "webserver_funcs.h"

int main()
{
	int index = 0, each_flag = 0;

	char codes[5][25] = {"200 OK", "400 Bad Request", "404 Not Found", "414 Request-URI Too Long", "501 Not Implemented"};
	char content_types[6][15] = {"text/html", "text/css", "application/js", "image/jpeg", "image/png", "image/gif"};
	char mime_files[11][20] = 
	{
		"hello.html", "awesome.css", "coolstuff.js", "vacation.jpeg", "favicon.png", "grumpycat.gif",
		"hello.htlm", "css", ".", ";", "\n"
	};
	char caps[5][20] =
	{
		"test-1.html", "ace.css", "testing", "123", "temp.txt"
	};

	char *tmp_string = NULL;

	int lengths[5] = {10, 20, 30, 40, 50};

	char nulls[4][4] = {"uno", "dos", "one", "two"};

	int flags[8] = {-1, 0, 1, 2, 3, 4, 5, 100};

	char concats[6][2][10] =
	{
		{"/foo", "./bar"},
		{"/foo/", "./bar"},
		{"/foo", "./bar/"},
		{"/foo/", "./bar/"},
		{"foo/", "./bar/"},
		{"foo", "./bar/"}
	};

	char requests[5][2][200] =
	{
		{"GET /test.txt\n", "./"},
		{"GET /CS_360/client.c\n", "/home/kyle/flashdrive"},
		{"POST /\n", "./"},
		{"GET d/\n", "./"},
		{"GET /11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\n", "./"}
	};

	//null byte test
	printf("-------------BEGIN NULL BYTE TESTS-------------\n");
	for(index = 0; index < 4; ++index)
	{
		if(strlen(nulls[index]) < 3 || nulls[index][3] != '\0')
			printf("Failed: %s\n", nulls[index]);
		else
			printf("Succeeded: %s\n", nulls[index]);
	}
	printf("-------------FINISH NULL BYTE TESTS-------------\n");

	//header tests
	printf("-------------BEGIN HEADER TESTS-------------\n");

	for(index = 0; index < 5; ++index)
	{
		tmp_string = make_header(codes[index], content_types[index], lengths[index]);
		printf("<START>\n%s<END>\n", tmp_string);
		free(tmp_string);
	}

	tmp_string = NULL;

	printf("-------------FINISH HEADER TESTS-------------\n");

	//file size tests
	printf("-------------BEGIN FILE SIZE TESTS-------------\n");
	printf("%d total bytes\n", (int) get_file_size((char*) "test.txt"));
	printf("-------------FINISH FILE SIZE TESTS-------------\n");

	//MIME tests
	printf("-------------BEGIN MIME TESTS-------------\n");

	for(index = 0; index < 11; ++index)
	{
		tmp_string = get_MIME_type(mime_files[index]);
		if(strcmp(tmp_string, "NONE") == 0)
			printf("Failed: %s\n", mime_files[index]);
		else
			printf("Succeeded: %s\n", mime_files[index]);
		free(tmp_string);
	}

	tmp_string = NULL;

	printf("-------------FINISH MIME TESTS-------------\n");


	//capitalizing tests
	printf("-------------BEGIN CAPITALIZE TESTS-------------\n");
	
	for(index = 0; index < 5; ++index)
	{
		printf("Original: %s\n", caps[index]);

		tmp_string = capitalize(caps[index]);
		printf("Capitalized: %s\n\n", tmp_string);
		free(tmp_string);
	}

	tmp_string = NULL;

	printf("-------------FINISH CAPITALIZE TESTS-------------\n");


	printf("-------------BEGIN ROOT TESTS-------------\n");
	for(index = 0; index < 3; ++index)
	{
		tmp_string = get_root();
		if(tmp_string != NULL)
			printf("Get Root Succeeded: %s\n", tmp_string);
		else
			printf("Get Root Failed: %s\n", tmp_string);

		if(check_root_exists(tmp_string) == 1)
		{
			printf("Check Root Succeeded: %s\n", tmp_string);
			free(tmp_string);
		}
		else
			printf("Check Root Failed: %s\n", tmp_string);
	}
	printf("-------------FINISH ROOT TESTS-------------\n");

	tmp_string = NULL;

	printf("-------------BEGIN FLAG TESTS-------------\n");
	for(index = 0; index < 8; ++index)
	{
		tmp_string = make_code(flags[index]);
		if(tmp_string != NULL)
		{
			printf("Code: %d; Result: %s\n", flags[index], tmp_string);
			free(tmp_string);
		}
		
	}
	printf("-------------FINISH FLAG TESTS-------------\n");

	tmp_string = NULL;

	printf("-------------BEGIN CONCAT TESTS-------------\n");
	for(index = 0; index < 6; ++index)
	{
		tmp_string = concat_file_path(concats[index][0], concats[index][1]);
		if(tmp_string != NULL)
		{
			printf("Result: %s\n", tmp_string);
			free(tmp_string);
		}
	}
	printf("-------------FINISH CONCAT TESTS-------------\n");

	tmp_string = NULL;

	printf("-------------BEGIN CHECK REQUEST TESTS-------------\n");
	for(index = 0; index < 5; ++index)
	{
		each_flag = check_request(requests[index][0], requests[index][1]);
		printf("Request: %s; Root: %s; Code Flag: %d\n\n", requests[index][0], requests[index][1], each_flag);
	}
	printf("-------------FINISH CHECK REQUEST TESTS-------------\n");


	printf("-------------BEGIN GET PATH TESTS-------------\n");
	for(index = 0; index < 5; ++index)
	{
		tmp_string = get_file_path(requests[index][0], requests[index][1]);
		if(tmp_string != NULL)
		{
			printf("Result: %s|break|\n", tmp_string);
			free(tmp_string);
		}
	}
	printf("-------------FINISH GET PATH TESTS-------------\n");

	tmp_string = NULL;

	FILE *tmp = fopen("/home/kyle/flashdrive/CS_360/client.c\n", "r");
	if(tmp == NULL)
		printf("Got NULL\n");
	else
		printf("It worked!\n");

	return 0;
}
