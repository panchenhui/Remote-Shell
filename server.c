/*
    Server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>      // strlen
#include <sys/socket.h>  // socket
#include <arpa/inet.h>   // inet_addr
#include <unistd.h>      // write
#include <ctype.h>

#define BUFFER 1024

#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>	// file status 

int transfer_file(char* file_name, int csock){
     FILE *fp = fopen(file_name, "rb");
    if (fp == NULL)
    {
        printf("File open failed\n");
        return 1;
    }

    while(1){
        unsigned char buffer[BUFFER] = {0};
        int nread = fread(buffer,1, BUFFER, fp);
        printf("Bytes read from the txt file: %d\n",nread);

        if (nread > 0)
        {
            printf("Sending...\n");
            write(csock,buffer,nread);
        }

        if (nread < BUFFER)
        {
            if (feof(fp))
                printf("End of file\n");
            break;
            if (ferror(fp))
                printf("Error reading\n");
            
        }
    }

    fclose(fp);
}


void trim_string(char *str){
    char *start, *end;
    int len = strlen(str);

    if(str[len-1] == '\n'){
        len--;
        str[len] = 0;
    }

    start = str;
    end = str + len -1;
    while(*start && isspace(*start) )
        start++;
    while (&end && isspace(*end))
        *end-- = 0;
    strcpy(str, start);

}

int main(int argc , char *argv[])
{
    // Variables
    int ssock, csock, addrlen, read_size;
    struct sockaddr_in server, client;
    short port = 8888;
    char commands[2000];
    char file_name[200];
    int end = 0, invalid_command = 0, sendLen = 1;

    printf("------------------------server------------------------\n\n");
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


    char sendBuff[1024][1024];
    FILE *pipein_fp, *pipeout_fp;
    FILE *command_log = fopen("log.txt", "w");

    // Receive commands from the client
    while((read_size = read(csock, commands, sizeof(commands))) > 0 ){
        commands[read_size] = '\0';
        if(strcmp(commands, "end\n") == 0){
            printf("The client has stopped sending commands\n");
            break;
        }
        else if(strcmp(commands, "ls\n") == 0){
            printf("Command: ls\n");
            system(commands);

            if((pipein_fp = popen("ls", "r")) == NULL)
            {
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
        }
	// transfer file from server
        else if (strstr(commands,"transfer"))
        {
            strncpy(file_name, commands + 9, strlen(commands));
            printf("%s\n", file_name);
            trim_string(file_name);
            transfer_file(file_name,csock);
            printf("The file has been sent to the client\n");
            
        }

	// change directory 
	else if (strstr(commands,"cd"))
        {
		char directory[2000];
            	strncpy(directory, commands + 3, strlen(commands));
            	printf("%s\n", directory);
            	trim_string(directory);
		
		//change the directory
		int ret = chdir(directory);
      		if(ret!=0){
          		perror("Error:");
        	}

        }

        else if(strcmp(commands, "pwd\n") == 0)
        {
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

            
        }
        // print current date
        else if(strcmp(commands, "date\n") == 0)
        {
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
        }
        else{
            printf("Client sent the command: %s\n", commands);
            system(commands);
        }

    }
    if(read_size == 0){
        printf("Invalid command\n");
    }
 

    /* Close the pipes */
    fclose(command_log);
    close(csock);
    close(ssock);
    printf("End connection\n");

    return 0;
}

