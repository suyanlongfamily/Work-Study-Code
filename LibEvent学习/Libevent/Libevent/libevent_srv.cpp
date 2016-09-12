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

#include "libevent_srv.h"
#include "TContian.h"

//gBase 是基本事件管理员。 对所有连接仅此一个.
//因在多个函数中出现，故声明为全局变量
struct event_base* gBase;  
std::vector<int> sock_verc;
static int sessionID;	

int main(int argc, char* argv[])
{
	   	 
	int adsfads = sizeof(HeadData);
#if WIN32

	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return FALSE;

#endif //WIN32		 
	int sock = socket(AF_INET, SOCK_STREAM, 0);	
	//	设置了该选项后，在父子进程模型中，当子进程为客户服务的时候如果父进程退出，可以重新启动程序完成服务的无缝升级，
	//	否则在所有父子进程完全退出前再启动程序会在该端口上绑定失败，也即不能完成无缝升级.
	//  SO_REUSEADDR, 地址可以重用，不必time_wait 2个MSL 时间  (maximum segment lifetime)
	int yes = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int));

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(sockaddr_in));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bind(sock, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr));

	printf("socket listen now\n");
	listen(sock, BACKLOG);

	//创建一个事件处理的全局变量，可以理解为这是一个负责集中处理各种出入IO事件的总管家，
	//它负责接收和派发所有输入输出IO事件的信息，这里调用的是函数event_base_new(), 很多程序里这里用的是event_init()，
	//但event_init 是一个过时的接口，官方建议采用event_base_new()
	gBase = event_base_new();	
	struct sock_li_ev listen_fd; 
	listen_fd.port = PORT;
	listen_fd.sock = sock;
	strcpy(listen_fd.ipDocDecimal,"127.0.0.1");

	//struct event* listen_ev;
	//struct event listen_ev;
	//listen_en这个事件监听sock这个描述字的读操作，当读消息到达时调用on_accept函数，
	//EV_PERSIST参数告诉系统持续的监听sock上的读事件，如果不加该参数，每次要监听该事件时就要重复的调用event_add函数，
	//sock这个描述符是bind到本地的socket端口上，因此其对应的可读事件自然就是来自客户端的连接到达
	//event_set(&listen_ev, sock, EV_READ|EV_PERSIST, on_accept, NULL);	  
	//listen_ev = event_new(gBase,sock,EV_READ|EV_PERSIST,on_accept,NULL);

	//监听与客户的分开定义。
	//监控客户连接过来，用的得事件也是EV_REAVD类型。
	listen_fd.accept_ev = event_new(gBase,sock,EV_READ|EV_PERSIST,on_accept,NULL); 

	//设置附属 reactor实例
	event_base_set(gBase, listen_fd.accept_ev ); 
	//注册事件
	event_add(listen_fd.accept_ev , NULL);	
	//正式启动libevent的事件处理机制，使系统运行起来，
	//event_base_dispatch是一个无限循环 , 跟windows 编程的事件处理机制还真是相似啊。
	//通过此例测试可知，dispatch 是个单线程模型，不能够并发工作，当一个事件被处理时，下一个事件不会被同时处理。
	//当一个事件被长时间处理时，后面的事件会被堆积。总之，事件被顺序的一个一个执行。
	event_base_dispatch(gBase);
	event_base_free(gBase);//释放libenvent实例。	 
	return 0;
}


void on_accept(int socklisenter, short event, void* arg)
{
	int sid = GetSessionID();
	printf("session id:%d\n",sid);

	//	socket的描述字可以封装一个结构体sock_ev 来保护读、写的事件以及数据缓冲区
	//	多个客户端因而对应多个newfd, 并对应多个sock_ev 变量
	struct sock_ev* evClient = (struct sock_ev*)malloc(sizeof(struct sock_ev));  
	//ev->read_ev = (struct event*)malloc(sizeof(struct event));
	//ev->write_ev = (struct event*)malloc(sizeof(struct event));
	//ev->timer_ev = (struct event*)malloc(sizeof(struct event)); 

	//数据长度 malloc一块地址。至于连续与否，操作系统知道。
	//需要自己实现准备一块内存。recv函数里面是copy出数据从内核缓冲区里面。
	evClient->buffer = (char*)malloc(MAX_RTSP_BUFFER); 

	int sin_size = sizeof(struct sockaddr_in);
	struct sockaddr_in cli_addr;
	int newfd = accept(socklisenter, (struct sockaddr*)&cli_addr, &sin_size);
	evClient->sock = newfd;	
	sock_verc.push_back(newfd);

	char IPDotDecimal[INET_ADDRSTRLEN];	   
	struct in_addr addr = cli_addr.sin_addr;	
	inet_ntop(AF_INET,&addr,IPDotDecimal,sizeof(IPDotDecimal));//分析IP port信息	
	strcpy(evClient->ipDocDecimal,IPDotDecimal);
	evClient->port = cli_addr.sin_port;
	printf("ip:%s, port:%d\n",IPDotDecimal, cli_addr.sin_port);	

	//在客户描述字newfd上监听可读事件，当有数据到达是调用on_read函数。read_ev作为参数传递给了on_read函数。
	//注意:read_ev需要从堆里malloc出来，如果是在栈上分配，那么当函数返回时变量占用的内存会被释放，
	//此时事件主循环event_base_dispatch会访问无效的内存而导致进程崩溃(即crash)；

	//event_set(ev->read_ev, newfd, EV_READ|EV_PERSIST, on_read, ev); 
	evClient->read_ev = event_new(gBase,newfd,EV_READ|EV_PERSIST,on_read,evClient);
	
	event_base_set(gBase, evClient->read_ev);
	event_add(evClient->read_ev, NULL);

	// 增加一个定时器：每一个sockt都会绑定：定时器、读事件、写事件对象。 修改监听套接字启动定时器。
	//evClient->timer_ev = evtimer_new(gBase,Timer_CB,evClient); 	
	////evtimer_set(ev->timer_ev, Timer_CB, ev);

	//event_base_set(gBase,evClient->timer_ev);
	//struct timeval timer_v;	
	//timer_v.tv_sec = 2;	
	//timer_v.tv_usec = 0;
	//evtimer_add(evClient->timer_ev,&timer_v);
}

