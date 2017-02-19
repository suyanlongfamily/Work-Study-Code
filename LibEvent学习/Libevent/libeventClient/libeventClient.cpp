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
#include <iostream>
using namespace std;

#define MAXLINE 256
#define SERV_PORT 8800

void do_loop(int sockfd)
{
	char sendline[MAXLINE] ;  
	memset(sendline,0,sizeof(sendline));//清空 
	fgets(sendline, MAXLINE, stdin);
	while(fgets(sendline, MAXLINE, stdin) != NULL)
	{
		/* read a line and send to server */
		send(sockfd, sendline, strlen(sendline),0);	
		memset(sendline,0,sizeof(sendline));//清空  
	}
}

void on_read(int fd,short event,void* arg);
void on_IORead(int fd,short event,void* arg);

struct event_base* gBase;	
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

	gBase = event_base_new();
	int iii = 0;
	printf( "The file descriptor for stdin is %d\n", _fileno( stdin ) );

	
	struct event* ev_read = event_new(gBase,sockfd,EV_READ|EV_PERSIST,on_read,NULL); 
	//struct event* ev_IOread = event_new(gBase,_fileno( stdin ),EV_READ,on_IORead,NULL); 

	//event_base_set(gBase,ev_IOread);
	event_base_set(gBase,ev_read);	

	HANDLE thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)do_loop,(void*)sockfd,0,NULL);

    //do_loop(stdin, sockfd);	 
	event_add(ev_read,NULL);
	//event_add(ev_IOread,NULL);
	event_base_dispatch(gBase);	

	return 0;
}

void on_IORead(int fd,short event,void* arg){
	int i = 0;
	char sendline[MAXLINE] ;  
	memset(sendline,0,sizeof(sendline));//清空 
	cin>>sendline;
	//while(fgets(sendline, MAXLINE, fp) != NULL)
	//{
	/* read a line and send to server */
	send(fd, sendline, strlen(sendline),0);	
	memset(sendline,0,sizeof(sendline));//清空  
		
}

void on_read(int fd,short event,void* arg)
{
	int n;
	char recvline[MAXLINE + 1] ;
	memset(recvline,0,sizeof(recvline));//清空

	/* receive data from server */
	n = recv(fd, recvline, MAXLINE,0);
	if(n == -1)
	{
		perror("read error");
		exit(1);
	}
	recvline[n] = 0; /* terminate string */
	printf("\nreceiv:%s\n",recvline);
	memset(recvline,0,sizeof(recvline));//清空   
}

