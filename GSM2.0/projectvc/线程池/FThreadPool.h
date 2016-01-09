
#ifndef _ThreadPool_H_
#define _ThreadPool_H_
/*
基本上是拿来就用了，对WIN32 API不熟，但对线程池的逻辑还是比较熟的，认为这个线程池写得很清晰，我拿来用在一个多线程下载的模块中。很实用的东东。
调用方法   
void threadfunc(void *p) 
{	
YourClass* yourObject = (YourClass*)p;
//...
}
ThreadPool tp;
for(i=0; i<100; i++)
tp.Call(threadfunc);  
ThreadPool tp(20);//20为初始线程池规模
tp.Call(threadfunc, lpPara);  
使用时注意几点：	
1. ThreadJob  没什么用，直接写线程函数吧。
2. 线程函数（threadfunc）的入口参数void* 可以转成自定义的类型对象，这个对象可以记录下线程运行中的数据，并设置线程当前状态，以此与线程进行交互。
3. 线程池有一个EndAndWait函数，用于让线程池中所有计算正常结束。有时线程池中的一个线程可能要运行很长时间，怎么办？可以通过线程函数threadfunc的入口参数对象来处理，
比如：
class YourClass 
{
int cmd; // cmd = 1是上线程停止计算，正常退出。
};	  
threadfunc(void* p)
{	
YourClass* yourObject = (YourClass*)p;
while (true)
{	
// do some calculation	 
if (yourClass->cmd == 1) 
break;

}

} 
在主线程中设置yourClass->cmd = 1，该线程就会自然结束。
很简洁通用的线程池实现。 
*/

#pragma once

#pragma warning(disable: 4530)
#pragma warning(disable: 4786)	
#include <cassert>
#include <vector>
#include <queue>
#include <windows.h>  

class ThreadJob  //工作基类
{
public:
	//供线程池调用的虚函数
	virtual void DoJob(void *pPara) = 0;
};

class ThreadPool
{	
public:
	//dwNum 线程池规模 带参数数值的构造函数，很重要，可以覆盖默认构函数
	ThreadPool(DWORD dwNum = 4) : _lThreadNum(0), _lRunningNum(0) 
	{
		InitializeCriticalSection(&_csThreadVector);
		InitializeCriticalSection(&_csWorkQueue);

		_EventComplete = CreateEvent(0, false, false, NULL);
		_EventEnd = CreateEvent(0, true, false, NULL);
		_SemaphoreCall = CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
		_SemaphoreDel =  CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);

		assert(_SemaphoreCall != INVALID_HANDLE_VALUE);
		assert(_EventComplete != INVALID_HANDLE_VALUE);
		assert(_EventEnd != INVALID_HANDLE_VALUE);
		assert(_SemaphoreDel != INVALID_HANDLE_VALUE);

		AdjustSize(dwNum <= 0 ? 4 : dwNum);
	}

	~ThreadPool()
	{
		DeleteCriticalSection(&_csWorkQueue);

		CloseHandle(_EventEnd);
		CloseHandle(_EventComplete);
		CloseHandle(_SemaphoreCall);
		CloseHandle(_SemaphoreDel);

		std::vector<ThreadItem* >::iterator iter = _ThreadVector.begin();
		for(; iter != _ThreadVector.end(); iter++)
		{
			if(*iter)
				delete (*iter);
		}

		DeleteCriticalSection(&_csThreadVector);
	}
	//调整线程池规模
	int AdjustSize(int iNum)
	{
		if(iNum > 0)
		{
			ThreadItem *pNew;
			EnterCriticalSection(&_csThreadVector);
			for(int _i=0; _i<iNum; _i++)
			{
				_ThreadVector.push_back(pNew = new ThreadItem(this)); 
				assert(pNew);
				pNew->_Handle = CreateThread(NULL, 0, DefaultJobProc, pNew, 0, NULL);
				assert(pNew->_Handle);
			}
			LeaveCriticalSection(&_csThreadVector);
		}
		else
		{
			iNum *= -1;
			ReleaseSemaphore(_SemaphoreDel,  iNum > _lThreadNum ? _lThreadNum : iNum, NULL);
		}
		return (int)_lThreadNum;
	}
	//调用线程池
	void Call(void (*pFunc)(void  *), void *pPara = NULL)
	{
		assert(pFunc);

		EnterCriticalSection(&_csWorkQueue);
		_JobQueue.push(new JobItem(pFunc, pPara));
		LeaveCriticalSection(&_csWorkQueue);

		ReleaseSemaphore(_SemaphoreCall, 1, NULL);
	}
	//调用线程池
	inline void Call(ThreadJob * p, void *pPara = NULL)
	{
		Call(CallProc, new CallProcPara(p, pPara));
	}
	//结束线程池, 并同步等待
	bool EndAndWait(DWORD dwWaitTime = INFINITE)
	{
		SetEvent(_EventEnd);
		return WaitForSingleObject(_EventComplete, dwWaitTime) == WAIT_OBJECT_0;
	}
	//结束线程池
	inline void End()
	{
		SetEvent(_EventEnd);
	}
	inline DWORD Size()
	{
		return (DWORD)_lThreadNum;
	}
	inline DWORD GetRunningSize()
	{
		return (DWORD)_lRunningNum;
	}
	bool IsRunning()
	{
		return _lRunningNum > 0;
	}

