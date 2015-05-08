#include "StdAfx.h"
#include "SynchronousSocket.h"

CSynchronousSocket::CSynchronousSocket(void)
{
	m_Socket = INVALID_SOCKET ;
	IsEnd = true;
}

CSynchronousSocket::~CSynchronousSocket(void)
{
	m_Socket = INVALID_SOCKET ;
	IsEnd = true;
	OnClose();
}
BOOL CSynchronousSocket::OnInitSocket()
{
	buffer.clear();
	RecvDataThread();
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

				hSocket = INVALID_SOCKET;
				closesocket(hSocket);
			}
		}
	}
	return hSocket ;
}
BOOL CSynchronousSocket::OnSend(const char* sCmdBuff , int nLen)
{
	if ( INVALID_SOCKET == m_Socket ||  NULL == sCmdBuff || 0 == nLen ) return FALSE ;

	if (send(m_Socket, sCmdBuff, nLen, 0) == -1){
		return FALSE ;
	}
	return TRUE ;
}
void CSynchronousSocket::RecvDataThread()
{
	HANDLE hRecvThread;
	UINT uRecvThreadId = 0;
	hRecvThread = (HANDLE)_beginthreadex(NULL,	0,	RecvDataProc,this,CREATE_SUSPENDED, &uRecvThreadId);
	if ( hRecvThread != INVALID_HANDLE_VALUE)
	{
		SetThreadPriority(hRecvThread, THREAD_PRIORITY_NORMAL);
		ResumeThread( hRecvThread );
	}
}
int CSynchronousSocket::GetRpcRes(const string ip,int port,string cmd,string & rev,int timerout)
{
	CSynchronousSocket te;
	if(te.OnInitSocket())
		if (te.OnConnnect(ip.c_str(),port))
			if(te.OnSend(cmd.c_str(),cmd.length()))
			{
				do 
				{
					if(timerout < 0)
					{
						return -1;
					}
					timerout -= 100;
					Sleep(100);
				} while (te.IsEnd==false);
			}		
			string tep(te.buffer.begin(),te.buffer.end());
			rev =tep;
			return rev.length();
}
UINT WINAPI CSynchronousSocket::RecvDataProc(LPVOID pParam)
{
	unsigned char tempbuffer[10*1024];
	CSynchronousSocket * pRecv  = (CSynchronousSocket*)pParam ;
	bool isSartRev = false;
	pRecv -> IsEnd = false;
	if ( NULL != pRecv ) {
		while (true)
		{
			if ( INVALID_SOCKET != pRecv->m_Socket) {
				isSartRev = true;
				memset(tempbuffer,0,sizeof(tempbuffer));
				int bufferSize = sizeof(tempbuffer) - 1;
				int nRes = recv( pRecv->m_Socket , (char*)tempbuffer , bufferSize , 0);
				if ( nRes > 0) {
					pRecv->buffer.insert(pRecv->buffer.end(),tempbuffer,tempbuffer+nRes);
					continue;
				}
				else
				{
					break;
				}
			}
			else if(isSartRev == true)
			{
				break;
			}
		}
	}
	pRecv -> IsEnd = true;
	return 0;
}





void CSynchronousSocket::OnClose()
{
	if ( m_Socket !=INVALID_SOCKET ) {
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET ;
	}
}
