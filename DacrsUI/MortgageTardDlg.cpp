// MortgageTardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "MortgageTardDlg.h"
#include "afxdialogex.h"
#include "Out.h"
#include "ReCharge.h"
#include "RpcCmd.h"
// CMortgageTardDlg 对话框

IMPLEMENT_DYNAMIC(CMortgageTardDlg, CDialogBar)

CMortgageTardDlg::CMortgageTardDlg()
{
	m_pBmp = NULL ;
	m_seltab = 0;
	m_pagecount = 0;
	m_curpage = 0;
	m_PoolList.clear();
	m_pagesize = 5;
	m_selPooltab =0;
}

CMortgageTardDlg::~CMortgageTardDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}	

	v_linkCtrl.InternalRelease();
	v_linkCtrl.ExternalRelease();
	v_linkCtrl.OnFinalRelease();
	v_linkCtrl.DestroyWindow();
}

void CMortgageTardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BALANCE , m_Balance);
	DDX_Control(pDX, IDC_STATIC_NOT_DRAW , m_NotDraw);
	DDX_Control(pDX, IDC_STATIC_DW , m_Dw);

	DDX_Control(pDX, IDC_BUTTON_WITHD , m_rBtnWithd);
	DDX_Control(pDX, IDC_BUTTON_RECH , m_rBtnRech);
	DDX_Control(pDX, IDC_BUTTON_COMMRED, m_rBtnMale);
	DDX_Control(pDX, IDC_BUTTON_SPECAILRED , m_rBtnWoman); 
	DDX_Control(pDX, IDC_COMBO_ADDRES, m_addrbook);
//	DDX_Control(pDX, IDC_LIST_BONUS, m_BonusListBox);
	DDX_Control(pDX, IDC_BUTTON_REFRESH_1, m_rBtnRefresh1);
	DDX_Control(pDX, IDC_BUTTON_REFRESH_2, m_rBtnRefresh2);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Control(pDX, IDC_TAB1, m_tabpool);

	//DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	//DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	//DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
	DDX_Control(pDX ,IDC_STATIC_MONEY4 ,m_money ) ;
	DDX_Control(pDX, IDC_MFCLINK1, v_linkCtrl);
	
	DDX_Control(pDX, IDC_WINERLOUSER, m_rBtnWinerloser);
	DDX_Control(pDX, IDC_ONEWINER, m_rBtnAddrWinerloser);

}


BEGIN_MESSAGE_MAP(CMortgageTardDlg, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE( WM_BN_CLICK, &CMortgageTardDlg::onBnCLick)
	ON_BN_CLICKED(IDC_BUTTON_COMMRED, &CMortgageTardDlg::OnBnClickedButtonCommred)
	ON_CBN_SELCHANGE(IDC_COMBO_ADDRES, &CMortgageTardDlg::OnCbnSelchangeComboAddres)
	ON_BN_CLICKED(IDC_BUTTON_WITHD, &CMortgageTardDlg::OnBnClickedButtonWithd)
	ON_BN_CLICKED(IDC_BUTTON_RECH, &CMortgageTardDlg::OnBnClickedButtonRech)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CMortgageTardDlg::OnTcnSelchangeTab)
	ON_MESSAGE(MSG_USER_REDPACKET_UI , &CMortgageTardDlg::OnShowListCtrol )
	//ON_BN_CLICKED(IDC_BUTTON_UP, &CMortgageTardDlg::OnBnClickedButtonUp)
	//ON_BN_CLICKED(IDC_BUTTON_NEXT, &CMortgageTardDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_1, &CMortgageTardDlg::OnBnClickedButtonRefresh1)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_2, &CMortgageTardDlg::OnBnClickedButtonRefresh2)
	ON_BN_CLICKED(IDC_BUTTON_SPECAILRED, &CMortgageTardDlg::OnBnClickedButtonSpecailred)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMortgageTardDlg::OnTcnSelchangeTab1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMortgageTardDlg 消息处理程序

void CMortgageTardDlg::SetBkBmpNid( UINT nBitmapIn ) 
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
int CMortgageTardDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_REDPACKET);
	return 0;
}


BOOL CMortgageTardDlg::OnEraseBkgnd(CDC* pDC)
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


