/*
    Client
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>       // strlen
#include <sys/socket.h>   // socket
#include <arpa/inet.h>    // inet_addr
#include <unistd.h>		  // write
#include <fcntl.h>
#include <ctype.h>

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
    int sock, read_size,addrlen;
    char recvBuff[1024][1024], commands[2000];
    struct sockaddr_in server;
    char trans_commands[2000];
    //char *addr = "45.55.64.18";
    char *addr = "127.0.0.1";
    short port = 8888;
    char file_name[200];

    printf("------------------------client ------------------------\n\n");
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


	printf("\nAvailable commands: \n");
	printf("ls: list all the file in current folder\n");
	printf("cd directory_name: change to directory directory_name\n");
	printf("pwd: display the current path\n");
	printf("date: display current date\n");
	printf("rm name: delete the file called name\n");
	printf("transfer filename: transfer the file from server\n\n");

    // Send commands to server
    while(1){

        // enter commands
        printf("Enter commands: ");
        commands[2000] = '\0';
        fgets(commands,2000, stdin);
        printf(commands);
        if(strcmp(commands, "end\n") ==0) {
            printf("Stop entering commands to server\n");
            write(sock, commands, strlen(commands));
            break;
        }
        else if (strlen(commands) <= 0){
            printf("Please enter an valid command\n");
        }
	// process pwd commands
        else if(strcmp(commands, "pwd\n") == 0){
		   	write(sock, commands, strlen(commands));

			int len = 0;
			recv(sock, &len, sizeof(len), 0);
			for(int i = 0; i < len; i++){
				recv(sock, recvBuff[i], sizeof(recvBuff[i]), 0);
				printf(recvBuff[i]);
			}
	}
	// process list commands
	else if(strcmp(commands, "ls\n") == 0)
	{
		write(sock, commands, strlen(commands));

		int len = 0;
		recv(sock, &len, sizeof(len), 0);
		printf("Total number of files: %d\n", len);
		for(int i = 0; i < len; i++){
			recv(sock, recvBuff[i], sizeof(recvBuff[i]), 0);
			printf(recvBuff[i]);
		}
	}
	// process cd commands
	else if(strcmp(commands, "cd") == 0)
	{
		write(sock, commands, strlen(commands));
		printf("Directory changed\n");

	}
	// process date commands
	else if(strcmp(commands, "date\n") == 0)
	{
	   	write(sock, commands, strlen(commands));
		int len = 0;
		recv(sock, &len, sizeof(len), 0);

		for(int i = 0; i < len; i++){
			recv(sock, recvBuff[i], sizeof(recvBuff[i]), 0);
			printf(recvBuff[i]);
		}
	}
	// process transfer file command
        else if (strstr(commands,"transfer"))
        {
        	strcpy(trans_commands,commands);
        	write(sock, commands, strlen(commands));
        	   
        	strncpy(file_name, trans_commands + 9, strlen(trans_commands));
            	printf("%s\n", file_name);

        	// Get file from server
		FILE *fp;
		printf("opening %s\n", file_name);
	    	fp = fopen(file_name,"ab");
	   	if(NULL == fp){
		    	printf("Error opening file");
		    	return 1;
		}

		printf("Fetching log file from server...\n");
		while((read_size = read(sock, recvBuff, 1024)) > 0 ){
		    	printf("Bytes received %d\n", read_size);
		    	fwrite(recvBuff, 1, read_size, fp);
		    	printf("The info that we got :\n %s\n",recvBuff );
		    	break;
		}
		if(recvBuff < 0){
		    	printf("Read Error\n");
		}
		printf("file closed\n");
		fclose(fp);
        }
        else{
            write(sock, commands, strlen(commands));
        }
    }

    strncpy(file_name, trans_commands + 9, strlen(trans_commands));
    printf("%s\n", file_name);
    trim_string(file_name);




    close(sock);
    return 0;
}
