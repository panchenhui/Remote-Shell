### Description: 
This is a remote shell malware which is able to execute command sent to the server and file transfer. This malware equiped with auto start future, every time when it started you can't see it from ps command, however when you see the root running process you are able to find it. In the folder we provided a VPS for you to test it out. The login information is in Server Information.txt file. (During our test we find out that ls will not work properly in VPS, however it will work fine when you test locally. Use IP address as 127.0.0.1. So please DO NOT use ls commands when testing on the server)

### Steps needed to test on the server:
	1. Open the folder named CSE2431_Final_Project, Read README.md first.
	2. Open an new terminal and follows the instruction provided in Server Information.txt 
	3. Run ls command, you can see a folder named CSE2431_Final_Project, open the folder
	4. Run following comands:
	chmod 700 start.sh
	./start.sh
	5. Now you will see the server will rebooting, and it require re-login.
	6. After re-login, open a new termial, and open the the folder named CSE2431_Final_Project
	7. Run following commands:
	make
	./client
	8. Now you can see you are connected to the target computer, you can execute commands like: pwd , cd .. , cd <path> , data , rm <filename> , etc. (WARNING: Please DO NOT execute ls command!)
	9. In order to transfer files from target computer, please run the following command: transfer <filename(include suffix, DO NOT support path)> . The file will be stored in the same folder named CSE2431_Final_Project.
	10. To end the connection, execute end command in the client side. Both program will be terminated.
	11. If you wish to remove auto start future in the VPS, please run following commands: 
	chmod 700 stop.sh
	./stop.sh

### Step needed to test locally (Especially testing ls commands):
	1. Open a termial, open the folder named CSE2431_Final_Project
	2. Open client.c and chenge IP address by comment out line 40 and uncomment line 41, save the file
	3. Run make command
	4. Then run ./server command in the terminal
	5. Open a new terminal, open the folder named CSE2431_Final_Project
	6. Run ./client command
	7. Now you can enter command in the client side and the server side will execute, in this case ls will works, and you can use cd to open other directory to test ls commands. 
	(WARNING: DO NOT attempt to transfer log.txt in the directory.)
	8. Type end command to terminate both program.
