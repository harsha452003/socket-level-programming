/*    THE CLIENT PROCESS */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
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

	
	   
	/* IF YOUR SERVER RUNS ON SOME OTHER MACHINE, YOU MUST CHANGE 
           THE IP ADDRESS SPECIFIED BELOW TO THE IP ADDRESS OF THE 
           MACHINE WHERE YOU ARE RUNNING THE SERVER. 
    	*/

	serv_addr.sin_family	= AF_INET;
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	int port_num = atoi(argv[1]);
	serv_addr.sin_port	= htons(port_num);

	/* With the information specified in serv_addr, the connect()
	   system call establishes a connection with the server process.
	*/
	if ((connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0) {
		perror("Unable to connect to Load Balancer\n");
		exit(0);
	}
	else
	{
		printf("\nConnected to the Load Balancer\n\n");
	}

	
	for(i=0; i < 100; i++) buf[i] = '\0';
	while(1)
	{
		char temp[51];
		memset(temp,0,sizeof(temp));
		int x= recv(sockfd,temp,50,0);
		if(x < 0)
		{
			printf("Unable to read from socket\n\n");
			close(sockfd);
			exit(0);
		}
		else if(x == 0) 
		{
			printf("Connection closed by Load Balancer\n\n");
			close(sockfd);
			exit(0);
		}
		strcat(buf, temp);
		if(temp[x - 1] == '\0') break;
	}
	printf("%s\n", buf);
		
	close(sockfd);
	return 0;
}

