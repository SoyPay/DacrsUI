// NewSendAddr.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "NewSendAddr.h"
#include "afxdialogex.h"


// CNewSendAddr 对话框

IMPLEMENT_DYNAMIC(CNewSendAddr, CDialogEx)

CNewSendAddr::CNewSendAddr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewSendAddr::IDD, pParent)
{

}

CNewSendAddr::~CNewSendAddr()
{
}

void CNewSendAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewSendAddr, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewSendAddr::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewSendAddr 消息处理程序


void CNewSendAddr::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString lebel =_T("");
	CString addr  = _T("");
	GetDlgItem(IDC_EDIT_Lebel)->GetWindowText(lebel);
	GetDlgItem(IDC_EDIT_ADDR)->GetWindowText(addr);
	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
	}
	CString strSource;
	CString strP;
	strP.Format(_T("address"));
	strSource.Format(_T("'%s'"),addr);
	int  item = theApp.m_SqliteDeal.FindInDB(_T("addrbook") , strSource ,strP);
	if (item != 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址簿中已存在此地址") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	m_selectAddr.lebel = lebel;
	m_selectAddr.address = addr;
	CDialogEx::OnOK();
}
void CNewSendAddr::GetAddrbook(uistruct::ADDRBOOK_t &addr)
{
	addr = m_selectAddr;
}