// SignAccountsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SignAccountsDlg.h"
#include "afxdialogex.h"


// CSignAccountsDlg �Ի���

IMPLEMENT_DYNAMIC(CSignAccountsDlg, CDialogEx)

CSignAccountsDlg::CSignAccountsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignAccountsDlg::IDD, pParent)
{

}

CSignAccountsDlg::~CSignAccountsDlg()
{
}

void CSignAccountsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSignAccountsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSignAccountsDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CSignAccountsDlg ��Ϣ�������


void CSignAccountsDlg::OnBnClickedButtonSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString address;
	GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
	if ( _T("") != address ) {
		CString strCommand , strShowData,strFee ;

		strCommand.Format(_T("%s %s %lld"),_T("getaccountinfo") ,address);
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		if (strShowData == _T(""))
		{
			::MessageBox( this->GetSafeHwnd() ,_T("������û�з�Ӧ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		}
		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(strShowData.GetString(), root)) 
			return  ;


		GetDlgItem(IDC_EDIT_FEE)->GetWindowText(strFee);
		strCommand.Format(_T("%s %s %lld"),_T("registaccounttx") ,address  , (INT64)REAL_MONEY(atof(strFee)) );

		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == _T(""))
		{
			::MessageBox( this->GetSafeHwnd() ,_T("������û�з�Ӧ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		}

		if (!reader.parse(strShowData.GetString(), root)) 
			return  ;

		CString strData;
		int pos = strShowData.Find("hash");

		if ( pos >=0 ) {
			//���뵽���ݿ�
			CString strHash , strHash1;
			strHash1.Format(_T("%s") , root["hash"].asCString() );
			theApp.cs_SqlData.Lock();
			int nItem =  theApp.m_SqliteDeal.FindDB(_T("revtransaction") , strHash1 ,_T("hash") ) ;
			theApp.cs_SqlData.Unlock();
			strHash.Format(_T("'%s'") , root["hash"].asCString() );
			if ( 0 == nItem ) {

				CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
				postmsg.SetData(strHash);
				theApp.m_MsgQueue.push(postmsg);

			}
		}

		if ( pos >=0 ) {
			strData.Format( _T("��ϲ�ɹ������˻�\n%s") , root["hash"].asCString() ) ;
		}else{
			strData.Format( _T("�����˻�ʧ��!") ) ;
		}
		::MessageBox( this->GetSafeHwnd() ,strData , _T("��ʾ") , MB_ICONINFORMATION ) ;
	}

}
void CSignAccountsDlg::SetShowAddr(CString addr)
{
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(addr);
}