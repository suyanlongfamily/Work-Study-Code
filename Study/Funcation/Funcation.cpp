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
	struct MyStruct* ms= (struct MyStruct*)param;
	cout << "ת����ĵ�ַ��" << ms << endl;
	(*ms->param) = 100;
	ms->chr = 's';
	ms->k = 10000;
	ms->p = 1000.1;
	param = (int*)ms;
}
//�˽��ڴ沼�֡����ṹ������Ķ�����ڴ沼�֡�

int _tmain(int argc, _TCHAR* argv[])
{
	
	struct MyStruct mystrcuct;
	cout << "����ĵ�ַ��" << (int)&mystrcuct << endl;
	memset(&mystrcuct, 0, sizeof(mystrcuct));
	int i = 100;
	//��һ����ָ�������ָ������塣ָ����һ����ַ��ָ�����������ָ�루��ַ����������
	//�ڶ����𣬽ṹ����ָ�������Ҫ��ʹ��ָ�������һ��Ҫָ��һ���ڴ��ĵĵ�ַ������Ҫnew��ҵ����
	//��������ָ����ʵ����һ������������������ʲô���͵�ָ�룬���Զ������໥��ת����
	mystrcuct.param = new int;
	int size = sizeof(mystrcuct.param);
	//����ת������һ��Ҫѧ�ᣬ���Դ�������������Ϣ����Ҳ��ָ��ǿ��֮��������Ҫ������
	//��Ҳ�����������˵����ʵ����һ�������Ϳ����ˣ�����windwos�����߳�ʱ����ȥ���Ǹ�����һ����
	//���ּ�����ʲô���ɣ���ʵ�����βα�����һ��ָ�루��ַ��������������ʲô���ͣ���ʱ��ǿ��ת���Ϳ����ˡ�
	//���Ҳ�����ʲô���͵�ָ�루��ַ��������һ���̶����ȣ������Ҫ������ֵ�ñ����Ϳ����ˡ�
	fun((int*)&mystrcuct);
	cout << "��1����Ա����"<<(*mystrcuct.param)<< endl;
	cout << "��2����Ա����" << mystrcuct.chr<< endl;
	cout << "��3����Ա����" << mystrcuct.k<< endl;

	//----------------------------------------------------------------------------------
	//1���ṹ�����������Ա�ĵ�ַ���������ֲ��ģ�����new�����ڶ�ջ�ϵĲ�һ����
	//2��
	cout << "����ĵ�ַ��"<<(int)&mystrcuct << endl;
	cout << "��1����Ա����ĵ�ַ��" << (int)mystrcuct.param << endl;
	cout << "��2����Ա����ĵ�ַ��" << (int)&mystrcuct.chr << endl;
	cout << "��3����Ա����ĵ�ַ��" << (int)&mystrcuct.k << endl;
	cout << "��4����Ա����ĵ�ַ��" << (int)&mystrcuct.p << endl;
	

	return 0;
}

