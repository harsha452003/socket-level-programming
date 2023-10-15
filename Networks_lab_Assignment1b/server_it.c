/*
			NETWORK PROGRAMMING WITH SOCKETS
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#define BUFFF 102

			/* OPERATIONS FUNCTION */


float oper(float a, float b, char ch)
{
    if(ch == '+') return a + b;
    else if(ch == '-') return a - b;
    else if(ch == '*') return a * b;
    else if(ch == '/') return a / b;
    else if(ch == '\0') return a;
}


            /* EXPRESSION EVALUATION FUNCTION */


float compute(char* s1)
{
	int flag = 0;
    float m = 1;
    float k = 10;
    float a=0,b=0;
    char ch  = '\0';
    //printf("%ld\n",strlen(s1));
    for(long int i=0; i<strlen(s1); i++)
    {
        if(s1[i] == 32) continue;
        else if((s1[i] == 46)||(s1[i]>=48 && s1[i]<=57))
        {
            if(s1[i] == 46)
            {
                m = 0.1;
                k = 1;
                continue;
            }
            if(flag == 0)
            {
                if(s1[i]>=48 && s1[i]<=57)
                {
                    a = a*k + (s1[i] - 48)*m;
                }
            }
            else if(flag == 1)
            {
                if(s1[i]>=48 && s1[i]<=57)
                {
                    b = b*k + (s1[i] - 48)*m;
                }
            }
            if(m<1) m = m/10;
        }
        else if(s1[i] == '+' || s1[i] == '-' || s1[i] == '*' || s1[i] == '/')
        {
            if(flag == 0)
            {
                ch = s1[i];
                flag = 1;
                m = 1;
                k = 10;
            }
            else if(flag == 1)
            {
                a = oper(a,b,ch);
                ch = s1[i];
                flag = 1;
                m = 1;
                k = 10;
                b = 0;
            }
        }
        else if(s1[i] == '(')
        {
            int flag1 = 0;
            float x=0,y=0;
            float m1 = 1;
            float k1 = 10;
            char c = '\0';
            i++;
            while(s1[i] != ')')
            {
                //printf("%ld\n",i);
                if(s1[i] == 32)
                {
                    i++;
                    continue;
                }
                else if((s1[i] == 46)||(s1[i]>=48 && s1[i]<=57))
                {
                    if(s1[i] == 46)
                    {
                        //printf("a\n");
                        i++;
                        m1 = 0.1;
                        k1 = 1;
                        continue;
                    }
                    else if(flag1 == 0)
                    {
                        if(s1[i]>=48 && s1[i]<=57)
                        {
                            //printf("b\n");
                            x = x*k1 + (s1[i] - 48)*m1;
                            //printf("%f\n",x);
                        }
                    }
                    else if(flag1 == 1)
                    {
                        if(s1[i]>=48 && s1[i]<=57)
                        {
                            //printf("c\n");
                            y = y*k1 + (s1[i] - 48)*m1;
                        }
                    }
                    if(m1<1) m1 = m1/10;
                }
                else if(s1[i] == '+' || s1[i] == '-' || s1[i] == '*' || s1[i] == '/')
                {
                    if(flag1 == 0)
                    {
                        //printf("d\n");
                        c = s1[i];
                        flag1 = 1;
                        m1 = 1;
                        k1 = 10;
                    }
                    else if(flag1 == 1)
                    {
                        //printf("e\n");
                        x = oper(x,y,c);
                        c = s1[i];
                        flag1 = 1;
                        m1 = 1;
                        k1 = 10;
                        y = 0;
                    }
                }
                i++; 
                //printf("%f\n",x);
                //printf("%d\n",flag1);   
            }
            if(flag == 0)
            {
                a = oper(x,y,c);
            }
            else
            {
                b = oper(x,y,c);
            }
            //printf("%f\n",b);
            //printf("%c\n",c);
            //printf("%f\n",x);
            //printf("%f\n",y);
        }
    }
    float res = oper(a,b,ch);
    return res;
    //printf("%c\n",ch);
}

            /* FLOATING POINT NUMBER TO STRING CONVERSION FUNCTION */


