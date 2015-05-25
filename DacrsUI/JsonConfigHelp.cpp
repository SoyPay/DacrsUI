#include "StdAfx.h"
#include "JsonConfigHelp.h"

IMPLEMENT_SINGLETON(CJsonConfigHelp)
void CJsonConfigHelp::Init()
{

}

void CJsonConfigHelp::ReadJsonConfig(const CString& strFilePath)
{
	ifstream ifs;
	ifs.open(strFilePath,ios::in);
	if (!ifs.is_open())
	{
		return ;
	}
	//ifs.read()
	Json::Reader reader;  
	Json::Value root;  
	if (reader.parse(ifs, root,false))
	{
		ReadScriptCfgData(root);
		ReadDarkCoinCfgData(root);
		ReadP2PCfgData(root);
		ReadNetParmCfgData(root);
		ReadLogParamCfg(root);
	}
	ifs.close();
}

void CJsonConfigHelp::ReadMainCfgData(const Json::Value& root)
{
	Json::Value mainCfg = root["mainconfig"];
	ASSERT(!mainCfg.isNull());	
	m_MainCfg.IntIsChinese=mainCfg["chinese"].asInt();
	m_MainCfg.bStartServer = mainCfg["startserver"].asBool();
	m_MainCfg.strServerCfgName = mainCfg["server_config_name"].asString();
}

void CJsonConfigHelp::ReadSesureTradeCfgData(const Json::Value& root)
{
	Json::Value sesureTrade = root["sesuretrade"];
	ASSERT(!sesureTrade.isNull());

	m_SesureTradeCfg.strAddr = sesureTrade["signaddr"].asString();
	m_SesureTradeCfg.strBuyerID = sesureTrade["buyerid"].asString();
	m_SesureTradeCfg.strSellerID = sesureTrade["sellerid"].asString();
	m_SesureTradeCfg.strArID = sesureTrade["arid"].asString();
	m_SesureTradeCfg.nTxFee = sesureTrade["txfee"].asInt();
	m_SesureTradeCfg.nHeight = sesureTrade["height"].asInt();
	m_SesureTradeCfg.nDeposite = sesureTrade["deposite"].asInt();
	m_SesureTradeCfg.nPay = sesureTrade["pay"].asInt();
	m_SesureTradeCfg.nFine = sesureTrade["fine"].asInt();
	m_SesureTradeCfg.nArFee = sesureTrade["arfee"].asInt();
	m_SesureTradeCfg.nMaxFine = sesureTrade["maxfine"].asInt();
	m_SesureTradeCfg.strBinPath = sesureTrade["binpath"].asString();
}

void CJsonConfigHelp::ReadDarkCoinCfgData(const Json::Value& root)
{
	Json::Value darkcoin = root["darkcoin"];
	ASSERT(!darkcoin.isNull());
	m_DarkCfg.SendBuyernFee= darkcoin["SendBuyernFee"].asInt64();
	m_DarkCfg.SendSellernFee = darkcoin["SendSellernFee"].asInt64();
	m_DarkCfg.SendBuyerConfirednFee = darkcoin["SendBuyerConfirednFee"].asInt64();
	m_DarkCfg.SendBuyerCancelnFee = darkcoin["SendBuyerCancelnFee"].asInt64();
}

void CJsonConfigHelp::ReadP2PCfgData(const Json::Value& root)
{
	Json::Value p2pbet = root["p2pbet"];
	ASSERT(!p2pbet.isNull());
	m_P2PBetCfg.SendBetFee = p2pbet["SendBetFee"].asInt64();
	m_P2PBetCfg.AcceptBetnFee = p2pbet["AcceptBetnFee"].asInt64();
	m_P2PBetCfg.OpenBetnFee = p2pbet["OpenBetnFee"].asInt64();
	m_P2PBetCfg.GetAppAmountnFee = p2pbet["GetAppAmountnFee"].asInt64();
	m_P2PBetCfg.GetRechangeFee = p2pbet["GetRechangeFee"].asInt64();

}


void CJsonConfigHelp::ReadAnonymCfgData(const Json::Value& root)
{
	Json::Value anonym = root["anonym"];
	ASSERT(!anonym.isNull());
	m_AnonymCfg.strBinPath = anonym["binpath"].asString();
	m_AnonymCfg.strSendID = anonym["sendid"].asString();
	m_AnonymCfg.strRecvID1 = anonym["recvid1"].asString();
	m_AnonymCfg.strRecvID2 = anonym["recvid2"].asString();
	m_AnonymCfg.nFee = anonym["txfee"].asInt();
	m_AnonymCfg.nHeight = anonym["height"].asInt();
	m_AnonymCfg.nMoney = anonym["money"].asInt();
}
void CJsonConfigHelp::ReadScriptCfgData(const Json::Value& root){
	Json::Value script = root["script"];
	ASSERT(!script.isNull());
	m_Scriptid.strScriptBetid = script["betscript"].asCString();
	m_Scriptid.strSrcriptDarkid = script["darkscript"].asCString();
	m_Scriptid.m_ipoScritptid = script["iposcript"].asCString();
	
	
}
void CJsonConfigHelp::GetMainCfgData(CMainCfg& mainCfg)
{
	mainCfg = m_MainCfg;
}

