#ifndef SafeSTL_H
#define SafeSTL_H
/*
 * �̰߳�ȫ��ģ���  
 * 		����C++ Standard Library ��Posix���̱߳��
 *			����:��־��, 2004.4.4
 *
 */


#include <pthread.h>
#include <iostream>
#include <list>
#include <string>
#include <time.h>


/*!
 Unix���߳�ͨѶ����Ϣ����ģ��,��Ϣ����T����Ϊ������������

	Ӧ�÷�Χ:
	-	���Խ����ݼ򵥵Ĵ��������߳��ƽ���ʹ�����߳�
	-	��Unix���̱߳��ʱ,���ĳ���߳�רע�ڴ���ĳЩ����,��Щ�����������̲߳���,
		��ʹ�ñ�ģ���ܹ�ʹ�����̰߳�ȫ�ķ��͵����߳�(���ǳ�֮Ϊ"���߳�"),
		���ҿ�����û����Ҫ���������ʱ,�ö��̴߳�������״̬,�Լ���CPUռ��.

	ԭ��:
	-	���ǽ���һ����Ϣ����,�����ݱ���������ݵĵ�ַ��Ϊ��ϢT,ʹ���̻߳�����ź����������̰߳�ȫ�Ĳ���.  
	-	�����ݲ�����һ��long int���͵ĳ���ʱ,����Ҫ������ڴ����
	-	�����ݳ���һ��long int���͵ĳ���ʱ,	�������������ڴ�,ʹ��PostMessage�������ݵĵ�ַ��Ϊ��Ϣ���
		������,Ȼ�������߾�ʧȥ���ڴ�Ŀ���Ȩ. ���߳�(ʹ����)ʹ��GetMessage����ȡ��Ϣ����ȡ���ڴ�Ŀ���
		Ȩ,������Ϻ����ͷ��ڴ�. 

	ע��:
	-	ʹ�ñ�ģ�������ѭ������/ʹ���ߵ��ڴ����ԭ��.

   	��T�Ľ���: 
	-	����������(<=sizeof(long)),Tֱ��������.
	-	����ǳ��������ͻ��߶���,Tһ��Ҫ��ָ��,��Ϊ���������Լ���������,��PostMessage֮ǰ
		һ��Ҫnew,GetMessage֮��һ��Ҫdelete.

    ʹ�þ���:
	-	ʹ��TemplateMessageQueue <pthread_t> MyMessageQueue;��������һ������߳̾������Ϣ����.
	-	ʹ��TemplateMessageQueue <char *> MyCharMsgQ;��������һ�����buffer����Ϣ����.

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
		����Ϣ�����л�ȡ����,Ϊ��ʱ�߳���������ʽ�ȴ�,���ٶ�CPU��ռ��
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
		�������ݵ���Ϣ����,�ڶ���ԭ��Ϊ��ʱ,�Զ����������Ķ��߳�
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
		����ģ������ԭlist,�����������
	*/
	std::list<T> * List () { return FList; };

	/*!
		���ض��߳����һ�ζ���Ϣ��ʱ��
	*/
	const time_t LastTime () { return (const time_t) FLastTime; };
};



/*!
  ��Ϣ���ȼ�����,ʵ���Ͽ��Զ������,����û��Ҫ
*/

typedef enum { mpHigh, mpNormal, mpLow } TMessagePriority;	


/*!
	Unix�̼߳�ͨѶ����Ϣ����ģ��,��3�����ȿ���,�μ�TemplateMessageQueue˵��
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
		����Ϣ�����л�ȡ����,Ϊ��ʱ�߳���������ʽ�ȴ�,���ٶ�CPU��ռ��
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
		�������ݵ���Ϣ����,ʹ��prָ���������ȼ�
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
		����ģ������ԭlist,�����������
	*/
	std::list<T> * List (int index) { return FList[index]; }
	const time_t LastTime () { return (const time_t) FLastTime; }
};

/*!
	��ʹ����TXNetSock��Ӧ�ó�����,������TMessage *����ΪӦ�ó����TXNetSock����Ϣ���Ͷ���
*/
typedef struct {
	/*!
		��Ϣ����
		-	������Ӧ�ó�����,��Ϣ���ͱ���ͳһ����
	*/
	int Type;
	/*!
		��Ϣ����
		-	��ͨ��Type�˽⵽������ΪTXNetSockMessageʱ,����ʹ��(TXNetSockMessage *)��ת����Ϣ
	*/
	long Param[4];
} TMessage;

typedef TemplateMessageQueue <TMessage *> TMessageQueue;

}

#endif