void on_write(int sockClient, short event, void* arg)
{
	char* buffer = (char*)arg;
	for (int i=0;i<sock_verc.size();++i)
	{
		if (sock_verc[i] != sockClient)
		{ 
			int size  = send(sock_verc[i], buffer, strlen(buffer), 0);
			printf("%d sockt send other sock:%d data length: %d\n",sockClient,sock_verc[i],size);
		}
		
	}

}

void on_read(int sockClient, short event, void* arg)
{
	struct event* write_ev;
	int size;
	struct sock_ev* evClient = (struct sock_ev*)arg;
	memset(evClient->buffer,0,MAX_RTSP_BUFFER);
	size = recv(sockClient, evClient->buffer, MAX_RTSP_BUFFER, 0); //返回接受数据的长度。

	if(0 >= size){
		//已经关闭了连接, 释放资源
		release_sock_event(evClient);
		return;
	}
	printf("---receive data:---, size:%d\n",size);
	evClient->buffer[size]='\0';
	printf("%s\n", evClient->buffer);
	//	在on_read函数中从socket读取数据后程序就可以直接调用write/send接口向客户回写数据了，
	//  直接调用write/send函数向客户端写数据可能导致程序较长时间阻塞在IO操作上，
	//	比如socket的输出缓冲区已满，则write/send操作阻塞到有可用的缓冲区之后才能进行实际的写操作，
	//	而通过注册on_write函数，那么libevent会在合适的时间调用我们的callback函数
	//在sock注册 on_write事件对象	  socket也有状态的，就是ＴＣＰ／ＩＰ状态迁移图，可是可到网上查看一下。
	evClient->write_ev = event_new(gBase,sockClient,EV_WRITE,on_write,evClient->buffer);
	//event_set(ev->write_ev, sock, EV_WRITE, on_write, ev->buffer);
	event_base_set(gBase, evClient->write_ev);//注册事件对象，可以注册同类型事件的不同事件对象。  
	event_add(evClient->write_ev, NULL);
}
void Timer_CB(int fd, short event , void *arg)
{
	struct sock_ev * ev = (struct sock_ev *) arg;
	printf("Timer_CB called sleep 2s,socket = %d----emulate a long process\n",ev->sock);
	printf("socket resource :%s %d\n",ev->ipDocDecimal,ev->port);
	//sleep(2);
	Sleep(2);
	struct timeval timer_v;	
	timer_v.tv_sec = 2;	
	timer_v.tv_usec = 0;
	evtimer_add(ev->timer_ev,&timer_v);
	time_t time_count;
	time(&time_count);
	printf("time_count:%d\n",time_count);
}

void release_sock_event(struct sock_ev* ev)
{
	printf("socket resource released:%s %d\n",ev->ipDocDecimal,ev->port);
	event_del(ev->read_ev);
	event_del(ev->write_ev);   //没有注册上时，调用删除函数会发生错误，
	//event_del(ev->timer_ev);
	//free(ev->read_ev);
	//free(ev->write_ev);
	//free(ev->timer_ev); 	
	free(ev->buffer);
	sessionID--;
	closesocket(ev->sock);

	//移除socket 
	vector<int>::iterator iter = sock_verc.begin();
	for (; iter != sock_verc.end();){
		if (*iter == ev->sock){
			//返回下一个迭代器
			iter = sock_verc.erase(iter);
			break;
		}else
		{
			++iter;
		}
	}	  
	free(ev);
}

int  GetSessionID()
{
	static int sessionID = 1; 
	// 对于成千上万的连接请求,如果有并发存在,需要枷锁保护sessionID 的数据更新
	// 但libevent 是单线程模型，所以不用加锁。
	return sessionID++;       
}
