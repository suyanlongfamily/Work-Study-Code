#ifndef _TCONTIAN_H 
#define _TCONTIAN_H	  
#pragma once

#include <string>
#include <map>
#include <iostream>
#include <vector>
using namespace std; 

/*======================================================================================================================*/
//这种方式，只能使用在本地环境上传输。但是用在socket之间就不行了。还是记住吧！
//其实，只要能确定长度就可以，但是，string类型，是动态申请的，所以在编译期间是无法通过sizeof实现的。
typedef struct THeadData{
	char version[10];
	char way;  
	char flag;
}HeadData; 

typedef struct TBodyData{
	string title; 
	char* pdata; //数据指针
	int length;	
}BodyData; 

typedef struct TPackData{
	HeadData headdata;
	BodyData bodyData;	
} PackData; 
/*以上代码只能使用在同一内存下======================================================================================================================*/


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
