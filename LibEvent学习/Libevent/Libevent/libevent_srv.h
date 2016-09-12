#ifndef __LIBEVENT_TST
#define __LIBEVENT_TST	

#define PORT 8800
#define BACKLOG 5
#define MAX_RTSP_BUFFER 2048  


//�ͻ��׽���
struct sock_ev {
    struct event* read_ev; //���¼�
    struct event* write_ev;//д�¼�
    struct event* timer_ev;//��ʱ���¼�
    char* buffer;     
    // record client info
	int sock;//�ͻ��׽��֡�
    char ipDocDecimal[INET_ADDRSTRLEN];	//IP
    unsigned short port;
};		 

//�����׽��֡�
struct sock_li_ev{
	struct event* accept_ev;//�����¼���socket
	struct event* timer_ev;//��ʱ���¼���	
	int sock;//�������׽��֡�
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
