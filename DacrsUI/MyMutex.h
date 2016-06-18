#pragma once
#include <windows.h>

class CMyMutex
{
public:
	CMyMutex(void);
	~CMyMutex(void);
public:
    bool Lock();
	bool Unlock();
private:
	CRITICAL_SECTION m_cs;
	int m_leave;
};

class CAutoMutex
{
public:
	CAutoMutex(CCriticalSection  *pCs){
		m_pCs = pCs;
		m_pCs->Lock();
	}
	~CAutoMutex(){
		m_pCs->Unlock();
	}
private:
	CCriticalSection *m_pCs;
};

#define LOCK(cs) CAutoMutex autoMutex(cs);