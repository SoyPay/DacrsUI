// TxDetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TxDetailDlg.h"
#include "afxdialogex.h"


// CTxDetailDlg 对话框

IMPLEMENT_DYNAMIC(CTxDetailDlg, CDialogBase)

CTxDetailDlg::CTxDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CTxDetailDlg::IDD, pParent)
{

}

CTxDetailDlg::~CTxDetailDlg()
{
}

void CTxDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CTxDetailDlg, CDialogBase)
	ON_BN_CLICKED(IDC_CLOSE, &CTxDetailDlg::OnBnClickedClose)
END_MESSAGE_MAP()

string CTxDetailDlg::GetContacrDetail(uistruct::REVTRANSACTION_t tx)
{
	string strCommand,strShowData;
		strCommand = strprintf("%s %s","getaccountinfo" ,tx.desaddr.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData  ==""){		
			return _T("");
		}
		Json::Reader reader;  
		Json::Value root; 
	
		if (reader.parse(strShowData, root)) {		

			if (strShowData.find("RegID") >0)
			{
				tx.desregid = root["RegID"].asString();
			}
		}
		
			string txdetail ="";
		string nValue = tx.Contract;
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);

		string strContract = CSoyPayHelp::getInstance()->HexStr(vTemp);
		txdetail +=strprintf("%s:   %d\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT" ,theApp.gsLanguage) ,strContract.c_str() );


	if (!strcmp(tx.desregid.c_str(),theApp.m_betScritptid.c_str()))
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

			string reward = "";
			if (Openbet.dhash[32] == 1)
			{
				reward= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_BROTHER" ,theApp.gsLanguage);
			}else
			{
				reward= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SISTER" ,theApp.gsLanguage);
			}

			txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage) ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GUESS_SISTER" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage) ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LOTTERY" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LOTTERY_CARDS" ,theApp.gsLanguage),reward.c_str() );

			std::vector<unsigned char> vTemp;
			vTemp.assign(Openbet.txhash,Openbet.txhash+sizeof(Openbet.txhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_TRADID" ,theApp.gsLanguage) ,txhash.c_str() );
			// temp = CSoyPayHelp::getInstance()->HexStr(Openbet.accepthash );

			 vTemp.assign(Openbet.accepthash,Openbet.accepthash+sizeof(Openbet.accepthash));
			 reverse(vTemp.begin(),vTemp.end());
			 txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ORDERS_TRADID" ,theApp.gsLanguage) ,txhash.c_str() );
			//OpenBetRecord(vTemp,transcion);
		}else if (vTemp[0] == TX_ACCEPTBET)
		{
			//AcceptBetRecord(vTemp,transcion);
			if(vTemp.size()==0)
				return _T("");;
			ACCEPT_DATA acceptcbet;
			memset(&acceptcbet , 0 , sizeof(ACCEPT_DATA));
			memcpy(&acceptcbet, &vTemp[0], sizeof(ACCEPT_DATA));

			string guess = "";
			if (acceptcbet.data == 1)
			{
				guess= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SISTER" ,theApp.gsLanguage);
			}else
			{
				guess= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_BROTHER" ,theApp.gsLanguage);
			}

			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage) ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GUESS_SISTER" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ORDERS" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_QUIZ_CONTENT" ,theApp.gsLanguage) ,guess.c_str() );

			std::vector<unsigned char> vTemp;
			vTemp.assign(acceptcbet.txhash,acceptcbet.txhash+sizeof(acceptcbet.txhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_TRADID" ,theApp.gsLanguage) ,txhash.c_str() );
			txdetail+=strprintf("%s:     %.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_MONEY" ,theApp.gsLanguage) ,(acceptcbet.money*1.0)/COIN);
		}else if (vTemp[0] == TX_SENDBET)
		{
			if(vTemp.size()==0)
				return _T("");;

			SEND_DATA sendbetdata;
			memset(&sendbetdata,0,sizeof(SEND_DATA));
			memcpy(&sendbetdata, &vTemp[0],sizeof(SEND_DATA));
	
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GUESS_SISTER" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_QUIZ" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:%.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_MONEY" ,theApp.gsLanguage),(sendbetdata.money*1.0)/COIN );
		}else if (vTemp[0] == 0xff)
		{
			if (vTemp[1] == 0x02)    /// 充值
			{
				txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GUESS_SISTER" ,theApp.gsLanguage) );
				txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_RECHARGE" ,theApp.gsLanguage) );
			}else if (vTemp[1] == 0x01|| vTemp[1] == 0x03)   /// 提现
			{
				txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GUESS_SISTER" ,theApp.gsLanguage) );
				txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WITHDRAWALS" ,theApp.gsLanguage) );
				if (vTemp.size() == sizeof(APPACC))
				{
					APPACC drawtx;
					memcpy(&drawtx, &vTemp[0],sizeof(APPACC));
					txdetail+=strprintf("%s:  %.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WITHDRAWALS_MONEY" ,theApp.gsLanguage) ,(drawtx.money*1.0)/COIN);
				}
			}
		}
	}else  if (!strcmp(tx.desregid.c_str(),theApp.m_redPacketScriptid.c_str())){ //// 接龙红包	
		string nValue = tx.Contract;

		if (vTemp[0] == TX_COMM_SENDREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(SEND_DATA));
	
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_REDENVELOPE" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_REDENVELOPE" ,theApp.gsLanguage) );

			txdetail+=strprintf("%s:%.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_REDENVELOPE_MONEY" ,theApp.gsLanguage),(redpacket.money*1.0)/COIN);
			txdetail+=strprintf("%s:%d\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_REDENVELOPE_NUM" ,theApp.gsLanguage),redpacket.number);
			string message = "";
			message+=strprintf("%s",redpacket.message);
			if (message != "")
			{
				
				txdetail+=strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_REDENVELOPE_GDY" ,theApp.gsLanguage),redpacket.message);
			}
			//SendRePacketCommtRecord(vTemp,transcion);

		}else if (vTemp[0] == TX_COMM_ACCEPTREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			ACCEPT_RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(ACCEPT_RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(ACCEPT_RED_PACKET));


			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_REDENVELOPE" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_PT_REDENVELOPE" ,theApp.gsLanguage) );

			std::vector<unsigned char> vTemp;
			vTemp.assign(redpacket.redhash,redpacket.redhash+sizeof(redpacket.redhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_PT_REDENVELOPE_ID" ,theApp.gsLanguage) ,txhash.c_str());
			//AcceptRePacketCommtRecord(vTemp,transcion);
		}else if (vTemp[0] == TX_SPECIAL_SENDREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(SEND_DATA));

			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_REDENVELOPE" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_JL_REDENVELOPE" ,theApp.gsLanguage) );

			txdetail+=strprintf("%s:  %.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_JL_REDENVELOPE_MONEY" ,theApp.gsLanguage) ,(redpacket.money*1.0)/COIN);
			txdetail+=strprintf("%s:  %d\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_JL_REDENVELOPE_NUM" ,theApp.gsLanguage),redpacket.number);
			//SendRePacketSpecailRecord(vTemp,transcion);
		}else if (vTemp[0] == TX_SPECIAL_ACCEPTREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			ACCEPT_RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(ACCEPT_RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(ACCEPT_RED_PACKET));

	
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_REDENVELOPE" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_JL_REDENVELOPE" ,theApp.gsLanguage));

			std::vector<unsigned char> vTemp;
			vTemp.assign(redpacket.redhash,redpacket.redhash+sizeof(redpacket.redhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_JL_REDENVELOPE_ID" ,theApp.gsLanguage) ,txhash.c_str());
		}else if (vTemp[0] == 0xff)
		{
			if (vTemp[1] == 0x02)    /// 充值
			{
				txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_REDENVELOPE" ,theApp.gsLanguage) );
				txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_RECHARGE" ,theApp.gsLanguage) );
			}else if (vTemp[1] == 0x01 || vTemp[1] == 0x03)   /// 提现
			{
				txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_REDENVELOPE" ,theApp.gsLanguage) );
				txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WITHDRAWALS" ,theApp.gsLanguage) );
				if (vTemp.size() == sizeof(APPACC))
				{
					APPACC draw;
					memcpy(&draw, &vTemp[0],sizeof(APPACC));
					txdetail+=strprintf("%s:  %.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WITHDRAWALS_MONEY" ,theApp.gsLanguage),(draw.money*1.0)/COIN);
				}
			}
		}
	}else if (!strcmp(tx.desregid.c_str(),theApp.m_ipoScritptid.c_str()))
	{

		if (vTemp[1] == 0x02)    /// 充值
		{
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_IPO_CURRENCY" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage) ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_RECHARGE" ,theApp.gsLanguage) );
		}else if (vTemp[1] == 0x01 || vTemp[1] == 0x03)   /// 提现
		{
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_APP_NAME" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_IPO_CURRENCY" ,theApp.gsLanguage) );
			txdetail+=strprintf("%s:  %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WITHDRAWALS" ,theApp.gsLanguage) );
			if (vTemp.size() == sizeof(APPACC))
			{
				APPACC draw;
				memcpy(&draw, &vTemp[0],sizeof(APPACC));
				txdetail+=strprintf("%s:  %.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WITHDRAWALS_MONEY" ,theApp.gsLanguage),(draw.money*1.0)/COIN);
			}
		}
	}
	return txdetail;
}
// CTxDetailDlg 消息处理程序
void CTxDetailDlg::ShowTxDetail(CString jsontx)
{
	if (jsontx == _T(""))
	{
		string strShowData;
		strShowData+=strprintf("%s" ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_NOT" ,theApp.gsLanguage)) ;
		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(strShowData.c_str());
		return;
	}
	//string txdetail = tx.ToJson();
	uistruct::REVTRANSACTION_t tx;
	tx.JsonToStruct(jsontx.GetString());
	string txdetail,strShowData;

	string txtype = tx.txtype;
	if (!strcmp(txtype.c_str(),"REWARD_TX"))
	{
		strShowData+=strprintf("%s" ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_MIN_REWARD_TRAN" ,theApp.gsLanguage)) ;
	}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
	{
		strShowData+=strprintf("%s" ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_REG_ACCOUNT" ,theApp.gsLanguage)) ;
	}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
	{
		strShowData+=strprintf("%s" ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRANSF_TRANSA" ,theApp.gsLanguage)) ;
	}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
	{
		strShowData+=strprintf("%s" ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT_RANSA" ,theApp.gsLanguage)) ;
	}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
	{
		strShowData+=strprintf("%s" ,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_REGISTERED_TRADE" ,theApp.gsLanguage)) ;
	}
	txdetail=strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),strShowData.c_str());
	txdetail+=strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ID" ,theApp.gsLanguage),tx.txhash);
	txdetail+=strprintf("%s:   %d\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_VERSION_NUMBER" ,theApp.gsLanguage),tx.ver);
	txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SOURCE_ADDRESS" ,theApp.gsLanguage),tx.addr.c_str());
	if (tx.pubkey != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_PUBLIC_KEY" ,theApp.gsLanguage),tx.pubkey.c_str());
	}
	if (tx.miner_pubkey != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_COLD_PUBLICKEY" ,theApp.gsLanguage),tx.miner_pubkey.c_str());
	}
	if (tx.fees != 0)
	{
		txdetail+=strprintf("%s:   %.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TIP" ,theApp.gsLanguage),tx.fees*COIN);
	}
	if (tx.height != 0)
	{
		int height = tx.height +250; 
		txdetail+=strprintf("%s:   %d\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_TIMEOUT_HEIGHT" ,theApp.gsLanguage),height);
	}
	if (tx.desaddr != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_DESTINATION_ADDRESS" ,theApp.gsLanguage),tx.desaddr.c_str() );
	}
	if (tx.money != 0)
	{
		txdetail+=strprintf("%s:   %.8f\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRADMONEY" ,theApp.gsLanguage),tx.money*COIN);
	}
	if (tx.Contract != "")
	{
		//txdetail.AppendFormat(_T("合约内容:   %d\r\n\r\n"),tx.Contract.c_str() );
		txdetail+=strprintf("%s",GetContacrDetail(tx).c_str());
		
	}

	if (tx.confirmedHeight != 0)
	{
		txdetail+=strprintf("%s:   %d\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_HEIGHT" ,theApp.gsLanguage),tx.confirmedHeight );
	}
	if (tx.confirmedtime != 0)
	{
		SYSTEMTIME curTime = UiFun::Time_tToSystemTime(tx.confirmedtime);
		txdetail+=strprintf("%s:   %04d-%02d-%02d %02d:%02d:%02d\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}
	if (tx.blockhash != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_BLOCK" ,theApp.gsLanguage),tx.blockhash.c_str());
	}
	if ( theApp.blocktipheight != 0 && tx.confirmedHeight != 0)
	{
		unsigned int quredCount = theApp.blocktipheight - tx.confirmedHeight;
		if (quredCount >=0)
		{
			txdetail+=strprintf("%s:  %d\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_NUM" ,theApp.gsLanguage),quredCount );
		}	
	}

	GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail.c_str());
}

BOOL CTxDetailDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(100, _T("微软雅黑"));
	m_headText.SetTextColor(RGB(255,255,255));	

	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	CRect rect ;
	m_rBtnClose.GetClientRect(rect);

	RECT ret;
	GetWindowRect(&ret);
	m_rBtnClose.SetWindowPos(NULL ,(ret.right-ret.left)-rect.Width() , 2 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_CONFIRM" ,theApp.gsLanguage)) ;
	//m_rBtnOk.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();
	// TODO:  在此添加额外的初始化
	ShowTxDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CTxDetailDlg::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogBase::OnCancel();
}
