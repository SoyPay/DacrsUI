// TxDetailDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TxDetailDlg.h"
#include "afxdialogex.h"


// CTxDetailDlg 对话框

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

// CTxDetailDlg 消息处理程序
void CTxDetailDlg::ShowTxDetail(CString jsontx)
{
	//string txdetail = tx.ToJson();
	uistruct::REVTRANSACTION_t tx;
	tx.JsonToStruct(jsontx.GetString());
	CString txdetail,strShowData;

	string txtype = tx.txtype;
	if (!strcmp(txtype.c_str(),"REWARD_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("挖矿奖励交易")) ;
	}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("注册账户交易")) ;
	}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("转账交易")) ;
	}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("合约交易")) ;
	}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
	{
		strShowData.AppendFormat(_T("%s") ,_T("注册应用交易")) ;
	}
	txdetail.Format(_T("交易类型: %s\r\n\r\n"),strShowData);
	txdetail.AppendFormat(_T("交易ID: %s\r\n\r\n"),tx.txhash);
	txdetail.AppendFormat(_T("版本号:   %d\r\n\r\n"),tx.ver);
	txdetail.AppendFormat(_T("源地址:   %s\r\n\r\n"),tx.addr.c_str());
	if (tx.pubkey != "")
	{
		txdetail.AppendFormat(_T("公钥:   %s\r\n\r\n"),tx.pubkey.c_str());
	}
	if (tx.miner_pubkey != "")
	{
		txdetail.AppendFormat(_T("冷挖矿公钥:   %s\r\n\r\n"),tx.miner_pubkey.c_str());
	}
	if (tx.fees != 0)
	{
		txdetail.AppendFormat(_T("小费:   %.8f\r\n\r\n"),tx.fees*COIN);
	}
	if (tx.height != 0)
	{
		int height = tx.height +250; 
		txdetail.AppendFormat(_T("交易超时高度:   %d\r\n\r\n"),height);
	}
	if (tx.desaddr != "")
	{
		txdetail.AppendFormat(_T("目的地址:   %s\r\n\r\n"),tx.desaddr.c_str() );
	}
	if (tx.money != 0)
	{
		txdetail.AppendFormat(_T("交易金额:   %.8f\r\n\r\n"),tx.money*COIN);
	}
	if (tx.Contract != "")
	{
		txdetail.AppendFormat(_T("合约内容:   %d\r\n\r\n"),tx.Contract.c_str() );
	}

	if (tx.confirmedHeight != 0)
	{
		txdetail.AppendFormat(_T("确认高度:   %d\r\n\r\n"),tx.confirmedHeight );
	}
	if (tx.confirmedtime != 0)
	{
		SYSTEMTIME curTime = UiFun::Time_tToSystemTime(tx.confirmedtime);
		txdetail.AppendFormat(_T("确认时间:   %04d-%02d-%02d %02d:%02d:%02d\r\n\r\n"),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}
	if (tx.blockhash != "")
	{
		txdetail.AppendFormat(_T("确认blockHash:   %s\r\n\r\n"),tx.blockhash.c_str());
	}

	GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail);
}

BOOL CTxDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowTxDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
