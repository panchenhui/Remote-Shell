/*
    Client
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>       // strlen
#include <sys/socket.h>   // socket
#include <arpa/inet.h>    // inet_addr
#include <unistd.h>    
   // write
#include <fcntl.h>

#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1024

int main(int argc , char *argv[])
{
    // Variables
    int sock, addrlen, n;
    char recvBuff[1024], commands[2000];
    struct sockaddr_in server;

    char *addr = "127.0.0.1";
    short port = 8888;

    printf("########## This is the client end ############\n\n");
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

    printf("Connecting to server...\n");

    // Connect to remote server
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Error establishing connection\n");
        close(sock);
        exit(0);
    }

    printf("Connection is established\n");
	
	addrlen = sizeof(struct sockaddr_in);

    // Send commands to server
    while(1){
        printf("Enter command: ");
        commands[2000] = '\0'; // set zero for commands buffer

        scanf("%s" , commands);
        if(strcmp(commands, "end") == 0) {
            printf("Stop entering commands to server\n");
            write(sock, commands, strlen(commands));
            break;
        }
        else if (strlen(commands) <= 0){
            printf("Please enter an valid command\n");
        }
        else{
            write(sock, commands, strlen(commands));
        }
    }
    
    close(sock);
    return 0;
}
