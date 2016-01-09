/*
 * TitleList for XNet v1.2.2 2010-11-29
 *    by ��־��
 *
 * TitleList ��������,ɾ��,ʹ��XNet Title
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
#define mtNoneChar  '*'		// ͨ���ַ�
#define mtLeftChar  '>'		// ����ƥ���ַ�
#define mtSplitChar ';'	    // �ָ��ַ�

/*!
	ע��Title����,��������ƥ��Title
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
		char *leftstr;				// ǰƥ���ִ�
		char *rightstr;				// ��ƥ���ִ�
		int leftlen;				// ǰ�ִ���
		int rightlen;				// ���ִ���
		int matchflag;				// ƥ��ģʽ
	} TDotPartten;
	void Init (const char *Partten, const int Length);
	TDotPartten *Item;			// ��dot�������
	string str;					// ԭ��
	int count;					// dot����
	bool leftflag;				// ����ƥ���־
#ifdef _UNIX
public:
#endif
	inline TTitlePartten () {};
	/*!
		���캯��,����ָ�����ȵ��ַ�������
		-	Partten ����
		-	Length ��������
	*/
	TTitlePartten (const char *Partten, const int Length);
	/*!
		���캯��,��������ֵ0��β���ַ�������
	*/
	TTitlePartten (const char *Partten);
	~TTitlePartten ();
public:
	/*!
		���캯��,����ָ�����ȵ��ַ�������
		-	Partten ����
		-	Length ��������
	*/
    static TTitlePartten *NewObject (const char *Partten, const int Length);
	/*!
		���캯��,��������ֵ0��β���ַ�������
	*/
    static TTitlePartten *NewObject (const char *Partten);
    static void DelObject (TTitlePartten *Object);
	/*!
		�Ƿ�ƥ������ֵ0��β��Title, ����ֵ
		-	true	ƥ��
		-	false	��ƥ��
	*/
	bool Match (const char *Title);
    /*!
        ����ԭ�ַ���
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
