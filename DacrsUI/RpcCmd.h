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
	void SetHeadString(const CStringA	 &sendPreHeadstr,const CStringA &sendendHeadstr)
	{
		m_sendPreHeadstr = sendPreHeadstr.GetString();
		m_sendendHeadstr = sendendHeadstr.GetString();
	}
	void SetPort(int port)
	{
		mPort = port;
	}
	void SetSeverIp(const CStringA & ip){
		mIp = ip.GetString();
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
	int SendContactRpc(CString cmd,string &rev);
};

extern bool IsAllDigtal(const char* pData);