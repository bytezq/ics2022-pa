CC = gcc
CFLAGS = -g -Wall

all: main

main: pstree.o
	$(CC) $(CFLAGS) -o $@ $^

pstree.o: pstree.c
	$(CC) $(CFLAGS) -c $<

test: main
	./main arg1 arg2 arg3

clean:
	rm -f main main.o