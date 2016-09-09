#include <WinSock2.h>
#ifndef WIN32
#include <sys/queue.h>
#include <unistd.h>
#endif
#include <time.h>	  
#ifdef _EVENT_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif		   
#include <stdio.h>	
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>	
#ifdef WIN32   
#include <Windows.h>
#include <ws2ipdef.h>	
#include <ws2def.h>
#include <WinDef.h>
#include <WS2tcpip.h>  
#endif 	   
#include <event2/event.h>	
#include <event2/event-config.h> 

#define MAXLINE 256
#define SERV_PORT 8800

void do_loop(FILE *fp, int sockfd)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1] ;
	memset(recvline,0,sizeof(recvline));//清空
	memset(sendline,0,sizeof(sendline));//清空

	while(fgets(sendline, MAXLINE, fp) != NULL)
	{
		/* read a line and send to server */
		send(sockfd, sendline, strlen(sendline),0);


		/* receive data from server */
		n = recv(sockfd, recvline, MAXLINE,0);
		if(n == -1)
		{
			perror("read error");
			exit(1);
		}
		recvline[n] = 0; /* terminate string */
		printf("\nreceiv:%s\n",recvline);
		memset(recvline,0,sizeof(recvline));//清空
		memset(sendline,0,sizeof(sendline));//清空

	}
}

int main(int argc, char **argv)
{

#if WIN32
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return FALSE; 
#endif //WIN32

	int sockfd;
	struct sockaddr_in servaddr;

	/* check args */
	if(argc != 2)
	{
		printf("usage: udpclient <IPaddress>\n");
		exit(1);
	}

	/* init servaddr */
	memset(&servaddr,0,sizeof(servaddr));
	//bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
	{
		printf("[%s] is not a valid IPaddress\n", argv[1]);
		exit(1);
	}

	//	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	/* connect to server */
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("connect error");
		exit(1);
	}

	do_loop(stdin, sockfd);

	return 0;
}