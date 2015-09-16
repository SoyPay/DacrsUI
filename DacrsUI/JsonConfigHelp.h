#pragma once
#include "CommonStruct.h"
#include "AutoComplete.h"

class CScriptCfg
{
public:
	string	strScriptBetid;
	string	strSrcriptDarkid;
	string	m_ipoScritptid;
	string m_redpacketScriptid;
};

class CNewestScriptCfg
{
public:
	string	strNewScriptBetid;
	string	strNewSrcriptRedPacektid;
	string	strNewipoScritptid;
};

class CMainCfg 
{
public:
	CMainCfg()
	{
		bStartServer = false;
		IntIsChinese = -1;
	}

	bool	bStartServer;
	int	 IntIsChinese;
	
	string	strServerCfgName;
};

class CDarkTxCfg
{
public:
	INT64	SendBuyernFee;
	INT64	SendSellernFee;
	INT64	SendBuyerConfirednFee;
	INT64	SendBuyerCancelnFee;
};

class CAnonmyCfg
{
public:
	string	strBinPath;
	string	strSendID;
	string	strRecvID1;
	string	strRecvID2;
	int		nMoney;
	int		nHeight;
	int		nFee;
};

class CSesureTradeCfg
{
public:
	string	strBinPath;
	string strAddr;
	string strBuyerID;
	string strSellerID;
	string strArID;
	int nTxFee;
	int nHeight;
	int nDeposite;
	int nPay;
	int nMaxFine;
	int nFine;
	int nArFee;
};

class CP2PBetCfg
{
public:
	INT64	SendBetFee;
	INT64	AcceptBetnFee;
	INT64	OpenBetnFee;
	INT64	GetAppAmountnFee;
	INT64   GetRechangeFee;
};
class CRedPacketCfg
{
public:
	INT64	SendRedPacketCommFee;
	INT64	AcceptRedPacketCommFee;
	INT64	SendRedPacketSpecailFee;
	INT64	AcceptRedPacketSpecailFee;
};

class CRedPacketStepCfg
{
public:
	INT64	SendRedPacketCommStep;
	INT64	AcceptRedPacketCommStep;
	INT64	SendRedPacketSpecailStep;
	INT64	AcceptRedPacketSpecailStep;
};

class CP2PBetStepCfg
{
public:
	INT64	SendBetStep;
	INT64	AcceptBetnStep;
	INT64	OpenBetnStep;
	INT64	GetAppAmountnStep;
	INT64   GetRechangeStep;
};

class CNetParamCfg
{
public:
	CString	server_ip;
	CString	rpc_port;
	CString server_ui_port;
	CString rpc_user;
	CString rpc_password;
};

class CLogParamCfg
{
public:
	vector<CString> vTag;
	BOOL   bPrintFileLine;
	BOOL   bPrinttimestamps;	
	BOOL   bLogFlag;
	int    nMaxLogSize;         //日志文件最大值（单位M）
public:
	CLogParamCfg():bPrintFileLine(TRUE), bPrinttimestamps(TRUE), bLogFlag(TRUE), nMaxLogSize(100){
		vTag.clear();
	}
	~CLogParamCfg(){};
};

class CJsonConfigHelp
{
	DEFINE_SINGLETON(CJsonConfigHelp)
public:
	void Init();
	void ReadJsonConfig(const string& strFilePath);
	void GetMainCfgData(CMainCfg& mainCfg);
	void GetDarkCfgData(CDarkTxCfg& darkCfg);
	void GetAnonymCfgData(CAnonmyCfg& anonymCfg);
	void GetSesureTradeCfgData(CSesureTradeCfg& sesureCfg);
	void GetP2PBetCfgData(CP2PBetCfg& p2pCfg);
	void GetScriptCfgData(CScriptCfg& scriptCfg);
	void GetNetParamCfgData(CNetParamCfg& netparm);
	void GetLogParamCfg(CLogParamCfg &logCfg);
	void GetRedPacketCfgData(CRedPacketCfg &redpacketcfg);
	void GetClosConfig(int &closeconf);
	void GetPopTipsConfig(bool &flag);
	void AddItemString(const CString& strFilePath,CAutoComplete &m_comboxinput);
	string  GetConfigRootStr(const string& strFilePath);
	void GetNewestScriptData(CNewestScriptCfg &newScript);
	void GetRedPacketStep(CRedPacketStepCfg &redPackestep);
	void GetP2pBetStep(CP2PBetStepCfg &p2pbetstep);
private:
	void ReadMainCfgData(const Json::Value& root);
	void ReadSesureTradeCfgData(const Json::Value& root);
	void ReadDarkCoinCfgData(const Json::Value& root);
	void ReadP2PCfgData(const Json::Value& root);
	void ReadAnonymCfgData(const Json::Value& root);
	void ReadScriptCfgData(const Json::Value& root);
	void ReadNetParmCfgData(const Json::Value& root);
	void ReadLogParamCfg(const Json::Value &root);
	void ReadRedPacketCfgData(const Json::Value &root);
	void ReadCloseCfgData(const Json::Value &root);
	void ReadNewestCfgData(const Json::Value &root);
	void ReadRedPacketStep(const Json::Value &root);
	void ReadP2pBetStep(const Json::Value &root);
	void AddString(const Json::Value& root,CAutoComplete &m_comboxinput);
public:
	void ModifyAppFeeCfgData( Json::Value& root,const CString &LeaderKey,const CString &Key,const CString &KeyValue);

private:
	BOOL			m_bInvalid;
	CMainCfg		m_MainCfg;
	CDarkTxCfg		m_DarkCfg;
	CAnonmyCfg		m_AnonymCfg;
	CP2PBetCfg		m_P2PBetCfg;
	CSesureTradeCfg	m_SesureTradeCfg;
	CScriptCfg      m_Scriptid;
	CNetParamCfg    m_NetParam;
	CLogParamCfg     m_LogParamCfg;
	CRedPacketCfg    m_RedPacketCfg;
	int              m_close;                     /// 0 提示关闭对话框 1 不提示关闭对话框并且隐藏 2不提示关闭对话框并且关闭钱包
	CNewestScriptCfg m_newScript;
	CRedPacketStepCfg m_redPackestep;
	CP2PBetStepCfg    m_p2pbetstep;
	bool              m_poptip;
};

