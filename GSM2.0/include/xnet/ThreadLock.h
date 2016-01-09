/*
 * ThreadLock.h
 * for xnetd v1.2.2 2010-11-3
 */
 
//---------------------------------------------------------------------------

#ifndef ThreadLockH
#define ThreadLockH

#ifdef _UNIX
#include <pthread.h>
#else
#include <windows.h>
#endif

#ifdef __BORLANDC__
#include <Forms.hpp>
class PACKAGE TThreadLock
#else
class TThreadLock
#endif
{
public:
    static TThreadLock *NewObject ();
    static void DelObject (TThreadLock *Object);
    void Lock ();
    void Unlock ();
private:
    TThreadLock ();
    ~TThreadLock ();
#ifdef _UNIX
	pthread_mutex_t FMutex;
#else
    CRITICAL_SECTION CriticalSection;
#endif
};

#endif
