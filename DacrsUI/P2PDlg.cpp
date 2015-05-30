// P2PDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "P2PDlg.h"
#include "afxdialogex.h"
#include "Out.h"
#include "ReCharge.h"
#include "GuessNum.h"
#include "RpcCmd.h"

#define OUT_HEIGHT  10

// CP2PDlg 对话框

IMPLEMENT_DYNAMIC(CP2PDlg, CDialogBar)

CP2PDlg::CP2PDlg()
{
	m_pBmp = NULL ;
	m_seltab = 0;
	m_pagecount = 0;
	m_curpage = 0;
	m_PoolList.clear();
	m_pagesize = 5;
}

CP2PDlg::~CP2PDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CP2PDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BALANCE , m_Balance);
	DDX_Control(pDX, IDC_STATIC_NOT_DRAW , m_NotDraw);
	DDX_Control(pDX, IDC_STATIC_DW , m_Dw);
	DDX_Control(pDX, IDC_STATIC_VS , m_VS);

	DDX_Control(pDX, IDC_BUTTON_WITHD , m_rBtnWithd);
	DDX_Control(pDX, IDC_BUTTON_RECH , m_rBtnRech);
	DDX_Control(pDX, IDC_BUTTON_MALE , m_rBtnMale);
	DDX_Control(pDX, IDC_BUTTON_WOMAN , m_rBtnWoman); 
	DDX_Control(pDX, IDC_COMBO_ADDRES, m_addrbook);
	DDX_Control(pDX, IDC_LIST_BONUS, m_BonusListBox);
	DDX_Control(pDX, IDC_BUTTON_REFRESH_1, m_rBtnRefresh1);
	DDX_Control(pDX, IDC_BUTTON_REFRESH_2, m_rBtnRefresh2);
	DDX_Control(pDX, IDC_TAB, m_tab);

	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CP2PDlg, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE( WM_BN_CLICK, &CP2PDlg::onBnCLick)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_ADDRES, &CP2PDlg::OnCbnSelchangeComboAddres)
	ON_MESSAGE(MSG_USER_P2P_UI , &CP2PDlg::OnShowListCtrol )
	ON_BN_CLICKED(IDC_BUTTON_WITHD, &CP2PDlg::OnBnClickedButtonWithd)
	ON_BN_CLICKED(IDC_BUTTON_RECH, &CP2PDlg::OnBnClickedButtonRech)
	ON_BN_CLICKED(IDC_BUTTON_MALE, &CP2PDlg::OnBnClickedButtonMale)
	ON_BN_CLICKED(IDC_BUTTON_WOMAN, &CP2PDlg::OnBnClickedButtonWoman)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CP2PDlg::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_2, &CP2PDlg::OnBnClickedButtonRefresh2)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_1, &CP2PDlg::OnBnClickedButtonRefresh1)
	ON_NOTIFY(NM_THEMECHANGED, IDC_LIST_BONUS, &CP2PDlg::OnNMThemeChangedListBonus)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CP2PDlg::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CP2PDlg::OnBnClickedButtonNext)
END_MESSAGE_MAP()


// CP2PDlg 消息处理程序
void CP2PDlg::SetBkBmpNid( UINT nBitmapIn ) 
{
	if( NULL != m_pBmp ) {
		::DeleteObject( m_pBmp ) ;
		m_pBmp = NULL ;
	}
	m_pBmp	= NULL ;
	HINSTANCE	hInstResource = NULL;	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);
	if( NULL != hInstResource ) {
		m_pBmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
	}
}

int CP2PDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_P2P_BJ);
	return 0;
}


