// ChangPassWord.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ChangPassWord.h"
#include "afxdialogex.h"


// CChangPassWord 对话框

IMPLEMENT_DYNAMIC(CChangPassWord, CDialogBase)

CChangPassWord::CChangPassWord(CWnd* pParent /*=NULL*/)
	: CDialogBase(CChangPassWord::IDD, pParent)
{

}

CChangPassWord::~CChangPassWord()
{
}

void CChangPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICODLE, m_oldpassword);
	DDX_Control(pDX, IDC_STATICNEW, m_newpassword);	
	DDX_Control(pDX, IDC_STATICCOMFIRED, m_comfiredpassword);	
	DDX_Control(pDX, IDC_STATIC4, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CChangPassWord, CDialogBase)
	ON_BN_CLICKED(IDOK, &CChangPassWord::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CChangPassWord::OnBnClickedClose)
END_MESSAGE_MAP()


// CChangPassWord 消息处理程序


void CChangPassWord::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString inputOld = _T("");
	CString confiedNew= _T("");
	CString confiedPassWord = _T("");
	GetDlgItem(IDC_EDIT_OLDPASSWORD)->GetWindowText(inputOld);
	GetDlgItem(IDC_EDIT_NEWPASSWORD)->GetWindowText(confiedNew);
	GetDlgItem(IDC_EDIT_CONFIREDPASSWORD)->GetWindowText(confiedPassWord);
	int pos = confiedNew.Find(_T(" "));
	if (pos >=0)
	{
		UiFun::MessageBoxEx(_T("输入新密码中不能有空格") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}

	if (inputOld == _T(""))
	{
		UiFun::MessageBoxEx(_T("请输入旧密码") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}
	if (confiedNew == _T(""))
	{
		UiFun::MessageBoxEx(_T("请输入新密码") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}
	if (confiedPassWord == _T(""))
	{
		UiFun::MessageBoxEx(_T("请输入确定新密码") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}
	if (IsAllDigtal(confiedNew))
	{
		UiFun::MessageBoxEx(_T("密码不能全是数字") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}

	if (confiedNew.GetLength()<6)
	{
		UiFun::MessageBoxEx(_T("密码个数最少六位") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}

	if (strcmp(confiedPassWord,confiedNew) != 0)
	{
		UiFun::MessageBoxEx(_T("确认密码和输入密码不一致") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}

	string strCommand;
	strCommand = strprintf("%s %s %s",_T("walletpassphrasechange"),inputOld,confiedNew);

	Json::Value root; 
	if (!CSoyPayHelp::getInstance()->SendRpc(strCommand.c_str(),root))
	{
		TRACE("rpccmd walletpassphrasechange error");
		return;
	}

	bool isEntryp = root["chgpwd"].asBool();
	if (!isEntryp)
	{
		UiFun::MessageBoxEx(_T("输入旧密码不正确,请重新输入") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}else{
		UiFun::MessageBoxEx(_T("恭喜修改密码成功") , _T("提示") ,MFB_OK|MFB_TIP );
	}
	
	CDialogBase::OnOK();
}


void CChangPassWord::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogBase::OnCancel();
}


BOOL CChangPassWord::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// TODO:  在此添加额外的初始化
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
	m_rBtnOk.SetWindowText("确 定") ;
	//m_rBtnOk.SetFontEx(12 , _T("宋体"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText("取 消") ;
	//m_rBtnCancel.SetFontEx(12 , _T("宋体"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
