#pragma once
#include <string>
using namespace std;
class CRpcCmd
{
public:
	CRpcCmd(void);
	CRpcCmd(const string ip,int port,int timerout)
	{
	mIp = ip;
	mPort = port;
	mTimerout = timerout;
	}
	void SetHeadString(const CStringA	 &sendPreHeadstr,const CStringA &sendendHeadstr)
	{
		m_sendPreHeadstr = sendPreHeadstr;
		m_sendendHeadstr = sendendHeadstr;
	}
	void SetPort(int port)
	{
		mPort = port;
	}

	 CStringA RpcJosnStr;
    void BuildSendString(const CString	 &cmd,CStringA &sendStr);
	
	~CRpcCmd(void);
private:
	CStringA				m_sendPreHeadstr;
	CStringA				m_sendendHeadstr;
	string mIp;
	int mPort;
	int mTimerout;
	void RPCCommandToJson(const CString& strRPCCommand,CStringA& strSendData);
public:
	int SendRpc(CString cmd,string &rev);
	int SendContactRpc(CString cmd,string &rev);
};

