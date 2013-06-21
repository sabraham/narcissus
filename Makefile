all: server client

server: server.c
	gcc -g3 -Wall server.c -o server

client: client.c
	gcc -g3 -Wall client.c -o client

clean:
	rm -f *~ *.o server client
	rm -rf *.dSYM/
