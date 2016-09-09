#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 256
#define SERV_PORT 8800

void do_loop(FILE *fp, int sockfd)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1];

	while(fgets(sendline, MAXLINE, fp) != NULL)
	{
		/* read a line and send to server */
		write(sockfd, sendline, strlen(sendline));
		/* receive data from server */
		n = read(sockfd, recvline, MAXLINE);
		if(n == -1)
		{
			perror("read error");
			exit(1);
		}
		recvline[n] = 0; /* terminate string */
		printf("\nreceiv:%s\n",recvline);
	}
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;

	/* check args */
	if(argc != 2)
	{
		printf("usage: udpclient <IPaddress>\n");
		exit(1);
	}

	/* init servaddr */
	bzero(&servaddr, sizeof(servaddr));
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