//---------------------------------------------------------------------------
// compress.h
// for XNet v1.2.2 2010-11-03

#ifndef compressH
#define compressH

#ifdef __BORLANDC__
#include <Forms.hpp>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define COMPRESS_N		    4096
#define COMPRESS_F		    18
#define THRESHOLD	2
#define COMPRESS_NIL		COMPRESS_N
#define MaxMem      1024*1024


#ifdef __BORLANDC__
class PACKAGE TCompressLZ77
#else
class TCompressLZ77
#endif
{
private:
    char *FData;
    int FLength ;
    unsigned long int textsize ;
	unsigned long int codesize ;
	unsigned long int printcount ;
    unsigned char text_buf[COMPRESS_N + COMPRESS_F - 1];
    int		match_position ;
    int     match_length ;
    int     lson[COMPRESS_N + 1] ;
    int     rson[COMPRESS_N + 257] ;
    int     dad[COMPRESS_N + 1] ;
    ////////////////////////////
    void InitTree(void) ;
    void InsertNode(int r) ;
    void DeleteNode(int p) ;
#ifdef _UNIX    
public:
#endif
    TCompressLZ77 ();
    virtual ~TCompressLZ77();
public:
    static TCompressLZ77 * NewObject();
    static void DelObject (TCompressLZ77 *Object);
    bool CompressData(const char* src, int srclen) ;
    void DecompressData(const char* src, int srclen) ;
    char *Data(){ return FData ;} ;
    int Length(){ return FLength ;} ;
};
//---------------------------------------------------------------------------

#endif
