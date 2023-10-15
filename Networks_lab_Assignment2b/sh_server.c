/*
			THE CONCURRENT TCPSERVER

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define PATH_MAX 4096
#define SIZE 250

			/* THE SERVER PROCESS */
int check_user(char* buf)
{
	FILE *fptr;
	fptr = fopen("users.txt","r");
	if(fptr == NULL)
	{
		printf("Error in opening file\n");
		exit(1);
	}
	char line[50];
	for(int i=0; i < 50; i++) line[i] = '\0';
	while (fgets(line,1000,fptr) != NULL)
	{
		long int m = strlen(line);
		line[m-1] = '\0';
		//printf ("Read line %s\n",line);
		//printf ("%ld\n",strlen(line));
		m = strcmp(buf,line);
		if(m==0) return 1;
	}
	return 0;
}


int main()
{
	int			sockfd, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr;

	int i;
	char buf[50];		/* We will use this buffer for communication */

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Cannot create socket\n");
		exit(0);
	}

	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port		= htons(20000);

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call. 
	*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0) {
		printf("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5); /* This specifies that up to 5 concurrent client
			      requests will be queued up while the system is
			      executing the "accept" system call below.
			   */

	while (1) {

		
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
					&clilen) ;

		if (newsockfd < 0) {
			printf("Accept error\n");
			exit(0);
		}
		else
        {
            printf("\nConnected to the client\n\n");
        }

		if (fork() == 0) {

			/* This child process will now communicate with the
			   client through the send() and recv() system calls.
			*/
			close(sockfd);	/* Close the old socket since all
					   communications will be through
					   the new socket.
					*/

		
			send(newsockfd, "LOGIN:", strlen("LOGIN:") + 1, 0);

				/* We again initialize the buffer, and receive a 
				message from the client. 
				*/
			int flag = 0,r = 0;
			for(i=0; i < 50; i++) buf[i] = '\0';
			while(1)
			{
				char temp[50];
				memset(temp,0,sizeof(temp));
				int x= recv(newsockfd,temp,50,0);
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
			printf("The user name recieved from client :%s\n", buf);
			int ret = check_user(buf);
			for(i=0; i < 50; i++) buf[i] = '\0';
			if(ret == 1) strcat(buf,"FOUND");
			else strcat(buf,"NOT-FOUND");
			printf("User Name %s\n", buf);
			send(newsockfd, buf, strlen(buf) + 1, 0);
			char exist[4096];
			char p[4096];
			for(i=0; i < 4096; i++) 
			{
				p[i] = '\0';
				exist[i] = '\0';
			}
			while(1)
			{
				char out[SIZE];
				for(i=0; i < SIZE; i++) out[i] = '\0';
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
						printf("Connection closed by client\n\n");
						close(newsockfd);
						exit(0);
					}
					strcat(out, temp);
					//printf("%s\n", out);
					if(temp[x - 1] == '\0') break;
				}
				for(i=0; i < 50; i++) buf[i] = '\0';
				//printf("%s\n", out);
				int j = 0;
				for(long int i=0; i<strlen(out); i++)		//removing starting spaaces of command
				{
					if(out[i] == 32) continue;
					else 
					{
						j = i;
						break;
					}
				}
				for(long int i=strlen(out)-1; i>=0; i--)		//removing ending spaces for command
				{
					if(out[i] == 32) out[i] = '\0';
					else break;
				}
				printf("The command recieved from client after removing starting and ending spaces :%s\n", out+j);
				if(out[j] == 'p' && out[j+1] == 'w' && out[j+2] == 'd' && (strlen(out+j)==3 || out[j+3] == 32))
				{
					char path[PATH_MAX];
					if (getcwd(path, sizeof(path)) == NULL)
						perror("getcwd() error");
					else printf("The current working directory path :%s\n", path);
					int m = strlen(path)+1;
					path[m] = '\0';
					//printf("%ld\n", strlen(expr));
					if(m/50 == 0) send(newsockfd,path,m,0);
					else
					{
						int y = 0;
						while(m/50 != 0)
						{
							send(newsockfd,path+y,50,0);
							//printf("%s\n",expr+y);
							y = y + 50;
							m = m-50;
						}
						send(newsockfd,path+y,m,0);
					}
				}
				else if(out[j] == 'd' && out[j+1] == 'i' && out[j+2] == 'r' && (strlen(out+j)==3 || out[j+3] == 32))
				{
					long int res = 1;
					for(long int i=j+3;i<strlen(out); i++) //To find position for start of argument
					{
						if(out[i] != 32)
						{
							res = i;
							break;
						}
					}
					//printf("%ld\n",res);
					if(res == 1)
					{
						DIR *folder;
						struct dirent *entry;
						char dir[500];
						for(int i = 0; i<500; i++) dir[i] = '\0';
						folder = opendir(".");
						if(folder == NULL)
						{
							send(newsockfd, "####", 5, 0);
							continue;	
						}

						while( (entry=readdir(folder)) != NULL )
						{
							strcpy(dir,entry->d_name);
							char ch = '\n';
							strncat(dir, &ch, 1);
							int m = strlen(dir);
							//printf("%ld\n", strlen(expr));
							if(m/50 == 0) send(newsockfd,dir,m,0);
							else
							{
								int y = 0;
								while(m/50 != 0)
								{
									send(newsockfd,dir+y,50,0);
									//printf("%s\n",expr+y);
									y = y + 50;
									m = m-50;
								}
								send(newsockfd,dir+y,m,0);
							}
							printf("%s",dir);
						}
						send(newsockfd,"\0",2,0);
						closedir(folder);
					}
					else
					{
						DIR *folder;
						struct dirent *entry;
						char dir[500];
						for(int i = 0; i<500; i++) dir[i] = '\0';
						folder = opendir(out+res);
						if(folder == NULL)
						{
							send(newsockfd, "####", 5, 0);
							continue;
						}

						while( (entry=readdir(folder)) != NULL )
						{
							strcpy(dir,entry->d_name);
							char ch = '\n';
							strncat(dir, &ch, 1);
							printf("%s",dir);
							int m = strlen(dir);
							//printf("%ld\n", strlen(expr));
							if(m/50 == 0) send(newsockfd,dir,m,0);
							else
							{
								int y = 0;
								while(m/50 != 0)
								{
									send(newsockfd,dir+y,50,0);
									//printf("%s\n",expr+y);
									y = y + 50;
									m = m-50;
								}
								send(newsockfd,dir+y,m,0);
							}
						}
						send(newsockfd,"\0",2,0);
						closedir(folder);
					}
				}
				else if(out[j] == 'c' && out[j+1] == 'd' && (strlen(out+j)==2 || out[j+2] == 32))
				{
					//printf("%d\n",flag);
					/*
					if(flag == 1)
					{
						printf("%s\n",p);
					}
					*/
					getcwd(p,4096);
					long int res = 1;
					for(long int i=j+2;i<strlen(out); i++)		//checking position of start of 1st argument
					{
						if(out[i] != 32)
						{
							res = i;
							break;
						}
					}
					//printf("%ld\n",res);
					if(res == 1)
					{
						strcpy(exist,p);
						chdir(getenv("HOME"));
						printf("Done\n");
						send(newsockfd, "Done", 5, 0);
						flag = 1;
						r = 1;
					}
					
					else if(((res+1) == strlen(out)) && out[res] == '~')
					{
						strcpy(exist,p);
						chdir(getenv("HOME"));
						printf("Done\n");
						send(newsockfd, "Done", 5, 0);
						flag = 1;
						r = 1;
					}

					else if(((res+1) == strlen(out)) && out[res] == '-')
					{
						if(flag == 0)
						{
							send(newsockfd, "####", 5, 0);
							continue;
							//perror("chdir() to /usr failed")
						}
						else
						{
							chdir(exist);
							printf("Done\n");
							send(newsockfd, "Done", 5, 0);
							flag = 1;
							r = 1;
							strcpy(exist,p);
						}
					}
					else
					{
						if (chdir(out+res) != 0)
						{
							send(newsockfd, "####", 5, 0);
							continue;
							//perror("chdir() to /usr failed");
							r = 0;
						}
						else 
						{
							strcpy(exist,p);
							flag = 1;
							printf("Done\n");
							send(newsockfd, "Done", 5, 0);
							r = 1;
						}
					}
				}
				else
				{
					printf("Invalid Command\n");
					send(newsockfd, "$$$$", 5, 0);
				}
			}
			

			close(newsockfd);
			exit(0);
		}

		close(newsockfd);
	}
	return 0;
}