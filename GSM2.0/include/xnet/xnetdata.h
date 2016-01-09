#ifndef XNetDataH
#define XNetDataH

#include <string>

#ifdef __BORLANDC__
#include <Forms.hpp>
#endif

using namespace std;
/*
 * TXNetData for xnetd v1.2.2 2010-11-3
 *    by ��־��
 */

namespace xnet {


#define xnetDefaultPort     12200
#define xnetSysLengthLen 	3
#define xnetLengthLen 		6
#define xnetMaxHeadLen		10
#define xnetMaxDataLength	999999

#define xnetLZ77Compress '7'
#define xnetAutoCompress ' '

typedef struct {
	char x_begin;
	char x_command;						// �����ʽ,= xnetFirstCommand + TXNetCommandֵ
										// ���ط�ʽ, = xnetFirstReturn + TXNetCommandֵ
	char x_compress;
	char x_syslen[xnetSysLengthLen];
	char x_datalen[xnetLengthLen];
	char x_retcode[xnetLengthLen];
}TXNetHead;

#define xnetHeadLen (sizeof(TXNetHead))

typedef enum
{
	xerSoftClose = 999900,			// ���ִ�йرն���
	xerRelogin,						// �ظ�ע��
	xerUniqTerm,					// �����ѱ�ʹ��
	xerMaxTerm,						// �Ѵﵽ���������
	xerRemote,						// Զ��δ����
	xerTarget,						// Ŀ���ַ���ɴ�
	xerCommand,						// �������
	xerHeadError					// ���ݰ�ͷ����
} TXNetErrorCode;

typedef enum {			// Term״̬��־
	csNull,				// ��ʼΪ��ֵ
	csConnect,			// ���ӳɹ�
	csLogin,			// xnetע��ɹ�
	csClose
} TCommState;

/*!
	XNet v1.2.2���ݰ�����
*/
typedef enum
{
	xcTitle = 'A',		// Title����
	xcTitleReturn,		// ��������, ʹ��SetReturnCode���Զ���xcTitle���͸�ΪxcTitleReturn
	xcMutiTitle,		// Title�ಥ
	xcTermLogin,		// Termע��, TXNetSocket����TXNetSock
	xcBoxLogin,			// TXNetBoxע��
	xcDeamonLogin,		// XNetdע��
	xcLoginReturn,		// XNetע�᷵��
	xcRegTitle,			// ע��Title
	xcDelTitle,			// ע��Title
	xcCheckOnline,		// ���߼��
	xcCheckReturn,		// ���߼�ⷵ��
	xcGetTermList,		// ��ȡ�����б�
	xcRetTermList,		// ���������б�
	xcRegWatch,			// ע����
	xcRetWatch,			// ���ؼ����Ϣ
	/*
		XNet V1.2.2 ����������
	*/
	xcGroupQuest,        // �����
	xcGroupReturn,       // �鷵��, ʹ��SetReturnCode���Զ���xcGroup���͸�ΪxcGroupReturn
	xcRegGroup,          // ע����
	xcDelGroup           // ע����
} TXNetCommand;

/*!
	cloneType����
*/
typedef enum
{
	cloneAll,			// �����������ݰ�
	cloneHead			// ���������ݰ�ͷ
} TClonetType;

/*!
 XNet v1.2.2 ���ݰ�����

 -	XNet v1.2��һ������ͨѶ����ϵͳ���������û����������,ϵͳ��������XNet�ڲ�����,����Ӧ�ÿ����߿���,�û���������ȫ��Ӧ�ÿ��������й���.
 -	TXNetData����Ӧ�ÿ�����,�ṩ��XNet���ݰ����û�����������,����XNetϵͳ������������Ч�ı���.
 -	����TXNetData��Ӧ�ÿ����ߵĸ������Ծ�Ϊconstֻ��,�����ڱ����ڷ��ֲ��ֶ�TXNetData�ķǷ�����
*/

#define xnetInitLength 5000
#define xnetBackLength 1000

#ifdef __BORLANDC__
class PACKAGE TXNetData
#else
class TXNetData
#endif
{
	friend class TXNetSock;			// for Unix
public:
	/*!
		��ͨ���캯��,������ϵͳ���������ڴ�
	*/
	static TXNetData *NewObject ();
	/*!
		��¡���ݵĹ��캯��,����cloneType�Ĺ����¡SourceData

		cloneType:
		-	cloneAll �����������ݰ�
		-	cloneHead ���������ݰ�ͷ
	*/
	static TXNetData *NewObject (const TXNetData *SourceData, TClonetType cloneType);
	/*!
		����Source��Title�������ݰ�����Ӧ�ó���ʹ��
	*/
	static TXNetData *NewObject (const char *Source, const char *Title);
	/*!
		��������
	*/
	static void DelObject (TXNetData *Object);
	/*!
		�����û��������Ļ�����
	*/
	inline const char *Data () { return (const char *)FData; };
	/*!
		�����û����������ֽڳ���
	*/
	inline int Length() { return FLength; };
	/*!
		������,ϵͳ��Ӧ�ó�����
		-	�������������Ҫͳһ����. ��ֵ���Դ�-99999 �� 999999
	*/
	inline int ReturnCode () { return FReturnCode; };
	/*!
		��������Դ���߼�����
	*/
	inline const char *Source() { return (const char *)FSource; };
	/*!
		����Ŀ����߼�����
	*/
	inline const char *Target() { return (const char *)FTarget; };
	/*!
		���ر���Title
	*/
	inline const char *Title () { return (const char *)FTitle; };
	/*!
		���ر��Mark
	*/
	inline const char *Mark () { return (const char *)FMark; };
	/*!
		�����û�������,�Զ��������賤��,�������ݸ���һ��
		-	Data �ڴ��ַ,
		-	Length �ֽڳ���
	*/
	void SetData (const char *Data, int Length);
	/*!
		���÷�����,�����Ҫ�����ݰ���Ϊ����ģʽ
	*/	
	void SetReturnCode (int ReturnCode);
	/*!
		���ñ���Title, Title����ֵ0��������dot�ָ������в����пո�
	*/
	void SetTitle (const char *Title);
	/*!
		���ñ�ǣ��ñ��ΪӦ�ó������ã������ϵͳ�������ڿ���ʹ�á�����ֵ0������
	*/
	void SetMark (const char *Mark);
	/*!
		����Ŀ���߼�����Target

		��ΪXNet v1.2.2����Title��GroupѰַ,һ���������������Target
	*/
	void SetTarget (const char *Target);
	/*!
		�������ݰ�����
	*/
	const TXNetCommand Command ();
	/*!
		����ԴĿ���߼�����,�ڴ������Ҫ�Ѵ��������ظ�����ʱʹ��
	*/
	void Exchange ();
	/*!
		�������ݰ�����

TXNetCommand command: (δ˵����ΪXNet����)
-	xcTitle = 'A',		// Title����
-	xcTitleReturn,		// ��������, ʹ��SetReturnCode���Զ���xcTitle���͸�ΪxcTitleReturn
-	xcMutiTitle,		// Title�ಥ
-	xcRegTitle,			// ע��Title
-	xcDelTitle,			// ע��Title
-   xcGroupQuest,        // �����
-   xcGroupReturn,       // �鷵��, ʹ��SetReturnCode���Զ���xcGroup���͸�ΪxcGroupReturn
-   xcRegGroup,          // ע����
-   xcDelGroup           // ע����
	*/
	void SetCommand (TXNetCommand command);
	/*!
		��Ӧ�ó���ʹ��
	*/
	inline int SysLength () { return FSysLength; };							// ����ϵͳ���ֽ���
	/*!
		��Ӧ�ó���ʹ��
	*/
	const char *Head () { return FHead; };									// �������ݰ�ͷ��ַ
	/*!
		��Ӧ�ó���ʹ��
	*/
	void SetSource (const char *Source);									// ����Դ����
	/*!
		��Ӧ�ó���ʹ��
	*/
	const char Compress ();													// ����ѹ����־
	/*!
		��Ӧ�ó���ʹ��
	*/
	void SetCompressData (const char *Data, const int Length);      // ����ѹ������û�������
	/*!
		��Ӧ�ó���ʹ��
	*/
	bool CheckHead ();														// ������ݰ�ͷ,�����û����ڴ�
	/*!
		��Ӧ�ó���ʹ��
	*/
	void CheckSysData ();													// ���ϵͳ������
#ifdef _WINDOWS_	
private:
#endif
	TXNetData ();
	TXNetData (const TXNetData *SourceData, TClonetType cloneType);
	/*!
		��������
	*/
	~TXNetData ();
	TXNetData (const char *Source, const char *Title);
private:	
	typedef enum {stTitle, stTarget, stSource, stMark } TValueType;
	void SetValue (TValueType type, const char *Value);
	char FBuffer[xnetInitLength];
	size_t FBufferLength;
	void BaseSetData (const char *Data, const int Length);
	void SetDataLength ();
	inline int HeadLength () { return (FSysLength + xnetHeadLen); };		// ��������ͷ�ֽ���
	void SetSysLength (int Length);										// ����ϵͳ���ֽ���
	void SetName (const char *Source, const char *Target);					// ����Դ��Ŀ������
	void SetCompress (char value);											// ����ѹ����־
	char *FHead;
	char *FData;
	int FLength;
	int FReturnCode;
	int FSysLength;
	char *FSource;
	char *FTarget;
	char *FTitle;
	char *FMark;
};

} // endof namespace xnet


#endif
