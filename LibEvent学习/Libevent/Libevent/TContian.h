#ifndef _TCONTIAN_H 
#define _TCONTIAN_H	  
#pragma once

#include <string>
#include <map>
#include <iostream>
#include <vector>
using namespace std; 

/*======================================================================================================================*/
//���ַ�ʽ��ֻ��ʹ���ڱ��ػ����ϴ��䡣��������socket֮��Ͳ����ˡ����Ǽ�ס�ɣ�
//��ʵ��ֻҪ��ȷ�����ȾͿ��ԣ����ǣ�string���ͣ��Ƕ�̬����ģ������ڱ����ڼ����޷�ͨ��sizeofʵ�ֵġ�
typedef struct THeadData{
	char version[10];
	char way;  
	char flag;
}HeadData; 

typedef struct TBodyData{
	string title; 
	char* pdata; //����ָ��
	int length;	
}BodyData; 

typedef struct TPackData{
	HeadData headdata;
	BodyData bodyData;	
} PackData; 
/*���ϴ���ֻ��ʹ����ͬһ�ڴ���======================================================================================================================*/


class TContian
{
public:
	TContian(void);
	~TContian(void); 
	int SendDataByTitle(const std::string& title,void* ptr,int length);
	bool SetTitle(const std::string& title,const int sock);
private:   	
	std::map<string,std::vector<int>> tileMap;//title  map
	//std::map<int,std::map<string,int>> //
};


#endif//_TCONTIAN_H
