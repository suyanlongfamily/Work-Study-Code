#pragma once

#include<stdlib.h>
#include <time.h>
#include <Windows.h>     
#include <iostream>   

using namespace std; 
//DWORD WINAPI Thread1(LPVOID lpParmeter);
//DWORD WINAPI Thread2(LPVOID lpParmeter);     
//假设数据类型为 int； 
typedef int datatype; 

struct taskParam{
	int buf[1000];
	int sleep[100];
	int curpos; 
	CRITICAL_SECTION  cs;   
};

struct ThreadParam{
	int no;
	int len;
	int start;
	taskParam *ptask;
};    

void InitTask(taskParam &task){
	task.curpos=0;
	for(int i=0;i<100;i++)
		task.buf[i]=i; 
	srand(time(NULL));//方便下面的模拟操作 

	InitializeCriticalSection(&task.cs);
}

void FinshedTask(taskParam &task){
	task.curpos=0;
	DeleteCriticalSection(&task.cs);
}

void PrintMsg(ThreadParam *pParam,char *msg){ 
	EnterCriticalSection(&pParam->ptask->cs); 
	cout<<msg<<": Thread "<<pParam->no <<endl;
	cout<<" pos =" << pParam->start<<",length= "<<pParam->len<<endl; 
	LeaveCriticalSection(&pParam->ptask->cs);     
}

int getTask(ThreadParam *pParam){
	int pos=1000;
	int ret=0;
	EnterCriticalSection(&pParam->ptask->cs); 
	pos =pParam->ptask->curpos;
	if(pos<1000){
		pParam->start =pos;
		pParam->len=10;
		pParam->ptask->curpos+=10;
		ret =10;
	}
	else ret =0;

	LeaveCriticalSection(&pParam->ptask->cs);     
	// PrintMsg(pParam,"gettask task");
	return ret; 
}  

void DealTask(ThreadParam *pParam){
	// 处理任务 
	//int n=1.0*rand()/RAND_MAX *500+10;
	char msg[1024]="deal task";
	//sprintf(msg,"deal task sleep %d",n);     
	PrintMsg(pParam, msg);
	// 这用来，模拟处理任务。 
	for (int i=pParam->start; i< pParam->start+ pParam->len;i++)
		pParam->ptask->buf[i]*=2; //假设实际这样处理 
	//Sleep(n); 
}

DWORD WINAPI ThreadProc(LPVOID lpParmeter);
int main()    
{         
	HANDLE hThread1 ;         
	HANDLE hThread2 ;

	taskParam task;
	InitTask(task);

	ThreadParam param[2]={{0,0,0,&task},{1,0,0,&task}};

	hThread1 = CreateThread(NULL, 0, ThreadProc, param, 0, NULL);
	hThread2 = CreateThread(NULL, 0, ThreadProc, param+1, 0, NULL);
	HANDLE h[2]={hThread1,hThread2};       

	WaitForMultipleObjects(2,h,true, INFINITE);//等待所有线程完成后，退出           
	CloseHandle(hThread1);         
	CloseHandle(hThread2);   
	FinshedTask(task);

	system("PAUSE");         
	return 0;     
} 


DWORD WINAPI ThreadProc(LPVOID lpParmeter){
	ThreadParam *pParam=(ThreadParam *)lpParmeter;
	while(getTask(pParam))
	{    
		DealTask(pParam);   
	}
	return 0; 
}  

// DWORD WINAPI Thread2(LPVOID lpParameter)//thread data     
//{         //100到200的处理过程       
//for each 100 to 200           
//deal(...);         return 0;     
//}   


//////////////////////////////////////////////////////////////////////////
/*！
 *2016/01/05 19:11:52
 *\author suyanlong
 * 另一个线程池的实现
 */
