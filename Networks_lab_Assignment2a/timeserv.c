// A Simple UDP Server that sends a HELLO message
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
  
#define MAXLINE 1024 
  
int main() { 
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Create socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    servaddr.sin_family    = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(8181); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    printf("\nServer Running....\n");
  
    int n; 
    socklen_t len;
    char buffer[MAXLINE]; 
    char buf[100];
    len = sizeof(cliaddr);
    while(1)
    {
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len); 
        buffer[n] = '\0'; 
        //printf("%s\n", buffer); 

        time_t ticks;
        ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\n", ctime(&ticks));
        //sleep(8);
        sendto(sockfd, buf, strlen(buf) + 1, 0,( struct sockaddr *) &cliaddr, sizeof(cliaddr));

        printf("Time on server is %s\n",buf);
        for(int i=0; i<MAXLINE; i++)buffer[i] = '\0';
        for(int i=0; i<100; i++)buf[i] = '\0';
    }
    close(sockfd);
    return 0; 
} 
