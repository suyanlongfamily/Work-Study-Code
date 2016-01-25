// 在谈继承与派生.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;
class TBase
{
public:
	TBase(){}
	~TBase(){}
	virtual void Display(){
		cout << "基类的虚函数。" << endl;
	}

	void fun(){ cout << "--------基类---" << endl; }

	void Basefun(){

		cout << "基类独有的函数" << endl;
	}
};
class TDrived :public TBase
{
public:
	virtual void Display(){
		cout << "派生类的虚函数。" << endl;
	}
	void fun(){ cout << "-------派生类----" << endl; }
	void TDrivedfun(){

		cout << "派生类独有的函数" << endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	//第一种
	//TBase* tb = new TDrived;
	//tb->Display();

	//第二种
	//TBase* tbb = dynamic_cast<TBase*>(new TDrived);	 
	//tbb->Display();

	//第三种，无法执行。
	//TDrived* tdd = dynamic_cast<TDrived*>(new TBase);
	//tdd->Display();

	//第四种
	//TBase Tbase;
	//TDrived Tdrived; 	
	//Tbase.Display();
	//Tbase = (TBase)Tdrived;
	//Tbase.Display();

	//第五种  无法转换。
	//TBase Tbase;
	//TDrived Tdrived; 	
	//Tdrived.Display();
	//Tdrived = (TDrived)Tbase;
	//Tdrived.Display();


	//指针，对象，组合形式。

	//第六种。无法运行，因为一定要明白其中的道理。仅仅拷贝了pDrived里面基类的部分。
	//TBase* pbase = new TBase;
	//TDrived* pDrived = dynamic_cast<TDrived*>(pbase);
	//pDrived->fun();
	//pDrived->Basefun();
	//pDrived->Display();//	这种情况就不行了，编译过去但是无法运行。

	//第七种。
	TBase* pbase = new TBase;
	//pbase->fun();
	//pbase->Display();
	//delete pbase;

	//pbase = new TDrived;
	//pbase->fun();//调用的基类的fun，与指针类型有关。
	//pbase->Display();//调用的是派生类的。
	//pbase->TBase::Display(); //显示调用基类的虚函数。

	TDrived* pDrived = dynamic_cast<TDrived*>(pbase);

	//pDrived->Display();
	pDrived->TBase::Display();
	pDrived->Basefun();
	pDrived->fun();
	pDrived->TDrivedfun();//这个居然也能执行！

	TDrived* pDrived1 = (TDrived*)pbase;
	pDrived1->Display();
	pDrived1->TBase::Display();
	pDrived1->Basefun();
	pDrived1->fun();//调用基类的
	pDrived1->TDrivedfun();//同样也可以调用，只是相当于，一个公共函数。
	//-----------------
	//对比以上两个类型转化。明白。
	//1、指针变量转换，不管是类继承里面还是不相同类型转换的，都不会引起多大问题，都是只是一个内存的地址。即32位的正整数。
	//2、区别（T*）、与dynamic_cast<TDrived*>指针类型转换的区别。
	//3、指针本质，不管什么类型的转换，其实就是形参表达就是一个指针（地址）容器，不管是什么类型，都是只是一个内存的地址。即32位的正整数，到时候强制转换就可以了。
	//以上才是指针的强大之处，是指针本质，其实，指针就是汇编里面的地址寻址，的知识点！
	//-----------------
	//指针的转换与对象之间的转换要区别开来。
	//对象之间的转换其实就是，赋值构造函数与复制构造函数的应用！与指针不一样，指针都是指向同一个地址。不存在赋值构造函数与复制构造函数，仅仅只是地址的复制、赋值。
	TDrived td;
	TBase base = td;


	//第八种
	/*TBase tbase;*/
	//TDrived tdrived;
	////tbase.Display();
	////tbase.Basefun();
	////tbase.fun();

	//TBase tbase = (TBase)tdrived;
	//tbase.Display();//这种情况只是，把派生类中基类对象的部分赋值给了tbase对象，包括基类的虚函数表。
	//tbase.Basefun();//所以调用还是基类的。
	//tbase.fun();

	//第九种
	//tdrived = (TDrived)tbase;无法编译，只能是指针才能转换。
	
	//总结：1、拷贝构造函数（复制构造函数）在里面作怪，2、注意基类指针指向派生类对象，或者派生类指针指向基类实例对象，调用的方法，如果调用是虚函数，就与动态类型有关，否则就与指针类型有关，也可能发生截断现象，内存泄漏。
	

	system("pause");

	return 0;
}

