/*
    C client example using sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>       // strlen
#include <sys/socket.h>   // socket
#include <arpa/inet.h>    // inet_addr
#include <unistd.h>       // write
#include <fcntl.h>

int main(int argc , char *argv[])
{
    // Variables
    int sock, read_size,addrlen;
    char recvBuff[1024];
    struct sockaddr_in server;

    char *addr = "127.0.0.1";
    short port = 8888;

    // Create a socket. Return value is a file descriptor for the socket.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Error creating socket\n");
        exit(0);
    }
    printf("Client socket created\n");

    // Set the server ip address, connection family and port
    server.sin_addr.s_addr = inet_addr(addr);
    server.sin_family      = AF_INET;
    server.sin_port        = htons(port);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Error establishing connection\n");
        close(sock);
        exit(0);
    }
    printf("Connecting to server...\n");
	
	addrlen = sizeof(struct sockaddr_in);
    
    FILE *fp;
    fp = fopen("apple.pdf","ab");
    if(NULL == fp){
    	printf("Error opening file");
    	return 1;
    }

    while((read_size = read(sock, recvBuff, 1024)) > 0 ){
    	printf("Bytes received %d\n", read_size);
    	fwrite(recvBuff, 1, read_size, fp);
    	printf("%s\n",recvBuff );
    }

    if(recvBuff < 0){
    	printf("Read Error\n");
    }

    close(sock);
    return 0;
}
