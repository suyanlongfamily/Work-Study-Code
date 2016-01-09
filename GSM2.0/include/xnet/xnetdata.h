#ifndef XNetDataH
#define XNetDataH

#include <string>

#ifdef __BORLANDC__
#include <Forms.hpp>
#endif

using namespace std;
/*
 * TXNetData for xnetd v1.2.2 2010-11-3
 *    by 刘志军
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
	char x_command;						// 在命令方式,= xnetFirstCommand + TXNetCommand值
										// 返回方式, = xnetFirstReturn + TXNetCommand值
	char x_compress;
	char x_syslen[xnetSysLengthLen];
	char x_datalen[xnetLengthLen];
	char x_retcode[xnetLengthLen];
}TXNetHead;

#define xnetHeadLen (sizeof(TXNetHead))

typedef enum
{
	xerSoftClose = 999900,			// 软件执行关闭动作
	xerRelogin,						// 重复注册
	xerUniqTerm,					// 名称已被使用
	xerMaxTerm,						// 已达到最大连接数
	xerRemote,						// 远程未连接
	xerTarget,						// 目标地址不可达
	xerCommand,						// 命令错误
	xerHeadError					// 数据包头错误
} TXNetErrorCode;

typedef enum {			// Term状态标志
	csNull,				// 初始为空值
	csConnect,			// 连接成功
	csLogin,			// xnet注册成功
	csClose
} TCommState;

/*!
	XNet v1.2.2数据包类型
*/
typedef enum
{
	xcTitle = 'A',		// Title单播
	xcTitleReturn,		// 单播返回, 使用SetReturnCode会自动将xcTitle类型改为xcTitleReturn
	xcMutiTitle,		// Title多播
	xcTermLogin,		// Term注册, TXNetSocket或者TXNetSock
	xcBoxLogin,			// TXNetBox注册
	xcDeamonLogin,		// XNetd注册
	xcLoginReturn,		// XNet注册返回
	xcRegTitle,			// 注册Title
	xcDelTitle,			// 注消Title
	xcCheckOnline,		// 在线检测
	xcCheckReturn,		// 在线检测返回
	xcGetTermList,		// 获取连接列表
	xcRetTermList,		// 返回连接列表
	xcRegWatch,			// 注册监控
	xcRetWatch,			// 返回监控信息
	/*
		XNet V1.2.2 新增交易码
	*/
	xcGroupQuest,        // 组调用
	xcGroupReturn,       // 组返回, 使用SetReturnCode会自动将xcGroup类型改为xcGroupReturn
	xcRegGroup,          // 注册组
	xcDelGroup           // 注销组
} TXNetCommand;

/*!
	cloneType定义
*/
typedef enum
{
	cloneAll,			// 复制整个数据包
	cloneHead			// 仅复制数据包头
} TClonetType;

