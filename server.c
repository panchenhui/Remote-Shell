/*
    Server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>      // strlen
#include <sys/socket.h>  // socket
#include <arpa/inet.h>   // inet_addr
#include <unistd.h>      // write

<<<<<<< HEAD
#define BUFFER 1024

=======
#include <sys/types.h>
#include <sys/stat.h>	// file status 

/* Function: check the access status for the file
	print out the status message for the file. 
*/
char* access_status(char* filename) {
	char* message[2000];

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
char* change_file_mode(char* filename, char* mode){
	char* message[2000];
	// set file to read permission for owner
	if(strcmp(mode, "user_read") == 0){
		chmod(filename, S_IRUSR);
		strcpy(message, "The file "+ filename + " has been changed to read_only mode for owner"); 

	// set file to write permission for owner
	}else if(strcmp(mode, "user_write") == 0){
		chmod(filename, S_IWUSR);
		strcpy(message, "The file "+ filename + " has been changed to write_only mode for owner"); 

	// set file to read permission for others
	}else if(strcmp(mode, "other_read"){
		chmod(filename, S_IROTH);
		strcpy(message, "The file "+ filename + " has been changed to read_only mode for others"); 

	// set file to write permission for others
	}else if(strcmp(mode, "other_write"){
		chmod(filename, S_IWOTH);
		strcpy(message, "The file " + filename + " has been changed to write_only mode for others");
	}else{
		strcpy(message, "The mode entered does not exist !"); 
	}
	return message;
}

/* 
	Function: call linux command
	pass the linux commnad and do system call
*/
void call_command(char* command){
	if(strcmp(command, "ls -l") == 0){
		printf("List all the file in current directory.\n");
	}
	system(command);
}

/* 
	Function: remove the file
*/
char* remove_file(char* filename){
	char* message[200];
	if( remove(filename) == 0 ){ 
		strcpy(message, "Removed " + filename);
	}else{
		strcpy(message, "Removed failed.");
	}
	return message
}
>>>>>>> 1d1fc53ca5a066dc21e1d518044965edacde7fa0
int main(int argc , char *argv[])
{
    // Variables
    int ssock, csock, addrlen, read_size;
    struct sockaddr_in server, client;
    short port = 8888;
    char commands[2000];

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
    while((read_size = read(csock, commands, sizeof(commands))) > 0 ){
        commands[read_size] = '\0';
        if(strcmp(commands, "end") == 0){
            printf("The client has stopped sending commands\n");
            break;
        }
        else{
            printf("client sent command: %s\n", commands);
        }
    }
    if(read_size == 0){
        printf("Invalid command\n");
    }


    FILE *fp = fopen("server_log.txt", "rb");
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
            if (ferror(fp))
                printf("Error reading\n");
            break;
        }
    }


    printf("The file has been sent to the client\n");

    close(csock);
    close(ssock);
    printf("End connection\n");

    return 0;
}
