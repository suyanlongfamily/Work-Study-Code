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
	��ʹ����TXNetSock��Ӧ�ó�����,������TXNetSockMessage *������TXNetSock����Ϣ
*/
typedef struct {
	/*!
		��Ϣ����
		-	������Ӧ�ó�����,��Ϣ���ͱ���ͳһ����
	*/
	int Type;
	/*!
		��Ϣ������
	*/
	TXNetSock *Term;
	/*!
		�����¼�������,��Ϊ����
	*/
	TXNetData *Data;
	/*!
		Ϊ��TMessage�໥ת����������Ч����
	*/
	long filler[2];
} TXNetSockMessage;

/*!
	��ʹ����TXNetSock��Ӧ�ó�����,������TMessage *����ΪӦ�ó����TXNetSock����Ϣ���Ͷ���
*/
typedef struct {
	/*!
		��Ϣ����
		-	������Ӧ�ó�����,��Ϣ���ͱ���ͳһ����
	*/
	int Type;
	/*!
		��Ϣ����
		-	��ͨ��Type�˽⵽������ΪTXNetSockMessageʱ,����ʹ��(TXNetSockMessage *)��ת����Ϣ
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
	XNet v1.2.2 for VC 6.0 �ͻ��˶���
 */
#ifdef __BORLANDC__
class PACKAGE TXNetSock
#else
class TXNetSock
#endif
{
public:




	/*!
		��������,��TermName@ip:port����
	*/
	const std::string &Name () { return FName; };
	/*!
		����XNet�߼�����
	*/
	const std::string &TermName () { return FTermName; };
	/*!
		����ͨѶ״̬
		-	csNull     ��ʼδ����
		-	csConnect  ����
		-	csLogin    ע��,ֻ����csLogin״̬�²��������շ�����
        -   csClose    �ر�
	*/
	TCommState State () { return FState; };
	/*!
		��ʼ����xnetd,���Զ�ע��
	*/
	void Open ();
	/*!
		�ر����ӣ�����ʱ���Զ�����
	*/
	void Close ();
	/*!
		��ֱ����xnetd��������
		-	ϵͳ�Զ�����һ��ԭʼ���ݻ�����ѹ���������ݷ��͵�xnetd
		-	Data�����ɳ���Ա��ʽ���ͷ�(delete)
		-	Ҳ����ʹ��SendThenDrop���������ݲ��ڷ��ͳɹ����Զ��ͷ�
	*/
	void SendData (TXNetData *Data);
	/*!
		���ͷ��ص�����
		-   Data �յ���ԭʼ���ݣ�����������������û�ж�ϵͳ��������
		-	ReturnCode ������, ����дʱĬ��Ϊ0
        - SendReturn���Զ�����Exchange, SetReturnCode, SendData��
	*/
	void SendReturn (TXNetData *Data, int ReturnCode=0);
    /*!
        ����ʱ���Ե������ಥ������õ����ַ�ʽ��������
        - Command ���ͷ�ʽ�������������һ��: xcTitle, xcMutiTitle, xcGroupQuest��
        - Title Title��Group����
        - Buffer �û�������ָ��
        - length ����, length����ʱ��������\0��β��Buffer������
        - Buffer��length������дʱ����������������Ϊ0�İ�
        - CommandBuffer���Զ�����XNetData�������Ȼ��SendData
    */
    void CommandBuffer (TXNetCommand Command, const char *Title, const char *Buffer=NULL, int length=0);
    /*!
        ����ʱ���Ե������ಥ������õ����ַ�ʽ��������
        - Command ���ͷ�ʽ�������������һ��: xcTitle, xcMutiTitle, xcGroupQuest��
        - Title Title��Group����
        - Mark  �Զ�����,����ϵͳ������
        - Buffer �û�������ָ��
        - length ����, length����ʱ��������\0��β��Buffer������
        - CommandBuffer���Զ�����XNetData�������Ȼ��SendData
    */
    void CommandBuffer (TXNetCommand Command, const char *Title, const char *Market, const char *Buffer, int length=0);
    /*!
        ע����Ϣ����
        - Titles �����б�,�ԷֺŸ��������Ϳ�����*,>ͨ���,�������пո�
        - AddTitles��loginOK֮ǰ֮����ɵ��ã�XNetSock�Զ����ݱ�����Ϣ�ȶԺ����SendData������ע�����Ϣ����,������������Ϣ
        - AddTitlesֻ��Ҫ����һ��, �����Զ�������XNetSock���Զ������������ע����Ϣ�����б�
    */
    void AddTitles (const char *Titles);
    /*!
        ע����Ϣ����
        - Titles �����б�,�ԷֺŸ��������Ϳ�����*,>ͨ���,�������пո�
    */
    void DelTitles (const char *Titles);
    /*!
        ע�������
        - Groups ������б�,�ԷֺŸ�����Group��֧��ͨ���,�������пո�
        - AddGroups��loginOK֮ǰ֮����ɵ��ã�XNetSock�Զ����ݱ�����Ϣ�ȶԺ����SendData������ע��������,������������Ϣ
        - AddGroupsֻ��Ҫ����һ��, �����Զ�������XNetSock���Զ������������������б�
    */
    void AddGroups (const char *Groups);
    /*!
        ע�������
        - Groups ������б�,�ԷֺŸ�����Group��֧��ͨ���,�������пո�
    */
    void DelGroups (const char *Groups);
	/*!
		�����߼�����,��������Ψһ��ʶҪ��ĳ��ϣ�������е����ķ�����򡢽ӿڳ���ȡ�
        - ���������߼�����ʱ��loginOK֮��ϵͳ���Զ�����һ���������е��߼�����
	*/
	void SetConfigName (const std::string & name);
	/*!
		����ѹ����ʼ�ֽ���,�����͵�TXNetData���û�������������ֵʱ,ϵͳ�Զ�ѹ��
	*/
	inline void SetMiniSize (int value) { if (value >= 1024) FMiniSize = value; };
	/*!
		����ѹ����ʼ�ֽ���
	*/
	inline int MiniSize () { return FMiniSize; };
    /*!
        �����Զ���������
    */
    inline void SetInterval (int value) { FInterval = value; };
    /*!
        �����Զ���������
    */
    inline int Interval () { return FInterval; };
    /*!
        �����������ʱ�����������ڶ�ͨѶ����Ҫ��ܸߵĳ��ϣ��������򡢽ӿڳ���,��СΪ5��
    */
    inline void SetOnlineCheckSecond (int value) { if (value >= MINI_CHECK_SECOND) FOnlineCheckSecond = value;};
    /*!
        ����������Ӧʱ�䣬�����ڶ�ͨѶ����Ҫ��ܸߵĳ��ϣ��������򡢽ӿڳ�����СΪ2��
    */
    inline void SetOnlineEchoSecond (int value) { if (value >= MINI_ECHO_SECOND) FOnlineEchoSecond = value;};
    /*!
        �����������ʱ����
    */
    inline int OnlineCheckSecond () { return FOnlineCheckSecond; };
    /*!
        ����������Ӧʱ��
    */
    inline int OnlineEchoSecond () { return FOnlineEchoSecond; };
#ifdef _WINDOWS_
	/*!
		���캯��, ��NewObject���� new
		-	Address      xnetd ip��ַ
		-	Port         xnetd Port
        -   Handle      ���ڴ���XNET�¼��Ĵ�������XNetSock����PostMessage to Handle��ʽ���������ݸ�������
            ������Handle����ʱ��XNetSock����PostThreadMessage �ķ�ʽ����������
	*/
	static TXNetSock *NewObject (char *Address, int Port, HWND Handle=0);
    /*!
        ��������������delete
    */
	static void DelObject (TXNetSock *Object);
private:
	/*!
		���캯��
		-	Address      xnetd ip��ַ
		-	Port         xnetd Port
        -   Handle      ���ڴ���XNET�¼��Ĵ�������XNetSock����PostMessage to Handle��ʽ���������ݸ�������
        -           ��Handle=0����XNetSock����PostThreadMessage �ķ�ʽ����������
	*/
	TXNetSock (char *Address, int Port, HWND Handle=0);
    HWND FHandle;
	~TXNetSock ();

#else
	/*!
		���캯��
		-	Address      xnetd ip��ַ
		-	Port         xnetd Port
		-	MainQueue    ���̵߳���Ϣ����
		-	BackLog		��־����
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
    int FInterval;         // �Զ������ļ������
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

