// EncryptWallet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "EncryptWallet.h"
#include "afxdialogex.h"
#include "DacrsUIDlg.h"

// CEncryptWallet �Ի���

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


// CEncryptWallet ��Ϣ�������


void CEncryptWallet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString inputE = _T("");
	CString confiedE = _T("");
	GetDlgItem(IDC_INPUTMIMA)->GetWindowText(inputE);
	GetDlgItem(IDC_CONFIREDMIMA)->GetWindowText(confiedE);
	if (inputE == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("��������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (confiedE == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("������ȷ������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (inputE.GetLength()<6)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("�������������λ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAllDigtal(inputE))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("���벻��ȫ������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (strcmp(inputE,confiedE) != 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ȷ��������������벻һ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
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
			MessageBox(_T("Ǯ�����ܳɹ�����������Ǯ��"));

		}else
		{
			MessageBox(_T("Ǯ������ʧ��"));
		}
	}

	CDialogEx::OnOK();
	if (isClose)
	{
		((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
	}
}
