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