void CJsonConfigHelp::GetDarkCfgData(CDarkTxCfg& darkCfg)
{
	darkCfg = m_DarkCfg;
}

void CJsonConfigHelp::GetAnonymCfgData(CAnonmyCfg& anonymCfg)
{
	anonymCfg = m_AnonymCfg;
}

void CJsonConfigHelp::GetSesureTradeCfgData(CSesureTradeCfg& sesureCfg)
{
	sesureCfg = m_SesureTradeCfg;
}

void CJsonConfigHelp::GetP2PBetCfgData(CP2PBetCfg& p2pCfg)
{
	p2pCfg = m_P2PBetCfg;
}
void CJsonConfigHelp::GetScriptCfgData(CScriptCfg& scriptCfg){
	scriptCfg =m_Scriptid;
}
//void CJsonConfigHelp::AddString(const Json::Value& root,CAutoComplete &m_comboxinput){
//	Json::Value rpccommand = root["rpccommand"];
//	ASSERT(!rpccommand.isNull());
//	m_comboxinput.GetStringList().Add(rpccommand["getinfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["help"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["stop"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getnetworkinfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["addnode"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getaddednodeinfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getconnectioncount"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getnettotals"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getpeerinfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["ping"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getblockchaininfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getbestblockhash"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getblockcount"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getblock"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getblockhash"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getdifficulty"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getrawmempool"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["verifychain"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getmininginfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getnetworkhashps"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["submitblock"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["verifymessage"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["backupwallet"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["dumpprivkey"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["dumpwallet"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["encryptwallet"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getaccountinfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getnewaddress"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["gettxdetail"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["listunconfirmedtx"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getwalletinfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["importprivkey"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["prepareforcoolmining"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["importwallet"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["listaddr"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["listtx"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["registaccounttx"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["createcontracttx"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["registerapptx"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["settxfee"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["walletlock"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["walletpassphrasechange"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["walletpassphrase"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["setgenerate"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["listapp"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["generateblock"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["listtxcache"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getscriptdata"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["signmessage"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["sendtoaddress"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["sendtoaddresswithfee"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getbalance"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["sendtoaddressraw"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["submittx"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["createcontracttxraw"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["registerscripttxraw"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["sigstr"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getappaccinfo"].asCString());
//	m_comboxinput.GetStringList().Add(rpccommand["getappkeyvalue"].asCString());
//}
//void CJsonConfigHelp::AddItemString(const CString& strFilePath,CAutoComplete &m_comboxinput){
//
//	ifstream ifs;
//	ifs.open(strFilePath,ios::in);
//	if (!ifs.is_open())
//	{
//		return ;
//	}
//	//ifs.read()
//	Json::Reader reader;  
//	Json::Value root;  
//	if (reader.parse(ifs, root,false))
//	{
//		AddString(root,m_comboxinput);
//	}
//	ifs.close();
//}

void CJsonConfigHelp::ReadNetParmCfgData(const Json::Value& root){
	Json::Value netparam = root["netparam"];
	ASSERT(!netparam.isNull());
	m_NetParam.server_ip = netparam["server_ip"].asCString();
	m_NetParam.rpc_port= netparam["rpc_port"].asCString();
	m_NetParam.server_ui_port =netparam["server_ui_port"].asCString();
	m_NetParam.rpc_user =  netparam["rpc_user"].asCString();
	m_NetParam.rpc_password = netparam["rpc_password"].asCString();
}
void CJsonConfigHelp::GetNetParamCfgData(CNetParamCfg& netparm){
	netparm =m_NetParam;
}

void CJsonConfigHelp::ReadLogParamCfg(const Json::Value &root)
{
	Json::Value logParam = root["logcfg"];
	ASSERT(!logParam.isNull());
	m_LogParamCfg.bLogFlag = (logParam["log_flag"].asInt()>0);
	Json::Value  arrayTags = logParam["tags"];
	for(int i=0; i<arrayTags.size();++i)
	{
		m_LogParamCfg.vTag.push_back(arrayTags[i]["debug"].asCString());
	}
	m_LogParamCfg.bPrintFileLine = (logParam["print_fileline"].asInt()>0);
	m_LogParamCfg.bPrinttimestamps = (logParam["print_timestamps"].asInt()>0);
	
}

void CJsonConfigHelp::GetLogParamCfg(CLogParamCfg &logCfg)
{
	logCfg = m_LogParamCfg;
}