#ifndef SafeSTL_H
#define SafeSTL_H
/*
 * 线程安全的模板库  
 * 		基于C++ Standard Library 和Posix多线程编程
 *			作者:刘志军, 2004.4.4
 *
 */


#include <pthread.h>
#include <iostream>
#include <list>
#include <string>
#include <time.h>


/*!
 Unix多线程通讯用消息队列模板,消息类型T可以为任意数据类型

	应用范围:
	-	可以将数据简单的从生产者线程移交给使用者线程
	-	在Unix多线程编程时,如果某个线程专注于处理某些数据,这些数据由其它线程产生,
		则使用本模板能够使数据线程安全的发送到该线程(我们称之为"读线程"),
		并且可以在没有需要处理的数据时,让读线程处于阻塞状态,以减少CPU占用.

	原理:
	-	我们建立一个消息队列,把数据本身或者数据的地址作为消息T,使用线程互斥和信号量来进行线程安全的操作.  
	-	当数据不超过一个long int类型的长度时,不需要额外的内存管理
	-	当数据超过一个long int类型的长度时,	由生产者申请内存,使用PostMessage将该数据的地址作为消息添加
		到队列,然后生产者就失去对内存的控制权. 读线程(使用者)使用GetMessage来读取消息并获取对内存的控制
		权,处理完毕后负责释放内存. 

	注意:
	-	使用本模板必须遵循生产者/使用者的内存管理原则.

   	对T的建议: 
	-	短数据类型(<=sizeof(long)),T直接用类型.
	-	如果是长数据类型或者对象,T一定要用指针,因为变量都有自己的作用域,在PostMessage之前
		一定要new,GetMessage之后一定要delete.

    使用举例:
	-	使用TemplateMessageQueue <pthread_t> MyMessageQueue;可以申明一个存放线程句柄的消息队列.
	-	使用TemplateMessageQueue <char *> MyCharMsgQ;可以申明一个存放buffer的消息队列.

*/


namespace xnet {


template<class T> class TemplateMessageQueue {
private:
	std::list<T> *FList;
	pthread_mutex_t FMutex;
	pthread_cond_t FCond;
	time_t FLastTime;
public:

	TemplateMessageQueue ()
	{
		pthread_mutex_init (&FMutex, NULL);
		pthread_cond_init (&FCond, NULL);
		FList = new std::list<T>;
	}

	~TemplateMessageQueue ()
	{
		delete FList;
		pthread_cond_destroy (&FCond);
		pthread_mutex_destroy (&FMutex);
	}

	/*!
		从消息队列中获取数据,为空时线程以阻塞方式等待,减少对CPU的占用
	*/
	T GetMessage ()
	{
		T result;
		pthread_cleanup_push ((void (*)(void*))pthread_mutex_unlock, &FMutex);
		pthread_mutex_lock (&FMutex);
		while (1) {
			if (!FList->empty())
				break;
			pthread_cond_wait (&FCond, &FMutex);
		}
		result = FList->front ();
		FList->pop_front ();
		time (&FLastTime);
		pthread_cleanup_pop ((long)&FMutex);
		return result;
	}

	/*!
		发送数据到消息队列,在队列原来为空时,自动唤醒阻塞的读线程
	*/
	void PostMessage (T message)
	{
		pthread_cleanup_push ((void (*)(void*))pthread_mutex_unlock, &FMutex);
		pthread_mutex_lock (&FMutex);
		bool needsingal = FList->empty();
		FList->push_back (message);
		if (needsingal)
			pthread_cond_signal (&FCond);
		pthread_cleanup_pop ((long)&FMutex);
	}

	/*!
		返回模板所用原list,便于特殊操作
	*/
	std::list<T> * List () { return FList; };

	/*!
		返回读线程最后一次读消息的时间
	*/
	const time_t LastTime () { return (const time_t) FLastTime; };
};



/*!
  消息优先级定义,实际上可以定义更多,不过没必要
*/

typedef enum { mpHigh, mpNormal, mpLow } TMessagePriority;	


/*!
	Unix线程间通讯用消息队列模板,带3级优先控制,参见TemplateMessageQueue说明
*/

template<class T> class TemplatePriorityQueue {
private:
	std::list<T> *FList[3];
	pthread_mutex_t FMutex;
	pthread_cond_t FCond;
	time_t FLastTime;
public:

	TemplatePriorityQueue ()
	{
		pthread_mutex_init (&FMutex, NULL);
		pthread_cond_init (&FCond, NULL);
		for (int i = 0; i < 3; i++)
			FList[i] = new std::list<T>;
	};

	~TemplatePriorityQueue ()
	{
		for (int i = 0; i < 3; i++)
			delete FList[i];
		pthread_cond_destroy (&FCond);
		pthread_mutex_destroy (&FMutex);
	};

	/*!
		从消息队列中获取数据,为空时线程以阻塞方式等待,减少对CPU的占用
	*/
	T GetMessage ()
	{
		T result;
		pthread_cleanup_push ((void (*)(void*))pthread_mutex_unlock, &FMutex);
		pthread_mutex_lock (&FMutex);
		bool loopflag = true;
		while (loopflag) {
			for (int i = 0; i < 3; i++) {
				if (!FList[i]->empty()) {
					result = FList[i]->front ();
					FList[i]->pop_front ();
					time (&FLastTime);
					loopflag = false;
					break;
				}
			}
			if (loopflag)
				pthread_cond_wait (&FCond, &FMutex);
		}
		pthread_cleanup_pop ((long)&FMutex);
		return result;
	};

	/*!
		发送数据到消息队列,使用pr指明数据优先级
	*/
	void PostMessage (T message, const TMessagePriority pr)
	{
		pthread_cleanup_push ((void (*)(void*))pthread_mutex_unlock, &FMutex);
		pthread_mutex_lock (&FMutex);
		bool needsingal = FList[0]->empty() && FList[1]->empty() && FList[2]->empty();
		FList[pr]->push_back (message);
		if (needsingal)
			pthread_cond_signal (&FCond);
		pthread_cleanup_pop ((long)&FMutex);
	};

	/*!
		返回模板所用原list,便于特殊操作
	*/
	std::list<T> * List (int index) { return FList[index]; }
	const time_t LastTime () { return (const time_t) FLastTime; }
};

/*!
	在使用了TXNetSock的应用程序中,我们用TMessage *来作为应用程序和TXNetSock的消息传送对象
*/
typedef struct {
	/*!
		消息类型
		-	在整个应用程序中,消息类型必须统一分配
	*/
	int Type;
	/*!
		消息参数
		-	在通过Type了解到该类型为TXNetSockMessage时,可以使用(TXNetSockMessage *)来转换消息
	*/
	long Param[4];
} TMessage;

typedef TemplateMessageQueue <TMessage *> TMessageQueue;

}

#endif
