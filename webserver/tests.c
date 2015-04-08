#include "webserver_funcs.h"

int main()
{
	int index = 0;

	char *codes[5] = {"200 OK", "400 Bad Request", "404 Not Found", "414 Request-URI Too Long", "501 Not Implemented"};
	char *content_types[6] = {"text/html", "text/css", "application/js", "image/jpeg", "image/png", "image/gif"};
	char *mime_files[11] = 
	{
		"hello.html", "awesome.css", "coolstuff.js", "vacation.jpeg", "favicon.png", "grumpycat.gif",
		"hello.htlm", "css", ".", ";", "\n"
	};
	char *caps[5] =
	{
		"test-1.html", "ace.css", "testing", "123", "temp.txt"
	};

	char *tmp_string = NULL;

	int lengths[5] = {10, 20, 30, 40, 50};

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
	printf("%d total bytes\n", (int) get_file_size("test.txt"));
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

	printf("-------------FINISH MIME TESTS-------------\n");

	return 0;
}
