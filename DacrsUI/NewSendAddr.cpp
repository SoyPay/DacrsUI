// NewSendAddr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "NewSendAddr.h"
#include "afxdialogex.h"


// CNewSendAddr �Ի���

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


// CNewSendAddr ��Ϣ�������


void CNewSendAddr::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString label =_T("");
	CString addr  = _T("");
	GetDlgItem(IDC_EDIT_Lebel)->GetWindowText(label);
	GetDlgItem(IDC_EDIT_ADDR)->GetWindowText(addr);
	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("��ַ����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
	}
	uistruct::ADDRBOOK_t sAddrBookItem;
	string strCond;
	strCond = strprintf(" address='%s' ", addr);
	
	int  item = theApp.m_SqliteDeal.GetAddressBookItem(strCond, &sAddrBookItem);
	if (sAddrBookItem.address != _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("��ַ�����Ѵ��ڴ˵�ַ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	m_selectAddr.label = label;
	m_selectAddr.address = addr;
	CDialogEx::OnOK();
}
void CNewSendAddr::GetAddrbook(uistruct::ADDRBOOK_t &addr)
{
	addr = m_selectAddr;
}