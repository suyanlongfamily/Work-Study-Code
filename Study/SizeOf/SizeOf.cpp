// SizeOf.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{

	string str = "";
	int length = sizeof(str);

	str = "suyanlo1111111111111111111111111111111111111111111111111111111111111111111111111111111111ng";
	length = sizeof(str);

	//理解sizeof
	return 0;
}

