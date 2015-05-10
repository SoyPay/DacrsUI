// ReceiveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ReceiveDlg.h"
#include "afxdialogex.h"


// CReceiveDlg 对话框

IMPLEMENT_DYNAMIC(CReceiveDlg, CDialogBar)

CReceiveDlg::CReceiveDlg()
{
	m_accountDlg = NULL;
	m_newaddrDlg = NULL;
}

CReceiveDlg::~CReceiveDlg()
{
	if (m_accountDlg != NULL)
	{
		delete m_accountDlg;
		m_accountDlg = NULL;
	}
	if (m_newaddrDlg != NULL )
	{
		delete m_newaddrDlg;
		m_newaddrDlg = NULL;
	}
}

void CReceiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_listCtrl);
	DDX_Control(pDX, IDC_BUTTON_JHDZ, m_rBtnAcitve);
	DDX_Control(pDX, IDC_BUTTON_NEWADDRESS, m_rBtnNewaddr);
	DDX_Control(pDX, IDC_COPYADDRESS, m_rBtnCopyaddr);
}


BEGIN_MESSAGE_MAP(CReceiveDlg, CDialogBar)
	ON_BN_CLICKED(IDC_COPYADDRESS, &CReceiveDlg::OnBnClickedCopyaddress)
	ON_MESSAGE(MSG_USER_RECIVE_UI , &CReceiveDlg::OnShowListCtrl )
	ON_BN_CLICKED(IDC_BUTTON1, &CReceiveDlg::OnBnClickedButtonSignAccount)
	ON_BN_CLICKED(IDC_BUTTON_NEWADDRESS, &CReceiveDlg::OnBnClickedButtonNewaddress)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CReceiveDlg 消息处理程序

void CReceiveDlg::ShowListInfo()
{

	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.GetListaddrData(&m_pListaddrInfo);
	theApp.cs_SqlData.Unlock();

	if ( 0 == m_pListaddrInfo.size() ) return  ;

	m_listCtrl.DeleteAllItems();
	//加载到ComBox控件
	int nSubIdx = 0 , i = 0 ;
	CString strShowData = _T("");
	std::vector<uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_pListaddrInfo.begin() ; const_it != m_pListaddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		strShowData.Format(_T("%s") ,const_it->Lebel) ;
		int item = m_listCtrl.InsertItem( i , strShowData ) ;
		m_listCtrl.SetItemData(item , (DWORD_PTR)&(*const_it)) ;


		strShowData.Format(_T("%s") ,const_it->address) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		if (const_it->bSign == 1)
		{
			strShowData.Format(_T("已激活")) ;
		}else{
			strShowData.Format(_T("未激活")) ;
		}

		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;
		strShowData.Format(_T("%.8f") , const_it->fMoney ) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		if (const_it->nColdDig== 1)
		{
			strShowData.Format(_T("支持")) ;
		}else{
			strShowData.Format(_T("不支持")) ;
		}
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		i++;
	}
}

BOOL CReceiveDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes =   CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if (bRes)
	{
		m_rBtnAcitve.LoadBitmaps(IDB_BITMAP_ACTIVE1,IDB_BITMAP_ACTIVE2,IDB_BITMAP_ACTIVE3,IDB_BITMAP_ACTIVE3);
		m_rBtnNewaddr.LoadBitmaps(IDB_BITMAP_NEWADDR1,IDB_BITMAP_NEWADDR2,IDB_BITMAP_NEWADDR3,IDB_BITMAP_NEWADDR3);
		m_rBtnCopyaddr.LoadBitmaps(IDB_BITMAP_COPYADDR1,IDB_BITMAP_COPYADDR2,IDB_BITMAP_COPYADDR3,IDB_BITMAP_COPYADDR3);
		UpdateData(0);
		struct LISTCol {
			CString		name ;
			UINT		size ;
		} listcol[5]  = { {"标签" , 100 }     ,
		{"地址" , 275}  , 
		{"激活状态" , 150}  , 
		{"余额" , 140}  ,
		{"支持冷挖矿" ,100} 
		};
		m_listCtrl.SetBkColor(RGB(240,240,240));       
		m_listCtrl.SetRowHeigt(23);               
		m_listCtrl.SetHeaderHeight(1.5);         
		m_listCtrl.SetHeaderFontHW(16,0);
		m_listCtrl.SetHeaderBKColor(32,30,32,8); 
		m_listCtrl.SetTextColor(RGB(0,0,0));  
		for( int i = 0 ; i < 5 ; i++  ) {
			m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
		}
		m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );
		

		ShowListInfo();

		if( NULL == m_accountDlg ){
			m_accountDlg = new CSignAccountsDlg ;
			m_accountDlg->Create(CSignAccountsDlg::IDD, this) ;
			m_accountDlg->ShowWindow(SW_HIDE) ;
		}

		if (NULL == m_newaddrDlg)
		{
			m_newaddrDlg = new CNewAddressDlg ;
			m_newaddrDlg->Create(CNewAddressDlg::IDD, this) ;
			m_newaddrDlg->ShowWindow(SW_HIDE) ;
		}

		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_RECIVE_UI ) ;
	}
	return bRes;
}


