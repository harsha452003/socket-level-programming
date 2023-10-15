#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>


#define PORT 3460

int y = 0;
long double lat[50];        //for storing latencies without headers

int z = 0;
long double lat_data[50];   //for storing latencies with headers

// checksum function
uint16_t checksum(uint16_t *buf, int nwords)
{
    uint32_t sum;
    for(sum=0; nwords>1; nwords-=2)
        sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}

int is_valid_ip_address(const char *ip_address) {
    struct sockaddr_in sa;
    return inet_pton(AF_INET, ip_address, &(sa.sin_addr)) != 0;
}


long double compute_latency(int sockfd,char *ip,int n, int t)
{

    struct timeval tv;          //to wait max of 2 seconds on recieve from
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_aton(ip, &serv_addr.sin_addr);

    long double total_msec = 0;
    int count = 0;
    for(int i=0; i<n; i++)
    {
        struct timespec time_start, time_end;
        long double rtt_msec=0;

        char buf[1024];
        memset(buf,0,1024);

        struct icmphdr* icmp_hd = (struct icmphdr*)buf;
        icmp_hd->type = ICMP_ECHO;
        icmp_hd->code = 0;
        icmp_hd->un.echo.id = 37;
        icmp_hd->un.echo.sequence = 55;
        
        icmp_hd->checksum = checksum((unsigned short*)buf, sizeof(struct icmphdr));
        
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        int x = sendto(sockfd, buf,sizeof(struct icmphdr), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        
        char recv_buf[1024];
        memset(recv_buf,0,1024);

        cli_addr = (struct sockaddr_in){0};
        int y = recvfrom(sockfd, recv_buf, 1024, 0, (struct sockaddr *)&cli_addr, &cli_len);

        if(y <= 0)
        {
            printf("Request timed out\n");
            continue;
        }
        else
        {
            clock_gettime(CLOCK_MONOTONIC, &time_end);
            long double time_elapsed = ((long double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
            rtt_msec = ((time_end.tv_sec - time_start.tv_sec) * 1000.0) + time_elapsed;
            total_msec += rtt_msec;
            count++;
        }

        struct iphdr *ip_hdr = (struct iphdr *)recv_buf;
        struct icmphdr *icmp_hdr = (struct icmphdr *)(recv_buf + sizeof(struct iphdr));

        char *data = malloc(1024*sizeof(char));
        data = recv_buf + sizeof(struct iphdr) + sizeof(struct icmphdr);

        char* router_ip = malloc(1024*sizeof(char));   

        // get the router ip address
        if(cli_addr.sin_addr.s_addr != 0)
            router_ip = inet_ntoa(cli_addr.sin_addr);
        else
            strcpy(router_ip, "* * *");

            // print the router name and ip address
        //printf("%s\n", router_ip);
        //printing IP Headers

        if(icmp_hdr->type == ICMP_ECHOREPLY)
        {

                printf("\nMessage is recieved from IP: %s\n\n", router_ip);
                printf("\n\n###  IP Header  ####\n\n");
                printf("IP Header Length: %d\n", ip_hdr->ihl);
                printf("IP Header Version: %d\n", ip_hdr->version);
                printf("IP Header Type of Service: %d\n", ip_hdr->tos);
                printf("IP Header Total Length: %d\n", ip_hdr->tot_len);
                printf("IP Header Identification: %d\n", ip_hdr->id);
                printf("IP Header Fragment Offset: %d\n", ip_hdr->frag_off);
                printf("IP Header Time to Live: %d\n", ip_hdr->ttl);
                printf("IP Header Protocol: %d\n", ip_hdr->protocol);
                printf("IP Header Checksum: %d\n", ip_hdr->check);
                //printf("IP Header Source Address: %s\n", inet_ntoa(serv_addr.sin_addr));
                printf("IP Header Destination Address: %s\n", inet_ntoa(cli_addr.sin_addr));


                //printing ICMP Headers
                printf("\n\n###  ICMP Header  ####\n\n");
                printf("ICMP Header Type: %d\n", icmp_hdr->type);
                printf("ICMP Header Code: %d\n", icmp_hdr->code);
                printf("ICMP Header Checksum: %d\n", icmp_hdr->checksum);
                printf("ICMP Header ID: %d\n", icmp_hdr->un.echo.id);
                printf("ICMP Header Sequence: %d\n", icmp_hdr->un.echo.sequence);
        }
        else
        {
            printf("\n\nThe ICMP Message is not an echo reply\n\n");
        }

        sleep(t);      
    }
    if(count == 0)
    {
        return 0;
    }
    else
    {
       // printf("Total time: %Lf\n", total_msec);
        count = 2*count;
        printf("return: %Lf\n", total_msec/(long double)count);
        return total_msec/(long double)count;
    }
}

long double compute_latency_with_data(int sockfd,char *ip,int n, int t, char *data)
{
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);


    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_aton(ip, &serv_addr.sin_addr);

    long double total_msec = 0;
    int count = 0;
    for(int i=0; i<n; i++)
    {

        struct timespec time_start, time_end;
        long double rtt_msec=0;

        char buf[1024];
        memset(buf,0,1024);

        struct icmphdr* icmp_hd = (struct icmphdr*)buf;
        icmp_hd->type = ICMP_ECHO;
        icmp_hd->code = 0;
        icmp_hd->un.echo.id = 37;
        icmp_hd->un.echo.sequence = 55;
        char *data_send = (char *)(buf + sizeof(struct icmphdr));
        strcpy(data_send, data);
        icmp_hd->checksum = checksum((unsigned short*)buf, sizeof(struct icmphdr) + strlen(data_send)+1);
        
        clock_gettime(CLOCK_MONOTONIC, &time_start);
        int x = sendto(sockfd, buf,sizeof(struct icmphdr)+strlen(data_send)+1, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        char recv_buf[1024];
        memset(recv_buf,0,1024);
        cli_addr = (struct sockaddr_in){0};
        int y = recvfrom(sockfd, recv_buf, 1024, 0, (struct sockaddr *)&cli_addr, &cli_len);
        if(y <= 0)
        {
            printf("Request timed out\n");
            continue;
        }
        else
        {
            clock_gettime(CLOCK_MONOTONIC, &time_end);
            long double time_elapsed = ((long double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
           
            rtt_msec = ((time_end.tv_sec - time_start.tv_sec) * 1000.0) + time_elapsed;
            total_msec += rtt_msec;
            count++;
        }
        struct iphdr *ip_hdr = (struct iphdr *)recv_buf;
        struct icmphdr *icmp_hdr = (struct icmphdr *)(recv_buf + sizeof(struct iphdr)); 
        char *data = malloc(1024*sizeof(char));
        data = recv_buf + sizeof(struct iphdr) + sizeof(struct icmphdr);

        char* router_ip = malloc(1024*sizeof(char));    
            // get the router ip address
        if(cli_addr.sin_addr.s_addr != 0)
            router_ip = inet_ntoa(cli_addr.sin_addr);
        else
            strcpy(router_ip, "* * *");
            // print the router name and ip address
        printf("Message is recieved from IP: %s\n", router_ip);
        printf("Data of the Message recieved is : %s\n\n", data);

        if(icmp_hdr->type == ICMP_ECHOREPLY)
        {
                printf("\n\nMessage is recieved from IP: %s\n", router_ip);
                printf("Data of the Message recieved is : %s\n\n", data);
                printf("\n\n###  IP Header  ####\n\n");
                printf("IP Header Length: %d\n", ip_hdr->ihl);
                printf("IP Header Version: %d\n", ip_hdr->version);
                printf("IP Header Type of Service: %d\n", ip_hdr->tos);
                printf("IP Header Total Length: %d\n", ip_hdr->tot_len);
                printf("IP Header Identification: %d\n", ip_hdr->id);
                printf("IP Header Fragment Offset: %d\n", ip_hdr->frag_off);
                printf("IP Header Time to Live: %d\n", ip_hdr->ttl);
                printf("IP Header Protocol: %d\n", ip_hdr->protocol);
                printf("IP Header Checksum: %d\n", ip_hdr->check);
                //printf("IP Header Source Address: %s\n", inet_ntoa(serv_addr.sin_addr));
                printf("IP Header Destination Address: %s\n", inet_ntoa(cli_addr.sin_addr));


                //printing ICMP Headers
                printf("\n\n###  ICMP Header  ####\n\n");
                printf("ICMP Header Type: %d\n", icmp_hdr->type);
                printf("ICMP Header Code: %d\n", icmp_hdr->code);
                printf("ICMP Header Checksum: %d\n", icmp_hdr->checksum);
                printf("ICMP Header ID: %d\n", icmp_hdr->un.echo.id);
                printf("ICMP Header Sequence: %d\n", icmp_hdr->un.echo.sequence);
        }
        else
        {
            printf("\n\nThe ICMP Message is not an echo reply\n\n");
        }
        sleep(t);      
    }
    if(count == 0)
    {
        return 0;
    }
    else
    {
       // printf("Total time: %Lf\n", total_msec);
        count = 2*count;
        printf("return: %Lf\n", total_msec/(long double)count);
        return total_msec/(long double)count;
    }
}

int main(int argc, char* argv[])
{
    char *dest_ip = malloc(20*sizeof(char));
    struct sockaddr_in dest_addr;
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    

    if(argc != 4)
    {
        printf("Enter the command in the format given in the question\n");
        return 0;
    }

    int n = atoi(argv[2]);
    int T = atoi(argv[3]);

    if (is_valid_ip_address(argv[1]))
    {
        // Input is an IP address, use it directly
        dest_ip = strdup(argv[1]);
        
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        inet_aton(dest_ip, &dest_addr.sin_addr);

        printf("Destination IP address: %s\n", dest_ip);
    } 
    else 
    {
        // Input is a hostname, resolve it to an IP address
        struct hostent *dest;
        dest = gethostbyname(argv[1]);

        if(dest == NULL){
            printf("Error: %s\n", hstrerror(h_errno));
            exit(1);
        }
        // get destination ip address
        dest_ip = strdup(inet_ntoa(*((struct in_addr *)dest->h_addr_list[0])));

        dest_addr.sin_family = dest->h_addrtype;
        dest_addr.sin_port = htons(PORT);
        memcpy((char *) &dest_addr.sin_addr.s_addr, dest->h_addr_list[0], dest->h_length);
        printf("Destination IP address: %s\n", dest_ip);
    }

    
    // create socket
    int sockfd_icmp = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);

    //timeout on socket for every 1 second
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(sockfd_icmp, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    int ttl = 1;
    int flag = 0;
    char *prev_router_ip = malloc(20*sizeof(char));
    strcpy(prev_router_ip, "0.0.0.0");

    while(ttl<50)
    {
        char *router_ip = malloc(20*sizeof(char));
        for(int i=0; i<5; i++)
        {
            setsockopt(sockfd_icmp, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

            char buf[1024];
            memset(buf,0,1024);
            struct icmphdr* icmp_hd = (struct icmphdr*)buf;
            icmp_hd->type = ICMP_ECHO;
            icmp_hd->code = 0;
            icmp_hd->un.echo.id = 34;
            icmp_hd->un.echo.sequence = 56;
            //char *data_send = (char *)(buf + sizeof(struct icmphdr));
            //strcpy(data_send, "Hello");
            icmp_hd->checksum = checksum((unsigned short*)buf, sizeof(struct icmphdr));
            
            int x = sendto(sockfd_icmp, buf,sizeof(struct icmphdr), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            

            char recv_buf[1024];
            memset(recv_buf,0,1024);

            cli_addr = (struct sockaddr_in){0};
            recvfrom(sockfd_icmp, recv_buf, 1024, 0, (struct sockaddr *)&cli_addr, &cli_len);


            struct iphdr *ip_hdr = (struct iphdr *)recv_buf;
            struct icmphdr *icmp_hdr = (struct icmphdr *)(recv_buf + sizeof(struct iphdr));
            char *data = malloc(1024*sizeof(char));
            data = recv_buf + sizeof(struct iphdr) + sizeof(struct icmphdr);

            
            // get the router ip address
            if(cli_addr.sin_addr.s_addr != 0)
                router_ip = inet_ntoa(cli_addr.sin_addr);
            else
                strcpy(router_ip, "* * *");


            // print the router name and ip address
            printf("\n\n###   Data Packets recieved in calculating route to Destination ###\n\n");
            printf("ICMP Packet recieved from %s\n\n", router_ip);

 
            printf("\n\n###  IP Header  ####\n\n");
            printf("IP Header Length: %d\n", ip_hdr->ihl);
            printf("IP Header Version: %d\n", ip_hdr->version);
            printf("IP Header Type of Service: %d\n", ip_hdr->tos);
            printf("IP Header Total Length: %d\n", ip_hdr->tot_len);
            printf("IP Header Identification: %d\n", ip_hdr->id);
            printf("IP Header Fragment Offset: %d\n", ip_hdr->frag_off);
            printf("IP Header Time to Live: %d\n", ip_hdr->ttl);
            printf("IP Header Protocol: %d\n", ip_hdr->protocol);
            printf("IP Header Checksum: %d\n", ip_hdr->check);
            //printf("IP Header Source Address: %s\n", inet_ntoa(dest_addr.sin_addr));
            printf("IP Header Destination Address: %s\n", inet_ntoa(cli_addr.sin_addr));


                //printing ICMP Headers
            printf("\n\n###  ICMP Header  ####\n\n");
            printf("ICMP Header Type: %d\n", icmp_hdr->type);
            printf("ICMP Header Code: %d\n", icmp_hdr->code);
            printf("ICMP Header Checksum: %d\n", icmp_hdr->checksum);
            printf("ICMP Header ID: %d\n", icmp_hdr->un.echo.id);
            printf("ICMP Header Sequence: %d\n", icmp_hdr->un.echo.sequence);

            //compute_latency(router_ip, n, )
            sleep(1);
        }
        printf("\n\n  ###  Next Hop in the route %s  ### \n\n", router_ip);
        // check if the router is the host

        char *data = malloc(1024*sizeof(char));
        memset(data,0,1024);
        data = "MY NAME IS VEMURI HARSHA";
        long double data2 = strlen(data)+1;

        if(strcmp(router_ip,"* * *") == 0)
        {
            strcpy(prev_router_ip, router_ip);
            flag = 1;
            continue;
        }
        else if(strcmp(prev_router_ip, "* * *") == 0)
        {
            long double latency = compute_latency(sockfd_icmp,router_ip,n,T);
            //printf("Latency: %Lf\n", latency);
            lat[y] = latency;
            y++;
            latency = compute_latency_with_data(sockfd_icmp,router_ip,n,T,data);
            //printf("Latency with data: %Lf\n", latency);
            lat_data[z] = latency;
            z++;
        }
        else
        {
            if(flag == 0)
            {
                long double latency = compute_latency(sockfd_icmp,router_ip,n,T);
                printf("Latency of link in ms: %Lf\n", latency);
                lat[y] = latency;
                y++;
                long double latency_data = compute_latency_with_data(sockfd_icmp,router_ip,n,T,data);
                //printf("Latency with data: %Lf\n", latency);
                lat_data[z] = latency_data;
                z++;
                 
                long double bandwidth = (data2*8)/((latency_data-latency)*1000);

                printf("Bandwidth of link in Mbps: %Lf\n", bandwidth);
            }
            else
            {
                long double latency1 = compute_latency(sockfd_icmp,router_ip,n,T);
                //printf("Latency: %Lf\n", latency1);

                long double latency2 = lat[y-1];
                //printf("Latency: %Lf\n", latency2);

                lat[y] = latency1;
                y++;

                long double latency = (latency1-latency2);
                printf("Latency of link in ms: %Lf\n", latency);
                
                
                latency1 = compute_latency_with_data(sockfd_icmp,router_ip,n,T,data);
                //printf("Latency with data: %Lf\n", latency1);

                latency2 = lat_data[z-1];
                //printf("Latency with data: %Lf\n", latency2);

                lat_data[z] = latency1;
                z++;

                long double latency_data = (latency1-latency2);
                //printf("Latency: %Lf\n", latency_data);

                long double bandwidth = (data2*8)/((latency_data-latency)*1000);

                printf("Bandwidth of link in Mbps: %Lf\n", bandwidth);
            }
            flag = 1;
        }
        //printf("ttl is %d\n", ttl);
        if(strcmp(router_ip, dest_ip) == 0){
            printf("\nDestination Reached\n\n");
            break;
        }
       

        ttl++;
    }


    return 0;
}