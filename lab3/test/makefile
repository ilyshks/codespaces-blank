CC=gcc
CFLAGS=-I.

all: tests

tests : ../src/utils.o ../src/find_min_max.o ../src/utils.h ../src/find_min_max.h
	$(CC) -o tests tests.c unity.c ../src/find_min_max.c $(CFLAGS)

clean:
	rm tests