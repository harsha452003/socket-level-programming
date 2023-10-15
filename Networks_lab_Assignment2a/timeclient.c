// A Simple Client Implementation
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
int main() { 
    int sockfd; 
    struct sockaddr_in servaddr; 
    struct pollfd fdset;
    
    // Creating socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(8181); 
    inet_aton("127.0.0.1", &servaddr.sin_addr); 
      
    int n;
    socklen_t len; 
    char *hello = "CLIENT:HELLO"; 
       
    //printf("Hello message sent from client\n"); 
           
    for(int i=0; i<5; i++)
    {
        sendto(sockfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        //printf("%d\n",i);
        char buff[100];
        len = sizeof(servaddr);
        fdset.fd = sockfd;
        fdset.events = POLLIN;
        int timeout = 3*1000;
        int ret = poll(&fdset,1,timeout);
        if(ret == 0) continue;
        else
        {
            n = recvfrom(sockfd, (char *)buff, 100, 0, (struct sockaddr *) &servaddr, &len);
            buff[n] = '\0'; 
            printf("%s\n", buff);
            close(sockfd);
            exit(1);
        }
    } 
    printf("Timeout exceeded\n\n");
    close(sockfd); 
    return 0; 
} 
