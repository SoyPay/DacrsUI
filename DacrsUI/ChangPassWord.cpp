// ChangPassWord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ChangPassWord.h"
#include "afxdialogex.h"


// CChangPassWord �Ի���

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


// CChangPassWord ��Ϣ�������


void CChangPassWord::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString inputOld = _T("");
	CString confiedNew= _T("");
	CString confiedPassWord = _T("");
	GetDlgItem(IDC_EDIT_OLDPASSWORD)->GetWindowText(inputOld);
	GetDlgItem(IDC_EDIT_NEWPASSWORD)->GetWindowText(confiedNew);
	GetDlgItem(IDC_EDIT_CONFIREDPASSWORD)->GetWindowText(confiedPassWord);
	int pos = confiedNew.Find(_T(" "));
	if (pos >=0)
	{
		CMessageBoxEx message(_T("\n�����������в����пո�!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("�����������в����пո�") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	if (inputOld == _T(""))
	{
		CMessageBoxEx message(_T("\n�����������!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("�����������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (confiedNew == _T(""))
	{
		CMessageBoxEx message(_T("\n������������!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("������������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (confiedPassWord == _T(""))
	{
		CMessageBoxEx message(_T("\n������ȷ��������!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("������ȷ��������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAllDigtal(confiedNew))
	{
		CMessageBoxEx message(_T("\n���벻��ȫ������!") , 0 );
	    message.DoModal();
	//	::MessageBox( this->GetSafeHwnd() ,_T("���벻��ȫ������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	if (confiedNew.GetLength()<6)
	{
		CMessageBoxEx message(_T("\n�������������λ!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("�������������λ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	if (strcmp(confiedPassWord,confiedNew) != 0)
	{
		CMessageBoxEx message(_T("\nȷ��������������벻һ��!") , 0 );
	    message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("ȷ��������������벻һ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	string strCommand;
	strCommand = strprintf("%s %s %s",_T("walletpassphrasechange"),inputOld,confiedNew);
	string strShowData  = _T("");

	CSoyPayHelp::getInstance()->SendRpc(strCommand.c_str(),strShowData);
	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;

	if (strShowData.find("chgpwd") > 0)
	{
		bool isEntryp = root["chgpwd"].asBool();
		if (!isEntryp)
		{
			CMessageBoxEx message(_T("\n��������벻��ȷ,����������!") , 0 );
	        message.DoModal();
			//MessageBox(_T("��������벻��ȷ,����������"));
			return;
		}else{
			CMessageBoxEx message(_T("\n��ϲ�޸�����ɹ�!") , 0 );
	        message.DoModal();
			//MessageBox(_T("��ϲ�޸�����ɹ�"));
		}
	}else{
		CMessageBoxEx message(_T("\n��������벻��ȷ,����������!") , 0 );
	    message.DoModal();
		//MessageBox(_T("��������벻��ȷ,����������"));
		return;
	}
	

	CDialogEx::OnOK();
}
