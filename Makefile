all: client server
client: client.c server.h
	gcc -o client client.c server.h -lrt 

server: server.c data_structure.c server.h
	gcc -o server data_structure.c data_structure.h server.c server.h -lrt 
