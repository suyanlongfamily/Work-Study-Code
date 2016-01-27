/*
  This exmple program provides a trivial server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed.

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/

#include <string>
#include <iostream>

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

using namespace std;
//struct event evTime ;	  由于是透明的，即时空体，看不到数据成员。所以只能使用指针。不能使用成员变量。
//struct event_base eventbase;
struct event* evTime = NULL;
struct event_base* eventbase = NULL;
void ontime(int sock,short sevent,void* arg)
{
	cout << "Game Over!" << endl;  

	struct timeval tv;  
	tv.tv_sec = 0;  
	tv.tv_usec = 1300;  
	// 重新添加定时事件（定时事件触发后默认自动删除）  
	event_add(evTime, &tv); 

}


int main()
{
	// 初始化  
	//event_init()
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return FALSE;	 		
	eventbase = event_base_new();  
	evTime = evtimer_new(eventbase,ontime,NULL); 
	//event_set();	 这个函数已经废弃。
	//evtimer_set(&evTime, onTime, &evTime);  这个函数已经废弃。
	event_base_set(eventbase,evTime);	   
	struct timeval tv;  
	tv.tv_sec = 0;  
	tv.tv_usec = 100;  
	// 添加定时事件  
	event_add(evTime, &tv);  											
	// 事件循环  
	event_base_dispatch(eventbase);  
	std::system("pause");
	return 0;
}