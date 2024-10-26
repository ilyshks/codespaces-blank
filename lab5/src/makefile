CC=gcc
CFLAGS=-I.

all: mutex factorial deadlock

mutex : 
	$(CC) -pthread -o mutex mutex.c $(CFLAGS)

factorial : 
	$(CC) -pthread -o factorial factorial.c $(CFLAGS)

deadlock : 
	$(CC) -pthread -o deadlock deadlock.c $(CFLAGS)

clean :
	rm mutex factorial deadlock