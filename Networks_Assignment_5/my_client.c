#include "mysocket.h"

int main()
{
	int			sockfd ;
	struct sockaddr_in	serv_addr;

	int i;
	char buf[100];

	/* Opening a socket is exactly similar to the server process */
	if ((sockfd = my_socket(AF_INET, SOCK_MyTCP, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(0);
	}


	
	   
	/* IF YOUR SERVER RUNS ON SOME OTHER MACHINE, YOU MUST CHANGE 
           THE IP ADDRESS SPECIFIED BELOW TO THE IP ADDRESS OF THE 
           MACHINE WHERE YOU ARE RUNNING THE SERVER. 
    	*/

	serv_addr.sin_family	= AF_INET;
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port	= htons(3000);

	/* With the information specified in serv_addr, the connect()
	   system call establishes a connection with the server process.
	*/
	if ((my_connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0){
		perror("Unable to connect to server\n");
		exit(0);
	}
	else
	{
		printf("\nConnected to the Server\n\n");
	}

	
	for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);

	//printf("%d\n",x);
	for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);
	//printf("%d\n",x);
	for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);
	//printf("%d\n",x);
   
   for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);

for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);

for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);

for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);

for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);

for(i=0; i < 100; i++) buf[i] = '\0';
	
	my_recv(sockfd, buf, 18, 0);
	
	printf("Time is %s\n", buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,100,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);
	my_recv(sockfd,buf,10,0);
	printf("Recieved : %s\n",buf);
	memset(buf,0,100);

	
	my_send(sockfd, "My Name is Harsha", 18, 0);


	my_close(sockfd);
	return 0;

}

