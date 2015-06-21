// TxDetailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TxDetailDlg.h"
#include "afxdialogex.h"


// CTxDetailDlg �Ի���

IMPLEMENT_DYNAMIC(CTxDetailDlg, CDialogEx)

CTxDetailDlg::CTxDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTxDetailDlg::IDD, pParent)
{

}

CTxDetailDlg::~CTxDetailDlg()
{
}

void CTxDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTxDetailDlg, CDialogEx)
END_MESSAGE_MAP()

// CTxDetailDlg ��Ϣ�������
void CTxDetailDlg::ShowTxDetail(CString jsontx)
{
	//string txdetail = tx.ToJson();
	uistruct::REVTRANSACTION_t tx;
	tx.JsonToStruct(jsontx.GetString());
	CString txdetail,strShowData;

	string txtype = tx.txtype;
	if (!strcmp(txtype.c_str(),"REWARD_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("�ڿ�������")) ;
	}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("ע���˻�����")) ;
	}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("ת�˽���")) ;
	}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("��Լ����")) ;
	}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("ע��Ӧ�ý���")) ;
	}
	txdetail.Format(_T("��������: %s\r\n\r\n"),strShowData);
	txdetail.AppendFormat(_T("����ID: %s\r\n\r\n"),tx.txhash);
	txdetail.AppendFormat(_T("�汾��:   %d\r\n\r\n"),tx.ver);
	txdetail.AppendFormat(_T("Դ��ַ:   %s\r\n\r\n"),tx.addr.c_str());
	if (tx.pubkey != "")
	{
		txdetail.AppendFormat(_T("��Կ:   %s\r\n\r\n"),tx.pubkey.c_str());
	}
	if (tx.miner_pubkey != "")
	{
		txdetail.AppendFormat(_T("���ڿ�Կ:   %s\r\n\r\n"),tx.miner_pubkey.c_str());
	}
	if (tx.fees != 0)
	{
		txdetail.AppendFormat(_T("С��:   %.8f\r\n\r\n"),tx.fees*COIN);
	}
	if (tx.height != 0)
	{
		int height = tx.height +250; 
		txdetail.AppendFormat(_T("���׳�ʱ�߶�:   %d\r\n\r\n"),height);
	}
	if (tx.desaddr != "")
	{
		txdetail.AppendFormat(_T("Ŀ�ĵ�ַ:   %s\r\n\r\n"),tx.desaddr.c_str() );
	}
	if (tx.money != 0)
	{
		txdetail.AppendFormat(_T("���׽��:   %.8f\r\n\r\n"),tx.money*COIN);
	}
	if (tx.Contract != "")
	{
		txdetail.AppendFormat(_T("��Լ����:   %d\r\n\r\n"),tx.Contract.c_str() );
	}

	if (tx.confirmedHeight != 0)
	{
		txdetail.AppendFormat(_T("ȷ�ϸ߶�:   %d\r\n\r\n"),tx.confirmedHeight );
	}
	if (tx.confirmedtime != 0)
	{
		SYSTEMTIME curTime = UiFun::Time_tToSystemTime(tx.confirmedtime);
		txdetail.AppendFormat(_T("ȷ��ʱ��:   %04d-%02d-%02d %02d:%02d:%02d\r\n\r\n"),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}
	if (tx.blockhash != "")
	{
		txdetail.AppendFormat(_T("ȷ��blockHash:   %s\r\n\r\n"),tx.blockhash.c_str());
	}

	GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail);
}

BOOL CTxDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ShowTxDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
