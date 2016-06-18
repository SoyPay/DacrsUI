#pragma once
#include <string>
using namespace std;
class CRpcCmd
{
public:
	CRpcCmd(void);
	CRpcCmd(const CStringA	 ip,int port,int timerout)
	{
	mIp = ip;
	mPort = port;
	mTimerout = timerout;
	}
	void SetHeadString(const string	 &sendPreHeadstr,const string &sendendHeadstr)
	{
		m_sendPreHeadstr = sendPreHeadstr;
		m_sendendHeadstr = sendendHeadstr;
	}
	void SetPort(int port)
	{
		mPort = port;
	}
	void SetSeverIp(const string & ip){
		mIp = ip;
	}
	 CStringA RpcJosnStr;
    void BuildSendString(const string	 &cmd,string &sendStr);
	
	~CRpcCmd(void);
private:
	string				m_sendPreHeadstr;
	string				m_sendendHeadstr;
	string               mIp;
	int mPort;
	int mTimerout;
	void RPCCommandToJson(const string& strRPCCommand,string& strSendData);
public:
	int SendRpc(string cmd,string &rev);
	int SendContactRpc(string cmd,string &rev);
};

extern bool IsAllDigtal(const char* pData);