void CReceiveDlg::OnBnClickedCopyaddress()
{
	// TODO: 在此添加控件通知处理程序代码
	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
			int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
			uistruct::LISTADDR_t * pDbbetData = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
			CString source =pDbbetData->address;
			//文本内容保存在source变量中
			if(OpenClipboard())
			{
				HGLOBAL clipbuffer;
				char * buffer;
				EmptyClipboard();
				clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
				buffer = (char*)GlobalLock(clipbuffer);
				strcpy(buffer, LPCSTR(source));
				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();
		}else{
			StrShow.Format(_T("请选择地址!\n"));
			::MessageBox( this->GetSafeHwnd() ,StrShow , _T("提示") , MB_ICONINFORMATION ) ;
		}
	}
}

LRESULT  CReceiveDlg::OnShowListCtrl(  WPARAM wParam, LPARAM lParam )
{
	ShowListInfo();
	return 1;
}


void CReceiveDlg::OnBnClickedButtonSignAccount()
{
	// TODO: 在此添加控件通知处理程序代码
	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		uistruct::LISTADDR_t * pDbbetData = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
		m_accountDlg->SetShowAddr(pDbbetData->address);
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		m_accountDlg->MoveWindow(rcWindow.right/2+50,rcWindow.top+200,400,rcWindow.Height()/2);
		m_accountDlg->ShowWindow(SW_SHOW);
	}else{
		StrShow.Format(_T("请选择地址!\n"));
		::MessageBox( this->GetSafeHwnd() ,StrShow , _T("提示") , MB_ICONINFORMATION ) ;
	}
}


void CReceiveDlg::OnBnClickedButtonNewaddress()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	m_newaddrDlg->MoveWindow(rcWindow.right/2+50,rcWindow.top+200,400,rcWindow.Height()/2);
	m_newaddrDlg->ShowWindow(SW_SHOW);
}


BOOL CReceiveDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//判断是否按下键盘Enter键
		if (pMsg->wParam == VK_CONTROL || pMsg->wParam == VK_ESCAPE ||pMsg->wParam == VK_SHIFT )
		{
			return true;
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}


void CReceiveDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;
		CRect rc  ;
		GetClientRect( rc ) ;
		CButton *pList = (CButton*)GetDlgItem(IDC_LIST_SHOW);
		if( NULL != pList ) {	
		   pList->SetWindowPos(NULL ,0, 0 , 900 , 600 - 72 - 32 - 41 , SWP_SHOWWINDOW);
		}
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_JHDZ);
		if( NULL != pButton ) {	
		   CRect m_BtnRc ;
		   pButton->GetClientRect(&m_BtnRc);
		   pButton->SetWindowPos(NULL ,900 - 3*(103 + 5) , 600 - 72 - 32 - 39 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEWADDRESS);
		if( NULL != pButton ) {	
			CRect m_BtnRc ;
			pButton->GetClientRect(&m_BtnRc);
			pButton->SetWindowPos(NULL ,900 - 2*(103 + 5) , 600 - 72 - 32 - 39 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		}
		pButton = (CButton*)GetDlgItem(IDC_COPYADDRESS);
		if( NULL != pButton ) {	
			CRect m_BtnRc ;
			pButton->GetClientRect(&m_BtnRc);
			pButton->SetWindowPos(NULL ,900 - 1*(103 + 5) , 600 - 72 - 32 - 39 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		}
	}
}
