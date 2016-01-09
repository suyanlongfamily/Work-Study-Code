// 线程池.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "FThreadPool.h"
#include <iostream>
#include <string>
using namespace std;

void fun(void* p)
{
	cout<<"线程池！"<<endl;

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

int _tmain(int argc, _TCHAR* argv[])
{
	ThreadPool tp;
	for (int i=0;i<200;i++)
	{
		tp.Call(fun,0);
	}
	Test t;

	std::system("pause");
	return 0;
}

