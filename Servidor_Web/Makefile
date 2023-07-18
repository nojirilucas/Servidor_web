CC=gcc
FLAGS=-g -Wall -c

servidor:	servidores3.o
	$(CC) main.c servidores3.o -o servidor.out -lpthread

servidores3.o:	servidores3.c	server.h
	$(CC) $(FLAGS)	servidores3.c

clean:
	rm -rf *.o servidor.out
