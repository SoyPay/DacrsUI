// SetAppFee.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SetAppFee.h"
#include "afxdialogex.h"


// CSetAppFee �Ի���

IMPLEMENT_DYNAMIC(CSetAppFee, CDialogBase)

CSetAppFee::CSetAppFee(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSetAppFee::IDD, pParent)
{

}

CSetAppFee::~CSetAppFee()
{
}

void CSetAppFee::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LEAD , m_strTrading);
	DDX_Control(pDX, IDC_STATIC_SB , m_strTrading2);
	DDX_Control(pDX, IDC_STATIC_AB , m_strTrading3);
	DDX_Control(pDX, IDC_STATIC_OB , m_strTrading4);
	DDX_Control(pDX, IDC_STATIC_RLEAD , m_strTrading5);
	DDX_Control(pDX, IDC_STATIC_SCR , m_strTx1);
	DDX_Control(pDX, IDC_STATIC_GCR , m_strTx2);
	DDX_Control(pDX, IDC_STATIC_SSR , m_strTx3);
	DDX_Control(pDX, IDC_STATIC_ASR , m_strTx4);
	DDX_Control(pDX, IDC_STATIC_AR , m_strTx5);
	DDX_Control(pDX, IDC_STATIC_D , m_strDesTx1);

	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);


}


BEGIN_MESSAGE_MAP(CSetAppFee, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSetAppFee::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetAppFee::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetAppFee::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_CLOSE, &CSetAppFee::OnBnClickedClose)
END_MESSAGE_MAP()


// CSetAppFee ��Ϣ�������

bool CSetAppFee::IsAllDigtal(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!(pData[i] >= '0' && pData[i] <='9')&&pData[i] !='.')
			return false;
	}

	return true;
}
bool CSetAppFee::IsRegId(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!isdigit(pData[i]) && pData[i] !='-')
			return false;
	}

	return true;
}
void CSetAppFee::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (PathFileExistsA(theApp.str_InsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.str_InsPath);
		configpath+= strprintf("\\%s","dacrsclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;

		CString strTemp = _T("");
		GetDlgItem(IDC_SENDBETFEE)->GetWindowText(strTemp);
		if (strTemp != _T("") )
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("���Ͷ�ԼС�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("SendBetFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTEFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("�Ӷ�ԼС�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("AcceptBetnFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_OPENFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("����С�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("OpenBetnFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_SENDCOMREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("����ͨ���С�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("sendredcommFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTCOMREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("����ͨ���С�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("acceptredcommFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_SENDSPECAILREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("���������С�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("sendredspecalFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTSPECAILREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("�ӽ������С�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("acceptredspecalFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ReChange)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("��ֵС�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("GetRechangeFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_WITHDRAW)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				UiFun::MessageBoxEx(_T("����С�ѱ���������") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("GetAppAmountnFee"),strTemp);
			}
		}

		CStdioFile  File;
		string strpath = theApp.str_InsPath;
		strpath+="\\dacrsclient.conf";
		File.Open((LPCTSTR)(LPSTR)strpath.c_str(),CFile::modeWrite | CFile::modeCreate); 
		string strfile = root.toStyledString();
		File.WriteString(strfile.c_str());
		File.Close();
	}

	theApp.ParseUIConfigFile(theApp.str_InsPath);
	CDialogBase::OnOK();
}


void CSetAppFee::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDialogBase::OnCancel();
}

void CSetAppFee::SetDaluft()
{
	string strShow = "";
	strShow =strprintf("%.8f",(theApp.m_P2PBetCfg.SendBetFee*1.0)/COIN);
	GetDlgItem(IDC_SENDBETFEE)->SetWindowText(strShow.c_str());

	strShow =strprintf("%.8f",(theApp.m_P2PBetCfg.AcceptBetnFee*1.0)/COIN);
	GetDlgItem(IDC_ACCEPTEFEE)->SetWindowText(strShow.c_str());

	strShow =strprintf("%.8f",(theApp.m_P2PBetCfg.OpenBetnFee*1.0)/COIN);
	GetDlgItem(IDC_OPENFEE)->SetWindowText(strShow.c_str());

	strShow =strprintf("%.8f",(theApp.m_P2PBetCfg.GetRechangeFee*1.0)/COIN);
	GetDlgItem(IDC_ReChange)->SetWindowText(strShow.c_str());

	strShow =strprintf("%.8f",(theApp.m_P2PBetCfg.GetAppAmountnFee*1.0)/COIN);
	GetDlgItem(IDC_WITHDRAW)->SetWindowText(strShow.c_str());

	strShow =strprintf("%.8f",(theApp.m_RedPacketCfg.SendRedPacketCommFee*1.0)/COIN);
	GetDlgItem(IDC_SENDCOMREDFEE)->SetWindowText(strShow.c_str());

	strShow =strprintf("%.8f",(theApp.m_RedPacketCfg.AcceptRedPacketCommFee*1.0)/COIN);
	GetDlgItem(IDC_ACCEPTCOMREDFEE)->SetWindowText(strShow.c_str());

	strShow =strprintf("%.8f",(theApp.m_RedPacketCfg.SendRedPacketSpecailFee*1.0)/COIN);
	GetDlgItem(IDC_SENDSPECAILREDFEE)->SetWindowText(strShow.c_str());

	strShow =strprintf("%.8f",(theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee*1.0)/COIN);
	GetDlgItem(IDC_ACCEPTSPECAILREDFEE)->SetWindowText(strShow.c_str());

}
BOOL CSetAppFee::OnInitDialog()
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

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText("ȷ ��") ;
	//m_rBtnOk.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText("ȡ ��") ;
//	m_rBtnCancel.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();

	SetDaluft();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}




void CSetAppFee::OnSize(UINT nType, int cx, int cy)
{
	CDialogBase::OnSize(nType, cx, cy);
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;

		CRect rc ;
		GetClientRect( rc ) ;

		CWnd *pst = GetDlgItem( IDC_BUTTON_CLOSE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,rc.Width() - rect.Width() ,5  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
	}
	// TODO: �ڴ˴������Ϣ����������
}


void CSetAppFee::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		EndDialog(IDOK);
}


void CSetAppFee::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}
