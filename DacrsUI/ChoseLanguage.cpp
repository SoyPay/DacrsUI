// ChoseLanguage.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ChoseLanguage.h"
#include "afxdialogex.h"


// CChoseLanguage 对话框

IMPLEMENT_DYNAMIC(CChoseLanguage, CDialogBase)

CChoseLanguage::CChoseLanguage(CWnd* pParent /*=NULL*/)
	: CDialogBase(CChoseLanguage::IDD, pParent)
{
	m_language = theApp.gsLanguage;
}

CChoseLanguage::~CChoseLanguage()
{
}

void CChoseLanguage::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOLANGUAGE, m_laguabox);
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CChoseLanguage, CDialogBase)
	ON_BN_CLICKED(IDOK, &CChoseLanguage::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBOLANGUAGE, &CChoseLanguage::OnCbnSelchangeCombolanguage)
	ON_BN_CLICKED(IDC_CLOSE, &CChoseLanguage::OnBnClickedClose)
END_MESSAGE_MAP()


// CChoseLanguage 消息处理程序


void CChoseLanguage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	string strTemp = strprintf("%d",m_language);;
	string  strG;
	string strAppIni = theApp.str_InsPath;// + (CString)LANGUAGE_FILE;
	strAppIni += LANGUAGE_FILE;
	::WritePrivateProfileString("Language1","gsLanguage",strTemp.c_str(),(LPCTSTR)strAppIni.c_str());
	CDialogBase::OnOK();
}


void CChoseLanguage::OnCbnSelchangeCombolanguage()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_laguabox.GetCurSel();
	m_language =index+1;
}


BOOL CChoseLanguage::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(100, _T("微软雅黑"));
	m_headText.SetTextColor(RGB(255,255,255));	
	m_headText.SetWindowText(UiFun::UI_LoadString("REMINDER" , "REMINDER_CHOSE" ,theApp.gsLanguage));

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
	//m_rBtnOk.SetFontEx(12 , _T("宋体"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_CANCEL" ,theApp.gsLanguage)) ;
	//m_rBtnCancel.SetFontEx(12 , _T("宋体"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();

	m_laguabox.InsertString(0,_T("中文简体"));
	m_laguabox.InsertString(1,_T("English"));

	// TODO:  在此添加额外的初始化
	if (m_language == 2)
	{
		m_laguabox.SetCurSel(1);
	}else{
		m_laguabox.SetCurSel(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CChoseLanguage::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogBase::OnCancel();
}
