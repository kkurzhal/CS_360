all: server client

server: jokes-server/jokes-server.c
	g++ -Wall -g -o server jokes-server/jokes-server.c

client: jokes-client/jokes-client.c
	g++ -Wall -g -o client jokes-client/jokes-client.c

clean:
	rm -f *core server client *o
