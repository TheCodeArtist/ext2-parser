CC=gcc
CFLAGS=-c -Wall

all: ext cp

ext: ext.o
	$(CC) ext.o -o ext

cp: cp.o
	$(CC) cp.o -o cp

ext.o: ext.c
	$(CC) $(CFLAGS) ext.c

cp.o: cp.c
	$(CC) $(CFLAGS) cp.c

clean:
	rm -rf *.o ext cp
