// WalletPassPhrase.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "WalletPassPhrase.h"
#include "afxdialogex.h"


// CWalletPassPhrase 对话框

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


// CWalletPassPhrase 消息处理程序


void CWalletPassPhrase::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString PassWord= _T("");
	CString passtime = _T("");
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(PassWord);
	GetDlgItem(IDC_EDIT_TIEM)->GetWindowText(passtime);
	if (PassWord == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_INPUTE_PASSWORD" ,theApp.gsLanguage)  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}
	if (passtime == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_INPUTE_TIME" ,theApp.gsLanguage)  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}
	if (!IsAllDigtal(passtime))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_TIME_DIGHT" ,theApp.gsLanguage)  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}
	
	string strCommand;
	strCommand = strprintf("%s %s %d","walletpassphrase",PassWord,(atoi(passtime)*60));
	string strShowData =_T("");
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE(" rpccmd walletpassphrase error");
		UiFun::MessageBoxEx(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_ERROR" ,theApp.gsLanguage)  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}


	bool isEntryp = root["passphrase"].asBool();
	if (!isEntryp)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_ERROR" ,theApp.gsLanguage), UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return;
	}else{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_CORRECT" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
	}

	theApp.m_passlock = TRUE;
	CDialogBase::OnOK();
}


BOOL CWalletPassPhrase::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetWindowText(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_HEAD" ,theApp.gsLanguage));
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

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_CANCEL" ,theApp.gsLanguage)) ;
	//m_rBtnCancel.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();
	// TODO:  在此添加额外的初始化
	theApp.m_passlock = FALSE;
	GetDlgItem(IDC_EDIT_TIEM)->SetWindowText(_T("60"));
	GetDlgItem(IDC_PASSWORD)->SetWindowText(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_PASSWORD" ,theApp.gsLanguage));
	GetDlgItem(IDC_TIMEOUT)->SetWindowText(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_TIME" ,theApp.gsLanguage));
	GetDlgItem(IDC_MINUE)->SetWindowText(UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_MINUTE" ,theApp.gsLanguage));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CWalletPassPhrase::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_passlock = TRUE;
	CDialogBase::OnCancel();
}


void CWalletPassPhrase::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_passlock = TRUE;
	CDialogBase::OnCancel();
}
