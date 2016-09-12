#ifndef __LIBEVENT_TST
#define __LIBEVENT_TST	

#define PORT 8800
#define BACKLOG 5
#define MAX_RTSP_BUFFER 2048  


//客户套接字
struct sock_ev {
    struct event* read_ev; //读事件
    struct event* write_ev;//写事件
    struct event* timer_ev;//定时器事件
    char* buffer;     
    // record client info
	int sock;//客户套接字。
    char ipDocDecimal[INET_ADDRSTRLEN];	//IP
    unsigned short port;
};		 

//监听套接字。
struct sock_li_ev{
	struct event* accept_ev;//接收事件，socket
	struct event* timer_ev;//定时器事件。	
	int sock;//监听的套接字。
	char ipDocDecimal[INET_ADDRSTRLEN];	//IP
	unsigned short port;
};


int  GetSessionID();
void on_accept(int socklisenter, short event, void* arg);
void on_write(int sock, short event, void* arg);
void on_read(int sock, short event, void* arg);
void Timer_CB(int fd, short event , void *arg);
void release_sock_event(struct sock_ev* ev);

#endif
