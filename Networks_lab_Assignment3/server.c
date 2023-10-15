/*
			NETWORK PROGRAMMING WITH SOCKETS

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

			/* THE SERVER PROCESS */

int main(int argc, char *argv[])
{
	srand(time(0));
	int			sockfd, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr;

	int i;
	char buf[100];		/* We will use this buffer for communication */
	time_t ticks;

	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Cannot create socket\n");
		exit(0);
	}

	
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port		= htons(atoi(argv[1]));

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call. 
	*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0) {
		perror("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5); /* This specifies that up to 5 concurrent client
			      requests will be queued up while the system is
			      executing the "accept" system call below.
			   */

	while (1) {

		
		for(int i=0; i<100; i++) buf[i] = '\0';
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
					&clilen) ;

		if (newsockfd < 0) {
			perror("Accept error\n");
			exit(0);
		}
		else
		{
			printf("\nConnected to the Load Balancer\n\n");
		}

		/* We initialize the buffer, copy the message to it,
			and send the message to the client. 
		*/
		char out[100];
		for(int i=0; i<100; i++) out[i] = '\0';
		while(1)
		{
			char temp[51];
			memset(temp,0,sizeof(temp));
			int x= recv(newsockfd,temp,50,0);
			if(x < 0) 
			{
				printf("Unable to read from socket\n\n");
				close(newsockfd);
				exit(0);
			}
			else if(x == 0) 
			{
				printf("Connection closed by load balancer\n\n");
				close(newsockfd);
				exit(0);
			}
			strcat(out, temp);
			//printf("%s\n", out);
			if(temp[x - 1] == '\0') break;
		}
		if(strcmp(out,"Send Load") == 0)
		{
			int random_number = (rand() % 100) + 1;
			sprintf(buf, "%d", random_number);
			send(newsockfd, buf, strlen(buf)+1, 0);
			printf("Load sent: %s\n",buf);
		}
		else if(strcmp(out,"Send Time") == 0)
		{
			ticks = time(NULL);
			snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
			send(newsockfd, buf, strlen(buf) + 1, 0);
			printf("Time sent by server is %s\n",buf);
		}
		
		close(newsockfd);
	}
	return 0;
}
			