BOOL CP2PDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect   rect; 
	GetClientRect(&rect); 

	if(m_pBmp   !=   NULL) { 
		BITMAP   bm; 
		CDC   dcMem; 
		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
		pDC-> StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


BOOL CP2PDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnMale.LoadBitmaps(IDB_BITMAP_SISTER_1,IDB_BITMAP_SISTER_3,IDB_BITMAP_SISTER_2,IDB_BITMAP_SISTER_1);
		m_rBtnWoman.LoadBitmaps(IDB_BITMAP_BOTHER_1,IDB_BITMAP_BOTHER_3,IDB_BITMAP_BOTHER_2,IDB_BITMAP_BOTHER_1);
		UpdateData(0);
		m_Balance.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_Balance.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_Balance.SetWindowText(_T(""));

		m_NotDraw.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_NotDraw.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_NotDraw.SetWindowText(_T(""));

		m_Dw.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_Dw.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_Dw.SetWindowText(_T("SMC"));

		m_VS.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_VS.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_VS.SetWindowText(_T(" VS"));

		m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_sCountpage.SetWindowText(_T("共:"));

		m_rBtnWithd.SetBitmaps( IDB_BITMAP_P2PBUTTON_1 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_1 , RGB(255, 255, 255) );
		m_rBtnWithd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWithd.SetWindowText("提现") ;
		m_rBtnWithd.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnWithd.SizeToContent();

		m_rBtnRech.SetBitmaps( IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 255) );
		m_rBtnRech.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRech.SetWindowText("充值") ;
		m_rBtnRech.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRech.SizeToContent();

		m_rBtnRefresh1.SetBitmaps( IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 255) );
		m_rBtnRefresh1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh1.SetWindowText("刷新") ;
		m_rBtnRefresh1.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRefresh1.SizeToContent();

		m_rBtnRefresh2.SetBitmaps( IDB_BITMAP_BUTTON3 , RGB(255, 255, 0) , IDB_BITMAP_BUTTON3 , RGB(255, 255, 255) );
		m_rBtnRefresh2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh2.SetWindowText("刷新") ;
		m_rBtnRefresh2.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRefresh2.SizeToContent();

		
		m_rBtnUp.SetBitmaps( IDB_BITMAP_UP , RGB(255, 255, 0) , IDB_BITMAP_UP , RGB(255, 255, 255) );
		m_rBtnUp.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnUp.SetWindowText("") ;
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnUp.SizeToContent();

		m_rBtnNext.SetBitmaps( IDB_BITMAP_NEXT , RGB(255, 255, 0) , IDB_BITMAP_NEXT , RGB(255, 255, 255) );
		m_rBtnNext.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNext.SetWindowText("") ;
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNext.SizeToContent();

		

		m_tab.InsertItem(0,_T("投注记录"));  //添加参数一选项卡 
		m_tab.InsertItem(1,_T("发起记录"));  //添加参数二选项卡 

		CSize sz(80,30);
		m_tab.SetItemSize(sz);
		m_tab.SetMinTabWidth(80);
		m_tab.SetItemPadding(6);

		if ( !m_BetRecord.Create( (LPCTSTR)CBetRecord::IDD , &m_tab ))
			return -1;

		if ( !m_SendRecord.Create( (LPCTSTR)CSendRecord::IDD , &m_tab ))
			return -1;

		CRect rc;
		m_tab.GetClientRect(rc);
		rc.bottom -= 1;
		rc.left += 1;
		rc.top += 33;
		rc.right -= 2;
		m_BetRecord.MoveWindow(&rc);
		m_SendRecord.MoveWindow(&rc);
		m_pDialog.push_back(&m_BetRecord) ;
		m_pDialog.push_back(&m_SendRecord) ;
		OnSelectShowWin(0);

		AddListaddrDataBox();
		OnListPool();
		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_P2P_UI ) ;
	}
	return bRes ;
}
void CP2PDlg::OnSelectShowWin(int nCurSelTab)
{
	m_seltab = nCurSelTab;
	int nCurSelTabTmp = 0 ;
	std::vector<CDialog*>::iterator itDialog = m_pDialog.begin();
	for (;itDialog != m_pDialog.end();itDialog++)
	{  
		if(nCurSelTabTmp == nCurSelTab ){
			(*itDialog)->ShowWindow(SW_SHOW);
		}else{
			(*itDialog)->ShowWindow(SW_HIDE);
		}
		nCurSelTabTmp++ ;
	}
}

