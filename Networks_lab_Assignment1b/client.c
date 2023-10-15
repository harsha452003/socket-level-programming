
/*    THE CLIENT PROCESS */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int			sockfd ;
	struct sockaddr_in	serv_addr;

	int i;
	char buf[100];

	/* Opening a socket is exactly similar to the server process */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(0);
	}

	/* Recall that we specified INADDR_ANY when we specified the server
	   address in the server. Since the client can run on a different
	   machine, we must specify the IP address of the server. 

	   In this program, we assume that the server is running on the
	   same machine as the client. 127.0.0.1 is a special address
	   for "localhost" (this machine)
	   
	/* IF YOUR SERVER RUNS ON SOME OTHER MACHINE, YOU MUST CHANGE 
           THE IP ADDRESS SPECIFIED BELOW TO THE IP ADDRESS OF THE 
           MACHINE WHERE YOU ARE RUNNING THE SERVER. 
    	*/

	serv_addr.sin_family	= AF_INET;
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port	= htons(20000);

	/* With the information specified in serv_addr, the connect()
	   system call establishes a connection with the server process.
	*/
	if ((connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0) {
		perror("Unable to connect to server\n");
		exit(0);
	}
	else
	{
		printf("Connected to the server\n");
	}

	/* After connection, the client can send or receive messages.
	   However, please note that recv() will block when the
	   server is not sending and vice versa. Similarly send() will
	   block when the server is not receiving and vice versa. For
	   non-blocking modes, refer to the online man pages.
	*/

	while(1)
	{
		char *output = (char *)malloc(100*sizeof(char));
		long int l = 5;
		char *expr = (char *)malloc(sizeof(char));
		printf("\n\nEnter the valid expression to get evaluated or -1 to terminate: \n");
		while(1)
		{
			char ch;
			scanf("%c",&ch);
			if(ch == '\n') break;
			l += 1;
			expr = realloc(expr,l*sizeof(char));
			strncat(expr,&ch,1);
			
		}
		if((expr[0] == '-') && (expr[1] == '1') && (expr[2] == '\0')) break;
		int m = strlen(expr)+1;
		expr[m] = '\0';
		//printf("%ld\n", strlen(expr));
		if(m/100 == 0) send(sockfd,expr,strlen(expr)+1,0);
		else
		{
			int y = 0;
			while(m/100 != 0)
			{
				send(sockfd,expr+y,100,0);
				y = y + 100;
				m = m-100;
			}
			send(sockfd,expr+y,m,0);
		}
		printf("Length of expression entered : %ld\n", strlen(expr));
		free(expr);
		int x = 0;
		while(1)
		{
			char temp[100];
			memset(temp,0,sizeof(temp));
			x= recv(sockfd,temp,100,0);
			if(x < 0) 
			{
            	printf("Unable to read from socket\n");
            	exit(1);
        	}
        	else if(x == 0) 
			{
            	printf("Connection closed by server\n");
            	exit(1);
        	}
        	strcat(output, temp);
        	if(temp[x - 1] == '\0') break;
		}
		printf("Evalueted value recieved from server : %s\n", output);
		free(output);
	}
	//for(i=0; i < 100; i++) buf[i] = '\0';
	//recv(sockfd, buf, 100, 0);
		
	close(sockfd);
	return 0;

}

