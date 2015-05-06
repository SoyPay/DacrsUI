// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "MainDlg.h"
#include "afxdialogex.h"


// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogBar)

CMainDlg::CMainDlg()
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALLTXDETAIL , m_rBtnAllTxdetail);
	DDX_Control(pDX, IDC_TX_JY1 , m_strTrading);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogBar)
	ON_BN_CLICKED(IDC_ALLTXDETAIL, &CMainDlg::OnBnClickedAlltxdetail)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序



void CMainDlg::OnBnClickedAlltxdetail()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CMainDlg::SetCtrlText()
{
	CString strCommand,strShowData;
	strCommand.Format(_T("%s"),_T("getbalance"));
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	Json::Reader reader;  
	Json::Value root; 
	if (strShowData.Find("balance")>=0)
	{
		if (reader.parse(strShowData.GetString(), root)) 
		{
			double money = root["balance"].asDouble();
			strCommand.Format(_T("%.8f"),money);
			GetDlgItem(IDC_STATIC_AMOUNT)->GetWindowText(strCommand) ;
		}
	}else{
		strCommand.Format(_T("0.0"));
		GetDlgItem(IDC_STATIC_AMOUNT)->GetWindowText(strCommand) ;
	}
	strCommand.Format(_T("0"));
	theApp.cs_SqlData.Lock();
	int nItem =  theApp.m_SqliteDeal.FindINTDB(_T("revtransaction") , strCommand ,_T("confirmHeight")) ;
	theApp.cs_SqlData.Unlock();

	strCommand.Format(_T("%d"),nItem);
	GetDlgItem(IDC_STATIC_NOTCOF)->GetWindowText(strCommand) ;

	theApp.cs_SqlData.Lock();
	nItem =  theApp.m_SqliteDeal.GetTableCount(_T("revtransaction")) ;
	theApp.cs_SqlData.Unlock();

	strCommand.Format(_T("%d"),nItem);
	GetDlgItem(IDC_STATIC_COUNT)->GetWindowText(strCommand) ;

	CString Where,strSource;
	Where.Format(_T("'COMMON_TX' order by sign confirmedtime"));
	strSource.Format(_T("txtype"));
	uistruct::TRANSRECORDLIST pTransaction;
	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.FindDB(_T("revtransaction"),Where,strSource,&pTransaction);
	theApp.cs_SqlData.Unlock();

	int i = 1;
	strCommand.Format(_T("IDC_TX%d"),nItem);
	if (pTransaction.size() != 0  ) {
		int nSubIdx = 0 , i = 0 ;
		CString strShowData ;
		std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
		for (const_it = pTransaction.begin() ; const_it != pTransaction.end(),i<6 ; const_it++ ) {
			if (const_it->state == 1)
			{
				strSource.Format(_T("%s    -%.8f"),const_it->addr,const_it->money);
				i++;
			}else if (const_it->state == 2)
			{
				strSource.Format(_T("%s    -%.8f"),const_it->desaddr,const_it->money);
				i++;
			}
		}
	}
}

BOOL CMainDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes =  CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		SetCtrlText();
		m_rBtnAllTxdetail.LoadBitmaps(IDB_BITMAP_BUTTON_BJ,IDB_BITMAP_BUTTON_BJ,IDB_BITMAP_BUTTON_BJ,IDB_BITMAP_BUTTON_BJ);
		UpdateData(0);
		m_strTrading.SetFont(90, _T("宋体"));				//设置显示字体和大小
		m_strTrading.SetTextColor(RGB(255,0,0));			//字体颜色
		m_strTrading.SetWindowText(_T("-3.5smc")) ;
	}
	return bRes ;
}
