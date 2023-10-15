#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCK_MyTCP 45

// Maximum message length
#define MAX_MESSAGE_LENGTH 5000

// Maximum number of entries in the Send_Message and Received_Message tables
#define MAX_ENTRIES 10

#define T 2


// Function prototypes
int my_socket(int domain, int type, int protocol);
int my_bind(int sockfd, const struct sockaddr * serv_addr, socklen_t addrlen);
int my_listen(int sockfd, int backlog);
int my_accept(int sockfd, struct sockaddr * cli_addr, socklen_t *addrlen);
int my_connect(int sockfd, const struct sockaddr * serv_addr, socklen_t addrlen);
ssize_t my_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t my_recv(int sockfd, void *buf, size_t len, int flags);
int my_close(int sockfd);

#endif
