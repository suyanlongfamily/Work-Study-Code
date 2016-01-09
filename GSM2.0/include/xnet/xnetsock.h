/*
 * XNET Socket Head File xnetsock.h
 * for VC 6.0, BCB6.0, AIX
 * Program by Liu Zhi Jun 2010-11-29
 */
# ifndef xnetsockH
# define xnetsockH

# include <string>
# include <list>
# include <map>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>

#ifdef _UNIX

# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/time.h>
# include <arpa/inet.h>

# include <pthread.h>
# include "xnet/safestl.h"
# include "lang/BackLog.h"

#else

#include <winsock.h>
# include <winsock.h>
# include <time.h>

#endif

#ifdef __BORLANDC__
# include <Forms.hpp>
#endif

# include "xnet/xnetdata.h"
# include "xnet/compress.h"
# include "xnet/titlelist.h"
# include "xnet/ThreadLock.h"


namespace xnet {

typedef enum {
	mtXNetBegin,
	mtXNetClose, 		// when close Socket
	mtXNetRead,
	mtXNetLoginOK,
	mtXNetError,
	mtXNetSystem,
	mtXNetEnd
} TXNetMessageType;


#ifdef _UNIX

#define SOCKET int
#define DWORD int
#define HANDLE pthread_t
#define INVALID_SOCKET -1
#define WSAGetLastError() errno
#define GetLastError() errno
#define ESocketError Exception
#define TMyThreadFunc	void *
#define closesocket close


class TXNetSock;
/*!
	在使用了TXNetSock的应用程序中,我们用TXNetSockMessage *来传送TXNetSock的消息
*/
typedef struct {
	/*!
		消息类型
		-	在整个应用程序中,消息类型必须统一分配
	*/
	int Type;
	/*!
		消息发送者
	*/
	TXNetSock *Term;
	/*!
		若该事件有数据,则为数据
	*/
	TXNetData *Data;
	/*!
		为和TMessage相互转换而填充的无效数据
	*/
	long filler[2];
} TXNetSockMessage;

/*!
	在使用了TXNetSock的应用程序中,我们用TMessage *来作为应用程序和TXNetSock的消息传送对象
*/
typedef struct {
	/*!
		消息类型
		-	在整个应用程序中,消息类型必须统一分配
	*/
	int Type;
	/*!
		消息参数
		-	在通过Type了解到该类型为TXNetSockMessage时,可以使用(TXNetSockMessage *)来转换消息
	*/
	long Param[4];
} TMessage;

typedef TemplateMessageQueue <TMessage *> TMessageQueue;


#else   // WINDOWS

#define sleep(x) Sleep((x) * 1000)

#define WM_XNET_WRITE WM_USER+10
#define WM_XNET_READ WM_USER+11
#define WM_XNET_LOGIN WM_USER+12
#define WM_XNET_LOGINOK WM_USER+13
#define WM_XNET_ERROR WM_USER+14
#define WM_XNET_CLOSE WM_USER+15

#define TMyThreadFunc	DWORD WINAPI

#endif




#ifdef _DEBUG
#define MINI_CHECK_SECOND 2
#define MINI_ECHO_SECOND 1
#else
#define MINI_CHECK_SECOND 6
#define MINI_ECHO_SECOND 2
#endif



/*!
	XNet v1.2.2 for VC 6.0 客户端对象
 */
#ifdef __BORLANDC__
class PACKAGE TXNetSock
#else
class TXNetSock
#endif
{
public:




	/*!
		返回名称,由TermName@ip:port构成
	*/
	const std::string &Name () { return FName; };
	/*!
		返回XNet逻辑名称
	*/
	const std::string &TermName () { return FTermName; };
	/*!
		返回通讯状态
		-	csNull     初始未连接
		-	csConnect  连接
		-	csLogin    注册,只有在csLogin状态下才能正常收发数据
        -   csClose    关闭
	*/
	TCommState State () { return FState; };
	/*!
		开始连接xnetd,并自动注册
	*/
	void Open ();
	/*!
		关闭连接，在延时后自动重连
	*/
	void Close ();
	/*!
		向直连的xnetd发送数据
		-	系统自动复制一份原始数据或者是压缩过的数据发送到xnetd
		-	Data必须由程序员显式的释放(delete)
		-	也可以使用SendThenDrop来发送数据并在发送成功后自动释放
	*/
	void SendData (TXNetData *Data);
	/*!
		发送返回的数据
		-   Data 收到的原始数据，数据区经过处理，但没有对系统区操作过
		-	ReturnCode 返回码, 不填写时默认为0
        - SendReturn会自动调用Exchange, SetReturnCode, SendData。
	*/
	void SendReturn (TXNetData *Data, int ReturnCode=0);
    /*!
        在线时，以单播、多播、组调用等三种方式发送数据
        - Command 发送方式，可以是下面的一种: xcTitle, xcMutiTitle, xcGroupQuest。
        - Title Title或Group类型
        - Buffer 用户数据区指针
        - length 长度, length不填时，发送以\0结尾的Buffer数据区
        - Buffer、length均不填写时，发送数据区长度为0的包
        - CommandBuffer会自动调用XNetData的组包，然后SendData
    */
    void CommandBuffer (TXNetCommand Command, const char *Title, const char *Buffer=NULL, int length=0);
    /*!
        在线时，以单播、多播、组调用等三种方式发送数据
        - Command 发送方式，可以是下面的一种: xcTitle, xcMutiTitle, xcGroupQuest。
        - Title Title或Group类型
        - Mark  自定义标记,放在系统数据区
        - Buffer 用户数据区指针
        - length 长度, length不填时，发送以\0结尾的Buffer数据区
        - CommandBuffer会自动调用XNetData的组包，然后SendData
    */
    void CommandBuffer (TXNetCommand Command, const char *Title, const char *Market, const char *Buffer, int length=0);
    /*!
        注册消息类型
        - Titles 类型列表,以分号隔开。类型可以用*,>通配符,不允许有空格
        - AddTitles在loginOK之前之后均可调用，XNetSock自动根据本地信息比对后调用SendData发送新注册的消息类型,并保留本地信息
        - AddTitles只需要调用一次, 断连自动重连后XNetSock会自动向服务器发送注册消息类型列表
    */
    void AddTitles (const char *Titles);
    /*!
        注销消息类型
        - Titles 类型列表,以分号隔开。类型可以用*,>通配符,不允许有空格
    */
    void DelTitles (const char *Titles);
    /*!
        注册组服务
        - Groups 组服务列表,以分号隔开。Group不支持通配符,不允许有空格
        - AddGroups在loginOK之前之后均可调用，XNetSock自动根据本地信息比对后调用SendData发送新注册的组服务,并保留本地信息
        - AddGroups只需要调用一次, 断连自动重连后XNetSock会自动向服务器发送组服务列表
    */
    void AddGroups (const char *Groups);
    /*!
        注销组服务
        - Groups 组服务列表,以分号隔开。Group不支持通配符,不允许有空格
    */
    void DelGroups (const char *Groups);
	/*!
		设置逻辑域名,仅用于有唯一标识要求的场合，如仅运行单个的服务程序、接口程序等。
        - 当不配置逻辑域名时，loginOK之后系统会自动分配一个数字序列的逻辑域名
	*/
	void SetConfigName (const std::string & name);
	/*!
		设置压缩起始字节数,当发送的TXNetData的用户数据区超过该值时,系统自动压缩
	*/
	inline void SetMiniSize (int value) { if (value >= 1024) FMiniSize = value; };
	/*!
		返回压缩起始字节数
	*/
	inline int MiniSize () { return FMiniSize; };
    /*!
        设置自动重连秒数
    */
    inline void SetInterval (int value) { FInterval = value; };
    /*!
        返回自动重连秒数
    */
    inline int Interval () { return FInterval; };
    /*!
        设置心跳检测时间间隔，仅用于对通讯连接要求很高的场合，如服务程序、接口程序,最小为5秒
    */
    inline void SetOnlineCheckSecond (int value) { if (value >= MINI_CHECK_SECOND) FOnlineCheckSecond = value;};
    /*!
        设置心跳响应时间，仅用于对通讯连接要求很高的场合，如服务程序、接口程序，最小为2秒
    */
    inline void SetOnlineEchoSecond (int value) { if (value >= MINI_ECHO_SECOND) FOnlineEchoSecond = value;};
    /*!
        返回心跳检测时间间隔
    */
    inline int OnlineCheckSecond () { return FOnlineCheckSecond; };
    /*!
        返回心跳响应时间
    */
    inline int OnlineEchoSecond () { return FOnlineEchoSecond; };
#ifdef _WINDOWS_
	/*!
		构造函数, 用NewObject代替 new
		-	Address      xnetd ip地址
		-	Port         xnetd Port
        -   Handle      用于处理XNET事件的窗体句柄，XNetSock采用PostMessage to Handle方式来传递数据给调用者
            不传递Handle参数时，XNetSock采用PostThreadMessage 的方式来传递数据
	*/
	static TXNetSock *NewObject (char *Address, int Port, HWND Handle=0);
    /*!
        析构函数，代替delete
    */
	static void DelObject (TXNetSock *Object);
private:
	/*!
		构造函数
		-	Address      xnetd ip地址
		-	Port         xnetd Port
        -   Handle      用于处理XNET事件的窗体句柄，XNetSock采用PostMessage to Handle方式来传递数据给调用者
        -           若Handle=0，则XNetSock采用PostThreadMessage 的方式来传递数据
	*/
	TXNetSock (char *Address, int Port, HWND Handle=0);
    HWND FHandle;
	~TXNetSock ();

#else
	/*!
		构造函数
		-	Address      xnetd ip地址
		-	Port         xnetd Port
		-	MainQueue    主线程的消息队列
		-	BackLog		日志对象
	*/
	TXNetSock (char *Address, int Port, TMessageQueue *MainQueue, lang::TBackLog *BackLog);
	~TXNetSock ();

private:
	lang::TBackLog *FBackLog;
	TemplatePriorityQueue <TXNetData *>  *FQueue;
	TMessageQueue *FMainQueue;
	pthread_attr_t tattr;
	int FPrioritySize;
    void DoSendData (TXNetData *Data, TMessagePriority Priority);

#endif


    void DeCompress (TXNetData *Data);
    typedef list <TXNetSock *> TSockList;
	static TMyThreadFunc ConnectThread (void *Param);
	static TMyThreadFunc WriteThread (void *Param);
    static TMyThreadFunc CheckThread (void *Param);
    static TSockList CheckList;
    static DWORD CheckHandle;
	static HANDLE ptCheck;
	HANDLE ptWrite, ptConnect;

	static TThreadLock *FClassLock;
	TThreadLock *FLock;
	time_t ReadTime;
	bool inCheck;
	TTitleList *TitleList;
    TTitleList *GroupList;
	int FMiniSize;
    int FInterval;         // 自动重连的间隔秒数
	TCompressLZ77 *CompressLZ77, *DecompressLZ77;
	std::string FTermName;
	std::string ConfigName;
	std::string FName;
	char FAddress[30];    			// remote ip Address ip6 255.255.255.255.255.255 max=20 bytes
	int FPort;						// remote Port
	SOCKET SocketHandle;
	TCommState FState;
	int ReadStep;
	int Position;
	char *itoa (int value);
    void WriteExecute ();
    void ConnectExecute ();
    void CheckExecute ();
	void ProcessData (TXNetData *Data);
    void DoSendData (TXNetData *Data);
	void ReadProcess ();
	void CloseWhenRead (TXNetData *Data);
	DWORD FMainHandle, WriteHandle, ReadHandle;
    int FOnlineCheckSecond, FOnlineEchoSecond;
    HANDLE UniSemaphore;
};

}


#endif

