/*
			NETWORK PROGRAMMING WITH SOCKETS
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[])
{
	int			sockfd,sockfd_client, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr[3];
	struct pollfd fdset;

	int i;
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Cannot create socket\n");
		exit(0);
	}
	
	serv_addr[0].sin_family		= AF_INET;				//load server address
	serv_addr[0].sin_addr.s_addr	= INADDR_ANY;
	int port_num = atoi(argv[1]);
	serv_addr[0].sin_port		= htons(port_num);

	serv_addr[1].sin_family	= AF_INET;					//server1 address
	inet_aton("127.0.0.1", &serv_addr[1].sin_addr);
	port_num = atoi(argv[2]);
	serv_addr[1].sin_port	= htons(port_num);

	serv_addr[2].sin_family	= AF_INET;					//server2 address
	inet_aton("127.0.0.1", &serv_addr[2].sin_addr);
	port_num = atoi(argv[3]);
	serv_addr[2].sin_port	= htons(port_num);

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call. 
	*/

	if (bind(sockfd, (struct sockaddr *) &serv_addr[0],
					sizeof(serv_addr[0])) < 0) {
		printf("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5); /* This specifies that up to 5 concurrent client
			      requests will be queued up while the system is
			      executing the "accept" system call below.
			   */

	fdset.fd = sockfd;
	fdset.events = POLLIN;

	time_t past_time;
	time_t current_time;

	int timeout = 5*1000;
	int a = 0, b = 0;
	
	while (1) {

		
		time(&past_time);
		int ret = poll(&fdset,1,timeout);
		if(ret == 0)
		{
			char str[100];
			time_t ticks;
			ticks = time(NULL);
			snprintf(str, 50, "%.24s\n", (char *)ctime(&ticks));
			//printf("\nTime when recieving load : %.24s\n",str);				//checking for every 5 seconds to get load
			if ((sockfd_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				printf("Cannot create socket\n");
				exit(0);
			}
			if ((connect(sockfd_client, (struct sockaddr *) &serv_addr[1], sizeof(serv_addr[1]))) < 0) 
			{
				perror("Unable to connect to server1\n");
				exit(0);
			}
			else
			{
				//printf("\nConnected to the Server1 for taking load \n\n");
			}
			send(sockfd_client,"Send Load",10,0);
			char out[100];
			for(int i=0; i<100; i++) out[i] = '\0';
			while(1)
			{
				char temp[51];
				memset(temp,0,sizeof(temp));
				int x= recv(sockfd_client,temp,50,0);
				if(x < 0) 
				{
					printf("Unable to read from socket\n\n");
					close(sockfd_client);
					exit(0);
				}
				else if(x == 0) 
				{
					printf("Connection closed by load balancer\n\n");
					close(sockfd_client);
					exit(0);
				}
				strcat(out, temp);
				//printf("%s\n", out);
				if(temp[x - 1] == '\0') break;
			}
			a = atoi(out);
			printf("Load recieved from 127.0.0.1 Port No %d : %d\n",atoi(argv[2]),a);
			close(sockfd_client);

			if ((sockfd_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				printf("Cannot create socket\n");
				exit(0);
			}
			for(int i=0; i<100; i++) out[i] = '\0';
			if ((connect(sockfd_client, (struct sockaddr *) &serv_addr[2], sizeof(serv_addr[2]))) < 0) 
			{
				perror("Unable to connect to server2\n");
				exit(0);
			}
			else
			{
				//printf("\nConnected to the Server2 for taking load\n\n");
			}
			send(sockfd_client,"Send Load",10,0);
			while(1)
			{
				char temp[51];
				memset(temp,0,sizeof(temp));
				int x= recv(sockfd_client,temp,50,0);
				if(x < 0) 
				{
					printf("Unable to read from socket\n\n");
					close(sockfd_client);
					exit(0);
				}
				else if(x == 0) 
				{
					printf("Connection closed by load balancer\n\n");
					close(sockfd_client);
					exit(0);
				}
				strcat(out, temp);
				//printf("%s\n", out);
				if(temp[x - 1] == '\0') break;
			}
			b = atoi(out);
			printf("Load recieved from 127.0.0.1 Port No %d : %d\n\n\n",atoi(argv[3]),b);
			close(sockfd_client);
			timeout = 5000;
			continue;
		}

		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen) ;

		if (newsockfd < 0) {
			printf("Accept error\n");
			exit(0);
		}

		
		if (fork() == 0) 
		{

			/* This child process will now communicate with the
			   client through the send() and recv() system calls.
			*/
			close(sockfd);	/* Close the old socket since all
					   communications will be through
					   the new socket.
					*/
			struct sockaddr_in serv_adr;
			if(a<=b)
			{
				serv_adr = serv_addr[1];
				printf("\nSending client request to 127.0.0.1 Port No %d\n\n\n",atoi(argv[2]));
			}
			else
			{
				serv_adr = serv_addr[2];
				printf("\nSending client request to 127.0.0.1 Port No %d\n\n\n",atoi(argv[3]));
			}
			int sockfd_clnt;
			if ((sockfd_clnt = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				printf("Cannot create socket\n");
				exit(0);
			}
			if ((connect(sockfd_clnt, (struct sockaddr *) &serv_adr, sizeof(serv_adr))) < 0) 
			{
				perror("Unable to connect to server\n");
				exit(0);
			}
			else
			{
				//printf("\nConnected to the Server for recieving time\n\n");
			}
			send(sockfd_clnt,"Send Time",10,0);
			char buf[100];
			for(i=0; i < 100; i++) buf[i] = '\0';
			while(1)
			{
				char temp[51];
				memset(temp,0,sizeof(temp));
				int x= recv(sockfd_clnt,temp,50,0);
				//printf("%d\n",x);
				if(x < 0)
				{
					printf("Unable to read from socket\n\n");
					close(sockfd_clnt);
					close(newsockfd);
					exit(0);
				}
				else if(x == 0) 
				{
					printf("Connection closed by Server\n\n");
					close(sockfd_clnt);
					close(newsockfd);
					exit(0);
				}
				strcat(buf, temp);
				if(temp[x - 1] == '\0') break;
			}
			//printf("%s\n", buf);
			close(sockfd_clnt);
			send(newsockfd, buf, strlen(buf) + 1, 0);
			close(newsockfd);
			exit(0);
		}

		close(newsockfd);
		time(&current_time);
		if(past_time != 0)
		{
			if((current_time - past_time)*1000 > timeout)
			{
				timeout = 5*1000;
			}
			else
			{
				timeout = timeout - (current_time - past_time)*1000;
			}
		}
	}
	return 0;
}
			
