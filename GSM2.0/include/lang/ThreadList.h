#ifndef _XNET_THREADLIST_H
#define _XNET_THREADLIST_H
/*
 * �̰߳�ȫ���б�ģ��
 * 		����C++ Standard Library ��Posix���̱߳��
 *			����:��־��, 2005.9.9
 *
 */
#include <process.h>
#include <direct.h>
#include <windows.h>
#include <list>
#include <string>

/*!
	�̰߳�ȫ���б�ģ��,���ڷ����߳�ÿ�η���һ�����ݵ�һ���б�,�����߳�һ�λ�ȡ�����б��е�����
*/

namespace lang
{

template <class T> class TemplateMessageList {
private:
	std::list<T> *FList;
    CRITICAL_SECTION FMutex;
    HANDLE handle;
	bool exitflag;
public:
	/*!
		���캯��, pthreadidΪ�����̵߳�pt��ַ
	*/
	TemplateMessageList (HANDLE h)
	{
		handle = h;
		exitflag = false;
        InitializeCriticalSection( &FMutex);
		FList = new std::list<T>;
	};

	/*!
		��������,����֮ǰ��������ֹ�����߳�
	*/
	~TemplateMessageList ()
	{
		void *noused;
		exitflag = true;
        DeleteCriticalSection(&FMutex);
		delete FList;
	};
	/*!
		�����Ƿ�Ϊ��
	*/
	bool Empty() 
	{
		return FList->empty();	
	}
	/*!
		���б�������ʱ��ȡ�����б�,����ȴ��źš������߳�ʹ��
		����ΪNULLʱ�������̱߳�����ֹ�߳�
	*/
	std::list<T> * GetList ()
	{
		std::list <T> *result = NULL;
    	while (1) {
			if (!FList->empty())
				break;
			if (exitflag)
				break;
            Wait();
		}
	    Lock();
		if (!FList->empty()) {
			result = FList;
			FList = new std::list<T>;
		} else {
			result = NULL;
		}
        UnLock();
		return result;
	};
	/*!
		Lock ����
	*/
    void Lock()
    {
        EnterCriticalSection( &FMutex );
    }
	/*!
		UnLock ����
	*/
    void UnLock()
    {
        LeaveCriticalSection( &FMutex );
    }
	/*!
		Wait ����
	*/
    void Wait()
    {
		SuspendThread(handle);
    }
	/*!
		Resume ����
	*/
    void Resume()
    {
        ResumeThread(handle);
    }

	/*!
		�������ݵ��б���,����б�Ϊ��,���͵ȴ��źš������߳�ʹ��
	*/
	void PostMessage (T message)
	{
        Lock();
		bool needsingal = FList->empty();
		FList->push_back (message);
        UnLock();
		//if (needsingal)
        //{
			Resume();
        //}
	};
};

}
#endif

