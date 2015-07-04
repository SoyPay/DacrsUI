// ChangPassWord.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ChangPassWord.h"
#include "afxdialogex.h"


// CChangPassWord 对话框

IMPLEMENT_DYNAMIC(CChangPassWord, CDialogEx)

CChangPassWord::CChangPassWord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangPassWord::IDD, pParent)
{

}

CChangPassWord::~CChangPassWord()
{
}

void CChangPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChangPassWord, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChangPassWord::OnBnClickedOk)
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
	if (inputOld == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请输入旧密码") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (confiedNew == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请输入新密码") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (confiedPassWord == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请输入确定新密码") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAllDigtal(confiedNew))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("密码不能全是数字") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (confiedNew.GetLength()<6)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("密码个数最少六位") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (strcmp(confiedPassWord,confiedNew) != 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("确认密码和输入密码不一致") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CString strCommand;
	strCommand.Format(_T("%s %s %s"),_T("walletpassphrasechange"),inputOld,confiedNew);
	CStringA strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;

	if (strShowData.Find("chgpwd") > 0)
	{
		bool isEntryp = root["chgpwd"].asBool();
		if (!isEntryp)
		{
			MessageBox(_T("输入旧密码不正确,请重新输入"));
			return;
		}
	}
	

	CDialogEx::OnOK();
}
