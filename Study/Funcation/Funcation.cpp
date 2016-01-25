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
	struct MyStruct* ms = (struct MyStruct*)param;
	cout << "转化后的地址：" << ms << endl;
	(*ms->param) = 100;
	ms->chr = 's';
	ms->k = 10000;
	ms->p = 1000.1;
	param = (int*)ms;
}

class TBase
{
public:
	void display(){ cout << "你好基类" <<endl; }
	void virtual virFuncation(){ cout << "nihao基类" << endl; }
protected:
private:
	int year;
	string str;
};

//对比前面的结构体与这个却别。
class TDrived:public TBase
{
public:
	int dd;//声明！是声明，成员变量。
	void virtual virFuncation(){ cout << "nihao派生类" << endl; }
	void TDrivedDisplay(){ cout << "你好派生类" << endl; }
	void TDrivedDisplay1(){ cout << "你好派生类" <<dyear<< endl; }
protected:
private:
	int dyear;//这里面仅仅是声明，声明一个类，声明类的成员函数。
	string dstr;
};

void fun1(TBase* pbase)
{
	//1、不要被智能（代码提示）助手迷惑了！
	//2、理解（T*）转换规则的强大！对不相同类型指针也可以转换。
	//3、在（T*）转换规则下，都可以调用，并且也都成功。
	//--------------------------------------------------
	TBase* baseTemp = new TBase;
	TDrived* p1 = (TDrived*)(baseTemp);
	p1->TDrivedDisplay1();//能编译也能执行。

	TDrived* p2 = dynamic_cast<TDrived*>(baseTemp);
	p2->TDrivedDisplay1();//能编译过去，但是执行报错。
	//--------------------------------------------------

	TDrived* pDrived = (TDrived*)pbase;//其实pdirved与pbase容纳的地址值是一样，表示指针同一块地址。
	pDrived->display();
	pDrived->dd = 0;//这个就是定义，赋值。这个能编译过，也能顺利执行，是因为在类内是public成员，
	//且在类内已经声明，在声明pDrived指针变量时已经“统一表示过了”，所以在这里引用可以用，赋值,
	pDrived->TDrivedDisplay();//同样能编译过去，执行也成功。
	pDrived->TDrivedDisplay1();
	pDrived->virFuncation();
}



//了解内存布局。即结构体与类的对象的内存布局。

int _tmain(int argc, _TCHAR* argv[])
{
	
	struct MyStruct mystrcuct;
	cout << "对象的地址：" << (int)&mystrcuct << endl;
	memset(&mystrcuct, 0, sizeof(mystrcuct));
	int i = 100;
	//第一区别，指针变量与指针的意义。指针是一个地址，指针变量是容纳指针（地址）的容器。
	//第二区别，结构体有指针变量，要想使用指针变量，一定要指向一个内存块的的地址。所以要new出对象。
	//第三区别，指针其实就是一个正整形数，不管是什么类型的指针，所以都可以相互话转化。
	mystrcuct.param = new int;
	int size = sizeof(mystrcuct.param);
	//这种转换技巧一定要学会，可以带入参数更多的信息，这也是指针强大之处。很重要！！！
	//这也变向的像我们说明其实带入一个参数就可以了，类是windwos创建线程时穿进去的那个参数一样。
	//这种技巧是什么技巧？其实就是形参表达就是一个指针（地址）容器，不管是什么类型，到时候强制转换就可以了。
	//而且不管是什么类型的指针（地址），都是一个固定长度，因此需要能容纳值的指针变量就可以了。所以不要被形参的指针变脸类型迷惑了。
	//这个地方区别与C++基类与派生类。
	fun((int*)&mystrcuct);
	cout << "第1个成员对象："<<(*mystrcuct.param)<< endl;
	cout << "第2个成员对象：" << mystrcuct.chr<< endl;
	cout << "第3个成员对象：" << mystrcuct.k<< endl;

	//----------------------------------------------------------------------------------
	//1、结构体或者类对象成员的地址都是连续分布的，除了new出来在堆栈上的不一样。
	//2、C++、C里面的指针其实就是对应汇编里面的地址寻址，开放出来的。
	cout << "对象的地址："<<(int)&mystrcuct << endl;
	cout << "第1个成员对象的地址：" << (int)mystrcuct.param << endl;
	cout << "第2个成员对象的地址：" << (int)&mystrcuct.chr << endl;
	cout << "第3个成员对象的地址：" << (int)&mystrcuct.k << endl;
	cout << "第4个成员对象的地址：" << (int)&mystrcuct.p << endl;
	
	TBase* pbase = new TBase;
	fun1(pbase);

	return 0;
}

