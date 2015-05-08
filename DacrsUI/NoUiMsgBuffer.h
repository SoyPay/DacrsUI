#pragma once
#include <memory.h>
#include "MyMutex.h"

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
	char  m_Recvbuffer[10*1024];
private:
	CMyMutex        cs_NoUiNotifyMsg;  //noui֪ͨ��Ϣ�ٽ�
	std::deque<CString> m_dqNoUiMsg;
	int   m_nLength;
};