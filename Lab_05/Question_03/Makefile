all: main client server

main: main.c
	gcc -o main main.c

client: client.c
	gcc -o client client.c

receiver: server.c
	gcc -o server server.c

clean:
	rm -f main client server
