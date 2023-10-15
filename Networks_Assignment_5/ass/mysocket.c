#include "mysocket.h"

// Struct for storing a message
typedef struct {
    char data[MAX_MESSAGE_LENGTH];
    int length;
} Message;

// Struct for storing a message table
typedef struct {
    int in;
    int out;
    int count;
    Message Recieved_Message[MAX_ENTRIES];
    pthread_mutex_t mutex;
} Recieved_Message_Table;

typedef struct {
    int in;
    int out;
    int count;
    Message Send_Message[MAX_ENTRIES];
    pthread_mutex_t mutex;
} Send_Message_Table;

Send_Message_Table* send_message_table;
Recieved_Message_Table* recieved_message_table;

pthread_t R_thread;
pthread_t S_thread;

int newsockfd = -1;
int cond = 1;

void *recv_thread(void *arg) {
    while(1)
    {
        uint32_t recv_length;
        uint8_t buff[4];
        pthread_testcancel();       //cancellation point
        while(newsockfd == -1) sleep(1);
        int y = 0;
        while(y < 4)
        {
            int x = recv(newsockfd, buff+y, 4, 0);
            if(x<=0)  
            {
                cond=x;
                pthread_exit(NULL);
            }
            y += x;
        }
        recv_length = (buff[0] << 24) | (buff[1] << 16) | (buff[2] << 8) | buff[3];
        //recv_length = ntohl(recv_length);
        //printf("%d\n", recv_length);
        int recv_bytes = 0;
        char buffer[MAX_MESSAGE_LENGTH];
        while(recv_bytes < recv_length)
        {
            int bytes = recv(newsockfd, buffer+recv_bytes, recv_length-recv_bytes, 0);
            if(bytes <= 0)
            {
                  cond = bytes;
                 pthread_exit(NULL);
            }
            else recv_bytes += bytes;
        }
        //printf("%s1\n", buffer);
        //printf("Recieve table count is %d\n", recieved_message_table->count);
        while(recieved_message_table->count == MAX_ENTRIES) sleep(2);
        pthread_mutex_lock(&recieved_message_table->mutex);
        strcpy(recieved_message_table->Recieved_Message[recieved_message_table->in].data, buffer);
        recieved_message_table->Recieved_Message[recieved_message_table->in].length = recv_length;
        recieved_message_table->in = (recieved_message_table->in + 1)%MAX_ENTRIES;
        recieved_message_table->count++;
        pthread_mutex_unlock(&recieved_message_table->mutex);

        pthread_testcancel();           //cancellation point
    }
    pthread_exit(NULL);
}

void *send_thread(void *arg) {
    while(1)
    {
        sleep(T);
        while(send_message_table->count == 0) sleep(T);
        int length = send_message_table->Send_Message[send_message_table->out].length;
        uint32_t net_len = htonl(length);
        send(newsockfd, &net_len, 4, 0);
        if(length/1001 == 0) send(newsockfd,send_message_table->Send_Message[send_message_table->out].data,length,0);
		else
		{
			int y = 0;
			while(length/1001 != 0)
			{
				send(newsockfd,send_message_table->Send_Message[send_message_table->out].data+y,1000,0);
				y = y + 1000;
				length = length-1000;
			}
			send(newsockfd,send_message_table->Send_Message[send_message_table->out].data+y,length,0);
		}
        pthread_mutex_lock(&send_message_table->mutex);
        memset(send_message_table->Send_Message[send_message_table->out].data, 0 , MAX_MESSAGE_LENGTH);
        send_message_table->Send_Message[send_message_table->out].length = 0;
        send_message_table->out = (send_message_table->out + 1)%MAX_ENTRIES;
        send_message_table->count--;
        pthread_mutex_unlock(&send_message_table->mutex);
        pthread_testcancel();           //cancellation point
    }
}

