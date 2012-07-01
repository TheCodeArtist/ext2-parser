CC=gcc
CFLAGS=-c -Wall

all: ext-shell

ext-shell: ext-shell.o
	$(CC) ext-shell.o -o ext-shell

clean:
	rm -rf *.o ext-shell
