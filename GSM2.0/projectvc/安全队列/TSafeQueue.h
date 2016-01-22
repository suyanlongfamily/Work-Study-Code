#ifndef TSAFEQUEUE_H
#define TSAFEQUEUE_H
#include <string>
#include <queue>
#include <Windows.h>
 /*!
��ʹ����������֮ǰ����Ҫ����һ��CONDITION_VARIABLE���͵ı�����
CONDITION_VARIABLE
�����������йصĺ�����5��;
InitializeConditionVariable
SleepConditionVariableCS
SleepConditionVariableSRW
WakeConditionVariable
WakeAllConditionVariable
*/


#pragma once
template<class T>
class TSafeQueue
{
public:
	TSafeQueue(void){
		InitializeConditionVariable(&cv);
		InitializeCriticalSection(&cs);
	}

	void PostMessage(T t){

		EnterCriticalSection(&cs);
		queued.push(t);
		LeaveCriticalSection(&cs);
		WakeConditionVariable(&cv);
	}

	T  GetMessage()
	{
		if (queued.empty())
		{
			SleepConditionVariableCS(&cv,&cs,INFINITE);
		}
		return queued.front();
	}

	~TSafeQueue(void)
	{
		DeleteCriticalSection(&cs);
	}
private:
	std::queue<T> queued;
	CONDITION_VARIABLE cv;
	CRITICAL_SECTION cs;
};

#endif	TSAFEQUEUE_H