void CP2PDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {

		CRect rc ;
		GetClientRect( rc ) ;

		CWnd *pst = GetDlgItem( IDC_LIST_BONUS ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,455 , 85 , 432, 147  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_UP ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-129 , 232 + 5 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-115+11, 232 + 5 , 40, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_NEXT ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-70+11 , 232 + 5 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,875-50+11 , 232 + 8 , 50, 30  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_REFRESH_1 ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,800 , 20 ,  0 , 0 , SWP_NOSIZE ) ; 
		}
		pst = GetDlgItem( IDC_TAB ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,10 , 268 ,  878 , 218 , SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_REFRESH_2 ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,800 , 270 ,  0 , 0 , SWP_NOSIZE ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_BALANCE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*12 , (rc.Height()/100)*7 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_STATIC_NOT_DRAW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*12 , (rc.Height()/100)*13 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_BUTTON_WITHD ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*30 , (rc.Height()/100)*9 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_BUTTON_RECH ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*42 , (rc.Height()/100)*9 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_COMBO_ADDRES ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*12 , (rc.Height()/100)*20 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_EDIT_MONEY ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*14 , (rc.Height()/100)*42,  rect.Width() , rect.Height() + 5 , SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_STATIC_DW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*38 , (rc.Height()/100)*43,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_MALE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*17 , (rc.Height()/100)*52+2,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_WOMAN ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*33 ,  (rc.Height()/100)*52+2,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}



		

	}
}


void CP2PDlg::OnCbnSelchangeComboAddres()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return;
	}
	m_addrbook.GetLBText(sel,text);

	//m_addrbook.GetWindowText(text) ;
	if(text!=_T(""))
	{
		CString strCommand,strShowData;

		CString strCond;
		strCommand.Format(_T("%s %s %s"),_T("getappaccinfo") , theApp.m_betScritptid ,text);
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(strShowData.GetString(), root)) 
			return  ;

		int pos = strShowData.Find("FreeValues");
		INT64 nMoney = 0;
		if (pos >0)
		{
			nMoney = root["FreeValues"].asInt64() ;
		}
		double money = (nMoney*1.0/COIN);
		strShowData.Format(_T("%.8f"),money);

		((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(strShowData);
		Invalidate();
	}

	QueryNotDrawBalance(text);          /// blockchanged 刷新为开奖的数据
	ShowListItem(0);
	ShowListItem(1);
}

void CP2PDlg::InsertComboxIitem()
{
	CPostMsg postmsg;
	if (!theApp.m_UiP2pDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());

	CString addressd;
	addressd.Format(_T("%s"),addr.RegID);

	int item = m_addrbook.GetCount();
	m_addrbook.InsertString(item,addressd);
}

