// split.cpp : ??g?S???L???[??t???O??
//

#include <string>
#include <iostream>
using namespace std;

class person{
public:
	virtual void display() = 0;
};

class student:public person
{
public:
	void display(){

		cout<<"jiaoshi  "<<endl;
	}
		student(){	
			name = "";
			age = 0;
		}

		student(int a,string str):age(a),name(str){	 			
			//?C?? ?z?C????a?w????
			age = a;
			str = str;
			

			cout<<"-----------"<<endl;	 
		}
		student& operator = (student& t){
			cout<<"========"<<endl;	
		}

		student(student&);

		string name;
		int age;
};

student::student(student& t){  
	cout<<"=999999999="<<endl;	
	this->age = t.age;
	this->name = t.name;
}

void fun(student a){

	  a.age = 90;
}

void fun(int a){

	cout<<"========"<<endl;	

}

void fun(int a,int b){

	cout<<"========"<<endl;
}

void fun(int a,int b,int c){

	cout<<"========"<<endl;
}


class teacher:public person{

public:
	void display(){

		cout<<"jiaoshi  "<<endl;
	}
	
};


int funkkk(int k){

	if (k == 3)
	{
		throw 33;
	}
	return k;
}
int main()		 
{	
	//student k(12,"asdf");
	//student t;
	//t.display();
	//teacher ter;
	//ter.display();
	////fun(t);

	//person* pp = &t;

	//pp->display();

	//system("pause");
	//string strProd = "143124|51431|414324|1432";
	//while (1)
	//{
	//	int index = strProd.find_first_of('|');
	//	string strVul = strProd.substr(index);
	//}


//	char str[]="???????????";
//	char str1[]="???";
//	int length = sizeof(str);
//	length = strlen(str);	
//\u9713\u8679\u9F8D
//	int length1 = sizeof(str1);
//	length1 = strlen(str1);	
		int kkk = -1	  ;
	try
	{
		kkk = funkkk(3);
	}
	catch (...)
	{
		cout<<kkk<<endl;
	}
	
	

	return 0;
}


