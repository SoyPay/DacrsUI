// WalletPassPhrase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "WalletPassPhrase.h"
#include "afxdialogex.h"


// CWalletPassPhrase �Ի���

IMPLEMENT_DYNAMIC(CWalletPassPhrase, CDialogBase)

CWalletPassPhrase::CWalletPassPhrase(CWnd* pParent /*=NULL*/)
	: CDialogBase(CWalletPassPhrase::IDD, pParent)
{

}

CWalletPassPhrase::~CWalletPassPhrase()
{
}

void CWalletPassPhrase::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PASSWORD, m_password);
	DDX_Control(pDX, IDC_TIMEOUT, m_timeout);	
	DDX_Control(pDX, IDC_MINUE, m_minue);	
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CWalletPassPhrase, CDialogBase)
	ON_BN_CLICKED(IDOK, &CWalletPassPhrase::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWalletPassPhrase::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CLOSE, &CWalletPassPhrase::OnBnClickedClose)
END_MESSAGE_MAP()


// CWalletPassPhrase ��Ϣ�������


void CWalletPassPhrase::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString PassWord= _T("");
	CString passtime = _T("");
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(PassWord);
	GetDlgItem(IDC_EDIT_TIEM)->GetWindowText(passtime);
	if (PassWord == _T(""))
	{
		
		UiFun::MessageBoxEx(_T("����������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (passtime == _T(""))
	{
		
		UiFun::MessageBoxEx(_T("������ʱ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (!IsAllDigtal(passtime))
	{
		
		UiFun::MessageBoxEx(_T("ʱ���������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	
	string strCommand;
	strCommand = strprintf("%s %s %d","walletpassphrase",PassWord,(atoi(passtime)*60));
	string strShowData =_T("");
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE(" rpccmd walletpassphrase error");
		return;
	}


	bool isEntryp = root["passphrase"].asBool();
	if (!isEntryp)
	{
		UiFun::MessageBoxEx(_T("��������벻��ȷ,����������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}else{
		UiFun::MessageBoxEx(_T("��ϲǮ�������ɹ�") , _T("��ʾ") ,MFB_OK|MFB_TIP );
	}

	theApp.m_passlock = TRUE;
	CDialogBase::OnOK();
}


BOOL CWalletPassPhrase::OnInitDialog()
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
	//m_rBtnCancel.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	theApp.m_passlock = FALSE;
	GetDlgItem(IDC_EDIT_TIEM)->SetWindowText(_T("60"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CWalletPassPhrase::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.m_passlock = TRUE;
	CDialogBase::OnCancel();
}


void CWalletPassPhrase::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.m_passlock = TRUE;
	CDialogBase::OnCancel();
}
