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
//struct event evTime ;	  ������͸���ģ���ʱ���壬���������ݳ�Ա������ֻ��ʹ��ָ�롣����ʹ�ó�Ա������
//struct event_base eventbase;
struct event* evTime = NULL;
struct event_base* eventbase = NULL;
void ontime(int sock,short sevent,void* arg)
{
	cout << "Game Over!" << endl;  

	struct timeval tv;  
	tv.tv_sec = 0;  
	tv.tv_usec = 1300;  
	// ������Ӷ�ʱ�¼�����ʱ�¼�������Ĭ���Զ�ɾ����  
	event_add(evTime, &tv); 

}


int main()
{
	// ��ʼ��  
	//event_init()
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(1, 1);
	int nResult = WSAStartup(wVersionRequested, &wsaData);
	if (nResult != 0)
		return FALSE;	 		
	eventbase = event_base_new();  
	evTime = evtimer_new(eventbase,ontime,NULL); 
	//event_set();	 ��������Ѿ�������
	//evtimer_set(&evTime, onTime, &evTime);  ��������Ѿ�������
	event_base_set(eventbase,evTime);	   
	struct timeval tv;  
	tv.tv_sec = 0;  
	tv.tv_usec = 100;  
	// ��Ӷ�ʱ�¼�  
	event_add(evTime, &tv);  											
	// �¼�ѭ��  
	event_base_dispatch(eventbase);  
	std::system("pause");
	return 0;
}