/*!
 XNet v1.2.2 数据包对象

 -	XNet v1.2的一个完整通讯包由系统数据区和用户数据区组成,系统数据区由XNet内部管理,不对应用开发者开放,用户数据区完全由应用开发者自行管理.
 -	TXNetData面向应用开发者,提供对XNet数据包的用户数据区操作,并对XNet系统数据区进行有效的保护.
 -	由于TXNetData给应用开发者的各项属性均为const只读,可以在编译期发现部分对TXNetData的非法操作
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
		普通构造函数,仅申请系统数据区的内存
	*/
	static TXNetData *NewObject ();
	/*!
		克隆数据的构造函数,根据cloneType的规则克隆SourceData

		cloneType:
		-	cloneAll 复制整个数据包
		-	cloneHead 仅复制数据包头
	*/
	static TXNetData *NewObject (const TXNetData *SourceData, TClonetType cloneType);
	/*!
		根据Source和Title生成数据包，非应用程序使用
	*/
	static TXNetData *NewObject (const char *Source, const char *Title);
	/*!
		析构函数
	*/
	static void DelObject (TXNetData *Object);
	/*!
		返回用户数据区的缓冲区
	*/
	inline const char *Data () { return (const char *)FData; };
	/*!
		返回用户数据区的字节长度
	*/
	inline int Length() { return FLength; };
	/*!
		返回码,系统和应用程序公用
		-	返回码的设置需要统一管理. 数值可以从-99999 到 999999
	*/
	inline int ReturnCode () { return FReturnCode; };
	/*!
		返回数据源的逻辑名称
	*/
	inline const char *Source() { return (const char *)FSource; };
	/*!
		返回目标的逻辑名称
	*/
	inline const char *Target() { return (const char *)FTarget; };
	/*!
		返回标题Title
	*/
	inline const char *Title () { return (const char *)FTitle; };
	/*!
		返回标记Mark
	*/
	inline const char *Mark () { return (const char *)FMark; };
	/*!
		设置用户区数据,自动分配所需长度,并将数据复制一份
		-	Data 内存地址,
		-	Length 字节长度
	*/
	void SetData (const char *Data, int Length);
	/*!
		设置返回码,如果需要将数据包改为返回模式
	*/	
	void SetReturnCode (int ReturnCode);
	/*!
		设置标题Title, Title以数值0结束，以dot分隔，当中不能有空格。
	*/
	void SetTitle (const char *Title);
	/*!
		设置标记，该标记为应用程序自用，存放在系统区，便于快速使用。以数值0结束。
	*/
	void SetMark (const char *Mark);
	/*!
		设置目标逻辑名称Target

		因为XNet v1.2.2基于Title和Group寻址,一般情况下无需设置Target
	*/
	void SetTarget (const char *Target);
	/*!
		返回数据包类型
	*/
	const TXNetCommand Command ();
	/*!
		交换源目的逻辑名称,在处理后需要把处理结果返回给发起方时使用
	*/
	void Exchange ();
	/*!
		设置数据包类型

TXNetCommand command: (未说明的为XNet自用)
-	xcTitle = 'A',		// Title单播
-	xcTitleReturn,		// 单播返回, 使用SetReturnCode会自动将xcTitle类型改为xcTitleReturn
-	xcMutiTitle,		// Title多播
-	xcRegTitle,			// 注册Title
-	xcDelTitle,			// 注消Title
-   xcGroupQuest,        // 组调用
-   xcGroupReturn,       // 组返回, 使用SetReturnCode会自动将xcGroup类型改为xcGroupReturn
-   xcRegGroup,          // 注册组
-   xcDelGroup           // 注销组
	*/
	void SetCommand (TXNetCommand command);
	/*!
		非应用程序使用
	*/
	inline int SysLength () { return FSysLength; };							// 返回系统区字节数
	/*!
		非应用程序使用
	*/
	const char *Head () { return FHead; };									// 返回数据包头地址
	/*!
		非应用程序使用
	*/
	void SetSource (const char *Source);									// 设置源名称
	/*!
		非应用程序使用
	*/
	const char Compress ();													// 返回压缩标志
	/*!
		非应用程序使用
	*/
	void SetCompressData (const char *Data, const int Length);      // 设置压缩后的用户数据区
	/*!
		非应用程序使用
	*/
	bool CheckHead ();														// 检查数据包头,申请用户区内存
	/*!
		非应用程序使用
	*/
	void CheckSysData ();													// 检查系统区数据
#ifdef _WINDOWS_	
private:
#endif
	TXNetData ();
	TXNetData (const TXNetData *SourceData, TClonetType cloneType);
	/*!
		析构函数
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
	inline int HeadLength () { return (FSysLength + xnetHeadLen); };		// 返回数据头字节数
	void SetSysLength (int Length);										// 设置系统区字节数
	void SetName (const char *Source, const char *Target);					// 设置源和目的名称
	void SetCompress (char value);											// 设置压缩标志
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
