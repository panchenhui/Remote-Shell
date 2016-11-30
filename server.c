/*
    Server.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>      // strlen
#include <sys/socket.h>  // socket
#include <arpa/inet.h>   // inet_addr
#include <unistd.h>      // write

#define BUFFER_SIZE 1024

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>	// file status 


int main(int argc , char *argv[]){
    // Variables

    int ssock, csock, addrlen, read_size, write_size;
    struct sockaddr_in server, client;
    short port = 8888;
    char commands[2000];
    char line[2000];
    int end = 0, invalid_command = 0, sendLen = 1;


    printf("---------------------- server end ------------------------\n\n");
    // Create a socket. Return value is a file descriptor for the socket.
    ssock = socket(AF_INET, SOCK_STREAM, 0);
    if (ssock == -1) {
        perror("Error creating socket\n");
        exit(0);
    }
    printf("Server socket created\n");

    // Set the server ip address, connection family and port. INADDR_ANY means
    // all the ip addresses of the server can be used to set up connection.
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port        = htons(port);

    // Bind the socket for the connection
    if (bind(ssock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        //print the error message
        perror("Error binding\n");
        close(ssock);
        exit(0);
    }
    printf("Binding done\n");

    // Listen for incoming connections
    listen(ssock, SOMAXCONN);
    printf("Listening...\n");

    printf("Waiting for incoming connections...\n");
    addrlen = sizeof(struct sockaddr_in);

    // Accept connection from an incoming client
    csock = accept(ssock, (struct sockaddr *) &client, (socklen_t *) &addrlen);
    if (csock < 0) {
        perror("Error accepting connections\n");
        exit(0);
    }
    printf("Connection accepted\n");
 

    // Receive commands from the client
    char sendBuff[1024][1024];
    FILE *pipein_fp, *pipeout_fp;
    FILE *command_log = fopen("log.txt", "w");

    while(!end){
	   read_size = read(csock, commands, sizeof(commands));
 	if (read_size == 0) {
		invalid_command = 1;
                end = 1;
        	printf("Invalid command\n");

	}

        commands[read_size] = '\0'; // set zero for commands buffer

        if(strcmp(commands, "end") == 0){
            printf("The client stopped sending commands\n");
            break;
	// list files under the current directory
        }else if(strcmp(commands, "ls") == 0){
    		printf("List files under current folder: \n");
		system("ls");

          	/* Create one way pipe line with call to popen() */
		if((pipein_fp = popen("ls", "r")) == NULL){
		        perror("Cannot open the log file");
		        exit(1);
		}
		
		int num = 0; // record the time of sending
		/* Processing loop */
		while(fgets(sendBuff[num], sizeof(sendBuff), pipein_fp) != NULL){
	   		fputs(sendBuff[num], command_log);
			num++;
		}

		//send results back to client
		send(csock, &num, sizeof(int), 0); // send the number of sending time
		for(int i = 0; i < num; i++){
			send(csock, sendBuff[i], sizeof(sendBuff[i]), 0);
		}

    		/* Close the pipes */
    		pclose(pipein_fp);

	// print working directory
        }else if(strcmp(commands, "pwd") == 0){
		printf("Print working directory: \n");
		system("pwd");

        	/* Create one way pipe line with call to popen() */
		if((pipein_fp = popen("pwd", "r")) == NULL){
		        perror("Cannot open the log file");
		        exit(1);
		}
		int num = 0; // record the time of sending
		/* Processing loop */
		while(fgets(sendBuff[num], sizeof(sendBuff), pipein_fp) != NULL){
	   		fputs(sendBuff[num], command_log);
			num++;
		}

		//send results back to client
		send(csock, &num, sizeof(int), 0); // send the number of sending time
		for(int i = 0; i < num; i++){
			send(csock, sendBuff[i], sizeof(sendBuff[i]), 0);
		}

    		/* Close the pipes */
    		pclose(pipein_fp);

	// print current date
        }else if(strcmp(commands, "date") == 0){
		printf("The date: \n");
		system("date");

        	/* Create one way pipe line with call to popen() */
		if((pipein_fp = popen("date", "r")) == NULL){
		        perror("Cannot open the log file");
		        exit(1);
		}

		int num = 0; // record the time of sending
		/* Processing loop */
		while(fgets(sendBuff[num], sizeof(sendBuff), pipein_fp) != NULL){
	   		fputs(sendBuff[num], command_log);
			num++;
		}

		//send results back to client
		send(csock, &num, sizeof(int), 0); // send the number of sending time
		for(int i = 0; i < num; i++){
			send(csock, sendBuff[i], sizeof(sendBuff[i]), 0);
		}

    		/* Close the pipes */
    		pclose(pipein_fp);


	}else if(strcmp(commands, "^C") == 0) {
		end = 1;
	}else{
            	printf("The client sent the command: %s\n", commands);
		system(commands);
        }
    }

    /* Close the pipes */
    fclose(command_log);

    close(csock);
    close(ssock);
    printf("End connection\n");

    return 0;
}