BOOL CMortgageTardDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		//m_rBtnMale.LoadBitmaps(IDB_BITMAP_SISTER_1,IDB_BITMAP_SISTER_3,IDB_BITMAP_SISTER_2,IDB_BITMAP_SISTER_1);
		//m_rBtnWoman.LoadBitmaps(IDB_BITMAP_BOTHER_1,IDB_BITMAP_BOTHER_3,IDB_BITMAP_BOTHER_2,IDB_BITMAP_BOTHER_1);
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

		//m_VS.SetFont(120, _T("黑体"));				//设置显示字体和大小
		//m_VS.SetTextColor(RGB(0,0,0));			    //字体颜色	
		//m_VS.SetWindowText(_T(" VS"));

		//m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		//m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		//m_sCountpage.SetWindowText(_T("共:"));

		m_rBtnMale.SetBitmaps( IDB_BITMAP_REDPACKET_TYPE , RGB(255, 255, 0) , IDB_BITMAP_REDPACKET_TYPE , RGB(255, 255, 255) );
		m_rBtnMale.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnMale.SetWindowText("普通红包") ;
		m_rBtnMale.SetFontEx(23 , _T("微软雅黑"));
		m_rBtnMale.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 182, 0));
		m_rBtnMale.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnMale.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 182, 0));
		m_rBtnMale.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnMale.SizeToContent();

		m_rBtnWoman.SetBitmaps( IDB_BITMAP_REDPACKET_TYPE , RGB(255, 255, 0) , IDB_BITMAP_REDPACKET_TYPE , RGB(255, 255, 255) );
		m_rBtnWoman.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWoman.SetWindowText("接龙红包") ;
		m_rBtnWoman.SetFontEx(23 , _T("微软雅黑"));
		m_rBtnWoman.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 182, 0));
		m_rBtnWoman.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnWoman.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 182, 0));
		m_rBtnWoman.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnWoman.SizeToContent();


		m_rBtnWithd.SetBitmaps( IDB_BITMAP_REDPACKETDRAW , RGB(255, 255, 0) , IDB_BITMAP_REDPACKETDRAW , RGB(255, 255, 255) );
		m_rBtnWithd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWithd.SetWindowText("提现") ;
		m_rBtnWithd.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 182, 0));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 182, 0));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnWithd.SizeToContent();

		m_rBtnRech.SetBitmaps( IDB_BITMAP_REDRCHANGE , RGB(255, 76, 0) , IDB_BITMAP_REDRCHANGE , RGB(255, 76, 0) );
		m_rBtnRech.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRech.SetWindowText("充值") ;
		m_rBtnRech.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 76, 0));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 76, 0));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 76, 0));
		m_rBtnRech.SizeToContent();

		m_rBtnRefresh1.SetBitmaps( IDB_BITMAP_RED_FRESH , RGB(255, 255, 0) , IDB_BITMAP_RED_FRESH , RGB(255, 255, 255) );
		m_rBtnRefresh1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh1.SetWindowText("刷新") ;
		m_rBtnRefresh1.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 76, 0));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 76, 0));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 76, 0));
		m_rBtnRefresh1.SizeToContent();

		m_rBtnRefresh2.SetBitmaps( IDB_BITMAP_RED_FRESH , RGB(255, 255, 0) , IDB_BITMAP_RED_FRESH , RGB(255, 255, 255) );
		m_rBtnRefresh2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh2.SetWindowText("刷新") ;
		m_rBtnRefresh2.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 76, 0));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 76, 0));
		m_rBtnRefresh2.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 76, 0));
		m_rBtnRefresh2.SizeToContent();


	/*	m_rBtnUp.SetBitmaps( IDB_BITMAP_UP , RGB(255, 255, 0) , IDB_BITMAP_UP , RGB(255, 255, 255) );
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
		m_rBtnNext.SizeToContent();*/

		m_rBtnWinerloser.SetBitmaps( IDB_BITMAP_WINERLOUSER , RGB(255, 255, 0) , IDB_BITMAP_WINERLOUSER , RGB(255, 255, 255) );
		m_rBtnWinerloser.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWinerloser.SetWindowText("") ;
		m_rBtnWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
		m_rBtnWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
		m_rBtnWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
		m_rBtnWinerloser.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));
		m_rBtnWinerloser.SizeToContent();

		m_rBtnAddrWinerloser.SetBitmaps( IDB_BITMAP_WINERLOUSER , RGB(255, 255, 0) , IDB_BITMAP_WINERLOUSER , RGB(255, 255, 255) );
		m_rBtnAddrWinerloser.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnAddrWinerloser.SetWindowText("") ;
		m_rBtnAddrWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
		m_rBtnAddrWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
		m_rBtnAddrWinerloser.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
		m_rBtnAddrWinerloser.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));
		m_rBtnAddrWinerloser.SizeToContent();

		m_money.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_money.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_money.SetWindowText(_T(""));
		m_tab.SetBitmap(IDB_BITMAP_P2PBUTTON_2,2);

		m_tab.InsertItem(0,_T("抢到的红包"));  //添加参数一选项卡 
		m_tab.InsertItem(1,_T("发起的红包"));  //添加参数二选项卡 

		CSize sz(80,30);
		m_tab.SetItemSize(sz);
		m_tab.SetMinTabWidth(80);
		m_tab.SetItemPadding(6);

		if ( !m_BetRecord.Create( (LPCTSTR)CAcceptRedPacketRecord::IDD , &m_tab ))
			return -1;

		if ( !m_SendRecord.Create( (LPCTSTR)CSendRedPacketRecord::IDD , &m_tab ))
			return -1;

		CRect rc;
		m_tab.GetClientRect(rc);
		rc.bottom -= 1;
		rc.left += 1;
		rc.top += 30;
		rc.right -= 2;
		m_BetRecord.MoveWindow(&rc);
		m_SendRecord.MoveWindow(&rc);
		m_pDialog.push_back(&m_BetRecord) ;
		m_pDialog.push_back(&m_SendRecord) ;
		OnSelectShowWin(0);

		m_tabpool.SetBitmap(IDB_BITMAP_P2PBUTTON_2,2);

		m_tabpool.InsertItem(0,_T("普通红包"));  //添加参数一选项卡 
		m_tabpool.InsertItem(1,_T("接龙红包"));  //添加参数二选项卡 

		CSize sz1(80,30);
		m_tabpool.SetItemSize(sz1);
		m_tabpool.SetMinTabWidth(80);
		m_tabpool.SetItemPadding(6);

		if ( !m_GrabCommRedPacket.Create( (LPCTSTR)CGrabCommRedPacket::IDD , &m_tabpool ))
			return -1;

		if ( !m_GrabSpecailRedPacket.Create( (LPCTSTR)CGrabSpecalRedPacket::IDD , &m_tabpool ))
			return -1;

		CRect rc1;
		m_tabpool.GetClientRect(rc1);
		rc1.bottom -= 1;
		rc1.left += 1;
		rc1.top += 30;
		rc1.right -= 2;
		m_GrabCommRedPacket.MoveWindow(&rc1);
		m_GrabSpecailRedPacket.MoveWindow(&rc1);
		m_poopDialog.push_back(&m_GrabCommRedPacket) ;
		m_poopDialog.push_back(&m_GrabSpecailRedPacket) ;
		OnSelectShowPoolWin(0);

		AddListaddrDataBox();

		onShowLink();
		//OnListPool();
		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_REDPACKET_UI ) ;
	}
	return bRes ;
}


void CMortgageTardDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {

		CRect rc ;
		GetClientRect( rc ) ;

		CWnd *pst = GetDlgItem( IDC_TAB1 ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,470 , 60 , 423, 200  ,SWP_SHOWWINDOW ) ; 
		}
		/*pst = GetDlgItem( IDC_BUTTON_UP ) ;
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
		}*/
		pst = GetDlgItem( IDC_BUTTON_REFRESH_1 ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,800 , 60 ,  0 , 0 , SWP_NOSIZE ) ; 
		}
		pst = GetDlgItem( IDC_TAB ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,8 , 278 ,  884 , 200 , SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_REFRESH_2 ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,800 , 279 ,  0 , 0 , SWP_NOSIZE ) ; 
		}

		pst = GetDlgItem( IDC_WINERLOUSER ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,550 , 279 , 0 ,0, SWP_NOSIZE ) ; 
		}

		pst = GetDlgItem( IDC_ONEWINER ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,670 , 279 , 0 ,0, SWP_NOSIZE ) ; 
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

		pst = GetDlgItem( IDC_STATIC_MONEY4 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*32, (rc.Height()/100)*20 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_EDIT_NUM ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 , (rc.Height()/100)*47,  rect.Width() , rect.Height() + 5 , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_EDIT_MONEY ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 , (rc.Height()/100)*38,  rect.Width() , rect.Height() + 5 , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_DW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*39 , (rc.Height()/100)*39,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_COMMRED ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*17 , (rc.Height()/100)*54+2,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_MFCLINK1 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*51 ,  (rc.Height()/100)*59,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_SPECAILRED ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*33 ,  (rc.Height()/100)*54+2,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}	
	}
}