BOOL CP2PDlg::AddListaddrDataBox(){

	map<CString,uistruct::LISTADDR_t> m_mapAddrInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" sign=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->ResetContent();
	//加载到ComBox控件
	int nItem = 0;
	std::map<CString,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {

		((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->InsertString(nItem , const_it->second.RegID );
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->SetCurSel(0);

	CString address;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return FALSE;
	}
	m_addrbook.GetLBText(sel,address);
	OnCbnSelchangeComboAddres();
	return TRUE ;
}
LRESULT CP2PDlg::OnShowListCtrol( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_NEWADDRESS:
		{
			InsertComboxIitem();
		}
		break;
	case WM_UP_ADDRESS:
		{
			OnCbnSelchangeComboAddres();
		}
		break;
	default:
		break;

	}
	return 0 ;
}
void  CP2PDlg::QueryNotDrawBalance(CString addr)
{
	if (addr == _T(""))
	{
		return;
	}
	CString strCond;
	strCond.Format(_T(" (state != 2) and (left_addr ='%s' or right_addr = '%s') "),addr,addr);

	//double   nmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_p2p_quiz") , _T("amount") , strCond) ;
	//CString srtShow =_T("");
	//if (nmoney <0)
	//{
	//	srtShow = _T("0.0");
	//}else{
	//	srtShow.Format(_T("%.3lf"),nmoney);
	//}
	//((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(srtShow);

	uistruct::P2PBETRECORDLIST pPoolList;
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pPoolList);

	double money = 0.0;
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	for ( const_it = pPoolList.begin() ; const_it != pPoolList.end() ; const_it++ ) {
		if (const_it->state ==0 &&(500 + const_it->height)> theApp.blocktipheight || const_it->state ==3
			|| ((const_it->state ==1 || const_it->state ==4) &&(const_it->time_out + const_it->height)> theApp.blocktipheight))
		{
			break;
		}
		money += const_it->amount; 
	}

	CString srtShow =_T("");
	srtShow.Format(_T("%.3lf"),money);

	((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(srtShow);

	Invalidate();

}

void CP2PDlg::OnBnClickedButtonWithd()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!CheckBalance())
	{
		return;
	}

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	CString strShowData ;

	
	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CString strCommand , strMaddress , strMoney;

	string strContractData = m_P2PBetHelp.GetAppAccountMoneyContract(addr.GetString(),1,1);

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	int pos = strShowData.Find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		CString strHash;
		strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜提现成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( _T("恭喜提现成功，请等待1-2分钟确认交易\n")) ;
	}else{
		strTip.Format( _T("提现失败!") ) ;
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}


void CP2PDlg::OnBnClickedButtonRech()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CReCharge outdlg;
	if ( IDOK != outdlg.DoModal()){
		return;
	}

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	if (theApp.m_strAddress == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strShowData ;
	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CString strCommand , strMaddress , strMoney;

	string strContractData = m_P2PBetHelp.GetReChangContract();

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),addr.GetString(),strContractData,0,strTxFee,REAL_MONEY(atof(theApp.m_strAddress) ));
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	int pos = strShowData.Find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		CString strHash;
		strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜充值成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( _T("恭喜充值成功，请等待1-2分钟确认交易\n")) ;
	}else{
		strTip.Format( _T("充值失败!") ) ;
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}

void CP2PDlg::SendBet(int rewardnum)
{
	// TODO: 在此添加控件通知处理程序代码  PacketP2PSendContract

	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!CheckBalance())
	{
		return;
	}
	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double balance =atof(strMoney);

	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	if (atof(strTxMoney) > balance)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("赌注金额大于账户余额") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	//// 查询地址是否激活
	CString strCond;
	/*strCond.Format(_T(" address = '%s' "), addr);
	uistruct::LISTADDR_t addrsql;
	int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;
	if (!addrsql.bSign)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("账户未激活不能发生合约交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}*/
	char strTemp[34];
	memset(strTemp , 0 , 34 );
	memcpy(strTemp , UiFun::Rnd32() , 32 );
	strTemp[32] =rewardnum ;


	string temp(strTemp,strTemp+33);
	int aa = temp.length() ;
	CString strCommand;
	strCommand.Format(_T("%s %s"),_T("gethash") , strTemp );
	CStringA strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	int pos = strShowData.Find("hash");
	if ( pos < 0 ) return ;

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;

	CString strHash ;
	strHash.Format(_T("%s") ,  root["hash"].asCString() ) ;

	TRACE(_T("contect:%s\r\n"),strTemp);
	TRACE(_T("sendhash:%s"),strHash);
	string strContractData;
	string strRamdHash = CSoyPayHelp::getInstance()->GetReverseHash(strHash.GetString());
	strContractData = m_P2PBetHelp.PacketP2PSendContract((INT64)REAL_MONEY(atof(strTxMoney)),OUT_HEIGHT ,strRamdHash );

	INT64 strTxFee = theApp.m_P2PBetCfg.SendBetFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	pos = strShowData.Find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		CString strHash ;
		strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( _T("恭喜发送赌约成功，请等待1-2分钟确认交易\n")) ;
	}else{
		strTip.Format( _T("发送赌约失败!") ) ;
	}

	//保存到数据库
	if ( bRes ) {
		uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
		memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));
		SYSTEMTIME curTime ;
		memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
		GetLocalTime( &curTime ) ;
		CString strSendTime;
		strSendTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);
		p2pbetrecord.time_out  = OUT_HEIGHT ;
		memcpy(p2pbetrecord.tx_hash ,root["hash"].asCString() , sizeof(p2pbetrecord.tx_hash));
		p2pbetrecord.tx_hash[64] = '\0';
		memcpy(p2pbetrecord.left_addr ,addr , sizeof(p2pbetrecord.left_addr));
		p2pbetrecord.amount = atof(strTxMoney) ;
		memcpy(p2pbetrecord.content ,strTemp , sizeof(p2pbetrecord.content));

		p2pbetrecord.actor  = 0 ;
		p2pbetrecord.state  = 0 ;
		//插入到数据库
		CString strSourceData;
		strSourceData.Format(_T("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'") , \
			strSendTime , _T("") , p2pbetrecord.time_out , \
			p2pbetrecord.tx_hash ,  p2pbetrecord.left_addr , p2pbetrecord.right_addr ,p2pbetrecord.amount);

		strSourceData.AppendFormat(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
			p2pbetrecord.relate_hash ,p2pbetrecord.guess_num ) ;

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData.GetString();
		pDatabase.strTabName =  _T("t_p2p_quiz");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();
		CString strShow;
		strShow.Format(_T("%s"),strTemp.c_str());
		postmsg.SetData(strShow);
		theApp.m_MsgQueue.push(postmsg);
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}

