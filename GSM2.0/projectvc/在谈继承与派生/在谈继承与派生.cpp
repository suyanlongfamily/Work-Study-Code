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

	void fun(){ cout << "--------�������ͨ������---" << endl; }

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
	void fun(){ cout << "-------���������ͨ������----" << endl; }
	void TDrivedfun(){

		cout << "��������еĺ���" << endl;
	}
};


void fun(TBase& base)
{
	base.Display();
}


int _tmain(int argc, _TCHAR* argv[])
{
	//��һ��
	//TBase* tb = new TDrived;
	//tb->Display();//��̬�����������

	//�ڶ���
	//TBase* tbb = dynamic_cast<TBase*>(new TDrived);	 
	//tbb->Display();//��̬�����������

	//�����֣��޷�ִ�С�
	//TDrived* tdd = dynamic_cast<TDrived*>(new TBase);
	//tdd->Display();	 //�����ڣ��ض�����

	//������
	//TBase Tbase; //�Ѿ������ڴ�������
	//TDrived Tdrived; 	
	////Tbase.Display();
	////Tbase = (TBase)Tdrived;
	////Tbase.Display();
	//Tbase = Tdrived;
	//Tbase.Display();


	//������

	TDrived Tdrived; 
	TBase Tbase = Tdrived; //�Ѿ������ڴ������� ��ֵ���캯�� �ڴ�ĸ��ơ��൱�ڰ��������и��ಿ�ָ��ƹ�ȥ�ˡ�
	Tbase.Display();	//���ǻ����
	Tbase.fun();	  //���ǻ����

	TBase tbase = (TBase)Tdrived;
	tbase.Display();//�������ֻ�ǣ����������л������Ĳ��ָ�ֵ����tbase���󣬰���������麯����
	tbase.Basefun();//���Ե��û��ǻ���ġ�
	tbase.fun();


	TBase& base = Tdrived; //���� 
	base.Display();	 //�����ǵ������ຯ����
	base.fun();		//���ǻ������ͨ������

	TBase* pTBase =new TDrived;//ָ��ֵ�ĸ��ƣ��������и��ಿ�֣�û�и��ơ�
	pTBase->Display();// ������ �������麯����
	pTBase->fun(); //���ǻ������ͨ�����������Ƕ������ã�����ָ�룬���Ǳ�����������;�����
	//pTBase->TDrivedfun();//���������֪����ָ������������в��֡����������ı����֣������޷����ʡ�
	cout<<"-----------------"<<endl;
	TDrived * pDrived = (TDrived *)pTBase;	 
	pDrived->fun();
	pDrived->Display();
	pDrived->Basefun();
	pDrived->TDrivedfun(); 
	cout<<"-----------------"<<endl;
	fun(Tdrived);


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
	//TBase* pbase ;//= new TBase;
	//pbase->fun();
	//pbase->Display();
	//delete pbase;

	//pbase = new TDrived;
	//pbase->fun();//���õĻ����fun����ָ�������йء�

	//pbase->Display();//���õ���������ġ�
	//pbase->TBase::Display(); //��ʾ���û�����麯����

	//TDrived* pDrived = dynamic_cast<TDrived*>(pbase);

	////pDrived->Display();
	//pDrived->TBase::Display();
	//pDrived->Basefun();
	//pDrived->fun();


	//�ڰ���
	/*TBase tbase;*/
	//TDrived tdrived;
	////tbase.Display();
	////tbase.Basefun();
	////tbase.fun();



	//�ھ���
	//tdrived = (TDrived)tbase;//�޷����룬ֻ����ָ�����ת����

	//�ܽ᣺
	//1���������캯�������ƹ��캯�������������֣�
	//2��ע�����ָ��ָ����������󣬻���������ָ��ָ�����ʵ�����󣬵��õķ���������������麯�������붯̬�����йأ��������ָ�������йأ�Ҳ���ܷ����ض������ڴ�й©��

	/*
		�ٴ��ܽ᣺
		1������һ�ʣ������ڴ������÷�Χ����С�������򣬵�ַ�ĳ��ȡ�
		2��TDrived Tdrived; 
		   TBase Tbase = Tdrived; ������ʽ������ڶ����ƣ���������󲿷ָ�ֵ����һ��������󣬽���������װ��ˡ�
		3������֮ͬ��һ������˵ָ�����Ρ�
		   TBase* pTBase =new TDrived 
		   ��Ȼ˵�����ͣ����������в��ִ�С����Χ���߽磬���ԣ��൱��ָ������������ಿ�ֵ��ǿ��ڴ�λ�ã�
		   ����������ʽ��
		   pTBase->fun();//���õĻ��ǻ���ģ� 
		   pTBase->TDrivedfun();// ���ಿ���в���������������ض������ڴ�й©��
		   pTBase->Display();//�麯����������ͨ�����麯�������и������Ĳ��ֵ���д�ĺ�����

		4��java�ĸ�����д��û����ô�鷳��Java��ֻҪ�������븸����ͨ����ǩ��һ���Ϳ���ʵ����д���ܣ�ʵ�ֶ�̬�ԡ��ӿڡ����߸�������ߵĶ��������ʵ������ʵ�ֽӿ��븸�������ʵ����
		��Ҳ��Ϊɶ����Щ����һֱǿ����C++�ܶ���������ͨ����ǧ��Ҫ��д��ԭ��C ++�ж����Ҫ��Ϊһ���ӿڱ��붼��viutral����������abstract.


		�����ܽ���ϣ��ǵ���java�������˰ɣ�������
	*/

	system("pause");

	return 0;
}

