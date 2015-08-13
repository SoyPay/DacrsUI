// TxDetailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TxDetailDlg.h"
#include "afxdialogex.h"


// CTxDetailDlg �Ի���

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
		txdetail +=strprintf("��Լ����:   %d\r\n\r\n",strContract.c_str() );


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
				reward="��";
			}else
			{
				reward="��";
			}

			txdetail+=strprintf("Ӧ������:   %s\r\n\r\n","������" );
			txdetail+=strprintf("��Լ����:   %s\r\n\r\n","����" );
			txdetail+=strprintf("��������:%s\r\n\r\n",reward.c_str() );

			std::vector<unsigned char> vTemp;
			vTemp.assign(Openbet.txhash,Openbet.txhash+sizeof(Openbet.txhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("���𾺲½���ID:   %s\r\n\r\n",txhash.c_str() );
			// temp = CSoyPayHelp::getInstance()->HexStr(Openbet.accepthash );

			 vTemp.assign(Openbet.accepthash,Openbet.accepthash+sizeof(Openbet.accepthash));
			 reverse(vTemp.begin(),vTemp.end());
			 txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("�ӵ�����ID:   %s\r\n\r\n",txhash.c_str() );
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
				guess="��";
			}else
			{
				guess="��";
			}

			txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","������" );
			txdetail+=strprintf("��Լ����:   %s\r\n\r\n","�ӵ�" );
			txdetail+=strprintf("��������: %s\r\n\r\n",guess.c_str() );

			std::vector<unsigned char> vTemp;
			vTemp.assign(acceptcbet.txhash,acceptcbet.txhash+sizeof(acceptcbet.txhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("���𾺲½���ID:   %s\r\n\r\n",txhash.c_str() );
			txdetail+=strprintf("���:     %.8f\r\n\r\n",(acceptcbet.money*1.0)/COIN);
		}else if (vTemp[0] == TX_SENDBET)
		{
			if(vTemp.size()==0)
				return _T("");;

			SEND_DATA sendbetdata;
			memset(&sendbetdata,0,sizeof(SEND_DATA));
			memcpy(&sendbetdata, &vTemp[0],sizeof(SEND_DATA));
	
			txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","������" );
			txdetail+=strprintf("��Լ����:   %s\r\n\r\n","���𾺲�" );
			txdetail+=strprintf("���:%.8f\r\n\r\n",(sendbetdata.money*1.0)/COIN );
		}else if (vTemp[0] == 0xff)
		{
			if (vTemp[1] == 0x02)    /// ��ֵ
			{
				txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","������" );
				txdetail+=strprintf("��Լ����:  %s\r\n\r\n","��ֵ" );
			}else if (vTemp[1] == 0x01|| vTemp[1] == 0x03)   /// ����
			{
				txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","������" );
				txdetail+=strprintf("��Լ����:  %s\r\n\r\n","����" );
				if (vTemp.size() == sizeof(APPACC))
				{
					APPACC drawtx;
					memcpy(&drawtx, &vTemp[0],sizeof(APPACC));
					txdetail+=strprintf("���ֽ��:  %.8f\r\n\r\n",(drawtx.money*1.0)/COIN);
				}
			}
		}
	}else  if (!strcmp(tx.desregid.c_str(),theApp.m_redPacketScriptid.c_str())){ //// �������	
		string nValue = tx.Contract;

		if (vTemp[0] == TX_COMM_SENDREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(SEND_DATA));
	
			txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","�����" );
			txdetail+=strprintf("��Լ����:  %s\r\n\r\n","������ͨ���" );

			txdetail+=strprintf("����ͨ������:%.8f\r\n\r\n",(redpacket.money*1.0)/COIN);
			txdetail+=strprintf("����ͨ�������:%d\r\n\r\n",redpacket.number);
			string message = "";
			message+=strprintf("%s",redpacket.message);
			if (message != "")
			{
				
				txdetail+=strprintf("����ͨ��������: %s\r\n\r\n",redpacket.message);
			}
			//SendRePacketCommtRecord(vTemp,transcion);

		}else if (vTemp[0] == TX_COMM_ACCEPTREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			ACCEPT_RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(ACCEPT_RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(ACCEPT_RED_PACKET));


			txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","�����" );
			txdetail+=strprintf("��Լ����:  %s\r\n\r\n","����ͨ���" );

			std::vector<unsigned char> vTemp;
			vTemp.assign(redpacket.redhash,redpacket.redhash+sizeof(redpacket.redhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("��ͨ���ID: %s\r\n\r\n",txhash.c_str());
			//AcceptRePacketCommtRecord(vTemp,transcion);
		}else if (vTemp[0] == TX_SPECIAL_SENDREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(SEND_DATA));

			txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","�����" );
			txdetail+=strprintf("��Լ����:  %s\r\n\r\n","���ͽ������" );

			txdetail+=strprintf("������������:  %.8f\r\n\r\n",(redpacket.money*1.0)/COIN);
			txdetail+=strprintf("�������������:  %d\r\n\r\n",redpacket.number);
			//SendRePacketSpecailRecord(vTemp,transcion);
		}else if (vTemp[0] == TX_SPECIAL_ACCEPTREDPACKET)
		{
			if(vTemp.size()==0)
				return _T("");;
			ACCEPT_RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(ACCEPT_RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(ACCEPT_RED_PACKET));

	
			txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","�����" );
			txdetail+=strprintf("��Լ����:  %s\r\n\r\n","���������" );

			std::vector<unsigned char> vTemp;
			vTemp.assign(redpacket.redhash,redpacket.redhash+sizeof(redpacket.redhash));
			reverse(vTemp.begin(),vTemp.end());
			string txhash = CSoyPayHelp::getInstance()->HexStr(vTemp);

			txdetail+=strprintf("�������ID:   %s\r\n\r\n",txhash.c_str());
		}else if (vTemp[0] == 0xff)
		{
			if (vTemp[1] == 0x02)    /// ��ֵ
			{
				txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","�����" );
				txdetail+=strprintf("��Լ����:  %s\r\n\r\n","��ֵ" );
			}else if (vTemp[1] == 0x01 || vTemp[1] == 0x03)   /// ����
			{
				txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","�����" );
				txdetail+=strprintf("��Լ����:  %s\r\n\r\n","����" );
				if (vTemp.size() == sizeof(APPACC))
				{
					APPACC draw;
					memcpy(&draw, &vTemp[0],sizeof(APPACC));
					txdetail+=strprintf("���ֽ��:  %.8f\r\n\r\n",(draw.money*1.0)/COIN);
				}
			}
		}
	}else if (!strcmp(tx.desregid.c_str(),theApp.m_ipoScritptid.c_str()))
	{

		if (vTemp[1] == 0x02)    /// ��ֵ
		{
			txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","ipo���" );
			txdetail+=strprintf("��Լ����:  %s\r\n\r\n","��ֵ" );
		}else if (vTemp[1] == 0x01 || vTemp[1] == 0x03)   /// ����
		{
			txdetail+=strprintf("Ӧ������:  %s\r\n\r\n","ipo���" );
			txdetail+=strprintf("��Լ����:  %s\r\n\r\n","����" );
			if (vTemp.size() == sizeof(APPACC))
			{
				APPACC draw;
				memcpy(&draw, &vTemp[0],sizeof(APPACC));
				txdetail+=strprintf("���ֽ��:  %.8f\r\n\r\n",(draw.money*1.0)/COIN);
			}
		}
	}
	return txdetail;
}
// CTxDetailDlg ��Ϣ�������
void CTxDetailDlg::ShowTxDetail(CString jsontx)
{
	if (jsontx == _T(""))
	{
		string strShowData;
		strShowData+=strprintf("%s" ,_T("�˽��ײ�����")) ;
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
		strShowData+=strprintf("%s" ,_T("�ڿ�������")) ;
	}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
	{
		strShowData+=strprintf("%s" ,_T("ע���˻�����")) ;
	}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
	{
		strShowData+=strprintf("%s" ,_T("ת�˽���")) ;
	}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
	{
		strShowData+=strprintf("%s" ,_T("��Լ����")) ;
	}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
	{
		strShowData+=strprintf("%s" ,_T("ע��Ӧ�ý���")) ;
	}
	txdetail=strprintf("��������: %s\r\n\r\n",strShowData.c_str());
	txdetail+=strprintf("����ID: %s\r\n\r\n",tx.txhash);
	txdetail+=strprintf("�汾��:   %d\r\n\r\n",tx.ver);
	txdetail+=strprintf("Դ��ַ:   %s\r\n\r\n",tx.addr.c_str());
	if (tx.pubkey != "")
	{
		txdetail+=strprintf("��Կ:   %s\r\n\r\n",tx.pubkey.c_str());
	}
	if (tx.miner_pubkey != "")
	{
		txdetail+=strprintf("���ڿ�Կ:   %s\r\n\r\n",tx.miner_pubkey.c_str());
	}
	if (tx.fees != 0)
	{
		txdetail+=strprintf("С��:   %.8f\r\n\r\n",tx.fees*COIN);
	}
	if (tx.height != 0)
	{
		int height = tx.height +250; 
		txdetail+=strprintf("���׳�ʱ�߶�:   %d\r\n\r\n",height);
	}
	if (tx.desaddr != "")
	{
		txdetail+=strprintf("Ŀ�ĵ�ַ:   %s\r\n\r\n",tx.desaddr.c_str() );
	}
	if (tx.money != 0)
	{
		txdetail+=strprintf("���׽��:   %.8f\r\n\r\n",tx.money*COIN);
	}
	if (tx.Contract != "")
	{
		//txdetail.AppendFormat(_T("��Լ����:   %d\r\n\r\n"),tx.Contract.c_str() );
		txdetail+=strprintf("%s",GetContacrDetail(tx).c_str());
		
	}

	if (tx.confirmedHeight != 0)
	{
		txdetail+=strprintf("ȷ�ϸ߶�:   %d\r\n\r\n",tx.confirmedHeight );
	}
	if (tx.confirmedtime != 0)
	{
		SYSTEMTIME curTime = UiFun::Time_tToSystemTime(tx.confirmedtime);
		txdetail+=strprintf("ȷ��ʱ��:   %04d-%02d-%02d %02d:%02d:%02d\r\n\r\n",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}
	if (tx.blockhash != "")
	{
		txdetail+=strprintf("ȷ��blockHash:   %s\r\n\r\n",tx.blockhash.c_str());
	}
	if ( theApp.blocktipheight != 0 && tx.confirmedHeight != 0)
	{
		unsigned int quredCount = theApp.blocktipheight - tx.confirmedHeight;
		if (quredCount >=0)
		{
			txdetail+=strprintf("ȷ����:  %d\r\n\r\n",quredCount );
		}	
	}

	GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail.c_str());
}

BOOL CTxDetailDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(90, _T("΢���ź�"));
	m_headText.SetTextColor(RGB(255,255,255));	

	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(20 , _T("΢���ź�"));
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

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText("ȷ ��") ;
	m_rBtnOk.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ShowTxDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CTxDetailDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}
