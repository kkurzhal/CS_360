all: server client

server: daytime-server.c
	g++ -Wall -g -o server daytime-server.c

client: daytime-client.c
	g++ -Wall -g -o client daytime-client.c

clean:
	rm -f *core server client *o
