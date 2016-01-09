#ifndef _XNET_THREADLIST_H
#define _XNET_THREADLIST_H
/*
 * 线程安全的列表模板
 * 		基于C++ Standard Library 和Posix多线程编程
 *			作者:刘志军, 2005.9.9
 *
 */
#include <process.h>
#include <direct.h>
#include <windows.h>
#include <list>
#include <string>

/*!
	线程安全的列表模板,用于发送线程每次发送一笔数据到一个列表,处理线程一次获取所有列表中的数据
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
		构造函数, pthreadid为处理线程的pt地址
	*/
	TemplateMessageList (HANDLE h)
	{
		handle = h;
		exitflag = false;
        InitializeCriticalSection( &FMutex);
		FList = new std::list<T>;
	};

	/*!
		析构函数,析构之前必须先终止处理线程
	*/
	~TemplateMessageList ()
	{
		void *noused;
		exitflag = true;
        DeleteCriticalSection(&FMutex);
		delete FList;
	};
	/*!
		队列是否为空
	*/
	bool Empty() 
	{
		return FList->empty();	
	}
	/*!
		在列表有数据时获取整个列表,否则等待信号。处理线程使用
		返回为NULL时，处理线程必须终止线程
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
		Lock 队列
	*/
    void Lock()
    {
        EnterCriticalSection( &FMutex );
    }
	/*!
		UnLock 队列
	*/
    void UnLock()
    {
        LeaveCriticalSection( &FMutex );
    }
	/*!
		Wait 队列
	*/
    void Wait()
    {
		SuspendThread(handle);
    }
	/*!
		Resume 队列
	*/
    void Resume()
    {
        ResumeThread(handle);
    }

	/*!
		发送数据到列表中,如果列表为空,则发送等待信号。发送线程使用
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


