#include "stdafx.h"
#include "NoUiMsgBuffer.h"


CNoUiMsgBuffer::CNoUiMsgBuffer(){
	ClearBuffer();
	m_nLength = 0;
}

void CNoUiMsgBuffer::ClearBuffer()
{
	memset(m_Recvbuffer, 0, 10*1024);
};

bool CNoUiMsgBuffer::AddBytesToBuffer(char *pCh, int nLen)
{
	if(m_nLength+nLen > 10*1024-1)
		return false;
	memcpy(m_Recvbuffer+m_nLength, pCh, nLen);
	m_nLength += nLen;
	int nPos(0);
	while(nPos < m_nLength) {
		if(m_Recvbuffer[nPos] != '<')
			return false;

		if(nPos + 2 < m_nLength)
			break;

		unsigned short nLen(0);
		memcpy(&nLen, m_Recvbuffer+nPos+1, 2);
		if(nLen <= 0)
			return false;

		if(nPos + nLen + 4 < m_nLength)
			break;

		char cMsgData[10*1024]= {0};
		memcpy(&cMsgData, m_Recvbuffer+nPos+3, nLen);
		if(m_Recvbuffer[nPos+4+nLen] != '>')
			return false;
		nPos += nLen + 5;

		CString strMsg;
		strMsg.Format(_T("%s"), cMsgData);
		cs_NoUiNotifyMsg.Lock();
		m_dqNoUiMsg.push_back(strMsg);
		cs_NoUiNotifyMsg.Unlock();
	}
	if(nPos == m_nLength) {
		ClearBuffer();
	}else {
		memmove(m_Recvbuffer, m_Recvbuffer+nPos, m_nLength-nPos);
	}
	return true;
}

bool CNoUiMsgBuffer::HaveNoUiMsg() 
{
	cs_NoUiNotifyMsg.Lock();
	return !m_dqNoUiMsg.empty();
	cs_NoUiNotifyMsg.Unlock();
}
void CNoUiMsgBuffer::GetNoUiMsg(CString &strNoUiMsg)
{
	cs_NoUiNotifyMsg.Lock();
	if(!m_dqNoUiMsg.empty()) {
	  strNoUiMsg = m_dqNoUiMsg.front();
	  m_dqNoUiMsg.pop_front();
	}
	cs_NoUiNotifyMsg.Unlock();
}