// EncryptWallet.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "EncryptWallet.h"
#include "afxdialogex.h"
#include "DacrsUIDlg.h"

// CEncryptWallet 对话框

IMPLEMENT_DYNAMIC(CEncryptWallet, CDialogEx)

CEncryptWallet::CEncryptWallet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEncryptWallet::IDD, pParent)
{

}

CEncryptWallet::~CEncryptWallet()
{
}

void CEncryptWallet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEncryptWallet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEncryptWallet::OnBnClickedOk)
END_MESSAGE_MAP()


// CEncryptWallet 消息处理程序


void CEncryptWallet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString inputE = _T("");
	CString confiedE = _T("");
	GetDlgItem(IDC_INPUTMIMA)->GetWindowText(inputE);
	GetDlgItem(IDC_CONFIREDMIMA)->GetWindowText(confiedE);
	if (inputE == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("输入密码") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (confiedE == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请输入确认密码") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (inputE.GetLength()<6)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("密码个数最少六位") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAllDigtal(inputE))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("密码不能全是数字") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (strcmp(inputE,confiedE) != 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("确认密码和输入密码不一致") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CString strCommand;
	strCommand.Format(_T("%s %s"),_T("encryptwallet"),inputE);
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
		
	bool isClose = FALSE; 
	if (strShowData.Find("encrypt")>0)	{
		bool isEntryp = root["encrypt"].asBool();
		isClose = isEntryp;
		if (isEntryp)
		{
			MessageBox(_T("钱包加密成功请重新启动钱包"));

		}else
		{
			MessageBox(_T("钱包加密失败"));
		}
	}

	CDialogEx::OnOK();
	if (isClose)
	{
		((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
	}
}
