#include "mysocket.h"

			/* THE SERVER PROCESS */

int main()
{
	int			sockfd, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr;

	char buf[100];		/* We will use this buffer for communication */
	time_t ticks;

	/* The following system call opens a socket. The first parameter
	   indicates the family of the protocol to be followed. For internet
	   protocols we use AF_INET. For TCP sockets the second parameter
	   is SOCK_STREAM. The third parameter is set to 0 for user
	   applications.
	*/
	if ((sockfd = my_socket(AF_INET, SOCK_MyTCP, 0)) < 0) {
		perror("Cannot create socket\n");
		exit(0);
	}

	/* The structure "sockaddr_in" is defined in <netinet/in.h> for the
	   internet family of protocols. This has three main fields. The
 	   field "sin_family" specifies the family and is therefore AF_INET
	   for the internet family. The field "sin_addr" specifies the
	   internet address of the server. This field is set to INADDR_ANY
	   for machines having a single IP address. The field "sin_port"
	   specifies the port number of the server.
	*/
	serv_addr.sin_family		= AF_INET;
	serv_addr.sin_addr.s_addr	= INADDR_ANY;
	serv_addr.sin_port		= htons(3000);

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call. 
	*/
	if (my_bind(sockfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0) {
		perror("Unable to bind local address\n");
		exit(0);
	}

	my_listen(sockfd, 5); /* This specifies that up to 5 concurrent client
			      requests will be queued up while the system is
			      executing the "accept" system call below.
			   */

	/* In this program we are illustrating an iterative server -- one
	   which handles client connections one by one.i.e., no concurrency.
	   The accept() system call returns a new socket descriptor
	   which is used for communication with the server. After the
	   communication is over, the process comes back to wait again on
	   the original socket descriptor.
	*/
	//while (1) {

		/* The accept() system call accepts a client connection.
		   It blocks the server until a client request comes.

		   The accept() system call fills up the client's details
		   in a struct sockaddr which is passed as a parameter.
		   The length of the structure is noted in clilen. Note
		   that the new socket descriptor returned by the accept()
		   system call is stored in "newsockfd".
		*/
		clilen = sizeof(cli_addr);
		newsockfd = my_accept(sockfd, (struct sockaddr *) &cli_addr,
					&clilen) ;

		if (newsockfd < 0) {
			perror("Accept error\n");
			exit(0);
		}
		else
		{
			printf("\nConnected to the Client\n\n");
		}
		//printf("%d\n", newsockfd);
		char cli_ip[200];
		inet_ntop(AF_INET, &(cli_addr.sin_addr), cli_ip, INET_ADDRSTRLEN);
    		printf("Client IP: %s\n", cli_ip);
    		printf("Client Port: %d\n", ntohs(cli_addr.sin_port));
				/* We initialize the buffer, copy the message to it,
			and send the message to the client. 
		*/
		//int num;
		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

        ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
		my_send(newsockfd, buf, 50, 0);
		printf("Time on server is %s\n",buf);
		memset(buf,0,100);
		my_send(newsockfd, "", 18, 0);
	    my_send(newsockfd, "My Name is Harsha", 18, 0);
		my_recv(newsockfd,buf,13,0);
		printf("Recieved : %s\n",buf);
		memset(buf,0,100);

		my_close(newsockfd);
	//}
	return 0;
}
			

