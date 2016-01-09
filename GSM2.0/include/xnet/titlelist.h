/*
 * TitleList for XNet v1.2.2 2010-11-29
 *    by 刘志军
 *
 * TitleList 用于增加,删除,使用XNet Title
 *
 */
#ifndef TitleListH
#define TitleListH

#include <string>
#include <list>

#ifdef __BORLANDC__
#include <Forms.hpp>
#endif

using namespace std;

namespace xnet {

#define mtDotChar   '.'
#define mtNoneChar  '*'		// 通配字符
#define mtLeftChar  '>'		// 域左匹配字符
#define mtSplitChar ';'	    // 分隔字符

/*!
	注册Title参数,可以用来匹配Title
*/
#ifdef __BORLANDC__
class PACKAGE TTitlePartten
#else
class TTitlePartten
#endif
{
protected:
    friend class TTitleList;
	typedef struct {
		char *leftstr;				// 前匹配字串
		char *rightstr;				// 后匹配字串
		int leftlen;				// 前字串长
		int rightlen;				// 后字串长
		int matchflag;				// 匹配模式
	} TDotPartten;
	void Init (const char *Partten, const int Length);
	TDotPartten *Item;			// 按dot级数存放
	string str;					// 原串
	int count;					// dot级数
	bool leftflag;				// 域左匹配标志
#ifdef _UNIX
public:
#endif
	inline TTitlePartten () {};
	/*!
		构造函数,根据指定长度的字符串产生
		-	Partten 参数
		-	Length 参数长度
	*/
	TTitlePartten (const char *Partten, const int Length);
	/*!
		构造函数,根据以数值0结尾的字符串产生
	*/
	TTitlePartten (const char *Partten);
	~TTitlePartten ();
public:
	/*!
		构造函数,根据指定长度的字符串产生
		-	Partten 参数
		-	Length 参数长度
	*/
    static TTitlePartten *NewObject (const char *Partten, const int Length);
	/*!
		构造函数,根据以数值0结尾的字符串产生
	*/
    static TTitlePartten *NewObject (const char *Partten);
    static void DelObject (TTitlePartten *Object);
	/*!
		是否匹配以数值0结尾的Title, 返回值
		-	true	匹配
		-	false	不匹配
	*/
	bool Match (const char *Title);
    /*!
        返回原字符串
    */
    const string & Partten () { return str; };
};

#ifdef __BORLANDC__
class PACKAGE TTitleList
#else
class TTitleList
#endif
{
public:
	typedef list <TTitlePartten *> TList;
    static TTitleList *NewObject ();
    static void DelObject (TTitleList *Object);
	void Clear ();
	void AddTitles (const char *titles, const int length);
	void DelTitles (const char *titles, const int length);
	const string & NewAddTitles (const char *titles, const int length);
	const string & NewDelTitles (const char *titles, const int length);
	bool MatchTitle (const char *title);
	const string & Titles ();
	bool AddTitle (const char *, int);
	bool DelTitle (const char *, int);
    TList *List () {return FList;};
#ifdef _WINDOWS_    
private:
#endif
	TTitleList ();
	~TTitleList ();	
private:	
	string FTitles;
	TList *FList;
};

}

#endif