void CMortgageTardDlg::SendRedPacketComm(){

	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!CheckBalance())
	{
		return;
	}
	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;

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
		::MessageBox( this->GetSafeHwnd() ,_T("投注金额大于账户余额") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}


	CReCharge outdlg(NULL,_T("广告语句"),_T("广告词:"),_T("  "));
	outdlg.DoModal();
  
   CString message = theApp.m_strAddress;

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
	CString strrednum;
	int redNum = 0;
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(strrednum) ;
	redNum = atoi(strrednum);

	if (redNum < 2 || redNum >100)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请正确填写红包个数,红包的个数在2-100范围之内") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	double minamout = atof(strTxMoney)/redNum;
	if (minamout < 0.01)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("平均每个红包的金额不能小于0.001") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	string strContractData;
	double money = atof(strTxMoney);
	CString nTemp;
	nTemp.Format(_T("%.8f"),money);
	strContractData = m_RedPacketHelp.PacketSendCommContract((INT64)REAL_MONEY(atof(nTemp)),redNum,message.GetString());

	INT64 strTxFee = theApp.m_RedPacketCfg.SendRedPacketCommFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strShowData = _T("");
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid.GetString(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData ==_T(""))
	{
		return;
	}

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
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( _T("恭喜发送成功，请等待1-2分钟确认交易\n")) ;
	}else{
		strTip.Format( _T("发送红包失败!") ) ;
	}

	//保存到数据库
	if ( bRes ) {

		CString txhash = root["hash"].asCString();
		double amount = atof(nTemp);
		//插入到数据库
		CString strSourceData;
		strSourceData.Format(_T("'%s','%d','%lf','%d' , '%s' , '%d' , '%d'") , \
			                     txhash ,0, amount ,redNum , addr , 0 ,1);

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData.GetString();
		pDatabase.strTabName =  _T("t_red_packets_send");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();
		CString strShow;
		strShow.Format(_T("%s"),strTemp.c_str());
		postmsg.SetData(strShow);
		theApp.m_MsgQueue.push(postmsg);
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}
/// 发送普通红包
void CMortgageTardDlg::OnBnClickedButtonCommred()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	if (atof(strTxMoney)<0.0001)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("投注金额必须大于0.0001") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	CString strDisplay;
	strDisplay.Format(_T("确定要发红包--->金额:%s"), strTxMoney);
	COut outdlg(NULL, strDisplay,100);
	if ( IDOK != outdlg.DoModal()){
		return;
	}
	SendRedPacketComm();
}

