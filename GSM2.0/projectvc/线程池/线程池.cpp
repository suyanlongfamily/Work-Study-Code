// �̳߳�.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "FThreadPool.h"
#include <iostream>
#include <string>
using namespace std;

void fun(void* p)
{
	cout<<"�̳߳أ�"<<endl;

}


class Test
{
public:
	static void fun()
	{
		cout<<"---"<<endl;
	}
protected:
private:
};


void DealDataFun(
	__out LPDWORD lpNumberOfBytesTransferred,
	__out PULONG_PTR lpCompletionKey,
	__out LPOVERLAPPED *lpOverlapped
	)
{

	//����xnetd���͹������ݵ���Ҫ�̡߳�����������
	int* p = (int*)lpCompletionKey;

	while (*lpCompletionKey)
	{
		cout<<"----"<<*lpCompletionKey<<endl;
		(*lpCompletionKey)--;	
	} 
}

int _tmain(int argc, _TCHAR* argv[])
{
	//ThreadPool tp;
	//for (int i=0;i<200;i++)
	//{
	//	tp.Call(fun,0);
	//}
	//Test t;

	FThreadPool* threadPool = FThreadPool::GetTreadPool();

	threadPool->StartTreadPool(DealDataFun,NULL);
	

	int* p = new int(900);
	threadPool->PostTaskToQueue(0,(ULONG_PTR)p);


	//threadPool.StartTreadPool();

	std::system("pause");
	return 0;
}

