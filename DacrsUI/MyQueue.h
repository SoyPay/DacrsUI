#pragma once
#include<deque>
#include"MyMutex.h"
#include "PostMsg.h"
using namespace std;

class CPostMsg;
class CMyQueue
{
public:
	CMyQueue(void);
	~CMyQueue(void);
public:
	bool pop(CPostMsg&msg);
	bool push(CPostMsg& msg);
	bool pushFront(CPostMsg& msg);
	void ClearMessageType(int type);
	bool isEmpty();
	bool clear();
private:
	std::deque<CPostMsg>m_TaskQueue;
	CMyMutex m_mutex;
};