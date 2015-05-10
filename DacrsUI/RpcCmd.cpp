#include "StdAfx.h"
#include "RpcCmd.h"
#include "SynchronousSocket.h"
#include "json\reader.h"
#include "json\value.h"


CRpcCmd::CRpcCmd(void)
{
	mPort = 18332;
}



CRpcCmd::~CRpcCmd(void)
{
}
int CRpcCmd::SendContactRpc(CString cmd,string &rev){
	CStringA strSendData;

	RpcJosnStr = cmd;
	string tepsend = cmd.GetString();
	string nSendStr = "";
	nSendStr += m_sendPreHeadstr;
	CStringA nMidStr = "Content-Length: ";
	nMidStr.AppendFormat("%d\r\n",tepsend.length());
	nSendStr += nMidStr.GetString();
	nSendStr += m_sendendHeadstr.GetString();
	nSendStr += cmd;
	strSendData = nSendStr.c_str();
	string teprev;
	if(CSynchronousSocket::GetRpcRes(mIp.GetString(),mPort,strSendData.GetString(),teprev,7000)>0)
	{
		CStringA  message = teprev.c_str();
		int pos = message.Find("Server:");
		if (pos >=0)
		{
			pos = message.Find('\n',pos);
			if (pos >=0)
			{
				rev = message.Mid(pos);

			}
		}
	}	
	return rev.length();
}

int CRpcCmd::SendRpc(CString strCommand,string &rev)
{

	CStringA strSendData;
	BuildSendString(strCommand,strSendData);
	
	string teprev;
	if(CSynchronousSocket::GetRpcRes(mIp.GetString(),mPort,strSendData.GetString(),teprev,7000)>0)
	{
		CStringA  message = teprev.c_str();
		int pos = message.Find("Server:");
		if (pos >=0)
		{
			pos = message.Find('\n',pos);
			if (pos >=0)
			{
				rev = message.Mid(pos);

			}
		}
	}	
	return rev.length();
}
static bool IsAllDigtal(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!isdigit(pData[i]))
			return false;
	}

	return true;
}

void CRpcCmd:: BuildSendString(const CString &cmd,CStringA &sendStr)
{
   CStringA sendStrte;
   RPCCommandToJson(cmd,sendStrte);
   RpcJosnStr = sendStrte;
   string tepsend = sendStrte.GetString();
   string nSendStr = "";
   nSendStr += m_sendPreHeadstr;
   CStringA nMidStr = "Content-Length: ";
   nMidStr.AppendFormat("%d\r\n",tepsend.length());
   nSendStr += nMidStr.GetString();
   nSendStr += m_sendendHeadstr.GetString();
   nSendStr += sendStrte;
   sendStr = nSendStr.c_str();
}

void CRpcCmd::RPCCommandToJson(const CString& strRPCCommand,CStringA& strSendData)
{
	USES_CONVERSION;
	char* pData = W2A(A2CW((LPCSTR)strRPCCommand));
	CStringA rpcCommand(pData);

	Json::Value root;
	Json::Value arrayObj;  
	Json::Value item;

	int pos = rpcCommand.Find(" ");
	CStringA method;
	CStringA param;
	if (pos >=0)
	{
		method = rpcCommand.Left(pos);
		rpcCommand = rpcCommand.Mid(pos + 1);

	}
	else if (!rpcCommand.IsEmpty())
	{
		root["method"] = rpcCommand.GetString();
		root["params"]=arrayObj;
	}
	if (method != "")
	{
		root["method"] = method.GetString();
		pos = rpcCommand.Find(" ");
		while(pos >=0)
		{
			param = rpcCommand.Left(pos);
			param.TrimLeft();
			param.TrimRight();
			rpcCommand = rpcCommand.Mid(pos+1);
			rpcCommand = rpcCommand.TrimLeft();
			if (param.GetLength()<10&&IsAllDigtal(param))
			{
				root["params"].append(atoi(param));
			}
			else
			{
				root["params"].append(param.GetString());
			}
			pos = rpcCommand.Find(" ");
		}

		if (IsAllDigtal(rpcCommand) && rpcCommand.GetLength() != 12)
		{
			INT64 param;
			sscanf(rpcCommand,"%lld",&param);
			root["params"].append(atoi(rpcCommand));
		}
		else
		{
			if (rpcCommand == "true")
			{
				root["params"].append(true);
			}
			else if (rpcCommand == "false")
			{
				root["params"].append(false);
			}
			else
			{
				root["params"].append(rpcCommand.GetString());
			}

		}
	}

	strSendData = root.toStyledString().c_str();
}
