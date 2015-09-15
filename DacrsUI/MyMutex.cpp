#include "StdAfx.h"
#include "MyMutex.h"


CMyMutex::CMyMutex(void)
{
	InitializeCriticalSection(&m_cs);
	m_leave = false;
}


CMyMutex::~CMyMutex(void)
{
	if (!m_leave)
	{
		LeaveCriticalSection(&m_cs);
	}
	
	DeleteCriticalSection(&m_cs);
}

bool CMyMutex::Lock()
{
	EnterCriticalSection(&m_cs);
	return true;
}

bool CMyMutex::Unlock()
{
	LeaveCriticalSection(&m_cs);
	m_leave = true;
	return true;
}