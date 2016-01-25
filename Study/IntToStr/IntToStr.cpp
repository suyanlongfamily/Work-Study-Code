// IntToStr.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;


//************************************
// Method:    StrToUnsigned
// FullName:  StrToUnsigned
// Access:    public 
// Returns:   unsigned
// Qualifier://字符串整形转化为无符号整形。
// Parameter: char * str
//************************************
unsigned StrToUnsigned(char* str)
{
	int size = strlen(str);
	int num = 0;
	for (unsigned i = 0; i < size; ++i)
	{
		num *= 10;
		char temp = str[i];
		num += temp - '0';
	}
	return num;
}

string UnsignedToStr(unsigned unint)
{
	string str = "";
	unsigned length = 0;
	unsigned zheng = 0;
	do 
	{

		unsigned num = unint % 10;
		char poschar = num + '0';
		unint = unint / 10;
		str.insert(str.begin(), poschar);

	} while (unint / 10);
	return str;
}

int _tmain(int argc, _TCHAR* argv[])
{
	string str = "";
	cin >> str;
	string returnstr = UnsignedToStr(143124);
	
	cout << StrToUnsigned(const_cast<char*>(str.c_str())) << endl;
	std::system("pause");
	return 0;
}

