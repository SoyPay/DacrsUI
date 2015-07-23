// TxDetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TxDetailDlg.h"
#include "afxdialogex.h"


// CTxDetailDlg 对话框

IMPLEMENT_DYNAMIC(CTxDetailDlg, CDialogEx)

CTxDetailDlg::CTxDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTxDetailDlg::IDD, pParent)
{

}

CTxDetailDlg::~CTxDetailDlg()
{
}

void CTxDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTxDetailDlg, CDialogEx)
END_MESSAGE_MAP()

CString CTxDetailDlg::GetContacrDetail(uistruct::REVTRANSACTION_t tx)
{
	CString strCommand,strShowData;
		strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,tx.desaddr.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData  ==_T("")){		
			return _T("");
		}
		Json::Reader reader;  
		Json::Value root; 
	
		if (reader.parse(strShowData.GetString(), root)) {		

			if (strShowData.Find("RegID") >0)
			{
				tx.desregid.Format(_T("%s"),root["RegID"].asCString());
			}
		}
		
			CString txdetail =_T("");
		string nValue = tx.Contract;
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);

		string strContract = CSoyPayHelp::getInstance()->HexStr(vTemp);
		txdetail.AppendFormat(_T("合约内容:   %d\r\n\r\n"),strContract.c_str() );


	if (!strcmp(tx.desregid,theApp.m_betScritptid))
	{
		string nValue = tx.Contract;
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
		if (vTemp[0] == TX_OPENBET)
		{
			if(vTemp.size()==0)
				return _T("");
			OPEN_DATA Openbet;
			memset(&Openbet , 0 , sizeof(OPEN_DATA));
			memcpy(&Openbet, &vTemp[0], sizeof(OPEN_DATA));

			CString reward = _T("");
			if (Openbet.dhash[32] == 1)
			{
				reward.Format(_T("%s"),"妹");
			}else
			{
				reward.Format(_T("%s"),"哥");
			}

			txdetail.AppendFormat(_T("应用名称:   %s\r\n\r\n"),"猜你妹" );
			txdetail.AppendFormat(_T("合约类型:   %s\r\n\r\n"),"开奖" );
			txdetail.AppendFormat(_T("开奖底牌:%s\r\n\r\n"),reward );

			std::vector<unsigned char> vTemp;
			vTemp.assign(Openbet.txhash,Openbet.txhash+sizeof(Openbet.txhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail.AppendFormat(_T("发起竞猜交易ID:   %s\r\n\r\n"),txhash.c_str() );
			// temp = CSoyPayHelp::getInstance()->HexStr(Openbet.accepthash );

			 vTemp.assign(Openbet.accepthash,Openbet.accepthash+sizeof(Openbet.accepthash));
			 reverse(vTemp.begin(),vTemp.end());
			 txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail.AppendFormat(_T("接单交易ID:   %s\r\n\r\n"),txhash.c_str() );
			//OpenBetRecord(vTemp,transcion);
		}else if (vTemp[0] == TX_ACCEPTBET)
		{
			//AcceptBetRecord(vTemp,transcion);
			if(vTemp.size()==0)
				return _T("");;
			ACCEPT_DATA acceptcbet;
			memset(&acceptcbet , 0 , sizeof(ACCEPT_DATA));
			memcpy(&acceptcbet, &vTemp[0], sizeof(ACCEPT_DATA));

			CString guess = _T("");
			if (acceptcbet.data == 1)
			{
				guess.Format(_T("%s"),"妹");
			}else
			{
				guess.Format(_T("%s"),"哥");
			}

			txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"猜你妹" );
			txdetail.AppendFormat(_T("合约类型:   %s\r\n\r\n"),"接单" );
			txdetail.AppendFormat(_T("竞猜内容: %s\r\n\r\n"),guess );

			std::vector<unsigned char> vTemp;
			vTemp.assign(acceptcbet.txhash,acceptcbet.txhash+sizeof(acceptcbet.txhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail.AppendFormat(_T("发起竞猜交易ID:   %s\r\n\r\n"),txhash.c_str() );
			txdetail.AppendFormat(_T("金额:     %.8f\r\n\r\n"),(acceptcbet.money*1.0)/COIN);
		}else if (vTemp[0] == TX_SENDBET)
		{
			if(vTemp.size()==0)
				return _T("");;

			SEND_DATA sendbetdata;
			memset(&sendbetdata,0,sizeof(SEND_DATA));
			memcpy(&sendbetdata, &vTemp[0],sizeof(SEND_DATA));
	
			txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"猜你妹" );
			txdetail.AppendFormat(_T("合约类型:   %s\r\n\r\n"),"发起竞猜" );
			txdetail.AppendFormat(_T("金额:%.8f\r\n\r\n"),(sendbetdata.money*1.0)/COIN );
		}else if (vTemp[0] == 0xff)
		{
			if (vTemp[1] == 0x02)    /// 充值
			{
				txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"猜你妹" );
				txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"充值" );
			}else if (vTemp[1] == 0x01)   /// 提现
			{
				txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"猜你妹" );
				txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"提现" );
			}
		}
	}else  if (!strcmp(tx.desregid,theApp.m_redPacketScriptid)){ //// 接龙红包	
		string nValue = tx.Contract;

		if (vTemp[0] == TX_COMM_SENDREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(SEND_DATA));

			
			txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"抢红包" );
			txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"发送普通红包" );

			txdetail.AppendFormat(_T("发普通红包金额:%.8f\r\n\r\n"),(redpacket.money*1.0)/COIN);
			txdetail.AppendFormat(_T("发普通红包个数:%d\r\n\r\n"),redpacket.number);
			CString message = _T("");
			message.Format(_T("%s"),redpacket.message);
			if (message != _T(""))
			{
				
				txdetail.AppendFormat(_T("发普通红包广告语: %s\r\n\r\n"),redpacket.message);
			}
			//SendRePacketCommtRecord(vTemp,transcion);

		}else if (vTemp[0] == TX_COMM_ACCEPTREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			ACCEPT_RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(ACCEPT_RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(ACCEPT_RED_PACKET));


			txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"抢红包" );
			txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"抢普通红包" );

			std::vector<unsigned char> vTemp;
			vTemp.assign(redpacket.redhash,redpacket.redhash+sizeof(redpacket.redhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail.AppendFormat(_T("普通红包ID: %s\r\n\r\n"),txhash.c_str());
			//AcceptRePacketCommtRecord(vTemp,transcion);
		}else if (vTemp[0] == TX_SPECIAL_SENDREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(SEND_DATA));

			txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"抢红包" );
			txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"发送接龙红包" );

			txdetail.AppendFormat(_T("发接龙红包金额:  %.8f\r\n\r\n"),(redpacket.money*1.0)/COIN);
			txdetail.AppendFormat(_T("发接龙红包个数:  %d\r\n\r\n"),redpacket.number);
			//SendRePacketSpecailRecord(vTemp,transcion);
		}else if (vTemp[0] == TX_SPECIAL_ACCEPTREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			ACCEPT_RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(ACCEPT_RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(ACCEPT_RED_PACKET));

	
			txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"抢红包" );
			txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"抢接龙红包" );

			std::vector<unsigned char> vTemp;
			vTemp.assign(redpacket.redhash,redpacket.redhash+sizeof(redpacket.redhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail.AppendFormat(_T("接龙红包ID:   %s\r\n\r\n"),txhash.c_str());
		}else if (vTemp[0] == 0xff)
		{
			if (vTemp[1] == 0x02)    /// 充值
			{
				txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"抢红包" );
				txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"充值" );
			}else if (vTemp[1] == 0x01)   /// 提现
			{
				txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"抢红包" );
				txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"提现" );
			}
		}
	}else if (!strcmp(tx.desregid,theApp.m_ipoScritptid))
	{
		txdetail.AppendFormat(_T("应用名称:  %s\r\n\r\n"),"ipo领币" );
		txdetail.AppendFormat(_T("合约类型:  %s\r\n\r\n"),"提现" );
	}
	return txdetail;
}
// CTxDetailDlg 消息处理程序
void CTxDetailDlg::ShowTxDetail(CString jsontx)
{
	if (jsontx == _T(""))
	{
		CString strShowData;
		strShowData.AppendFormat(_T("%s") ,_T("此交易不存在")) ;
		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(strShowData);
		return;
	}
	//string txdetail = tx.ToJson();
	uistruct::REVTRANSACTION_t tx;
	tx.JsonToStruct(jsontx.GetString());
	CString txdetail,strShowData;

	string txtype = tx.txtype;
	if (!strcmp(txtype.c_str(),"REWARD_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("挖矿奖励交易")) ;
	}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("注册账户交易")) ;
	}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("转账交易")) ;
	}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("合约交易")) ;
	}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("注册应用交易")) ;
	}
	txdetail.Format(_T("交易类型: %s\r\n\r\n"),strShowData);
	txdetail.AppendFormat(_T("交易ID: %s\r\n\r\n"),tx.txhash);
	txdetail.AppendFormat(_T("版本号:   %d\r\n\r\n"),tx.ver);
	txdetail.AppendFormat(_T("源地址:   %s\r\n\r\n"),tx.addr.c_str());
	if (tx.pubkey != "")
	{
		txdetail.AppendFormat(_T("公钥:   %s\r\n\r\n"),tx.pubkey.c_str());
	}
	if (tx.miner_pubkey != "")
	{
		txdetail.AppendFormat(_T("冷挖矿公钥:   %s\r\n\r\n"),tx.miner_pubkey.c_str());
	}
	if (tx.fees != 0)
	{
		txdetail.AppendFormat(_T("小费:   %.8f\r\n\r\n"),tx.fees*COIN);
	}
	if (tx.height != 0)
	{
		int height = tx.height +250; 
		txdetail.AppendFormat(_T("交易超时高度:   %d\r\n\r\n"),height);
	}
	if (tx.desaddr != "")
	{
		txdetail.AppendFormat(_T("目的地址:   %s\r\n\r\n"),tx.desaddr.c_str() );
	}
	if (tx.money != 0)
	{
		txdetail.AppendFormat(_T("交易金额:   %.8f\r\n\r\n"),tx.money*COIN);
	}
	if (tx.Contract != "")
	{
		//txdetail.AppendFormat(_T("合约内容:   %d\r\n\r\n"),tx.Contract.c_str() );
		txdetail.AppendFormat(_T("%s"),GetContacrDetail(tx));
		
	}

	if (tx.confirmedHeight != 0)
	{
		txdetail.AppendFormat(_T("确认高度:   %d\r\n\r\n"),tx.confirmedHeight );
	}
	if (tx.confirmedtime != 0)
	{
		SYSTEMTIME curTime = UiFun::Time_tToSystemTime(tx.confirmedtime);
		txdetail.AppendFormat(_T("确认时间:   %04d-%02d-%02d %02d:%02d:%02d\r\n\r\n"),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}
	if (tx.blockhash != "")
	{
		txdetail.AppendFormat(_T("确认blockHash:   %s\r\n\r\n"),tx.blockhash.c_str());
	}
	if ( theApp.blocktipheight != 0)
	{
		unsigned int quredCount = theApp.blocktipheight - tx.confirmedHeight;
		if (quredCount >=0)
		{
			txdetail.AppendFormat(_T("确认数:  %d\r\n\r\n"),quredCount );
		}	
	}

	GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail);
}

BOOL CTxDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowTxDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
