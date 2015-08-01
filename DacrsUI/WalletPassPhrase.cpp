// WalletPassPhrase.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "WalletPassPhrase.h"
#include "afxdialogex.h"


// CWalletPassPhrase 对话框

IMPLEMENT_DYNAMIC(CWalletPassPhrase, CDialogEx)

CWalletPassPhrase::CWalletPassPhrase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWalletPassPhrase::IDD, pParent)
{

}

CWalletPassPhrase::~CWalletPassPhrase()
{
}

void CWalletPassPhrase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWalletPassPhrase, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWalletPassPhrase::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWalletPassPhrase::OnBnClickedCancel)
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
		CMessageBoxEx message(_T("\n请输入密码!")  , 0 );
	            message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("请输入密码") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (passtime == _T(""))
	{
		CMessageBoxEx message(_T("\n请输入时间!")  , 0 );
	            message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("请输入时间") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (!IsAllDigtal(passtime))
	{
		CMessageBoxEx message(_T("\n时间必须数字!")  , 0 );
	            message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("时间必须数字") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	
	string strCommand;
	strCommand = strprintf("%s %s %d","walletpassphrase",PassWord,(atoi(passtime)*60));
	string strShowData =_T("");

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;

	if (strShowData.find("passphrase") > 0)
	{
		bool isEntryp = root["passphrase"].asBool();
		if (!isEntryp)
		{
			CMessageBoxEx message(_T("\n输入就密码不正确,请重新输入!")  , 0 );
	            message.DoModal();
			//MessageBox(_T("输入就密码不正确,请重新输入"));
			return;
		}else{
			CMessageBoxEx message(_T("\n恭喜钱包解锁成功!")  , 0 );
	            message.DoModal();
			//MessageBox(_T("恭喜钱包解锁成功"));
		}
	}else{
		CMessageBoxEx message(_T("\n输入就密码不正确,请重新输入!")  , 0 );
	            message.DoModal();
		//MessageBox(_T("输入就密码不正确,请重新输入"));
		return;
	}
	theApp.m_passlock = TRUE;
	CDialogEx::OnOK();
}


BOOL CWalletPassPhrase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	theApp.m_passlock = FALSE;
	GetDlgItem(IDC_EDIT_TIEM)->SetWindowText(_T("60"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CWalletPassPhrase::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_passlock = TRUE;
	CDialogEx::OnCancel();
}
