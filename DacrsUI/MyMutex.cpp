#include "StdAfx.h"
#include "MyMutex.h"


CMyMutex::CMyMutex(void)
{
	InitializeCriticalSection(&m_cs);
	m_leave = 0;
}


CMyMutex::~CMyMutex(void)
{
	if (m_leave == 2)
	{
		LeaveCriticalSection(&m_cs);
	}
	
	DeleteCriticalSection(&m_cs);
}

bool CMyMutex::Lock()
{
	EnterCriticalSection(&m_cs);
	m_leave = 2;
	return true;
}

bool CMyMutex::Unlock()
{
	LeaveCriticalSection(&m_cs);
	m_leave = 1;
	return true;
}