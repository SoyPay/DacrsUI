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
	DDX_Control(pDX, IDC_TX_JY2 , m_strTrading);
	DDX_Control(pDX, IDC_TX_JY3 , m_strTrading2);
	DDX_Control(pDX, IDC_TX_JY4 , m_strTrading3);
	DDX_Control(pDX, IDC_TX_JY5 , m_strTrading4);
	DDX_Control(pDX, IDC_TX_JY7 , m_strTrading5);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogBar)
	ON_BN_CLICKED(IDC_ALLTXDETAIL, &CMainDlg::OnBnClickedAlltxdetail)
	ON_MESSAGE(MSG_USER_MAIN_UI , &CMainDlg::OnShowListCtorl )
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
			GetDlgItem(IDC_STATIC_AMOUNT)->SetWindowText(strCommand) ;
		}
	}else{
		strCommand.Format(_T("0.0"));
		GetDlgItem(IDC_STATIC_AMOUNT)->SetWindowText(strCommand) ;
	}
	strCommand.Format(_T("0"));
	theApp.cs_SqlData.Lock();
	int nItem =  theApp.m_SqliteDeal.FindINTDB(_T("revtransaction") , strCommand ,_T("confirmHeight")) ;
	theApp.cs_SqlData.Unlock();

	strCommand.Format(_T("%d"),nItem);
	GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(strCommand) ;

	theApp.cs_SqlData.Lock();
	nItem =  theApp.m_SqliteDeal.GetTableCount(_T("revtransaction")) ;
	theApp.cs_SqlData.Unlock();

	strCommand.Format(_T("%d"),nItem);
	GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(strCommand) ;

	CString Where,strSource;
	//Where.Format(_T("'COMMON_TX' order by confirmedtime"));
	Where.Format(_T("'CONTRACT_TX' order by confirmedtime"));
	strSource.Format(_T("txtype"));
	uistruct::TRANSRECORDLIST pTransaction;
	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.FindDB(_T("revtransaction"),Where,strSource,&pTransaction);
	theApp.cs_SqlData.Unlock();

	int i = 1;
	strCommand.Format(_T("IDC_TX%d"),nItem);
	int item = IDC_TX1;
	int item1 = IDC_TX_JY2;
	if (pTransaction.size() != 0  ) {
		int nSubIdx = 0 , i = 0 ;
		CString strShowaddr ;
		std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
		for (const_it = pTransaction.begin() ; const_it != pTransaction.end()&&i<6 ; const_it++ ) {
			if (const_it->state == 1)
			{
				strSource.Format(_T("-%.8f"),const_it->money);
				strShowaddr.Format(_T("%s "),const_it->addr.c_str());
				i++;
			}else if (const_it->state == 2)
			{
				strSource.Format(_T("+%.8f"),const_it->money);
				strShowaddr.Format(_T("%s "),const_it->desaddr);
				i++;
			}
			GetDlgItem(item)->SetWindowText(strShowaddr) ;
			if (const_it->state == 1)
			{
				if(i == 1)
				m_strTrading.SetTextColor(RGB(255,0,0));			//字体颜色
				if(i == 2)
				m_strTrading2.SetTextColor(RGB(255,0,0));
				if(i == 3)
					m_strTrading3.SetTextColor(RGB(255,0,0));			//字体颜色
				if(i == 4)
					m_strTrading4.SetTextColor(RGB(255,0,0));
				if(i == 5)
					m_strTrading5.SetTextColor(RGB(255,0,0));
			}else if (const_it->state == 2)
			{
				if(i == 1)
					m_strTrading.SetTextColor(RGB(166,162,247));			//字体颜色
				if(i == 2)
					m_strTrading2.SetTextColor(RGB(166,162,247));
				if(i == 3)
					m_strTrading3.SetTextColor(RGB(166,162,247));			//字体颜色
				if(i == 4)
					m_strTrading4.SetTextColor(RGB(166,162,247));
				if(i == 5)
					m_strTrading5.SetTextColor(RGB(166,162,247));

			}

			GetDlgItem(item1)->SetWindowText(strSource) ;
			item++;
			item1++;
		}
	}
}

BOOL CMainDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes =  CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnAllTxdetail.LoadBitmaps(IDB_BITMAP_BUTTON_BJ,IDB_BITMAP_BUTTON_BJ,IDB_BITMAP_BUTTON_BJ,IDB_BITMAP_BUTTON_BJ);
		UpdateData(0);
		SetCtrlText();
		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_MAIN_UI ) ;
	}
	return bRes ;
}
LRESULT CMainDlg::OnShowListCtorl( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	SetCtrlText();
	return 0 ;
}