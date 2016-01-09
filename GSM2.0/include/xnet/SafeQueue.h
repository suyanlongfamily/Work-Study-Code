#ifndef _XNet_SafeQueue_H
#define _XNet_SafeQueue_H

#include <pthread.h>
#include <iostream>
#include <list>
#include <string>

namespace xnet{

template<class T> class MultiMessageQueueList {
private:
	std::list<T> *FList;
	pthread_mutex_t FMutex;
	pthread_cond_t FCond;
public:
	MultiMessageQueueList()
	{
		pthread_mutex_init(&FMutex, NULL);
		pthread_cond_init(&FCond, NULL);
		FList = new std::list<T>;
	};

	~MultiMessageQueueList()
	{
		delete FList;
		pthread_cond_destroy(&FCond);
		pthread_mutex_destroy(&FMutex);
	};

	/*!
		从消息队列中获取数据,为空时线程以阻塞方式等待,减少对CPU的占用
	*/
	T GetMessageT()
	{
		T result;
		pthread_cleanup_push((void (*)(void*))pthread_mutex_unlock, &FMutex);
		pthread_mutex_lock(&FMutex);
		while (1) {
			if (!FList->empty())
				break;
			pthread_cond_wait(&FCond, &FMutex);
		}
		result = FList->front();
		FList->pop_front();
		pthread_cleanup_pop((long)&FMutex);
		return result;
	};

	/*!
		发送数据到消息队列,在队列原来为空时,自动唤醒阻塞的读线程
	*/
	void PostMessageT(T message)
	{
		pthread_cleanup_push((void (*)(void*))pthread_mutex_unlock, &FMutex);
		pthread_mutex_lock(&FMutex);
		bool needsingal = FList->empty();
		FList->push_back(message);
		if (needsingal)
			pthread_cond_broadcast(&FCond);
		pthread_cleanup_pop((long)&FMutex);
	};
	/*!
		从消息队列中获取数据,为空时线程以阻塞方式等待,减少对CPU的占用
	*/
	T GetMessage()
	{
		T result;
		pthread_cleanup_push((void (*)(void*))pthread_mutex_unlock, &FMutex);
		pthread_mutex_lock(&FMutex);
		while (1) {
			if (!FList->empty())
				break;
			pthread_cond_wait(&FCond, &FMutex);
		}
		result = FList->front();
		FList->pop_front();
		pthread_cleanup_pop((long)&FMutex);
		return result;
	};

	/*!
		发送数据到消息队列,在队列原来为空时,自动唤醒阻塞的读线程
	*/
	void PostMessage(T message)
	{
		pthread_cleanup_push((void (*)(void*))pthread_mutex_unlock, &FMutex);
		pthread_mutex_lock(&FMutex);
		bool needsingal = FList->empty();
		FList->push_back(message);
		if (needsingal)
			pthread_cond_broadcast(&FCond);
		pthread_cleanup_pop((long)&FMutex);
	};

	/*!
		返回模板所用原list,便于特殊操作
	*/
	std::list<T> *List() { return FList; };
};
typedef MultiMessageQueueList<std::list<std::string> *> TMultiMessageList;
}
#endif
