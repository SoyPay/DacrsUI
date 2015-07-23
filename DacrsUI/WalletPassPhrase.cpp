// WalletPassPhrase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "WalletPassPhrase.h"
#include "afxdialogex.h"


// CWalletPassPhrase �Ի���

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


// CWalletPassPhrase ��Ϣ�������


void CWalletPassPhrase::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString PassWord= _T("");
	CString passtime = _T("");
	GetDlgItem(IDC_EDIT_PASSWORD)->GetWindowText(PassWord);
	GetDlgItem(IDC_EDIT_TIEM)->GetWindowText(passtime);
	if (PassWord == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("����������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (passtime == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("������ʱ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (!IsAllDigtal(passtime))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ʱ���������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
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
			MessageBox(_T("��������벻��ȷ,����������"));
			return;
		}else{
			MessageBox(_T("��ϲǮ�������ɹ�"));
		}
	}else{
		MessageBox(_T("��������벻��ȷ,����������"));
		return;
	}
	theApp.m_passlock = TRUE;
	CDialogEx::OnOK();
}


BOOL CWalletPassPhrase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	theApp.m_passlock = FALSE;
	GetDlgItem(IDC_EDIT_TIEM)->SetWindowText(_T("60"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CWalletPassPhrase::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.m_passlock = TRUE;
	CDialogEx::OnCancel();
}