//循环向a函数每次发送200个字节长度（这个是固定的）的buffer,
//a函数中需要将循环传进来的buffer，组成240字节（也是固定的）的新buffer进行处理，
//在处理的时候每次从新buffer中取两个字节打印
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <io.h>
//Log{
#define MAXLOGSIZE 20000000
#define MAXLINSIZE 16000
#include <time.h>
#include <sys/timeb.h>
#include <stdarg.h>
char logfilename1[]="MyLog1.log";
char logfilename2[]="MyLog2.log";
static char logstr[MAXLINSIZE+1];
char datestr[16];
char timestr[16];
char mss[4];
CRITICAL_SECTION cs_log;
FILE *flog;
void Lock(CRITICAL_SECTION *l) {
	EnterCriticalSection(l);
}
void Unlock(CRITICAL_SECTION *l) {
	LeaveCriticalSection(l);
}
void LogV(const char *pszFmt,va_list argp) {
	struct tm *now;
	struct timeb tb;

	if (NULL==pszFmt||0==pszFmt[0]) return;
	_vsnprintf(logstr,MAXLINSIZE,pszFmt,argp);
	ftime(&tb);
	now=localtime(&tb.time);
	sprintf(datestr,"%04d-%02d-%02d",now->tm_year+1900,now->tm_mon+1,now->tm_mday);
	sprintf(timestr,"%02d:%02d:%02d",now->tm_hour     ,now->tm_min  ,now->tm_sec );
	sprintf(mss,"%03d",tb.millitm);
	printf("%s %s.%s %s",datestr,timestr,mss,logstr);
	flog=fopen(logfilename1,"a");
	if (NULL!=flog) {
		fprintf(flog,"%s %s.%s %s",datestr,timestr,mss,logstr);
		if (ftell(flog)>MAXLOGSIZE) {
			fclose(flog);
			if (rename(logfilename1,logfilename2)) {
				remove(logfilename2);
				rename(logfilename1,logfilename2);
			}
		} else {
			fclose(flog);
		}
	}
}
void Log(const char *pszFmt,...) {
	va_list argp;

	Lock(&cs_log);
	va_start(argp,pszFmt);
	LogV(pszFmt,argp);
	va_end(argp);
	Unlock(&cs_log);
}
//Log}
#define ASIZE    200
#define BSIZE    240
#define CSIZE      2
char Abuf[ASIZE];
char Cbuf[CSIZE];
CRITICAL_SECTION cs_HEX ;
CRITICAL_SECTION cs_BBB ;
struct FIFO_BUFFER {
	int  head;
	int  tail;
	int  size;
	char data[BSIZE];
} BBB;
int No_Loop=0;
void HexDump(int cn,char *buf,int len) {
	int i,j,k;
	char binstr[80];

	Lock(&cs_HEX);
	for (i=0;i<len;i++) {
		if (0==(i%16)) {
			sprintf(binstr,"%03d %04x -",cn,i);
			sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
		} else if (15==(i%16)) {
			sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
			sprintf(binstr,"%s  ",binstr);
			for (j=i-15;j<=i;j++) {
				sprintf(binstr,"%s%c",binstr,('!'<buf[j]&&buf[j]<='~')?buf[j]:'.');
			}
			Log("%s\n",binstr);
		} else {
			sprintf(binstr,"%s %02x",binstr,(unsigned char)buf[i]);
		}
	}
	if (0!=(i%16)) {
		k=16-(i%16);
		for (j=0;j<k;j++) {
			sprintf(binstr,"%s   ",binstr);
		}
		sprintf(binstr,"%s  ",binstr);
		k=16-k;
		for (j=i-k;j<i;j++) {
			sprintf(binstr,"%s%c",binstr,('!'<buf[j]&&buf[j]<='~')?buf[j]:'.');
		}
		Log("%s\n",binstr);
	}
	Unlock(&cs_HEX);
}
int GetFromRBuf(int cn,CRITICAL_SECTION *cs,FIFO_BUFFER *fbuf,char *buf,int len) {
	int lent,len1,len2;

	lent=0;
	Lock(cs);
	if (fbuf->size>=len) {
		lent=len;
		if (fbuf->head+lent>BSIZE) {
			len1=BSIZE-fbuf->head;
			memcpy(buf     ,fbuf->data+fbuf->head,len1);
			len2=lent-len1;
			memcpy(buf+len1,fbuf->data           ,len2);
			fbuf->head=len2;
		} else {
			memcpy(buf     ,fbuf->data+fbuf->head,lent);
			fbuf->head+=lent;
		}
		fbuf->size-=lent;
	}
	Unlock(cs);
	return lent;
}
void thdB(void *pcn)
{
	char*recv_buf;
	int  recv_nbytes;
	int  cn;
	int  wc;
	int  pb;

	cn=(int)pcn;
	Log("%03d thdB              thread begin...\n",cn);
	while (1) {
		Sleep(10);
		recv_buf=(char *)Cbuf;
		recv_nbytes=CSIZE;
		wc=0;
		while (1) {
			pb=GetFromRBuf(cn,&cs_BBB,&BBB,recv_buf,recv_nbytes);
			if (pb) {
				Log("%03d recv %d bytes\n",cn,pb);
				HexDump(cn,recv_buf,pb);
				Sleep(1);
			} else {
				Sleep(1000);
			}
			if (No_Loop) break;//
			wc++;
			if (wc>3600) Log("%03d %d==wc>3600!\n",cn,wc);
		}
		if (No_Loop) break;//
	}
}

