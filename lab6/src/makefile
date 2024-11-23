CC=gcc
CFLAGS=-I.

all: server client

client : 
	$(CC) -pthread -o client client.c -L. -lcommon $(CFLAGS)

# Переменные для клиента
K ?= 5
MOD ?= 10
SERVERS ?= config.txt

run_client : client
	./client --k $(K) --mod $(MOD) --servers $(SERVERS)

server : 
	$(CC) -pthread -o server server.c -L. -lcommon $(CFLAGS)

# Переменные для порта и количества потоков
PORT ?= 20000
TNUM ?= 4

run_server : server
	./server --port $(PORT) --tnum $(TNUM)

clean :
	rm client server