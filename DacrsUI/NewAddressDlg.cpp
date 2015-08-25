// NewAddressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "NewAddressDlg.h"
#include "afxdialogex.h"


// CNewAddressDlg �Ի���

IMPLEMENT_DYNAMIC(CNewAddressDlg, CDialogBase)

CNewAddressDlg::CNewAddressDlg(CWnd* pParent /*=NULL*/)
    : CDialogBase(CNewAddressDlg::IDD, pParent)
{
}

CNewAddressDlg::~CNewAddressDlg()
{
}

void CNewAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDC_BUTTON_SCDZ, m_rBtnNewAdd);
	DDX_Control(pDX, IDC_LABEL, m_label);
	DDX_Control(pDX, IDC_CODE, m_code);
	DDX_Control(pDX, IDC_HEAD, m_headText);	
}


BEGIN_MESSAGE_MAP(CNewAddressDlg, CDialogBase)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CNewAddressDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SCDZ, &CNewAddressDlg::OnBnClickedButtonScdz)
END_MESSAGE_MAP()


// CNewAddressDlg ��Ϣ�������
BOOL CNewAddressDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(100, _T("΢���ź�"));
	m_headText.SetTextColor(RGB(255,255,255));	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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

	m_rBtnNewAdd.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255));
	m_rBtnNewAdd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnNewAdd.SetWindowText("���ɵ�ַ") ;
	//m_rBtnNewAdd.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnNewAdd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnNewAdd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnNewAdd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnNewAdd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnNewAdd.SizeToContent();


	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_YES);
	radio->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CNewAddressDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(IDOK);
}


void CNewAddressDlg::OnBnClickedButtonScdz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (theApp.IsLockWallet())
	{
		return;
	}
	string strCommand;
	int nCold = 0;
	if( ((CButton*)GetDlgItem(IDC_RADIO_YES))->GetCheck() ) {
		strCommand = strprintf("%s",_T("getnewaddress true"));
		nCold = 1;
	}else if ( ((CButton*)GetDlgItem(IDC_RADIO_NO))->GetCheck()){
		strCommand = strprintf("%s",_T("getnewaddress"));
	}
	string strShowData ;
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("OnBnClickedButtonScdz rpccmd getnewaddress error");
		return;
	}

	string addr = root["addr"].asString();

	CString Lable;
	GetDlgItem(IDC_EDIT_Leble)->GetWindowText(Lable);

	uistruct::LISTADDR_t newaddr; 
	newaddr.address = addr;
	newaddr.nColdDig = nCold;
	newaddr.Label=strprintf("%s",Lable);

	string strSourceData;
	double money = 0.0;
	strSourceData =strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , addr.c_str() ,"",money ,nCold ,0,Lable) ;
	uistruct::DATABASEINFO_t   pDatabase;
	pDatabase.strSource = strSourceData;
	pDatabase.strcutjson = newaddr.ToJson();
	pDatabase.strTabName =  _T("t_wallet_address");
	CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
	string  strTemp = pDatabase.ToJson();
	postmsg.SetData(strTemp.c_str());
	theApp.m_MsgQueue.push(postmsg);

	

	strCommand = strprintf("��ϲ�����µ�ַ:\n%s\n���±���Ǯ���������µ���˽Կ",addr);

	if(IDOK == UiFun::MessageBoxEx( strCommand.c_str() , _T("��ʾ") , MFB_OK|MFB_TIP ))
	{
		EndDialog(IDOK);
	}
	
}
