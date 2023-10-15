
/*    THE CLIENT PROCESS */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SIZE 250

int main()
{
	int			sockfd ;
	struct sockaddr_in	serv_addr;

	int i;
	char buf[50];

	/* Opening a socket is exactly similar to the server process */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(0);
	}


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
		
	for(i=0; i < 50; i++) buf[i] = '\0';
	while(1)
	{
		char temp[50];
		memset(temp,0,sizeof(temp));
		int x= recv(sockfd,temp,50,0);
		if(x < 0) 
		{
			printf("Unable to read from socket\n");
			close(sockfd);
			exit(0);
		}
		else if(x == 0) 
		{
			printf("Connection closed by Server\n");
			close(sockfd);
			exit(0);
		}
		strcat(buf, temp);
		//printf("%s\n", out);
		if(temp[x - 1] == '\0') break;
	}
	printf("%s", buf);
	for(i=0; i < 50; i++) buf[i] = '\0';
	scanf("%[^\n]%*c", buf);
	send(sockfd, buf, strlen(buf) + 1, 0);
	for(i=0; i < 50; i++) buf[i] = '\0';
	while(1)
	{
		char temp[50];
		memset(temp,0,sizeof(temp));
		int x= recv(sockfd,temp,50,0);
		if(x < 0) 
		{
			printf("Unable to read from socket\n");
			close(sockfd);
			exit(0);
		}
		else if(x == 0) 
		{
			printf("Connection closed by Server\n");
			close(sockfd);
			exit(0);
		}
		strcat(buf, temp);
		//printf("%s\n", out);
		if(temp[x - 1] == '\0') break;
	}
	//printf("%s ", buf);
	int m = strcmp(buf,"FOUND");
	int n = strcmp(buf,"NOT-FOUND");
	if(n==0 && m!=0)
	{
		printf("Invalid username\n");
		close(sockfd);
		exit(0);
	}
	else if(m==0 && n!=0)
	{
		for(i=0; i < 50; i++) buf[i] = '\0';
		char cmd[SIZE];
		while(1)
		{
			printf("Enter the shell command to get executed : ");
			scanf("%[^\n]%*c", cmd);
			if(strcmp(cmd,"exit") == 0)
			{
				close(sockfd);
				exit(0);
			}
			m = strlen(cmd)+1;
			cmd[m] = '\0';
			//printf("%ld\n", strlen(expr));
			if(m/50 == 0) send(sockfd,cmd,m,0);
			else
			{
				int y = 0;
				while(m/50 != 0)
				{
					send(sockfd,cmd+y,50,0);
					y = y + 50;
					m = m-50;
				}
				send(sockfd,cmd+y,m,0);
			}

			char out[4096];
			for(i=0; i < 4096; i++) out[i] = '\0';
			while(1)
			{
				char temp[51];
				memset(temp,0,sizeof(temp));
				int x= recv(sockfd,temp,50,0);
				if(x < 0) 
				{
					printf("Unable to read from socket\n");
					close(sockfd);
					exit(0);
				}
				else if(x == 0) 
				{
					printf("Connection closed by Server\n");
					close(sockfd);
					exit(0);
				}
				strcat(out, temp);
				//printf("%s\n", out);
				if(temp[x - 1] == '\0') break;
			}
			
			if(strcmp(out,"$$$$") == 0)
			{
				printf("Invalid command\n\n");
				continue;
			}
			else if(strcmp(out,"####") == 0)
			{
				printf("Error in running command\n\n");
				continue;
			}
			else if(strcmp(out,"Done") == 0)
			{
				continue;
			}
			else
			{
				printf("%s\n\n", out);
				continue;
			}
		}
	}
		
	close(sockfd);
	return 0;

}