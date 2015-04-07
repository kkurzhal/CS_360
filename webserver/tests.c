#include "webserver_funcs.h"

int main()
{
	int size = 5, index = 0;

	char *codes[5] = {"200 OK", "400 Bad Request", "404 Not Found", "414 Request-URI Too Long", "501 Not Implemented"};
	char *content_types[6] = {"text/html", "text/css", "application/js", "image/jpeg", "image/png", "image/gif"};
	int lengths[5] = {10, 20, 30, 40, 50};

	for(; index < size; ++index)
	{
		printf("<START>\n%s<END>\n", make_header(codes[index], content_types[index], lengths[index]));
	}

	printf("It's working!\n");

	return 0;
}
