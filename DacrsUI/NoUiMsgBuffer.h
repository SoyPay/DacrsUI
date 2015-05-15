#pragma once
#include <memory.h>
#include "MyMutex.h"
const int nBufferMaxLength = 1024*1024;
class CNoUiMsgBuffer 
{
public:
	CNoUiMsgBuffer();
	~CNoUiMsgBuffer(){};
	void ClearBuffer();

	bool AddBytesToBuffer(char *pCh, int nSize);
	void GetNoUiMsg(CString &strMsg);
	bool HaveNoUiMsg(); 
public:
	char  m_Recvbuffer[nBufferMaxLength];
private:
	CMyMutex        cs_NoUiNotifyMsg;  //noui通知消息临界
	std::deque<CString> m_dqNoUiMsg;
	int   m_nLength;
};