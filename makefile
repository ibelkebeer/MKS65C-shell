all: shell.o
	gcc shell.o

shell.o: shell.c shell.h
	gcc -c shell.c

run: a.out
	./a.out
