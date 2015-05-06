// NewAddressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "NewAddressDlg.h"
#include "afxdialogex.h"

// CNewAddressDlg �Ի���

IMPLEMENT_DYNAMIC(CNewAddressDlg, CDialogEx)

CNewAddressDlg::CNewAddressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewAddressDlg::IDD, pParent)
{
}

CNewAddressDlg::~CNewAddressDlg()
{

}

void CNewAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewAddressDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CNewAddressDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CNewAddressDlg ��Ϣ�������


void CNewAddressDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strCommand;
	int nCold = 0;
	if( ((CButton*)GetDlgItem(IDC_RADIO_YES))->GetCheck() ) {
		strCommand.Format(_T("%s"),_T("getnewaddress true"));
		nCold = 1;
	}else if ( ((CButton*)GetDlgItem(IDC_RADIO_NO))->GetCheck()){
		strCommand.Format(_T("%s"),_T("getnewaddress"));
	}
	CStringA strShowData ;
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	int pos = strShowData.Find("addr");
	if ( pos < 0 ) return ;

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	CString addr = root["addr"].asCString();

	CString Leble;
	GetDlgItem(IDC_EDIT_Leble)->GetWindowText(Leble);

	CString strSourceData;
	strSourceData.Format(_T("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'") , addr ,"",0.0 ,nCold ,0,Leble) ;
	uistruct::DATABASEINFO_t   pDatabase;
	pDatabase.strSource = strSourceData.GetString();
	pDatabase.strTabName =  _T("p2p_bet_record");
	CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
	string  strTemp = pDatabase.ToJson();
	postmsg.SetData(strTemp.c_str());
	theApp.m_MsgQueue.push(postmsg);

}


BOOL CNewAddressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
