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

#include <time.h>

using namespace std;
//struct event evTime ;	  由于是透明的，即时空体，看不到数据成员。所以只能使用指针。不能使用成员变量。

struct event* evTime = NULL;
struct event_base* eventbase = NULL; //libevent句柄


//回调函数，
void ontime(int sock,short sevent,void* arg)
{
	cout << "Game Over!" << endl;
	time_t now;
	time(&now);
	cout<<now<<endl;

	struct timeval tv;  
	tv.tv_sec = 10;  
	tv.tv_usec = 1300;//微妙  

	// 重新添加定时事件（定时事件触发后默认自动删除）
	//tv,是超时的时间参数，
	//这里正好有个巧合，实现定时器，有两中方式，
	//一种是，相隔时间发生一次。另一种是指定时间发生（类似闹钟）
	//这里的巧合是，定时器是根据超时来实现的，第二个参数刚好是时间参数，不管第一个参数是什么事件对象，只要超出，就会发生
	//这也就是为什么说，定时事件不需要fd原因，并且定时事件是根据添加时（event_add）的超时值设定的，因此这里event也不需要设置。
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
	eventbase = event_base_new(); //初始化，libevent句柄。初始化一个Reactor实例；

	evTime = evtimer_new(eventbase,ontime,NULL); //准备定时事件对象。
	//event_set();	 这个函数已经废弃。
	//evtimer_set(&evTime, onTime, &evTime);  这个函数已经废弃。

	event_base_set(eventbase,evTime);//设置事件对象属于哪个Reactor实例。主要是事件对象可以绑定多个reactor实例上。 
	struct timeval tv;  
	tv.tv_sec = 0;  
	tv.tv_usec = 100;  
	// 添加定时事件 注册事件对象
	event_add(evTime, &tv); 											
	// 事件循环  
	event_base_dispatch(eventbase);  
	std::system("pause");
	return 0;
}