protected:

	//工作线程 静态的。
	static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL)
	{
		ThreadItem *pThread = static_cast<ThreadItem*>(lpParameter);
		assert(pThread);
		// 获取当前线程池的指针
		ThreadPool *pThreadPoolObj = pThread->_pThis;
		assert(pThreadPoolObj);

		InterlockedIncrement(&pThreadPoolObj->_lThreadNum);

		HANDLE hWaitHandle[3];
		hWaitHandle[0] = pThreadPoolObj->_SemaphoreCall;
		hWaitHandle[1] = pThreadPoolObj->_SemaphoreDel;
		hWaitHandle[2] = pThreadPoolObj->_EventEnd;

		JobItem *pJob;
		bool fHasJob;
		//下面一步是关键，一个死循环。 这样就可解决心目中的那些疑团了。  winbase.h 头文件要熟悉！
		for(;;)
		{
			DWORD wr = WaitForMultipleObjects(3, hWaitHandle, false, INFINITE);

			//响应删除线程信号
			if(wr == WAIT_OBJECT_0 + 1)  
				break;

			//从队列里取得用户作业
			EnterCriticalSection(&pThreadPoolObj->_csWorkQueue);
			if(fHasJob = !pThreadPoolObj->_JobQueue.empty())
			{
				pJob = pThreadPoolObj->_JobQueue.front();
				pThreadPoolObj->_JobQueue.pop();
				assert(pJob);
			}
			LeaveCriticalSection(&pThreadPoolObj->_csWorkQueue);

			//受到结束线程信号 确定是否结束线程(结束线程信号 && 是否还有工作)
			if(wr == WAIT_OBJECT_0 + 2 && !fHasJob)  
				break;

			if(fHasJob && pJob)
			{
				InterlockedIncrement(&pThreadPoolObj->_lRunningNum);
				pThread->_dwLastBeginTime = GetTickCount();
				pThread->_dwCount++;
				pThread->_fIsRunning = true;
				pJob->_pFunc(pJob->_pPara); //运行用户作业
				delete pJob; 
				pThread->_fIsRunning = false;
				InterlockedDecrement(&pThreadPoolObj->_lRunningNum);
			}
		}

		//删除自身结构
		EnterCriticalSection(&pThreadPoolObj->_csThreadVector);
		pThreadPoolObj->_ThreadVector.erase(find(pThreadPoolObj->_ThreadVector.begin(), pThreadPoolObj->_ThreadVector.end(), pThread));
		LeaveCriticalSection(&pThreadPoolObj->_csThreadVector);

		delete pThread;

		InterlockedDecrement(&pThreadPoolObj->_lThreadNum);

		if(!pThreadPoolObj->_lThreadNum)  //所有线程结束
			SetEvent(pThreadPoolObj->_EventComplete);

		return 0;
	}
	//调用用户对象虚函数
	static void CallProc(void *pPara) 
	{
		CallProcPara *cp = static_cast<CallProcPara *>(pPara);
		assert(cp);
		if(cp)
		{
			cp->_pObj->DoJob(cp->_pPara);
			delete cp;
		}
	}
	//用户对象结构
	struct CallProcPara  
	{
		ThreadJob* _pObj;//用户对象 
		void *_pPara;//用户参数
		CallProcPara(ThreadJob* p, void *pPara) : _pObj(p), _pPara(pPara) { };
	};
	//用户函数结构
	struct JobItem 
	{
		void (*_pFunc)(void  *);//函数
		void *_pPara; //参数
		JobItem(void (*pFunc)(void  *) = NULL, void *pPara = NULL) : _pFunc(pFunc), _pPara(pPara) { };
	};
	//线程池中的线程结构
	struct ThreadItem
	{
		HANDLE _Handle; //线程句柄
		ThreadPool *_pThis;  //线程池的指针，表明线程是属于那个线程池。
		DWORD _dwLastBeginTime; //最后一次运行开始时间
		DWORD _dwCount; //运行次数
		bool _fIsRunning;
		ThreadItem(ThreadPool *pthis) : _pThis(pthis), _Handle(NULL), _dwLastBeginTime(0), _dwCount(0), _fIsRunning(false) { };
		~ThreadItem()
		{
			if(_Handle)
			{
				CloseHandle(_Handle);
				_Handle = NULL;
			}
		}
	};

	std::queue<JobItem *> _JobQueue;  //工作队列
	std::vector<ThreadItem *>  _ThreadVector; //线程数据

	CRITICAL_SECTION _csThreadVector, _csWorkQueue; //工作队列临界, 线程数据临界

	HANDLE _EventEnd, _EventComplete, _SemaphoreCall, _SemaphoreDel;//结束通知, 完成事件, 工作信号， 删除线程信号
	long _lThreadNum, _lRunningNum; //线程数, 运行的线程数

};

#endif //_ThreadPool_H_



