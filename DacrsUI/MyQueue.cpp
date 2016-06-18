#include "StdAfx.h"
#include "MyQueue.h"
//#include"task.h"

CMyQueue::CMyQueue(void)
{
}


CMyQueue::~CMyQueue(void)
{
}

 bool CMyQueue::pop(CPostMsg&msg)
{
	m_mutex.Lock();
	bool flag = false;
	if(!m_TaskQueue.empty())
	{
		msg=m_TaskQueue.front();
		m_TaskQueue.pop_front();
		flag = true;
	}
	m_mutex.Unlock();
	return flag;
}

bool CMyQueue::push(CPostMsg&t)
{
	m_mutex.Lock();
	m_TaskQueue.push_back(t);
	m_mutex.Unlock();
	return true;
}

bool CMyQueue::isEmpty()
{
	bool ret=false;
	m_mutex.Lock();
	ret=m_TaskQueue.empty();
	m_mutex.Unlock();
	return ret;
}

bool CMyQueue::pushFront( CPostMsg&t )
{
	m_mutex.Lock();
	m_TaskQueue.push_front(t);
	m_mutex.Unlock();
	return true;
}

bool CMyQueue::clear()
{
	m_mutex.Lock();
// 	std::deque<CTask*>::iterator iter=m_TaskQueue.begin();
// 	for(;iter!=m_TaskQueue.end();iter++)
// 	{
// 		delete (*iter);
// 	}
	m_TaskQueue.clear();
	m_mutex.Unlock();
	return true;
}
void CMyQueue::ClearMessageType(int type)
{
	m_mutex.Lock();
	std::deque<CPostMsg>::iterator iter;
	for(iter=m_TaskQueue.begin();iter!=m_TaskQueue.end();)
	{
		if (iter->GetUItype() == type)
		{
			iter = m_TaskQueue.erase(iter);
		}else{
			++iter;
		}
	}
	m_mutex.Unlock();
}