int PutToRBuf(int cn,CRITICAL_SECTION *cs,FIFO_BUFFER *fbuf,char *buf,int len) {
	int lent,len1,len2;

	Lock(cs);
	lent=len;
	if (fbuf->size+lent>BSIZE) {
		lent=BSIZE-fbuf->size;
	}
	if (fbuf->tail+lent>BSIZE) {
		len1=BSIZE-fbuf->tail;
		memcpy(fbuf->data+fbuf->tail,buf     ,len1);
		len2=lent-len1;
		memcpy(fbuf->data           ,buf+len1,len2);
		fbuf->tail=len2;
	} else {
		memcpy(fbuf->data+fbuf->tail,buf     ,lent);
		fbuf->tail+=lent;
	}
	fbuf->size+=lent;
	Unlock(cs);
	return lent;
}
void thdA(void *pcn) {
	char        *send_buf;
	int          send_nbytes;
	int          cn;
	int          wc;
	int           a;
	int          pa;

	cn=(int)pcn;
	Log("%03d thdA              thread begin...\n",cn);
	a=0;
	while (1) {
		Sleep(100);
		memset(Abuf,a,ASIZE);
		a=(a+1)%256;
		if (16==a) {No_Loop=1;break;}//去掉这句可以让程序一直循环直到按Ctrl+C或Ctrl+Break或当前目录下存在文件No_Loop
		send_buf=(char *)Abuf;
		send_nbytes=ASIZE;
		Log("%03d sending %d bytes\n",cn,send_nbytes);
		HexDump(cn,send_buf,send_nbytes);
		wc=0;
		while (1) {
			pa=PutToRBuf(cn,&cs_BBB,&BBB,send_buf,send_nbytes);
			Log("%03d sent %d bytes\n",cn,pa);
			HexDump(cn,send_buf,pa);
			send_buf+=pa;
			send_nbytes-=pa;
			if (send_nbytes<=0) break;//
			Sleep(1000);
			if (No_Loop) break;//
			wc++;
			if (wc>3600) Log("%03d %d==wc>3600!\n",cn,wc);
		}
		if (No_Loop) break;//
	}
}

int main() {
	InitializeCriticalSection(&cs_log );
	Log("Start===========================================================\n");
	InitializeCriticalSection(&cs_HEX );
	InitializeCriticalSection(&cs_BBB );

	BBB.head=0;
	BBB.tail=0;
	BBB.size=0;

	_beginthread((void(__cdecl *)(void *))thdA,0,(void *)1);
	_beginthread((void(__cdecl *)(void *))thdB,0,(void *)2);

	if (!access("No_Loop",0)) {
		remove("No_Loop");
		if (!access("No_Loop",0)) {
			No_Loop=1;
		}
	}
	while (1) {
		Sleep(1000);
		if (No_Loop) break;//
		if (!access("No_Loop",0)) {
			No_Loop=1;
		}
	}
	Sleep(3000);
	DeleteCriticalSection(&cs_BBB );
	DeleteCriticalSection(&cs_HEX );
	Log("End=============================================================\n");
	DeleteCriticalSection(&cs_log );
	return 0;
}



