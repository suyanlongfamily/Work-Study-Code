/*
 * XNetSocket.h for XNet v1.2.2 2010-11-03
 *
 */
 
//---------------------------------------------------------------------------

#ifndef XNetSocketH
#define XNetSocketH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include "xnet/xnetsock.h"


using namespace xnet;

class TXNetSocket;

typedef void __fastcall (__closure * TXNetSocketErrorEvent)  // for XNetSocket, XNetClient
    (int ErrorCode, char *ErrorMessage);

typedef void __fastcall (__closure *TXNetSocketNotifyEvent)  // for XNetSocket
    (TXNetSocket * Sender, xnet::TXNetData* Package);

typedef void __fastcall (__closure * TXNetSocketLogEvent)
    (char *LogString, int LogType);     // LogType: 0 - WinSocket , 1 - System, 2 - Data, 3 - Error


//---------------------------------------------------------------------------
class PACKAGE TXNetSocket
{
protected:
    xnet::TXNetSock *FSock;
    AnsiString FAddress;
    int FPort;
    HWND FHandle;
    void __fastcall XNetProc (Messages::TMessage &Message);
    void __fastcall DoLogEvent (char *cp, int type);
    bool FRegistered;
    TXNetSocketLogEvent FOnLogEvent;
    TXNetSocketErrorEvent FOnXNetError;
    void __fastcall DoError (int ErrorCode);
    char * __fastcall GetErrorMessage (int ErrorCode);
    TXNetSocketNotifyEvent FOnXNetRead;
    TXNetSocketNotifyEvent FOnLoginOK;
    AnsiString FTermName;
    AnsiString FConfigName;
    int FMiniCompSize;
    int FSecond;
    __fastcall TXNetSocket();
    __fastcall ~TXNetSocket(){};
public:
    /*!
        构造函数,取代new
    */
    static TXNetSocket * NewObject ();
    /*!
        析构函数，取代delete
    */
    static void DelObject (TXNetSocket *Object);
    /*!
        客户端连接
    */
    inline TXNetSock *Sock() {return FSock; };
    /*!
        连接指定的Address和Port
    */
    void __fastcall Open();
    /*!
        断连
    */
    void __fastcall Close();
    /*!
        是否已注册
    */
    inline bool Registered () { return FRegistered; };
    /*!
        发送数据
    */
    void __fastcall SendData (TXNetData * datapkg);
    /*!
        逻辑域名，可以设置为空，由XNet自动分配
    */
    inline const AnsiString & ConfigName () {return FConfigName;};
    /*!
        逻辑域名，可以设置为空，由XNet自动分配
    */
    inline void SetConfigName (const AnsiString & AConfigName) {FConfigName = AConfigName;};
    /*!
        逻辑全名
        -   如果连接TXNetBox，该值一直为空。(因为TXNetBox的域名可能会变）
        -   如果连接xnetd，该值在注册成功后修改为新的名称。
    */
    inline const AnsiString & TermName() {return FTermName; };
    /*!
        服务器地址
    */
    inline const AnsiString & Address () {return FAddress; };
    /*!
        设置服务器地址
    */
    inline void SetAddress (const AnsiString & AAddress) {FAddress = AAddress; };
    /*!
        服务器侦听端口
    */
    inline int Port () { return  FPort; };
    /*!
        设置服务器侦听端口
    */
    void __fastcall SetMyPort (int APort);
    /*!
        压缩起始字节数,用户数据区超过该长度，系统自动压缩
    */
    inline int MiniCompSize()  { return FMiniCompSize; };
    /*!
        设置压缩起始字节数
    */
    inline void SetMiniCompSize (int AMiniCompSize) { FMiniCompSize = AMiniCompSize; };
    /*!
        自动重连秒数。注意以前版本用的是毫秒数，请除1000
    */
    inline int AutoSecond()  { return FSecond; };
    /*!
        设置自动重连秒数。注意以前版本用的是毫秒数，请除1000
    */
    inline void SetAutoSecond (int ASecond) {FSecond = ASecond; };
    /*!
        设置Log事件
    */
    inline void SetOnLogEvent (TXNetSocketLogEvent AEvent) { FOnLogEvent = AEvent; };
    /*!
        设置Error事件
    */
    inline void SetOnXNetError (TXNetSocketErrorEvent AEvent) { FOnXNetError = AEvent; };
    /*!
        设置读到完整数据包事件
    */
    inline void SetOnXNetRead (TXNetSocketNotifyEvent AEvent) { FOnXNetRead = AEvent; };
    /*!
        设置向服务器注册成功事件
    */
    inline void SetOnLoginOK (TXNetSocketNotifyEvent AEvent) { FOnLoginOK = AEvent; };
};
//---------------------------------------------------------------------------

#endif
