// ��̸�̳�������.cpp : �������̨Ӧ�ó������ڵ㡣
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
		cout << "������麯����" << endl;
	}

	void fun(){ cout << "--------����---" << endl; }

	void Basefun(){

		cout << "������еĺ���" << endl;
	}
};
class TDrived :public TBase
{
public:
	virtual void Display(){
		cout << "��������麯����" << endl;
	}
	void fun(){ cout << "-------������----" << endl; }
	void TDrivedfun(){

		cout << "��������еĺ���" << endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	//��һ��
	//TBase* tb = new TDrived;
	//tb->Display();

	//�ڶ���
	//TBase* tbb = dynamic_cast<TBase*>(new TDrived);	 
	//tbb->Display();

	//�����֣��޷�ִ�С�
	//TDrived* tdd = dynamic_cast<TDrived*>(new TBase);
	//tdd->Display();

	//������
	//TBase Tbase;
	//TDrived Tdrived; 	
	//Tbase.Display();
	//Tbase = (TBase)Tdrived;
	//Tbase.Display();

	//������  �޷�ת����
	//TBase Tbase;
	//TDrived Tdrived; 	
	//Tdrived.Display();
	//Tdrived = (TDrived)Tbase;
	//Tdrived.Display();


	//ָ�룬���������ʽ��

	//�����֡��޷����У���Ϊһ��Ҫ�������еĵ�������������pDrived�������Ĳ��֡�
	//TBase* pbase = new TBase;
	//TDrived* pDrived = dynamic_cast<TDrived*>(pbase);
	//pDrived->fun();
	//pDrived->Basefun();
	//pDrived->Display();//	��������Ͳ����ˣ������ȥ�����޷����С�

	//�����֡�
	TBase* pbase = new TBase;
	//pbase->fun();
	//pbase->Display();
	//delete pbase;

	//pbase = new TDrived;
	//pbase->fun();//���õĻ����fun����ָ�������йء�
	//pbase->Display();//���õ���������ġ�
	//pbase->TBase::Display(); //��ʾ���û�����麯����

	TDrived* pDrived = dynamic_cast<TDrived*>(pbase);

	//pDrived->Display();
	pDrived->TBase::Display();
	pDrived->Basefun();
	pDrived->fun();


	//�ڰ���
	/*TBase tbase;*/
	//TDrived tdrived;
	////tbase.Display();
	////tbase.Basefun();
	////tbase.fun();

	//TBase tbase = (TBase)tdrived;
	//tbase.Display();//�������ֻ�ǣ����������л������Ĳ��ָ�ֵ����tbase���󣬰���������麯����
	//tbase.Basefun();//���Ե��û��ǻ���ġ�
	//tbase.fun();

	//�ھ���
	//tdrived = (TDrived)tbase;�޷����룬ֻ����ָ�����ת����
	
	//�ܽ᣺1���������캯�������ƹ��캯�������������֣�2��ע�����ָ��ָ����������󣬻���������ָ��ָ�����ʵ�����󣬵��õķ���������������麯�������붯̬�����йأ��������ָ�������йأ�Ҳ���ܷ����ض������ڴ�й©��
	

	system("pause");

	return 0;
}

