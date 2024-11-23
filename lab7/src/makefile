CC = gcc
CFLAGS=-I.

all: tcpclient tcpserver udpclient udpserver

tcpclient:
	$(CC) $(CFLAGS) -o tcpclient tcpclient.c

tcpserver:
	$(CC) $(CFLAGS) -o tcpserver tcpserver.c

udpclient:
	$(CC) $(CFLAGS) -o udpclient udpclient.c

udpserver:
	$(CC) $(CFLAGS) -o udpserver udpserver.c

clean:
	rm -f tcpclient tcpserver udpclient udpserver