int my_socket(int domain, int type, int protocol){
    if(type != SOCK_MyTCP)
    {
        return -1;
    }
    else
    {
        int sockfd = socket(domain, SOCK_STREAM, protocol);
        //printf("%d\n", sockfd);
        if(sockfd >= 0)
        {
            int *arg = (int *)malloc(sizeof(int));
            *arg = sockfd;

            //Allocation and Initialization of Tables
            send_message_table = (Send_Message_Table *)malloc(sizeof(Send_Message_Table));
            recieved_message_table = (Recieved_Message_Table *)malloc(sizeof(Recieved_Message_Table));
            send_message_table->in = 0;
            send_message_table->out = 0;
            send_message_table->count = 0;
            recieved_message_table->in = 0;
            recieved_message_table->out = 0;
            recieved_message_table->count = 0;
            for(int i=0; i< MAX_ENTRIES; i++)
            {
                memset(send_message_table->Send_Message[i].data, 0, MAX_MESSAGE_LENGTH);
                memset(recieved_message_table->Recieved_Message[i].data, 0, MAX_MESSAGE_LENGTH);
                send_message_table->Send_Message[i].length = 0;
                recieved_message_table->Recieved_Message[i].length = 0;
            }
            pthread_mutex_init(&send_message_table->mutex, NULL);
            pthread_mutex_init(&recieved_message_table->mutex, NULL);

            //Thread Creation
            if (pthread_create(&R_thread, NULL, recv_thread, arg) != 0) 
            {
                perror("pthread_create R");
                return -1;
            }
            if (pthread_create(&S_thread, NULL, send_thread, arg) != 0) 
            {
                perror("pthread_create S");
                return -1;
            }
        }
        return sockfd;
    }
}

int my_bind(int sockfd, const struct sockaddr * serv_addr, socklen_t addrlen){
    int ret = bind(sockfd, serv_addr, addrlen);
    return ret;
}

int my_listen(int sockfd, int backlog) {
    int ret = listen(sockfd,backlog);
    return ret;
}

int my_accept(int sockfd, struct sockaddr * cli_addr, socklen_t *addrlen){
    newsockfd = accept(sockfd, cli_addr, addrlen);
    if(newsockfd < 0) return -1;
    else return newsockfd;
}

int my_connect(int sockfd, const struct sockaddr * serv_addr, socklen_t addrlen){
    int ret = connect(sockfd, serv_addr, addrlen);
    newsockfd = sockfd;
    return ret;
}

ssize_t my_send(int sockfd, const void *buf, size_t len, int flags){
    //printf("Table count is %d\n",send_message_table->count);
    while(send_message_table->count == MAX_ENTRIES) sleep(1);
    pthread_mutex_lock(&send_message_table->mutex);
    strcpy(send_message_table->Send_Message[send_message_table->in].data, buf);
    send_message_table->Send_Message[send_message_table->in].length = len;
    send_message_table->in = (send_message_table->in + 1)%MAX_ENTRIES;
    send_message_table->count++;
    pthread_mutex_unlock(&send_message_table->mutex);
    return len;
}

ssize_t my_recv(int sockfd, void *buf, size_t len, int flags){
    
    while(recieved_message_table->count == 0) 
    {
        if(cond <=0) return cond;
        sleep(1);
    }
    if(cond <=0 ) return cond;
    
    pthread_mutex_lock(&recieved_message_table->mutex);
    int length = recieved_message_table->Recieved_Message[recieved_message_table->out].length;
    if(length <= len) strcpy(buf, recieved_message_table->Recieved_Message[recieved_message_table->out].data);
    else
    {
        strncpy(buf, recieved_message_table->Recieved_Message[recieved_message_table->out].data,len);
        length = len;
    }
    memset(recieved_message_table->Recieved_Message[recieved_message_table->out].data, 0, MAX_MESSAGE_LENGTH);
    recieved_message_table->Recieved_Message[recieved_message_table->out].length = 0;
    recieved_message_table->out = (recieved_message_table->out + 1)%MAX_ENTRIES;
    recieved_message_table->count--;
    pthread_mutex_unlock(&recieved_message_table->mutex);
    return length;
}

int my_close(int sockfd){
    
    sleep(5);

    //killing the threads
    pthread_cancel(R_thread);
    pthread_cancel(S_thread);

    pthread_join(R_thread, NULL);
    pthread_join(S_thread, NULL);

    pthread_mutex_destroy(&send_message_table->mutex);
    pthread_mutex_destroy(&recieved_message_table->mutex);

    free(recieved_message_table);
    free(send_message_table);

    if(newsockfd != sockfd) close(newsockfd);
    int ret = close(sockfd);
    return ret;
}