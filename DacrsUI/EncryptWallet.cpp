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

	int pos = inputE.Find(_T(" "));
	if (pos >=0)
	{
		CMessageBoxEx message(_T("\n���������в����пո�!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("���������в����пո�") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (inputE == _T(""))
	{
		CMessageBoxEx message(_T("\n��������!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("��������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (confiedE == _T(""))
	{
		CMessageBoxEx message(_T("\n������ȷ������!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("������ȷ������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (inputE.GetLength()<6)
	{
		CMessageBoxEx message(_T("\n�������������λ!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("�������������λ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAllDigtal(inputE))
	{
		CMessageBoxEx message(_T("\n���벻��ȫ������!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("���벻��ȫ������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (strcmp(inputE,confiedE) != 0)
	{
		CMessageBoxEx message(_T("\nȷ��������������벻һ��!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("ȷ��������������벻һ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	string strCommand;
	strCommand = strprintf("%s %s",_T("encryptwallet"),inputE);
	string strShowData ="";

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;
		
	bool isClose = FALSE; 
	if (strShowData.find("encrypt")>0)	{
		bool isEntryp = root["encrypt"].asBool();
		isClose = isEntryp;
		if (isEntryp)
		{
			CMessageBoxEx message(_T("\nǮ�����ܳɹ�����������Ǯ��!") , 0 );
	        message.DoModal();
			//MessageBox(_T("Ǯ�����ܳɹ�����������Ǯ��"));

		}else
		{
			CMessageBoxEx message(_T("\nǮ������ʧ��!") , 0 );
	        message.DoModal();
		//	MessageBox(_T("Ǯ������ʧ��"));
		}
	}

	CDialogEx::OnOK();
	if (isClose)
	{
		((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
	}
}
