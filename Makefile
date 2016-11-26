CC = g++
DEBUG = -g
CFLAGS = -Wall $(DEBUG)

default :
	    $(CC) $(CFLAGS) -o server server.c; \
		$(CC) $(CFLAGS) -o client client.c; \
		$(CC) $(CFLAGS) -o popen popen.c
		$(CC) $(CFLAGS) -o popen2 popen2.c

clean:
	    \rm server client

tar:
	    tar czfv socket.tar.gz client.c server.c Makefile
