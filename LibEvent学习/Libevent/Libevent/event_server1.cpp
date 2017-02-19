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
#include<event2/listener.h>
#include<event2/bufferevent.h>
#include<event2/thread.h>


void listener_cb(evconnlistener *listener, evutil_socket_t fd,
struct sockaddr *sock, int socklen, void *arg);

void socket_read_cb(bufferevent *bev, void *arg);
void socket_event_cb(bufferevent *bev, short events, void *arg);
//
//int main()
//{		 
//#if WIN32	
//	//window����ر��ġ�
//	WSADATA wsaData	;
//	WORD wVersionRequested = MAKEWORD(1, 1);
//	int nResult = WSAStartup(wVersionRequested, &wsaData);
//	if (nResult != 0)
//		return FALSE; 
//#endif //WIN32	
//
//	struct sockaddr_in sin;
//	memset(&sin, 0, sizeof(struct sockaddr_in));
//	sin.sin_family = AF_INET;
//	sin.sin_port = htons(8800);
//									  
//	event_base *base = event_base_new();
//	//�󶨲�������ַ���ϲ��������ˡ���ַ�Ǽ�����������˿ڡ�
//	evconnlistener *listener = evconnlistener_new_bind(base, listener_cb, base,
//		LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE,
//		10, (struct sockaddr*)&sin,
//		sizeof(struct sockaddr_in));
//
//	event_base_dispatch(base);	  
//	evconnlistener_free(listener);
//	event_base_free(base);	   
//	return 0;
//}
//

//һ���¿ͻ��������Ϸ�������
void listener_cb(evconnlistener *listener, evutil_socket_t fd,
				struct sockaddr *sock, int socklen, void *arg)
{
	printf("accept a client %d", fd); 
	event_base *base = (event_base*)arg;
	//Ϊ����ͻ��˷���һ��bufferevent
	bufferevent *bev =  bufferevent_socket_new(base, fd,BEV_OPT_CLOSE_ON_FREE);	
	bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, NULL);
	bufferevent_enable(bev, EV_READ | EV_PERSIST);
}

/*!
 *2016/09/12 15:45:55
 *\author suyanlong
 * �����ݣ��ص�������
 */
void socket_read_cb(bufferevent *bev, void *arg)
{
	char msg[4096];					
	size_t len = bufferevent_read(bev, msg, sizeof(msg)-1 ); 
	msg[len] = '\0';
	printf("server read the data %s\n", msg);	   
	char reply[] = "I has read your data";
	bufferevent_write(bev, reply, strlen(reply) );
}

/*!
 *2016/09/12 15:46:28
 *\author suyanlong
 * ��������Ļص��¼���
 */
void socket_event_cb(bufferevent *bev, short events, void *arg)
{
	if (events & BEV_EVENT_EOF)
		printf("connection closed\n");
	else if (events & BEV_EVENT_ERROR)
		printf("some other error\n");

	//�⽫�Զ�close�׽��ֺ�free��д���������������������Զ��ر��ˣ��Ҳ鿴Դ����û�п�����������������
	bufferevent_free(bev);
}

#include <string>
using namespace std;

int main()
{

	string strProd = "143124|51431|414324|1432";
	while (1)
	{
		int index = strProd.find_first_of('|');
		string strVul = strProd.substr(index);
	}
	return 0;
}