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

	void fun(){ cout << "--------基类的普通函数。---" << endl; }

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
	void fun(){ cout << "-------派生类的普通函数。----" << endl; }
	void TDrivedfun(){

		cout << "派生类独有的函数" << endl;
	}
};


void fun(TBase& base)
{
	base.Display();
}


int _tmain(int argc, _TCHAR* argv[])
{
	//第一种
	//TBase* tb = new TDrived;
	//tb->Display();//多态，调用子类的

	//第二种
	//TBase* tbb = dynamic_cast<TBase*>(new TDrived);	 
	//tbb->Display();//多态，调用子类的

	//第三种，无法执行。
	//TDrived* tdd = dynamic_cast<TDrived*>(new TBase);
	//tdd->Display();	 //不存在，截断现象

	//第四种
	//TBase Tbase; //已经存在内存区域了
	//TDrived Tdrived; 	
	////Tbase.Display();
	////Tbase = (TBase)Tdrived;
	////Tbase.Display();
	//Tbase = Tdrived;
	//Tbase.Display();


	//第四种

	TDrived Tdrived; 
	TBase Tbase = Tdrived; //已经存在内存区域了 赋值构造函数 内存的复制。相当于把派生类中父类部分复制过去了。
	Tbase.Display();	//还是基类的
	Tbase.fun();	  //还是基类的

	TBase tbase = (TBase)Tdrived;
	tbase.Display();//这种情况只是，把派生类中基类对象的部分赋值给了tbase对象，包括基类的虚函数表。
	tbase.Basefun();//所以调用还是基类的。
	tbase.fun();


	TBase& base = Tdrived; //引用 
	base.Display();	 //调用是的派生类函数。
	base.fun();		//还是基类的普通函数。

	TBase* pTBase =new TDrived;//指针值的复制，派生类中父类部分，没有复制。
	pTBase->Display();// 派生类 。查找虚函数表。
	pTBase->fun(); //还是基类的普通函数。不管是对象，引用，还是指针，都是编译其左边类型决定。
	//pTBase->TDrivedfun();//看这个语句就知道，指向派生类对象中部分。派生类对象的本身部分，仍是无法访问。
	cout<<"-----------------"<<endl;
	TDrived * pDrived = (TDrived *)pTBase;	 
	pDrived->fun();
	pDrived->Display();
	pDrived->Basefun();
	pDrived->TDrivedfun(); 
	cout<<"-----------------"<<endl;
	fun(Tdrived);


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
	//TBase* pbase ;//= new TBase;
	//pbase->fun();
	//pbase->Display();
	//delete pbase;

	//pbase = new TDrived;
	//pbase->fun();//调用的基类的fun，与指针类型有关。

	//pbase->Display();//调用的是派生类的。
	//pbase->TBase::Display(); //显示调用基类的虚函数。

	//TDrived* pDrived = dynamic_cast<TDrived*>(pbase);

	////pDrived->Display();
	//pDrived->TBase::Display();
	//pDrived->Basefun();
	//pDrived->fun();


	//第八种
	/*TBase tbase;*/
	//TDrived tdrived;
	////tbase.Display();
	////tbase.Basefun();
	////tbase.fun();



	//第九种
	//tdrived = (TDrived)tbase;//无法编译，只能是指针才能转换。

	//总结：
	//1、拷贝构造函数（复制构造函数）在里面作怪，
	//2、注意基类指针指向派生类对象，或者派生类指针指向基类实例对象，调用的方法，如果调用是虚函数，就与动态类型有关，否则就与指针类型有关，也可能发生截断现象，内存泄漏。

	/*
		再次总结：
		1、类型一词，代表内存中作用范围，大小，限制域，地址的长度。
		2、TDrived Tdrived; 
		   TBase Tbase = Tdrived; 这种形式，相等于对象复制，即子类对象部分赋值给另一个子类对象，结果就明明白白了。
		3、引用同之争一样，就说指针情形。
		   TBase* pTBase =new TDrived 
		   既然说了类型，决定了内中布局大小，范围，边界，所以，相当于指向了派生类基类部分的那块内存位置，
		   所以以下形式。
		   pTBase->fun();//调用的还是基类的， 
		   pTBase->TDrivedfun();// 基类部分中不存在这个函数，截断现象，内存泄漏。
		   pTBase->Display();//虚函数，运行期通过查虚函数表。运行父类对象的部分的重写的函数。

		4、java的覆盖重写，没有这么麻烦。Java中只要，子类与父类普通方法签名一样就可以实现重写功能，实现多态性。接口、或者父类做左边的对象，左边是实例，是实现接口与父类的子类实例。
		这也是为啥，有些书上一直强调，C++周恩，父类普通函数千万不要重写的原因。C ++中而如果要作为一个接口必须都是viutral，或者类死abstract.


		以上总结完毕：记得与java的区别了吧！！！！
	*/

	system("pause");

	return 0;
}