void CP2PDlg::OnBnClickedButtonMale()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strDisplay;
	strDisplay.Format(_T("确定要发送赌约妹--->金额:%s"), strTxMoney);
	COut outdlg(NULL, strDisplay,100);
	if ( IDOK != outdlg.DoModal()){
		return;
	}
	SendBet(1);
}


void CP2PDlg::OnBnClickedButtonWoman()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strDisplay;
	strDisplay.Format(_T("确定要发送赌约哥--->金额:%s"), strTxMoney);
	COut outdlg(NULL, strDisplay,100);
	if ( IDOK != outdlg.DoModal()){
		return;
	}
	SendBet(2);
}
void CP2PDlg::OnListPool()
{
	m_PoolList.clear();
	m_curpage = 0;
	theApp.m_SqliteDeal.GetP2PQuizPoolList(_T(" 1=1 "), &m_PoolList);
	m_pagecount = (m_PoolList.size()%m_pagesize)==0?(m_PoolList.size()/m_pagesize):(m_PoolList.size()/m_pagesize)+1;
	
	CString temp;
	temp.Format(_T("共:%d"),m_pagecount);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp);
	Invalidate();
	m_BonusListBox.DeleteAllIndex();
	OnShowPagePool(1);
	//if (pPoolList.size() == 0)
	//{
	//	return ;
	//}

	//int nSubIdx = 0 , i = 0 ;
	//CString strShowData = _T("");
	//std::vector<uistruct::LISTP2POOL_T>::const_iterator const_it;
	//for ( const_it = pPoolList.begin() ; const_it != pPoolList.end() ; const_it++ ) {

	//	string nValue = const_it->data;
	//	uistruct::DBBET_DATA DBbet;
	//	memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
	//	std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
	//	memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

	//	CString addr,money;
	//	std::vector<unsigned char> vSendid;
	//	vSendid.assign(DBbet.sendbetid,DBbet.sendbetid+sizeof(DBbet.sendbetid));

	//	CString regid = CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);
	//	//CString strCond;
	//	//strCond.Format(_T(" reg_id = '%s' "), regid);
	//	//uistruct::LISTADDR_t addrsql;
	//	//int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

	//	//addr.Format(_T("%s"),addrsql.address);

	//	double dmoney = (DBbet.money*1.0)/COIN;
	//	money.Format(_T("%.4f"),dmoney);
	//	CString txhash, line;
	//	line.Format(_T("%d"),i);
	//	txhash.Format(_T("%s"),const_it->hash.c_str());
	//	m_BonusListBox.InsertStr(i,this->GetSafeHwnd());
	//	m_BonusListBox.SetIndexInage(i , IDB_BITMAP_P2P_LISTBOX_BUT);
	//	m_BonusListBox.SetIndexString(i , line,regid, _T("接"), money, txhash);
	//	i++;
	//}
}
 LRESULT CP2PDlg::onBnCLick( WPARAM wParam, LPARAM lParam )
 {
	List_AppendData* pinf = m_BonusListBox.GetAppendDataInfo((int)wParam);
	if ( NULL != pinf ) { 
		CString hash = pinf->pstr;
		CString conditon;
		conditon.Format(_T("hash='%s'"),hash);
		uistruct::LISTP2POOL_T pPoolList;
		theApp.m_SqliteDeal.GetP2PQuizPoolItem(conditon, &pPoolList);
		if (pPoolList.hash.length() != 0)
		{
			uistruct::DBBET_DATA DBbet;
			memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(pPoolList.data);
			memcpy(&DBbet, &vTemp[0], sizeof(DBbet));
			CString money,adddr;
			pinf->pSta2->GetWindowText(money);
			pinf->pSta1->GetWindowText(adddr);
			AcceptBet(hash,money,adddr,DBbet.hight);
		}
		
	}
	return 0;

}
void CP2PDlg::AcceptBet(CString hash,CString money,CString sendaddr,int timeout)
 {

	 if (!theApp.IsSyncBlock )
	 {
		 ::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		 return;
	 }
	 if (!CheckBalance())
	 {
		 return;
	 }
	 if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	 CString addr;
	 int sel = m_addrbook.GetCurSel();

	 if (sel < 0)
	 {
		 return ;
	 }
	 m_addrbook.GetLBText(sel,addr);

	 if (addr == _T(""))
	 {
		 ::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		 return;
	 }
	 //// 查询地址是否激活
	 //CString strCond;
	 //strCond.Format(_T(" address = '%s' "), addr);
	 //uistruct::LISTADDR_t addrsql;
	 //int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;
	 //if (!addrsql.bSign)
	 //{
		// ::MessageBox( this->GetSafeHwnd() ,_T("账户未激活不能发生合约交易") , _T("提示") , MB_ICONINFORMATION ) ;
		// return;
	 //}

	 CGuessNum guessdlg;
	 if (IDOK != guessdlg.DoModal())
	 {
		 return;
	 }
	 char guess = atoi(theApp.m_strAddress);

	 string strContractData,strHash;
	 strHash= CSoyPayHelp::getInstance()->GetReverseHash(hash.GetString());
	 strContractData = m_P2PBetHelp.PacketP2PAcceptContract((INT64)REAL_MONEY(atof(money)) ,strHash,guess);

	 if (  theApp.m_P2PBetCfg.AcceptBetnFee < 10000  ) {
		 return ;
	 }

	 CString strShowData;
	 string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),addr.GetString(),strContractData,0,theApp.m_P2PBetCfg.AcceptBetnFee,0);
	 CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	 Json::Reader reader;  
	 Json::Value root; 
	 if (!reader.parse(strShowData.GetString(), root)) 
		 return  ;
	 BOOL bRes = FALSE ;
	 CString strTip;
	 int pos = strShowData.Find("hash");

	 if ( pos >=0 ) {
		 //插入到交易记录数据库
		 CString strHash ;
		 strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	 }
	 if ( pos >=0 ) {
		 bRes = TRUE ;
		 //strTip.Format( _T("恭喜接赌成功!\n%s") , root["hash"].asCString() ) ;
		 strTip.Format( _T("恭喜接赌成功，请等待1-2分钟确认交易\n")) ;
	 }else{
		 strTip.Format( _T("此赌约已经被接!") ) ;
	 }

	 //保存到数据库
	 if ( bRes ) {

		 //插入到交易记录数据库

		 //// 查找数据库中是否存在此记录
		 CString conditon;
		 conditon.Format(_T("tx_hash ='%s'") , hash );
		 uistruct::P2P_QUIZ_RECORD_t pPoolItem;
		 int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
		 if (strlen(pPoolItem.tx_hash) == 0) ///此记录不存在,插入记录
		 {
			 uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
			 memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));
			 SYSTEMTIME curTime ;
			 memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			 GetLocalTime( &curTime ) ;
			 CString strSendTime;
			 strSendTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			 p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);

			 memcpy(p2pbetrecord.tx_hash ,hash , sizeof(p2pbetrecord.tx_hash));
			 p2pbetrecord.tx_hash[64] = '\0';

			 CString txhash = root["hash"].asCString();
			 memcpy(p2pbetrecord.relate_hash ,root["hash"].asCString() , sizeof(p2pbetrecord.tx_hash));
			 p2pbetrecord.relate_hash[64] = '\0';

			 memcpy(p2pbetrecord.right_addr ,addr , sizeof(p2pbetrecord.right_addr));
			 memcpy(p2pbetrecord.left_addr ,sendaddr , sizeof(p2pbetrecord.left_addr));
			 p2pbetrecord.amount = atof(money);

			 p2pbetrecord.state = 4;
			 p2pbetrecord.actor  = 1 ;
			 p2pbetrecord.guess_num = (int)guess ;
			 //插入到数据库
			 CString strSourceData ;
			 strSourceData.Format(_T("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'") , \
				 strSendTime , _T("") , timeout , \
				 p2pbetrecord.tx_hash ,  p2pbetrecord.left_addr , p2pbetrecord.right_addr ,p2pbetrecord.amount);

			 strSourceData.AppendFormat(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
				 p2pbetrecord.relate_hash ,p2pbetrecord.guess_num ) ;

			 uistruct::DATABASEINFO_t   pDatabase;
			 pDatabase.strSource = strSourceData.GetString();
			 pDatabase.strTabName =  _T("t_p2p_quiz");
			 CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
			 string strTemp = pDatabase.ToJson();
			 CString datastr;
			 datastr.Format(_T("%s"),strTemp.c_str());
			 postmsg.SetData(datastr);
			 theApp.m_MsgQueue.push(postmsg);

		 }else{        ///更新记录
			 CString txhash = root["hash"].asCString();
			 CString strSourceData  , strW ;
			 strSourceData.Format(_T("actor = %d , relate_hash = '%s' ,right_addr ='%s',") , 2 , txhash ,addr ) ;
			 strSourceData.AppendFormat(" guess_num = %d,state = %d", (int)guess,4);
			 strW.Format(_T("tx_hash = '%s'") , hash ) ;

			 uistruct::DATABASEINFO_t DatabaseInfo;
			 DatabaseInfo.strSource = strSourceData.GetString();
			 DatabaseInfo.strWhere = strW.GetString() ;
			 DatabaseInfo.strTabName = _T("t_p2p_quiz");
			 CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			 string strTemp = DatabaseInfo.ToJson();
			 CString datastr;
			 datastr.Format(_T("%s"),strTemp.c_str());
			 postmsg.SetData(datastr);
			 theApp.m_MsgQueue.push(postmsg);

		 }
	 }
	 ::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
 }
 bool CP2PDlg::CheckBalance()
 {
	 OnCbnSelchangeComboAddres();
	 CString strMoney;
	 ((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	 double money =atof(strMoney);
	if (money == 0.0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("账户金额为零,请先充值") , _T("提示") , MB_ICONINFORMATION ) ;
		return false;
	}
	return true;
 }
 void CP2PDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 // TODO: 在此添加控件通知处理程序代码
	 OnSelectShowWin(m_tab.GetCurSel());
	 *pResult = 0;
 }
 void    CP2PDlg::ShowListItem(int seltab)
 {
	 CString addr;
	 int sel = m_addrbook.GetCurSel();
	 if(sel == -1)
		 return;
	 m_addrbook.GetLBText(sel,addr);

	 if (addr == _T(""))
	 {
		 return;
	 }
	 if (seltab == 0)
	 {
		 m_BetRecord.Showlistbox(addr);
	 }else if (seltab == 1)
	 {
		 m_SendRecord.Showlistbox(addr);
	 }
 }

 void CP2PDlg::OnBnClickedButtonRefresh2()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 ShowListItem(m_seltab);
 }


 void CP2PDlg::OnBnClickedButtonRefresh1()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 OnListPool();
 }


 void CP2PDlg::OnNMThemeChangedListBonus(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 // 该功能要求使用 Windows XP 或更高版本。
	 // 符号 _WIN32_WINNT 必须 >= 0x0501。
	 // TODO: 在此添加控件通知处理程序代码
	 *pResult = 0;
 }
 void  CP2PDlg::OnShowPagePool(int page)
 {
	 if (page >m_pagecount || page == m_curpage || page <= 0)
	 {
		 return;
	 }


	 m_BonusListBox.DeleteAllIndex();
	 CString strpage;
	 strpage.Format(_T("%d"),page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage);
	 m_curpage = page;
	 int index = (page-1)*m_pagesize;
	 int count = (m_PoolList.size() -index)>=m_pagesize?m_pagesize:(m_PoolList.size() -index);
	 int i =0;
	 for (int k = index;k< (index+count);k++)
	 {
		 uistruct::LISTP2POOL_T const_it = m_PoolList.at(k);
		 string nValue = const_it.data;
		 uistruct::DBBET_DATA DBbet;
		 memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
		 std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
		 memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

		 CString addr,money;
		 std::vector<unsigned char> vSendid;
		 vSendid.assign(DBbet.sendbetid,DBbet.sendbetid+sizeof(DBbet.sendbetid));

		 CString regid = CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);

		 double dmoney = (DBbet.money*1.0)/COIN;
		 money.Format(_T("%.4f"),dmoney);

		 CString txhash, line;
		 line.Format(_T("%d"),i);
		 txhash.Format(_T("%s"),const_it.hash.c_str());
		 m_BonusListBox.InsertStr(i,this->GetSafeHwnd());
		 m_BonusListBox.SetIndexInage(i , IDB_BITMAP_P2P_LISTBOX_BUT);
		 m_BonusListBox.SetIndexString(i , line,regid, _T("接"), money, txhash);
		 i++;
	 }
 }

 BOOL CP2PDlg::PreTranslateMessage(MSG* pMsg)
 {
	 // TODO: 在此添加专用代码和/或调用基类
	 if(pMsg->message == WM_KEYDOWN)  
	 {   
		 if(pMsg->wParam == VK_RETURN)  
		 {  
			 if (GetDlgItem(IDC_EDIT_PAGE) == this->GetFocus())
			 {
				 CString num;
				 GetDlgItem(IDC_EDIT_PAGE)->GetWindowText(num);
				 if (IsAllDigtal(num))
				 {
					 OnShowPagePool(atoi(num));
				 }else
				 {
					  GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
					::MessageBox( this->GetSafeHwnd() ,_T("输入有误,请输入数字") , _T("提示") , MB_ICONINFORMATION ) ;
				 }
				 return TRUE;
			 }
		 }  
	 }  
	 return CDialogBar::PreTranslateMessage(pMsg);
 }


 void CP2PDlg::OnBnClickedButtonUp()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 OnShowPagePool((m_curpage-1));
 }


 void CP2PDlg::OnBnClickedButtonNext()
 {
	 // TODO: 在此添加控件通知处理程序代码
	 OnShowPagePool((m_curpage+1));
 }