void float_to_string(float f, char* num)
{
    int i = 0, bytes;
    int decimal_count = 0;
    int integral_part;
    if(f == 0)  num[i++] = '0';
    else
    {
        if(f<0)
        {
            num[i++] = '-';
            f = -f;
        }
        float temp = f;
        while(temp!= (int)temp)                 //taking the decimal count of the numbers
        {
            decimal_count++;
            temp = temp*10;
        }
        if(decimal_count >= 6) decimal_count = 6;   //to maintain precision count to 6

        integral_part = (int)f;                     //taking the integer part and applying same as printInt
        while(integral_part!=0)
        {
            int digit = integral_part%10;
            num[i++] = (char)('0' + digit);
            integral_part = integral_part/10;
        }
        int j = 0;
        if(num[0] == '-') j = 1;
        for(int k = i-1; j<k; k--,j++)
        {
            char temp = num[j];
            num[j] = num[k];
            num[k] = temp;
        }
    }
    if(i == 0) num[i++] = '0';
    if(i == 1 && num[0] == '-')  num[i++] = '0';
    num[i++] = '.';
    f = f - (int)f;
    if(decimal_count == 0) num[i++] = '0';
    else
    {
        for(int m = 0; m < decimal_count; m++)
        {
            f = f*10;                               //taking thecimal part as integer and doing same as printInt to get upto 6 decimal places
        }
        int fraction_part = (int)f;
        int j = i;
        for(int m = 0; m < decimal_count; m++)
        {
            int digit = fraction_part%10;
            num[i++] = (char)('0' + digit);
            fraction_part = fraction_part/10;
        }
        for(int k = i-1; j<k; k--,j++)
        {
            char temp = num[j];
            num[j] = num[k];
            num[k] = temp;
        }
        while(num[i-1] == '0')
        {
            i--;
        }
    }
    num[i] = '\0';
}

int main()
{
	int			sockfd, newsockfd ; /* Socket descriptors */
	int			clilen;
	struct sockaddr_in	cli_addr, serv_addr;

	int i;
	//char buf[100];		/* We will use this buffer for communication */

	/* The following system call opens a socket. The first parameter
	   indicates the family of the protocol to be followed. For internet
	   protocols we use AF_INET. For TCP sockets the second parameter
	   is SOCK_STREAM. The third parameter is set to 0 for user
	   applications.
	*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
	serv_addr.sin_port		= htons(20000);

	/* With the information provided in serv_addr, we associate the server
	   with its port using the bind() system call. 
	*/
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
					sizeof(serv_addr)) < 0) {
		perror("Unable to bind local address\n");
		exit(0);
	}

	listen(sockfd, 5); /* This specifies that up to 5 concurrent client
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
	while (1) {

		/* The accept() system call accepts a client connection.
		   It blocks the server until a client request comes.

		   The accept() system call fills up the client's details
		   in a struct sockaddr which is passed as a parameter.
		   The length of the structure is noted in clilen. Note
		   that the new socket descriptor returned by the accept()
		   system call is stored in "newsockfd".
		*/
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
					&clilen) ;

		if (newsockfd < 0) {
			perror("Accept error\n");
			exit(0);
		}
        else
        {
            printf("\nConnected to the client\n\n");
        }
		//printf("1\n");

		/* We initialize the buffer, copy the message to it,
			and send the message to the client. 
		*/
		while(1)
		{
			int flag = 0;
			char* out = (char *)malloc(5*sizeof(char));
			memset(out,0,sizeof(out));
			int x = 0;
			int m = 5;
			while(1)
			{
				char temp[101];
				memset(temp,0,sizeof(temp));
				x= recv(newsockfd,temp,100,0);
				m+=BUFFF;
				out = (char *)realloc(out,m*sizeof(char));
				//printf("%d\n",x);
				if(x < 0) 
				{
					printf("Unable to read from socket\n");
					//exit(1);
					flag = 1;
					break;
				}
				else if(x == 0) 
				{
					printf("Connection closed by client\n");
					//exit(1);
					flag = 1;
					break;
				}
				strcat(out, temp);
				//printf("%s\n", out);
				if(temp[x - 1] == '\0') break;
			}
			if(flag == 1)
			{
				free(out);
				break;
			}
            printf("Expression recieved from Client : \n");
			printf("%s\n",out);
            printf("Length of expression recieved : %ld\n", strlen(out));
			float res = compute(out);
			//printf("%c\n", out[100]);
			//printf("1\n");
			free(out);
			//printf("%f\n",res);
            char * buff = (char *)malloc(100*sizeof(char));
            float_to_string(res,buff);

            printf("Evaluated value of expression is %s\n\n\n",buff);
            send(newsockfd,buff,strlen(buff)+1,0);
            free(buff);
		}
		//send(newsockfd, buf, strlen(buf) + 1, 0);

		/* We now receive a message from the client. For this example
  		   we make an assumption that the entire message sent from the
  		   client will come together. In general, this need not be true
		   for TCP sockets (unlike UDPi sockets), and this program may not
		   always work (for this example, the chance is very low as the 
		   message is very short. But in general, there has to be some
	   	   mechanism for the receiving side to know when the entire message
		  is received. Look up the return value of recv() to see how you
		  can do this.
		*/ 
		//printf("Time on server is %s\n",buf);
		close(newsockfd);
	}
	return 0;
}
			

