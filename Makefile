all: client server
client: client.c server.h
	gcc -o client client.c server.h -lrt 

server: server.c server.h
	gcc -o server server.c server.h -lrt 
