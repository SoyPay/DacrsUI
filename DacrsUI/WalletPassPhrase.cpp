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
		::MessageBox( this->GetSafeHwnd() ,_T("请输入密码") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (passtime == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请输入时间") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (!IsAllDigtal(passtime))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("时间必须数字") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	
	CString strCommand;
	strCommand.Format(_T("%s %s %d"),_T("walletpassphrase"),PassWord,(atoi(passtime)*60));
	CStringA strShowData =_T("");

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;

	if (strShowData.Find("passphrase") > 0)
	{
		bool isEntryp = root["passphrase"].asBool();
		if (!isEntryp)
		{
			MessageBox(_T("输入就密码不正确,请重新输入"));
			return;
		}else{
			MessageBox(_T("恭喜钱包解锁成功"));
		}
	}else{
		MessageBox(_T("输入就密码不正确,请重新输入"));
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
