#ifndef __LIBEVENT_TST
#define __LIBEVENT_TST	

#define PORT 8800
#define BACKLOG 5
#define MAX_RTSP_BUFFER 2048  

struct sock_ev {
    struct event* read_ev; //读事件
    struct event* write_ev;//写事件
    struct event* timer_ev;//定时器事件
    char* buffer;     
    // record client info   
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
