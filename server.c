/*
    Server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>      // strlen
#include <sys/socket.h>  // socket
#include <arpa/inet.h>   // inet_addr
#include <unistd.h>      // write

#define BUFFER_SIZE 1024

#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>	// file status 

/* Function: check the access status for the file
	print out the status message for the file. 
*/
char* access_status(char* filename) {
	char message[2000];

	// check whether the file could be written. 
	if(access(filename, F_OK) == 0){
		strcpy(message, "The file exists.");
	}else{
		strcpy(message, "The file does not exist.");
	}

	// check whether the file could be red.
	if(access(filename, R_OK) == 0){
		strcpy(message, "The file could be read.");
	}else{
		strcpy(message, "The file could not be read.");
	}

	// check whether the file could be written. 
	if(access(filename, W_OK) == 0){
		strcpy(message, "The file could be written.");
	}else{
		strcpy(message, "The file could not be written.");
	}
	return message;
}


/*  
	Function: change file permission
	return the prompt message back to screen
*/
/*
char* change_file_mode(char* filename, char* mode){
	char* message[2000];
	// set file to read permission for owner
	if(strcmp(mode, "user_read") == 0){
		chmod(filename, S_IRUSR);
		strcpy(message, "The file has been changed to read_only mode for owner"); 

	// set file to write permission for owner
	}else if(strcmp(mode, "user_write") == 0){
		chmod(filename, S_IWUSR);
		strcpy(message, "The file has been changed to write_only mode for owner"); 

	// set file to read permission for others
	}else if(strcmp(mode, "other_read"){
		chmod(filename, S_IROTH);
		strcpy(message, "The file has been changed to read_only mode for others"); 

	// set file to write permission for others
	}else if(strcmp(mode, "other_write"){
		chmod(filename, S_IWOTH);
		strcpy(message, "The file has been changed to write_only mode for others");
	}else{
		strcpy(message, "The mode entered does not exist !"); 
	}
	return message;
}*/


int main(int argc , char *argv[])
{
    // Variables

    int ssock, csock, addrlen, read_size;
    struct sockaddr_in server, client;
    short port = 8888;
    char commands[2000];
    char line[2000];
    int end = 0, invalid_command = 0;

    // Varibles for 'ls' command
    FILE *pipein_fp, *pipeout_fp;
    FILE *command_log = fopen("log.txt", "w");
    char readbuf[800];

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
        }
	// list files under the current directory
        else if(strcmp(commands, "ls") == 0){
    		printf("List files under current folder: \n");
		system("ls");

          	/* Create one way pipe line with call to popen() */
		if((pipein_fp = popen("ls", "r")) == NULL){
		        perror("Cannot open the log file");
		        exit(1);
		}

		/* Processing loop */
		while(fgets(readbuf, 800, pipein_fp)){
	   		fputs(readbuf, command_log);
		}

	// print working directory
        }else if(strcmp(commands, "pwd") == 0){
		printf("Print working directory: \n");
		system("pwd");

        	/* Create one way pipe line with call to popen() */
		if((pipein_fp = popen("pwd", "r")) == NULL){
		        perror("Cannot open the log file");
		        exit(1);
		}

		/* Processing loop */
		while(fgets(readbuf, 800, pipein_fp)){
	   		fputs(readbuf, command_log);
		}

	// print current date
        }else if(strcmp(commands, "date") == 0){
		printf("The date: \n");
		system("date");

        	/* Create one way pipe line with call to popen() */
		if((pipein_fp = popen("date", "r")) == NULL){
		        perror("Cannot open the log file");
		        exit(1);
		}

		/* Processing loop */
		while(fgets(readbuf, 800, pipein_fp)){
	   		fputs(readbuf, command_log);
		}
	
	}else if(strcmp(commands, "^C") == 0) {
		end = 1;
	}else{
            	printf("The client sent the command: %s\n", commands);
		system(commands);
        }


	FILE *output = fopen("commands.txt", "r");
        commands[2000] = '\0'; // set zero for commands buffer
            if(!invalid_command) {
                if(output != NULL) {
                    while (fgets(line, BUFFER_SIZE, output) != NULL) {
                        
                        // allows one command to be sent
                        strcat(commands, line);
                        
                        // tracking of line sizes of each command
                        //printf("linesize: %d\n", (int)strlen(line));
                    }
                    
                    read_size = write(csock, commands, sizeof(commands));
                    line[2000] = '\0';
        	    commands[2000] = '\0'; // set zero for commands buffer
                    
                    fclose(output);
                }
	}
	if (read_size < 0) {
                perror("Server error: could not read from socket.");
 		exit(1);
        }
    }

    /* Close the pipes */
    pclose(pipein_fp);
    fclose(command_log);

    close(csock);
    close(ssock);
    printf("End connection\n");

    return 0;
}
