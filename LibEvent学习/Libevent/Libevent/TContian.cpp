#include <WinSock2.h>
#include "TContian.h"



TContian::TContian(void)
{
	//tileMap.
}


TContian::~TContian(void)
{
}


int TContian::SendDataByTitle(const std::string& title,void* ptr,int length){
	
	std::vector<int>& socke_ve = this->tileMap.find(title)->second;
	std::vector<int>::iterator iter = socke_ve.begin();

	for (;iter != socke_ve.end();++iter) {
		
		send(*iter,(char*)ptr,length,0);
	}  
	return 0;
}

bool TContian::SetTitle(const std::string& title,int sock){

	std::vector<int>& socke_ve = this->tileMap.find(title)->second;
	socke_ve.push_back(sock);
	return true;
}