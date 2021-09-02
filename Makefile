CC=gcc
CFLAGS=-Wall -Wextra

OBJS=server.o net.o 

all: server

server: $(OBJS)
	gcc -o $@ $^

net.o: net.c net.h

server.o: server.c net.h