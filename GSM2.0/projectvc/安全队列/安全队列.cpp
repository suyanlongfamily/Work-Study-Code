// 安全队列.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TSafeQueue.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	TSafeQueue<int> queued;
	queued.PostMessage(3);
	int i = queued.GetMessage();
	cout<<i<<endl;
	std::system("pause");
	return 0;
}

