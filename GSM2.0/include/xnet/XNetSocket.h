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
        ���캯��,ȡ��new
    */
    static TXNetSocket * NewObject ();
    /*!
        ����������ȡ��delete
    */
    static void DelObject (TXNetSocket *Object);
    /*!
        �ͻ�������
    */
    inline TXNetSock *Sock() {return FSock; };
    /*!
        ����ָ����Address��Port
    */
    void __fastcall Open();
    /*!
        ����
    */
    void __fastcall Close();
    /*!
        �Ƿ���ע��
    */
    inline bool Registered () { return FRegistered; };
    /*!
        ��������
    */
    void __fastcall SendData (TXNetData * datapkg);
    /*!
        �߼���������������Ϊ�գ���XNet�Զ�����
    */
    inline const AnsiString & ConfigName () {return FConfigName;};
    /*!
        �߼���������������Ϊ�գ���XNet�Զ�����
    */
    inline void SetConfigName (const AnsiString & AConfigName) {FConfigName = AConfigName;};
    /*!
        �߼�ȫ��
        -   �������TXNetBox����ֵһֱΪ�ա�(��ΪTXNetBox���������ܻ�䣩
        -   �������xnetd����ֵ��ע��ɹ����޸�Ϊ�µ����ơ�
    */
    inline const AnsiString & TermName() {return FTermName; };
    /*!
        ��������ַ
    */
    inline const AnsiString & Address () {return FAddress; };
    /*!
        ���÷�������ַ
    */
    inline void SetAddress (const AnsiString & AAddress) {FAddress = AAddress; };
    /*!
        �����������˿�
    */
    inline int Port () { return  FPort; };
    /*!
        ���÷����������˿�
    */
    void __fastcall SetMyPort (int APort);
    /*!
        ѹ����ʼ�ֽ���,�û������������ó��ȣ�ϵͳ�Զ�ѹ��
    */
    inline int MiniCompSize()  { return FMiniCompSize; };
    /*!
        ����ѹ����ʼ�ֽ���
    */
    inline void SetMiniCompSize (int AMiniCompSize) { FMiniCompSize = AMiniCompSize; };
    /*!
        �Զ�����������ע����ǰ�汾�õ��Ǻ����������1000
    */
    inline int AutoSecond()  { return FSecond; };
    /*!
        �����Զ�����������ע����ǰ�汾�õ��Ǻ����������1000
    */
    inline void SetAutoSecond (int ASecond) {FSecond = ASecond; };
    /*!
        ����Log�¼�
    */
    inline void SetOnLogEvent (TXNetSocketLogEvent AEvent) { FOnLogEvent = AEvent; };
    /*!
        ����Error�¼�
    */
    inline void SetOnXNetError (TXNetSocketErrorEvent AEvent) { FOnXNetError = AEvent; };
    /*!
        ���ö����������ݰ��¼�
    */
    inline void SetOnXNetRead (TXNetSocketNotifyEvent AEvent) { FOnXNetRead = AEvent; };
    /*!
        �����������ע��ɹ��¼�
    */
    inline void SetOnLoginOK (TXNetSocketNotifyEvent AEvent) { FOnLoginOK = AEvent; };
};
//---------------------------------------------------------------------------

#endif
