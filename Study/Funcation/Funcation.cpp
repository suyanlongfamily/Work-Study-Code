// Funcation.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

struct MyStruct
{
	int * param;
	char chr;
	int k;
	double p;
};

void fun(int* param)
{
	cout << "参数地址：" << param << endl;
	struct MyStruct* ms= (struct MyStruct*)param;
	cout << "转化后的地址：" << ms << endl;
	(*ms->param) = 100;
	ms->chr = 's';
	ms->k = 10000;
	ms->p = 1000.1;
	param = (int*)ms;
}
//了解内存布局。即结构体与类的对象的内存布局。

int _tmain(int argc, _TCHAR* argv[])
{
	
	struct MyStruct mystrcuct;
	cout << "对象的地址：" << (int)&mystrcuct << endl;
	memset(&mystrcuct, 0, sizeof(mystrcuct));
	int i = 100;
	//第一区别，指针变量与指针的意义。指针是一个地址，指针变量是容纳指针（地址）的容器。
	//第二区别，结构体有指针变量，要想使用指针变量，一定要指向一个内存块的的地址。所以要new厨业对象。
	//第三区别，指针其实就是一个正整形数，不管是什么类型的指针，所以都可以相互话转化。
	mystrcuct.param = new int;
	int size = sizeof(mystrcuct.param);
	//这种转换技巧一定要学会，可以带入参数更多的信息，这也是指针强大之处。很重要！！！
	//这也变向的像我们说明其实带入一个参数就可以了，类是windwos创建线程时穿进去的那个参数一样。
	//这种技巧是什么技巧？其实就是形参表达就是一个指针（地址）容器，不管是什么类型，到时候强制转换就可以了。
	//而且不管是什么类型的指针（地址），都是一个固定长度，因此需要能容纳值得变量就可以了。
	fun((int*)&mystrcuct);
	cout << "第1个成员对象："<<(*mystrcuct.param)<< endl;
	cout << "第2个成员对象：" << mystrcuct.chr<< endl;
	cout << "第3个成员对象：" << mystrcuct.k<< endl;

	//----------------------------------------------------------------------------------
	//1、结构体或者类对象成员的地址都是连续分布的，除了new出来在堆栈上的不一样。
	//2、
	cout << "对象的地址："<<(int)&mystrcuct << endl;
	cout << "第1个成员对象的地址：" << (int)mystrcuct.param << endl;
	cout << "第2个成员对象的地址：" << (int)&mystrcuct.chr << endl;
	cout << "第3个成员对象的地址：" << (int)&mystrcuct.k << endl;
	cout << "第4个成员对象的地址：" << (int)&mystrcuct.p << endl;
	

	return 0;
}

