#include "StdAfx.h"
#include "FThreadPool.h"


FThreadPool* FThreadPool::threaPool = NULL;	




FThreadPool* FThreadPool::GetTreadPool(){
	if (threaPool == NULL)
	{
		threaPool = new FThreadPool();
	}
	return threaPool;
}

FThreadPool::FThreadPool()
{  	
	
	SYSTEM_INFO si;   
	GetSystemInfo(&si); 
	cpuNumber = si.dwNumberOfProcessors;//��ȡCPU������ 
	memset(threadHandle,0,sizeof(threadHandle));
	
	callBack = NULL;
	threaPool = NULL;
	IOCPPort = NULL;
	IOCPPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0); 	

//SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//HANDLE hadle = CreateIoCompletionPort((HANDLE)sock,IOCPPort,NULL,2);	
	int i = 0;	
}

DWORD WINAPI FThreadPool::ThreadFun(LPVOID lpThreadParameter)
{
	FThreadPool* This = (FThreadPool*)lpThreadParameter;
	This->_ThreadFun(lpThreadParameter);
	return 0;
}

void FThreadPool::_ThreadFun(void* param)
{	
	//DWORD NumberOfBytesTransferred = 0;
	//DWORD CompletionKey = 0; 
	//LPOVERLAPPED *lpOverlapped = NULL;

	LPDWORD lpNumberOfBytesTransferred = NULL;
	PULONG_PTR lpCompletionKey = NULL;
	LPOVERLAPPED *lpOverlapped = NULL;
	while (true)
	{
		BOOL ret = GetQueuedCompletionStatus(IOCPPort,lpNumberOfBytesTransferred,lpCompletionKey,lpOverlapped,INFINITE);
		DWORD dwerror = GetLastError();
		
		if (callBack != NULL)
		{
			this->callBack(lpNumberOfBytesTransferred,lpCompletionKey,lpOverlapped);
		}	 		
	}
} 

//************************************
// Method:    StartTreadPool
// FullName:  FThreadPool::StartTreadPool
// Access:    public 
// Returns:   void
// Qualifier: //�����̳߳�
// Parameter: LPTHREAD_START_ROUTINE threadFunction �̳߳غ���
// Parameter: void * param ��Ӧ�̳߳غ���
//************************************
void FThreadPool::StartTreadPool(FThreadPool::CallBack _callback,void* param)
{
	callBack = _callback; 
	for (int i = 0; i < 1; ++i)
	{
		threadHandle[i] = CreateThread(NULL,0,ThreadFun,this,0,0);
	}
}


//�õ�IOCP�˿�
void* FThreadPool::GetIOCPPortHandle()
{
	return IOCPPort;
}

//Ͷ�������̳߳� 
//ʵ��Ӧ�þ� dwCompletionKey
BOOL FThreadPool::PostTaskToQueue(DWORD dwNumberOfBytesTransferred,ULONG_PTR dwCompletionKey,LPOVERLAPPED lpOverlapped)
{

//__in      DWORD dwNumberOfBytesTransferred,   
//__in      ULONG_PTR dwCompletionKey,  //����ֵ 
//__in_opt  LPOVERLAPPED lpOverlapped  

	BOOL ret = PostQueuedCompletionStatus(IOCPPort,dwNumberOfBytesTransferred,dwCompletionKey,lpOverlapped);
	return ret;

}

/*!
 *2016/06/20 12:34:47
 *\author suyanlong
 * ���»ص�����
 */
void FThreadPool::UpdateCallBack(CallBack _callback,void* param)
{
	callBack = _callback;
}

/*!
 *2016/06/20 12:35:04
 *\author suyanlong
 * ���� �ͷ���Դ
 */
FThreadPool::~FThreadPool(void)
{
	CloseHandle(IOCPPort);
	for (int i = 0;i < cpuNumber ;++i)
	{
		if (threadHandle[i] != 0)
		{
			CloseHandle(threadHandle[i]);
		} 		
	}	   
}

