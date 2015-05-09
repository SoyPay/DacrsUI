#include "stdafx.h"
#include "NoUiMsgBuffer.h"


CNoUiMsgBuffer::CNoUiMsgBuffer(){
	ClearBuffer();
	
}

void CNoUiMsgBuffer::ClearBuffer()
{
	memset(m_Recvbuffer, 0, 10*1024);
	m_nLength = 0;
};

bool CNoUiMsgBuffer::AddBytesToBuffer(char *pCh, int nLen)
{

	if(m_nLength+nLen > nBufferMaxLength-1) {
		TRACE("Buffer Overflowed Len:%d\n",m_nLength + nLen);
		return false;
	}
	memcpy(m_Recvbuffer+m_nLength, pCh, nLen);
	m_nLength += nLen;
	int nPos(0);
	/*TRACE("Buffer Data %d:", m_nLength);
	for(int i=0;i<m_nLength;++i)
		TRACE("%02X", (unsigned char)m_Recvbuffer[i]);
	TRACE("\n");*/
	while(nPos < m_nLength) {
		int nStart = nPos;
		if(m_Recvbuffer[nStart] != '<') {
			TRACE("起始字符不对\n");
			return false;
		}

		if(nStart + 2 >= m_nLength)
			break;
		unsigned short nDataLen(0);
		if(m_Recvbuffer[nStart + 2] == '{') {
			memcpy(&nDataLen, m_Recvbuffer+nStart+1, 1);
			nStart += 2;
		}
		else {
			memcpy(&nDataLen, m_Recvbuffer+nStart+1, 2);
			nStart += 3;
		}
		
		if(nDataLen <= 0){
			TRACE("数据长度小于或等于0\n");
			return false;
		}

		if(nStart + nDataLen + 1 > m_nLength)
			break;

		char cMsgData[10*1024]= {0};
		memcpy(&cMsgData, m_Recvbuffer+nStart, nDataLen);
		//TRACE("prase no ui msg:");
		//for(int i=0; i<nDataLen; ++i)
		//	TRACE("%02X", cMsgData[i]);
		//TRACE("\n");

		nStart += nDataLen;
		if(m_Recvbuffer[nStart] != '>') {
			TRACE("结束字符不对\n");
			return false;
		}
		nPos = nStart + 1;
		//TRACE("nPos:%d\n", nPos);
		cs_NoUiNotifyMsg.Lock();
		CString strMsg;
		strMsg.Format(_T("%s"), cMsgData);
		TRACE("push msg to deque:");
		TRACE("%s\n", cMsgData);
		m_dqNoUiMsg.push_back(strMsg);
		cs_NoUiNotifyMsg.Unlock();
	}
	if(nPos >= m_nLength) {
		ClearBuffer();
	}else {
		memmove(m_Recvbuffer, m_Recvbuffer+nPos, m_nLength-nPos);
		m_nLength = m_nLength-nPos;
	}
	return true;
}

bool CNoUiMsgBuffer::HaveNoUiMsg() 
{
	cs_NoUiNotifyMsg.Lock();
	bool bRet = !m_dqNoUiMsg.empty();
	cs_NoUiNotifyMsg.Unlock();
	return bRet;
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