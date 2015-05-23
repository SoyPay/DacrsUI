// GuessNum.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "GuessNum.h"
#include "afxdialogex.h"


// CGuessNum 对话框

IMPLEMENT_DYNAMIC(CGuessNum, CDialogEx)

CGuessNum::CGuessNum(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGuessNum::IDD, pParent)
{

}

CGuessNum::~CGuessNum()
{
}

void CGuessNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGuessNum, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGuessNum::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGuessNum::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGuessNum 消息处理程序


void CGuessNum::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress=_T("");
	char guess = 0x00;
	guess = ((CButton *)GetDlgItem(IDC_RADIO_MAN))->GetCheck();
	theApp.m_strAddress.Format(_T("%d"),(int)guess);
	CDialogEx::OnOK();
}


void CGuessNum::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress=_T("");
	CDialogEx::OnCancel();
}


BOOL CGuessNum::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_MAN);
	radio->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
