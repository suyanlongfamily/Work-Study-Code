// Funcation.cpp : �������̨Ӧ�ó������ڵ㡣
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
	cout << "������ַ��" << param << endl;
	struct MyStruct* ms = (struct MyStruct*)param;
	cout << "ת����ĵ�ַ��" << ms << endl;
	(*ms->param) = 100;
	ms->chr = 's';
	ms->k = 10000;
	ms->p = 1000.1;
	param = (int*)ms;
}

class TBase
{
public:
	void display(){ cout << "��û���" <<endl; }
	void virtual virFuncation(){ cout << "nihao����" << endl; }
protected:
private:
	int year;
	string str;
};

//�Ա�ǰ��Ľṹ�������ȴ��
class TDrived:public TBase
{
public:
	int dd;//����������������Ա������
	void virtual virFuncation(){ cout << "nihao������" << endl; }
	void TDrivedDisplay(){ cout << "���������" << endl; }
	void TDrivedDisplay1(){ cout << "���������" <<dyear<< endl; }
protected:
private:
	int dyear;//���������������������һ���࣬������ĳ�Ա������
	string dstr;
};

void fun1(TBase* pbase)
{
	//1����Ҫ�����ܣ�������ʾ�������Ի��ˣ�
	//2����⣨T*��ת�������ǿ�󣡶Բ���ͬ����ָ��Ҳ����ת����
	//3���ڣ�T*��ת�������£������Ե��ã�����Ҳ���ɹ���
	//--------------------------------------------------
	TBase* baseTemp = new TBase;
	TDrived* p1 = (TDrived*)(baseTemp);
	p1->TDrivedDisplay1();//�ܱ���Ҳ��ִ�С�

	TDrived* p2 = dynamic_cast<TDrived*>(baseTemp);
	p2->TDrivedDisplay1();//�ܱ����ȥ������ִ�б���
	//--------------------------------------------------

	TDrived* pDrived = (TDrived*)pbase;//��ʵpdirved��pbase���ɵĵ�ֵַ��һ������ʾָ��ͬһ���ַ��
	pDrived->display();
	pDrived->dd = 0;//������Ƕ��壬��ֵ������ܱ������Ҳ��˳��ִ�У�����Ϊ��������public��Ա��
	//���������Ѿ�������������pDrivedָ�����ʱ�Ѿ���ͳһ��ʾ���ˡ����������������ÿ����ã���ֵ,
	pDrived->TDrivedDisplay();//ͬ���ܱ����ȥ��ִ��Ҳ�ɹ���
	pDrived->TDrivedDisplay1();
	pDrived->virFuncation();
}



//�˽��ڴ沼�֡����ṹ������Ķ�����ڴ沼�֡�

int _tmain(int argc, _TCHAR* argv[])
{
	
	struct MyStruct mystrcuct;
	cout << "����ĵ�ַ��" << (int)&mystrcuct << endl;
	memset(&mystrcuct, 0, sizeof(mystrcuct));
	int i = 100;
	//��һ����ָ�������ָ������塣ָ����һ����ַ��ָ�����������ָ�루��ַ����������
	//�ڶ����𣬽ṹ����ָ�������Ҫ��ʹ��ָ�������һ��Ҫָ��һ���ڴ��ĵĵ�ַ������Ҫnew������
	//��������ָ����ʵ����һ������������������ʲô���͵�ָ�룬���Զ������໥��ת����
	mystrcuct.param = new int;
	int size = sizeof(mystrcuct.param);
	//����ת������һ��Ҫѧ�ᣬ���Դ�������������Ϣ����Ҳ��ָ��ǿ��֮��������Ҫ������
	//��Ҳ�����������˵����ʵ����һ�������Ϳ����ˣ�����windwos�����߳�ʱ����ȥ���Ǹ�����һ����
	//���ּ�����ʲô���ɣ���ʵ�����βα�����һ��ָ�루��ַ��������������ʲô���ͣ���ʱ��ǿ��ת���Ϳ����ˡ�
	//���Ҳ�����ʲô���͵�ָ�루��ַ��������һ���̶����ȣ������Ҫ������ֵ��ָ������Ϳ����ˡ����Բ�Ҫ���βε�ָ����������Ի��ˡ�
	//����ط�������C++�����������ࡣ
	fun((int*)&mystrcuct);
	cout << "��1����Ա����"<<(*mystrcuct.param)<< endl;
	cout << "��2����Ա����" << mystrcuct.chr<< endl;
	cout << "��3����Ա����" << mystrcuct.k<< endl;

	//----------------------------------------------------------------------------------
	//1���ṹ�����������Ա�ĵ�ַ���������ֲ��ģ�����new�����ڶ�ջ�ϵĲ�һ����
	//2��C++��C�����ָ����ʵ���Ƕ�Ӧ�������ĵ�ַѰַ�����ų����ġ�
	cout << "����ĵ�ַ��"<<(int)&mystrcuct << endl;
	cout << "��1����Ա����ĵ�ַ��" << (int)mystrcuct.param << endl;
	cout << "��2����Ա����ĵ�ַ��" << (int)&mystrcuct.chr << endl;
	cout << "��3����Ա����ĵ�ַ��" << (int)&mystrcuct.k << endl;
	cout << "��4����Ա����ĵ�ַ��" << (int)&mystrcuct.p << endl;
	
	TBase* pbase = new TBase;
	fun1(pbase);

	return 0;
}

