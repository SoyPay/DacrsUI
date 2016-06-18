#include "StdAfx.h"
#include "Log.h"
#include "SynchronousSocket.h"

CSynchronousSocket::CSynchronousSocket(void)
{
	m_Socket = INVALID_SOCKET ;
}

CSynchronousSocket::~CSynchronousSocket(void)
{
	vector<unsigned char> vTemp;
	buffer.swap(vTemp);
	OnClose();
}
BOOL CSynchronousSocket::OnInitSocket()
{
	buffer.clear();
	return TRUE ;
}
SOCKET CSynchronousSocket::OnConnnect(const char *pIpAddr, WORD nPort)
{
	struct sockaddr_in servAddr;
	SOCKET hSocket =  INVALID_SOCKET;
	memset(&servAddr, 0, sizeof(struct sockaddr_in));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(nPort);
	servAddr.sin_addr.s_addr = inet_addr(pIpAddr);    //inet_addr("127.0.0.1");
	if (servAddr.sin_addr.s_addr != INADDR_NONE){
		if ((hSocket = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET){
			if (connect(hSocket, (struct sockaddr *)&servAddr, sizeof(struct sockaddr_in)) == 0){
				m_Socket = hSocket;
			}else{
				int error;
				error = WSAGetLastError();
				hSocket = INVALID_SOCKET;
				closesocket(hSocket);
			}
		}
	}
	return hSocket ;
}

SOCKET CSynchronousSocket::OnblockConnnect(const char *pIpAddr, WORD nPort)
{
	struct sockaddr_in servAddr;
	SOCKET hSocket =  INVALID_SOCKET;
	memset(&servAddr, 0, sizeof(struct sockaddr_in));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(nPort);
	servAddr.sin_addr.s_addr = inet_addr(pIpAddr);//inet_addr("127.0.0.1");
	if (servAddr.sin_addr.s_addr != INADDR_NONE){
		if ((hSocket = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET){
			if (connect(hSocket, (struct sockaddr *)&servAddr, sizeof(struct sockaddr_in)) == 0){
				return hSocket ;
			}else{
				int error;
				error = WSAGetLastError();

				closesocket(hSocket);
				hSocket = INVALID_SOCKET;
				
			}
		}
	}
	return hSocket ;
}
BOOL CSynchronousSocket::OnSend(const char* sCmdBuff , int nLen)
{
	if ( INVALID_SOCKET == m_Socket ||  NULL == sCmdBuff || 0 == nLen ) return FALSE ;

	if (-1 == send(m_Socket, sCmdBuff, nLen, 0))
			return FALSE;

	return TRUE;
}

int CSynchronousSocket::GetRpcRes(const string &ip,int port,const string &cmd, std::string &strRecv,int timeout)
{
	CSynchronousSocket te;
	if(!te.OnInitSocket())
		return -1;
	if (INVALID_SOCKET == te.OnConnnect(ip.c_str(),port)) 
	{
		TRACE("Connect destisnation %s:%d failed!\n", ip.c_str(), port);
		return -1;
	}
	if(!te.OnSend(cmd.c_str(),cmd.length()))
	{
		TRACE("Send RPC Command failed!\n");
		return -1;
	}
	int ret = setsockopt(te.m_Socket,SOL_SOCKET,SO_RCVTIMEO, (char*) &timeout,sizeof(timeout));
//	TRACE("setsockopt rec timeout result:%d\n", ret);

	unsigned char tempbuffer[2048];
	int bufferSize = sizeof(tempbuffer) - 1;
	int nRes(0);
	do 
	{
		memset(tempbuffer,0,sizeof(tempbuffer));
		nRes = recv( te.m_Socket , (char*)tempbuffer , bufferSize , 0);
		if(nRes > 0) {
			static bool bTemp(false);
			string strTemp(tempbuffer,tempbuffer+nRes);
			if(!bTemp) {
				TRACE("tempbuffer:%s\r\n", strTemp.c_str());
				bTemp = true;
			}else {
				TRACE("%s", strTemp.c_str());
			}

			te.buffer.insert(te.buffer.end(),tempbuffer,tempbuffer+nRes);
		}
		else {
			int error;
			error = WSAGetLastError();
			TRACE("recv error code:%d\n", error);
			return -1;
		}					
	} while (nRes == bufferSize);
	bool errorFlag(FALSE);
	strRecv.assign(te.buffer.begin(), te.buffer.end());
	size_t BeginPos = strRecv.find("Content-Length: ");
	if( BeginPos != string::npos) {
		size_t EndPos = strRecv.find("Content-Type: ");
		if (EndPos == string::npos)
		{
			errorFlag = TRUE;
		}else {
			BeginPos += sizeof("Content-Length: ");
			string temp = strRecv.substr(BeginPos-1, EndPos-BeginPos-1);
			int length = atoi(temp.c_str());
			size_t subPos = strRecv.find("{\"result\":");
			if(subPos == string::npos) {
				errorFlag = TRUE;
			} else {
			string strContent = strRecv.substr(subPos, -1);
			if(length != strContent.length()) {
				errorFlag = TRUE;
				}
			}
		}
	}
	if(errorFlag) {
		LogPrint("INFO", "Call RPC Command : %s recevice data error!\n", cmd.c_str());
		LogPrint("INFO", "Receive Data %d: %s \n", strRecv.length(), strRecv.c_str());
	}
	//memcpy(pRecvStr.get(), &te.buffer[0], te.buffer.size());
	//TRACE("recv:%s\n", strRecv.c_str());
	return te.buffer.size()+1;
}

void CSynchronousSocket::OnClose()
{
	if ( m_Socket !=INVALID_SOCKET ) {
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET ;
	}
}
