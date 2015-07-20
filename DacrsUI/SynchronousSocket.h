#pragma once
#include <process.h>
#include <string>
#include <vector>
using namespace std;

class CSynchronousSocket
{
public:
	CSynchronousSocket(void);
	~CSynchronousSocket(void);
private:
	SOCKET  m_Socket ;
	BOOL    OnInitSocket();   //初始化SOCKET
	SOCKET  OnConnnect(const char *pIpAddr, WORD nPort);   //连接 返回SOCKET
	BOOL    OnSend(const char* sCmdBuff , int nLen);
	std::vector<unsigned char> buffer;
	void    RecvDataThread(); //接收数据线程
	static UINT WINAPI RecvDataProc(LPVOID pParam);
	void    OnClose();
	bool    IsEnd;
public:
	SOCKET  OnblockConnnect(const char *pIpAddr, WORD nPort);   //连接 返回SOCKET
public:
	//static int GetRpcRes(const string ip,int port,string cmd,string & rev,int timerout);
	static int GetRpcRes(const string ip,int port,const string cmd,string & rev,int timerout);
};
