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
		����Ϣ�����л�ȡ����,Ϊ��ʱ�߳���������ʽ�ȴ�,���ٶ�CPU��ռ��
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
		�������ݵ���Ϣ����,�ڶ���ԭ��Ϊ��ʱ,�Զ����������Ķ��߳�
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
		����Ϣ�����л�ȡ����,Ϊ��ʱ�߳���������ʽ�ȴ�,���ٶ�CPU��ռ��
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
		�������ݵ���Ϣ����,�ڶ���ԭ��Ϊ��ʱ,�Զ����������Ķ��߳�
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
		����ģ������ԭlist,�����������
	*/
	std::list<T> *List() { return FList; };
};
typedef MultiMessageQueueList<std::list<std::string> *> TMultiMessageList;
}
#endif