BOOL CMortgageTardDlg::AddListaddrDataBox(){

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

void CMortgageTardDlg::OnCbnSelchangeComboAddres()
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
		CString strCommand,strShowData =_T("");

		CString strCond;
		strCommand.Format(_T("%s %s %s"),_T("getappaccinfo") , theApp.m_redPacketScriptid ,text);
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == _T(""))
		{
			return;
		}
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
		CString strText= _T("");
		strText.Format(_T("%.4f"),money);

		//// 应用账户余额
		((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(strText);

		
		pos = strShowData.Find("vFreezedFund");
		if (pos >0)
		{
			Json::Value valuearray = root["vFreezedFund"]; 

			double vFreeAmount = 0.0;
			for(int i =0;i<valuearray.size();i++)
			{
				nMoney = valuearray[i]["value"].asInt64() ;
				vFreeAmount += (nMoney*1.0/COIN);
			}
			if(valuearray.size() >0)
			{
				strShowData.Format(_T("%.4f"),vFreeAmount);
				//// 应用冻结账户余额
				((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(strShowData);
			}else{
				((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(_T("0.0"));
			}
		}
		uistruct::LISTADDR_t pAddr;
		CString condon;
		condon.Format(_T("reg_id = '%s'"),text);
		theApp.m_SqliteDeal.GetWalletAddressItem(condon,&pAddr);
		condon.Format(_T("%.3f"),pAddr.fMoney);
		((CStatic*)GetDlgItem(IDC_STATIC_MONEY4))->SetWindowText(condon);

		Invalidate();

		SetGrabParam();
		ShowListItem(0);
		ShowListItem(1);
		ShowListPoolItem(0);
		ShowListPoolItem(1);
		ShowAllSpecailWinAndLoss();
		ShowAddressSpecailWinAndLoss(text);
	}
}

bool CMortgageTardDlg::CheckBalance()
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
void CMortgageTardDlg::OnBnClickedButtonWithd()
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

	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;

	CString strShowData = _T("");


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


	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid.GetBuffer(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData ==_T(""))
	{
		return;
	}
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


void CMortgageTardDlg::OnBnClickedButtonRech()
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

	if (!CheckRegIDValid( theApp.m_redPacketScriptid )) return ;

	if (theApp.m_strAddress == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	CString strShowData = _T("");
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

	CString strCondition;
	strCondition.Format(_T("reg_id = '%s'"),addr);
	uistruct::LISTADDR_t pAddr;
	theApp.m_SqliteDeal.GetWalletAddressItem(strCondition,&pAddr);
	double sub = pAddr.fMoney - atof(theApp.m_strAddress);
	if (sub <1.0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("系统账户最少余额1smc,作为后续合约交易小费") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CString strCommand , strMaddress , strMoney;

	string strContractData = m_P2PBetHelp.GetReChangContract();

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid.GetBuffer(),addr.GetString(),strContractData,0,strTxFee,REAL_MONEY(atof(theApp.m_strAddress) ));
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == _T(""))
	{
		return;
	}
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

void CMortgageTardDlg::OnSelectShowWin(int nCurSelTab)
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
void  CMortgageTardDlg::OnSelectShowPoolWin(int nCurSelTab)
{
	m_selPooltab = nCurSelTab;
	int nCurSelTabTmp = 0 ;
	std::vector<CDialog*>::iterator itDialog = m_poopDialog.begin();
	for (;itDialog != m_poopDialog.end();itDialog++)
	{  
		if(nCurSelTabTmp == nCurSelTab ){
			(*itDialog)->ShowWindow(SW_SHOW);
		}else{
			(*itDialog)->ShowWindow(SW_HIDE);
		}
		nCurSelTabTmp++ ;
	}
}
void CMortgageTardDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	OnSelectShowWin(m_tab.GetCurSel());
	*pResult = 0;
}
void   CMortgageTardDlg::ShowListItem(int seltab)
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
void  CMortgageTardDlg::ShowListPoolItem(int seltab)
{

	if (seltab == 0)
	{
		m_GrabCommRedPacket.Showlistbox();
	}else if (seltab == 1)
	{
		m_GrabSpecailRedPacket.Showlistbox();
		//m_SendRecord.Showlistbox(addr);
	}
}
void CMortgageTardDlg::InsertComboxIitem()
{
	CPostMsg postmsg;
	if (!theApp.m_UiRedPacketDlgQueue.pop(postmsg))
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
LRESULT CMortgageTardDlg::OnShowListCtrol( WPARAM wParam, LPARAM lParam ) 
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

BOOL CMortgageTardDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)  
	{   
		if(pMsg->wParam == VK_RETURN)  
		{  
			/*if (GetDlgItem(IDC_EDIT_PAGE) == this->GetFocus())
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
			}*/
		}  
	}  
	return CDialogBar::PreTranslateMessage(pMsg);
}


//void CMortgageTardDlg::OnBnClickedButtonUp()
//{
//	// TODO: 在此添加控件通知处理程序代码
//		 OnShowPagePool((m_curpage-1));
//}


//void CMortgageTardDlg::OnBnClickedButtonNext()
//{
//	// TODO: 在此添加控件通知处理程序代码
//		 OnShowPagePool((m_curpage+1));
//}


void CMortgageTardDlg::OnBnClickedButtonRefresh1()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnListPool();
	ShowListPoolItem(m_selPooltab);
}


void CMortgageTardDlg::OnBnClickedButtonRefresh2()
{
	// TODO: 在此添加控件通知处理程序代码
	 ShowListItem(m_seltab);
	 ShowAllSpecailWinAndLoss();
}
void CMortgageTardDlg::OnListPool()
{
	//m_PoolList.clear();
	//m_curpage = 0;
	//theApp.m_SqliteDeal.GetRedPacketPoolRecordList(_T(" 1=1 "), &m_PoolList);
	//m_pagecount = (m_PoolList.size()%m_pagesize)==0?(m_PoolList.size()/m_pagesize):(m_PoolList.size()/m_pagesize)+1;

	//CString temp;
	//temp.Format(_T("共:%d"),m_pagecount);
	//GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp);
	//GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	//Invalidate();
	//m_BonusListBox.DeleteAllIndex();
	//OnShowPagePool(1);
}
void  CMortgageTardDlg::OnShowPagePool(int page)
{
	if (page >m_pagecount || page == m_curpage || page <= 0)
	{
		return;
	}


	//m_BonusListBox.DeleteAllIndex();
	//CString strpage;
	//strpage.Format(_T("%d"),page);
	//GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage);
	//m_curpage = page;
	//int index = (page-1)*m_pagesize;
	//int count = (m_PoolList.size() -index)>=m_pagesize?m_pagesize:(m_PoolList.size() -index);
	//int i =0;
	//for (int k = index;k< (index+count);k++)
	//{
	//	uistruct::REDPACKETPOOL_t const_it = m_PoolList.at(k);

	//	CString strmoney;
	//	strmoney.Format(_T("%.8f"),const_it.total_amount);
	//	CString money;
	//	money.Format(_T("%.4f"),const_it.total_amount);

	//	CString txhash, line;
	//	line.Format(_T("%d"),(i+1));
	//	CString strShow;
	//	if (const_it.packet_type == 1)
	//	{
	//		strShow.Format(_T(("普通红包")));
	//	}else if (const_it.packet_type == 2)
	//	{
	//		strShow.Format(_T(("接龙红包")));
	//	}
	//	m_BonusListBox.InsertStr(i,this->GetSafeHwnd());
	//	m_BonusListBox.SetIndexInage(i , IDB_BITMAP_P2P_LISTBOX_BUT);
	//	m_BonusListBox.SetIndexString(i , line,const_it.send_acc_id, money,strShow,_T("抢"),const_it.send_hash,strmoney);
	//	i++;
	//}
}
void   CMortgageTardDlg::SendRedPackeSpecail(){
	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!CheckBalance())
	{
		return;
	}
	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;

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
		::MessageBox( this->GetSafeHwnd() ,_T("投注金额大于账户余额") , _T("提示") , MB_ICONINFORMATION ) ;
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

	CString strrednum;
	int redNum = 0;
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(strrednum) ;
	redNum = atoi(strrednum);

	if (redNum < 2 || redNum >20)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请正确填写红包个数,接龙红包的个数在2-20范围之内") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	string strContractData;
	double money = atof(strTxMoney);
	CString nTemp;
	nTemp.Format(_T("%.8f"),money);
	strContractData = m_RedPacketHelp.PacketSendSecpailContract((INT64)REAL_MONEY(atof(nTemp)),redNum,_T(""));

	INT64 strTxFee = theApp.m_RedPacketCfg.SendRedPacketSpecailFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strShowData =_T("");
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid.GetString(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == _T(""))
	{
		return;
	}

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
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( _T("恭喜发送成功，请等待1-2分钟确认交易\n")) ;
	}else{
		strTip.Format( _T("发送红包失败!") ) ;
	}

	//保存到数据库
	if ( bRes ) {

		CString txhash = root["hash"].asCString();
		double amount = atof(nTemp);
		//插入到数据库
		CString strSourceData;
		strSourceData.Format(_T("'%s','%s','%lf','%d' , '%s' , '%d' , '%d'") , \
			txhash ,0 , amount ,redNum , addr , 0 ,2);

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData.GetString();
		pDatabase.strTabName =  _T("t_red_packets_send");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();
		CString strShow;
		strShow.Format(_T("%s"),strTemp.c_str());
		postmsg.SetData(strShow);
		theApp.m_MsgQueue.push(postmsg);
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}
void CMortgageTardDlg::OnBnClickedButtonSpecailred()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	if (atof(strTxMoney)<0.0001)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("投注金额必须大于0.0001") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	CString strDisplay;
	strDisplay.Format(_T("确定要发红包--->金额:%s"), strTxMoney);
	COut outdlg(NULL, strDisplay,100);
	if ( IDOK != outdlg.DoModal()){
		return;
	}
	SendRedPackeSpecail();
}
void   CMortgageTardDlg::AcceptRedPacketComm(CString sendhash,uistruct::REDPACKETPOOL_t pPoolList)
{
	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;

	::MessageBox( this->GetSafeHwnd() ,pPoolList.message , _T("提示") , MB_ICONINFORMATION ) ;

	CString walletaddr;
	((CStatic*)GetDlgItem(IDC_STATIC_MONEY4))->GetWindowText(walletaddr);
	INT64 sub = (INT64)(atof(walletaddr)*COIN) - theApp.m_RedPacketCfg.AcceptRedPacketCommFee;
	if (sub < 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("此钱包账户金额不足付小费,请先充值") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
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
	if (strcmp(pPoolList.send_acc_id,addr) == 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("发红包地址不能抢红包") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAcceptRedPacket(addr,pPoolList))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("此地址已经抢过红包") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	string strContractData,strHash;
	strHash= CSoyPayHelp::getInstance()->GetReverseHash(sendhash.GetString());
	strContractData = m_RedPacketHelp.PacketAcceptCommContract(strHash);

	INT64 strTxFee = theApp.m_RedPacketCfg.AcceptRedPacketCommFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strShowData =_T("");
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid.GetString(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == _T(""))
	{
		return;
	}

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
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( _T("抢红包成功，请等待1-2分钟确认交易\n")) ;
	}else{
		strTip.Format( _T("红包已被抢!") ) ;
	}

	//保存到数据库
	if ( bRes ) {

		CString txhash = root["hash"].asCString();
		//插入到数据库
		CString strSourceData;
		strSourceData.Format(_T("'%s','%s','%d','%lf' , '%s' ,'%s' , '%d' , '%d','%d','%lf'") , \
			pPoolList.send_hash ,txhash , 0 ,0.0 ,  pPoolList.send_acc_id ,addr ,0,1,0,pPoolList.total_amount);

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData.GetString();
		pDatabase.strTabName =  _T("t_red_packets_grab");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();
		CString strShow;
		strShow.Format(_T("%s"),strTemp.c_str());
		postmsg.SetData(strShow);
		theApp.m_MsgQueue.push(postmsg);
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}
void   CMortgageTardDlg::AcceptRedPackeSpecail(CString sendhash,uistruct::REDPACKETPOOL_t pPoolList)
{
	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;


	CString walletaddr;
	INT64 sub = (INT64)(atof(walletaddr)*COIN) - theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee;
	if (sub < 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("此钱包账户金额不足付小费,请先充值") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(walletaddr);

	if (atof(walletaddr) < pPoolList.total_amount)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("此钱包账户金额小于接龙红包金额,请先充值") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
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

	if (strcmp(pPoolList.send_acc_id,addr) == 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("发红包地址不能抢红包") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAcceptRedPacket(addr,pPoolList))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("此地址已经抢过红包") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	string strContractData,strHash;
	strHash= CSoyPayHelp::getInstance()->GetReverseHash(sendhash.GetString());
	strContractData = m_RedPacketHelp.PacketAcceptSecpailContract(strHash);

	INT64 strTxFee = theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strShowData =_T("");
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid.GetString(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == _T(""))
	{
		return;
	}

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
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( _T("抢红包成功，请等待1-2分钟确认交易\n")) ;
	}else{
		strTip.Format( _T("红包已被抢!") ) ;
	}

	//保存到数据库
	if ( bRes ) {

		CString txhash = root["hash"].asCString();
		//插入到数据库
		CString strSourceData;
		strSourceData.Format(_T("'%s','%s','%d','%lf' , '%s' ,'%s' , '%d' , '%d','%d','%lf'") , \
			pPoolList.send_hash ,txhash , 0 ,0.0 ,pPoolList.send_acc_id , addr ,0,1,0,pPoolList.total_amount);

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData.GetString();
		pDatabase.strTabName =  _T("t_red_packets_grab");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();
		CString strShow;
		strShow.Format(_T("%s"),strTemp.c_str());
		postmsg.SetData(strShow);
		theApp.m_MsgQueue.push(postmsg);
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}
LRESULT CMortgageTardDlg::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	//List_RedAppendData* pinf = m_BonusListBox.GetAppendDataInfo((int)wParam);
	//if ( NULL != pinf ) { 
	//	CString hash = pinf->pstr;
	//	CString conditon;
	//	conditon.Format(_T("send_hash='%s'"),hash);
	//	uistruct::REDPACKETPOOL_t pPoolList;
	//	theApp.m_SqliteDeal.GetRedPacketPoolItem(conditon, &pPoolList);
	//	if (pPoolList.send_hash.GetLength() != 0)
	//	{
	//		if (pPoolList.packet_type == 1)
	//		{
	//			AcceptRedPacketComm(pPoolList.send_hash,pPoolList);
	//		}else if (pPoolList.packet_type == 2)
	//		{
	//			AcceptRedPackeSpecail(pPoolList.send_hash,pPoolList);
	//		}
	//	}

	//}
	return 0;

}
bool  CMortgageTardDlg::IsAcceptRedPacket(CString account,uistruct::REDPACKETPOOL_t pPoolList)
{
	//for (int i =0;i < pPoolList.packets_num;i++)
	//{
		CString strCommand,strShowData;
		strCommand.Format(_T("%s %s"),_T("gettxdetail") ,pPoolList.send_hash );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == _T(""))
		{
			return false;
		}
		Json::Reader reader; 
		Json::Value root;
		if (!reader.parse(strShowData.GetString(), root)) 
			return false;
		int npos = strShowData.Find("confirmHeight");
		int confirHeight = 1440;
		if ( npos >= 0 ) { //
			confirHeight += root["confirmHeight"].asInt() ;    //交易被确认的高度
		}

		vector<unsigned char>vHash;
		CSoyPayHelp::getInstance()->revert((char*)&confirHeight);

		char key[4];
		memset(key,0,4);

		memcpy(key,&confirHeight,sizeof(confirHeight));
		vHash.assign(key,key+sizeof(key));
		string strKeyHex = CSoyPayHelp::getInstance()->HexStr(vHash);

		vHash =CSoyPayHelp::getInstance()->ParseHex(pPoolList.send_hash.GetString());
		reverse(vHash.begin(),vHash.end());
		string SendHash = CSoyPayHelp::getInstance()->HexStr(vHash);

		CString keyValue;
		keyValue.Format(_T("%s%s"),strKeyHex.c_str(),SendHash.c_str());
		strCommand.Format(_T("%s %s %s"),_T("getscriptdata") ,theApp.m_redPacketScriptid,keyValue );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == _T("") || strShowData.Find("value") < 0)
			return false;

		if (!reader.parse(strShowData.GetString(), root)) 
			return false;;

		CString nValue = root["value"].asCString();
		uistruct::RED_DATA redPacket;
		memset(&redPacket , 0 , sizeof(uistruct::RED_DATA));
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue.GetString());
		memcpy(&redPacket, &vTemp[0], sizeof(uistruct::RED_DATA));

		for (int i =0;i <redPacket.dbdata.takennum;i++)
		{
			uistruct::USER_INFO userinfo = redPacket.userinfo[0];
			std::vector<unsigned char> vSendid;
			vSendid.assign(userinfo.regid,userinfo.regid+sizeof(userinfo.regid));
			string regid  =CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);
			if (strcmp(account,regid.c_str()) == 0)
			{
				return true;
			}
		}

	//}

	return false;
}
void CMortgageTardDlg::SetGrabParam()
{
	CString walletaddr,balance;
	((CStatic*)GetDlgItem(IDC_STATIC_MONEY4))->GetWindowText(walletaddr);
	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	m_GrabCommRedPacket.SetTxt(addr,walletaddr);
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(balance);
	m_GrabSpecailRedPacket.SetTxt(addr,walletaddr,balance);

}

void CMortgageTardDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	OnSelectShowPoolWin(m_tabpool.GetCurSel());
	*pResult = 0;
}


HBRUSH CMortgageTardDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
void  CMortgageTardDlg::onShowLink()
{
	v_linkCtrl.SetWindowText(_T("帮助"));
	v_linkCtrl.SetURL("http://www.dacrs.com/forum.php?mod=viewthread&tid=3503&extra=page%3D1");
}

double  CMortgageTardDlg::ComputeSpecailRedPacket(uistruct::REDPACKETGRABLIST  RedPackeGrabRecordList)
{
	//if (!theApp.IsSyncBlock)
	//{
	//	return;
	//}
	double result = 0.0;

	std::vector<uistruct::REDPACKETGRAB_t>::const_iterator const_it;
	for (const_it = RedPackeGrabRecordList.begin() ; const_it != RedPackeGrabRecordList.end() ; const_it++ ) {
		if (const_it->lucky_fortune == 2)
		{
			result -= const_it->total_amount;
			result += const_it->lucky_amount;
		}else if (const_it->lucky_fortune == 1)
		{
			result += const_it->lucky_amount;
		}
	}
	return result;
}

void    CMortgageTardDlg::ShowAllSpecailWinAndLoss(){
	uistruct::REDPACKETGRABLIST  RedPackeGrabRecordList;
	theApp.m_SqliteDeal.GetRedPacketGrabRecordList(_T(" packet_type = 2 "), &RedPackeGrabRecordList);
	double result = ComputeSpecailRedPacket(RedPackeGrabRecordList);
	CString show;
	show.Format(_T("接龙总盈亏:%.4f"),result);
	m_rBtnWinerloser.SetWindowText(show);
	m_rBtnWinerloser.Invalidate();
}
void   CMortgageTardDlg::ShowAddressSpecailWinAndLoss(CString addr)
{
	CString condtion;
	condtion.Format(_T("grab_acct_id = '%s'and packet_type = 2 "),addr);

	uistruct::REDPACKETGRABLIST  RedPackeGrabRecordList;
	theApp.m_SqliteDeal.GetRedPacketGrabRecordList(condtion, &RedPackeGrabRecordList);
	double result = ComputeSpecailRedPacket(RedPackeGrabRecordList);
	CString show;
	show.Format(_T("盈亏:%.4f"),result);
	m_rBtnAddrWinerloser.SetWindowText(show);
	m_rBtnAddrWinerloser.Invalidate();
}