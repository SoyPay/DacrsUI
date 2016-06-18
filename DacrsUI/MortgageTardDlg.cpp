// MortgageTardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "MortgageTardDlg.h"
#include "afxdialogex.h"
#include "Out.h"
#include "ReCharge.h"
#include "RpcCmd.h"
#include "CommonAddr.h"
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
	v_linkCtrl = NULL;
}

CMortgageTardDlg::~CMortgageTardDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}	
   if (v_linkCtrl != NULL)
   {
	   delete v_linkCtrl;
	   v_linkCtrl = NULL;
   }
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
	DDX_Control(pDX, IDC_BUTTON_REFRESH_1, m_rBtnRefresh1);
	DDX_Control(pDX, IDC_BUTTON_REFRESH_2, m_rBtnRefresh2);
	DDX_Control(pDX, IDC_TAB, m_tab);
	DDX_Control(pDX, IDC_TAB1, m_tabpool);

	DDX_Control(pDX ,IDC_STATIC_MONEY4 ,m_money ) ;
	
	DDX_Control(pDX, IDC_WINERLOUSER, m_rBtnWinerloser);
	DDX_Control(pDX, IDC_ONEWINER, m_rBtnAddrWinerloser);

	DDX_Control(pDX, IDC_BUTTON_SETADDR, m_rBtnSetCommonAddr);
}


BEGIN_MESSAGE_MAP(CMortgageTardDlg, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_COMMRED, &CMortgageTardDlg::OnBnClickedButtonCommred)
	ON_CBN_SELCHANGE(IDC_COMBO_ADDRES, &CMortgageTardDlg::OnCbnSelchangeComboAddres)
	ON_BN_CLICKED(IDC_BUTTON_WITHD, &CMortgageTardDlg::OnBnClickedButtonWithd)
	ON_BN_CLICKED(IDC_BUTTON_RECH, &CMortgageTardDlg::OnBnClickedButtonRech)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CMortgageTardDlg::OnTcnSelchangeTab)
	ON_MESSAGE(MSG_USER_REDPACKET_UI , &CMortgageTardDlg::OnShowListCtrol )

	ON_BN_CLICKED(IDC_BUTTON_REFRESH_1, &CMortgageTardDlg::OnBnClickedButtonRefresh1)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_2, &CMortgageTardDlg::OnBnClickedButtonRefresh2)
	ON_BN_CLICKED(IDC_BUTTON_SPECAILRED, &CMortgageTardDlg::OnBnClickedButtonSpecailred)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMortgageTardDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_SETADDR, &CMortgageTardDlg::OnBnClickedButtonSetaddr)
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
	//SetBkBmpNid(IDB_BITMAP_REDPACKET);
	SetBkBmpNid(UiFun::GetBmpId(IDB_BITMAP_REDPACKET,IDB_BITMAP_REDPACKETEN,theApp.language()));

	if (v_linkCtrl == NULL)
	{
		v_linkCtrl = new CMFCLinkCtrl;
		v_linkCtrl->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 80, 50), 
			this, 
			IDC_MFCLINK1);
	}
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
		m_rBtnMale.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_GENERAL_ENVELOPE" ,theApp.gsLanguage)) ;
		m_rBtnMale.SetFontEx(23 , _T("微软雅黑"));
		m_rBtnMale.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 182, 0));
		m_rBtnMale.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnMale.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 182, 0));
		m_rBtnMale.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnMale.SizeToContent();

		m_rBtnWoman.SetBitmaps( IDB_BITMAP_REDPACKET_TYPE , RGB(255, 255, 0) , IDB_BITMAP_REDPACKET_TYPE , RGB(255, 255, 255) );
		m_rBtnWoman.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWoman.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_RED_ENVELOPE" ,theApp.gsLanguage)) ;
		m_rBtnWoman.SetFontEx(23 , _T("微软雅黑"));
		m_rBtnWoman.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 182, 0));
		m_rBtnWoman.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnWoman.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 182, 0));
		m_rBtnWoman.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnWoman.SizeToContent();


		m_rBtnWithd.SetBitmaps( IDB_BITMAP_REDPACKETDRAW , RGB(255, 255, 0) , IDB_BITMAP_REDPACKETDRAW , RGB(255, 255, 255) );
		m_rBtnWithd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWithd.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS" ,theApp.gsLanguage)) ;
		m_rBtnWithd.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 182, 0));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 182, 0));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnWithd.SizeToContent();

		m_rBtnRech.SetBitmaps( IDB_BITMAP_REDRCHANGE , RGB(255, 76, 0) , IDB_BITMAP_REDRCHANGE , RGB(255, 76, 0) );
		m_rBtnRech.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRech.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_RECHARGE" ,theApp.gsLanguage)) ;
		m_rBtnRech.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 76, 0));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 76, 0));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 76, 0));
		m_rBtnRech.SizeToContent();

		m_rBtnRefresh1.SetBitmaps( IDB_BITMAP_RED_FRESH , RGB(255, 255, 0) , IDB_BITMAP_RED_FRESH , RGB(255, 255, 255) );
		m_rBtnRefresh1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh1.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_REFRESH" ,theApp.gsLanguage)) ;
		m_rBtnRefresh1.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 76, 0));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 76, 0));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 76, 0));
		m_rBtnRefresh1.SizeToContent();

		m_rBtnRefresh2.SetBitmaps( IDB_BITMAP_RED_FRESH , RGB(255, 255, 0) , IDB_BITMAP_RED_FRESH , RGB(255, 255, 255) );
		m_rBtnRefresh2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh2.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_REFRESH" ,theApp.gsLanguage)) ;
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

		m_rBtnSetCommonAddr.SetBitmaps( IDB_BITMAP_REDPACKETDRAW , RGB(255, 255, 0) , IDB_BITMAP_REDPACKETDRAW , RGB(255, 255, 255) );
		m_rBtnSetCommonAddr.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnSetCommonAddr.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_COMM_ADRESS" ,theApp.gsLanguage)) ;
		m_rBtnSetCommonAddr.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnSetCommonAddr.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 182, 0));
		m_rBtnSetCommonAddr.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 251, 240));
		m_rBtnSetCommonAddr.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 182, 0));
		m_rBtnSetCommonAddr.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnSetCommonAddr.SizeToContent();

		m_money.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_money.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_money.SetWindowText(_T(""));
		m_tab.SetBitmap(IDB_BITMAP_P2PBUTTON_2,2);

		m_tab.InsertItem(0,UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_GRAD_ENVELOPES" ,theApp.gsLanguage));  //添加参数一选项卡 
		m_tab.InsertItem(1,UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_SEND_ENVELOPES" ,theApp.gsLanguage));  //添加参数二选项卡 

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

		m_tabpool.InsertItem(0,UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_GENERAL_ENVELOPE" ,theApp.gsLanguage));  //添加参数一选项卡 
		m_tabpool.InsertItem(1,UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_RED_ENVELOPE" ,theApp.gsLanguage));  //添加参数二选项卡 

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
			pst->SetWindowPos( NULL ,(rc.Width()/100)*13 , (rc.Height()/100)*8 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_NOT_DRAW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*13 , (rc.Height()/100)*14 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_WITHD ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*28 , (rc.Height()/100)*9 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
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
			pst->SetWindowPos( NULL ,(rc.Width()/100)*12+5 , (rc.Height()/100)*20 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW); 
			
			CComboBox*   pCtl  = (CComboBox*)pst; 
			CWnd *p_edit = pCtl->GetDlgItem (0x3E9);
			p_edit->GetClientRect( rect );
			p_edit->SetWindowPos( NULL, (rc.Width()/100)*25, (rc.Height()/100)*25-3, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
		    pCtl->SetFont (&theApp.m_fontBlackbody);
			p_edit->SetFont (&theApp.m_fontBlackbody);

			pCtl->SendMessage(CB_SETITEMHEIGHT,(WPARAM)-1,(LPARAM)20);   //改变控件本身的高度
			pCtl->SetItemHeight(0,15);									 //改变下拉列表每个Item的高度
		}

		pst = GetDlgItem( IDC_STATIC_MONEY4 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			//pst->SetWindowPos( NULL ,(rc.Width()/100)*32, (rc.Height()/100)*20 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW); 
			pst->SetWindowPos( NULL ,(rc.Width()/100)*28+2, (rc.Height()/100)*20+2 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW );
		}

		pst = GetDlgItem( IDC_BUTTON_SETADDR ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*42, (rc.Height()/100)*18 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW );
		}

		pst = GetDlgItem( IDC_EDIT_NUM ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 , (rc.Height()/100)*47,  rect.Width() , rect.Height() + 5 , SWP_SHOWWINDOW); 
			pst->SetFont(&theApp.m_fontBlackbody);
		}

		pst = GetDlgItem( IDC_EDIT_MONEY ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 , (rc.Height()/100)*38,  rect.Width() , rect.Height() + 5 , SWP_SHOWWINDOW);
			pst->SetFont(&theApp.m_fontBlackbody);
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
			pst->SetWindowPos( NULL ,(rc.Width()/100)*51 ,  (rc.Height()/100)*59, 30 , 20, SWP_SHOWWINDOW ) ; 
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
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_UNFINISHED_BUSINESS" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;


	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	

	CReCharge outdlg(NULL,UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_AD_STATEMENT" ,theApp.gsLanguage),UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_AD_WORD" ,theApp.gsLanguage),_T("  "));
	if (outdlg.DoModal() == IDCANCEL)
	{
		return;
	}
  
   CString message = theApp.m_strAddress;

	CString addr;
	int sel = m_addrbook.GetCurSel();

	m_addrbook.GetLBText(sel,addr);

	CString strrednum;
	int redNum = 0;
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(strrednum) ;
	redNum = atoi(strrednum);

	string strContractData;
	double money = strtod(strTxMoney,NULL);
	CString nTemp;
	nTemp.Format(_T("%.8f"),money);
	strContractData = m_RedPacketHelp.PacketSendCommContract((INT64)REAL_MONEY(strtod(nTemp,NULL)),redNum,message.GetString());

	INT64 strTxFee = theApp.m_RedPacketCfg.SendRedPacketCommFee;
	if (  strTxFee < 10000  ) {
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_TIP_INSU" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	string strShowData = "";
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid,addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData =="")
	{
		return;
	}

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash =root["hash"].asString() ;
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_SEND_SUCCESS" ,theApp.gsLanguage);
	}else{
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_SENDRED_FAIL" ,theApp.gsLanguage) ;
	}

	//保存到数据库
	if ( bRes ) {

		string txhash = root["hash"].asString();
		double amount = strtod(nTemp,NULL);
		//插入到数据库
		string strSourceData;
		strSourceData= strprintf("'%s','%d','%lf','%d' , '%s' , '%d' , '%d'" , \
			                     txhash ,0, amount ,redNum , addr , 0 ,1);

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData;
		pDatabase.strTabName =  _T("t_red_packets_send");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();

		postmsg.SetData(strTemp);
		theApp.m_MsgQueue.push(postmsg);
	}
	UiFun::MessageBoxEx(strTip.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
}
/// 发送普通红包
void CMortgageTardDlg::OnBnClickedButtonCommred()
{
	// TODO: 在此添加控件通知处理程序代码

	string strshow= UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_UPDATATIP" ,theApp.gsLanguage);
	if (!UiFun::IsCurrentAppId(theApp.m_redPacketScriptid.c_str(),theApp.m_neststcriptid.strNewSrcriptRedPacektid.c_str(),strshow))
	{
		return;
	}

	if (!CheckBalance())
	{
		return;
	}

	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double balance =strtod(strMoney,NULL);

	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_MONEY_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	if (strtod(strTxMoney,NULL) > balance)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_REDMONEY_MORE_BALANCE" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	if (strtod(strTxMoney,NULL)<1)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_REDMONEY_MORE_1" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	CString strrednum;
	int redNum = 0;
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(strrednum) ;
	redNum = atoi(strrednum);

	if (redNum < 2 || redNum >100)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_RED_COUNT" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	double minamout = strtod(strTxMoney,NULL)/redNum;
	if (minamout < 1.0) /// 平均每个热你的红包吧不能少于1
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_AGERED_COUNT_1" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_SELECT_ADDRESS" ,theApp.gsLanguage), UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	CString strDisplay;
	strDisplay.Format(_T("%s:%s"),UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_GRADNUM_MONEY" ,theApp.gsLanguage), strTxMoney);

	if (IDCANCEL == UiFun::MessageBoxEx(strDisplay , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_OKCANCEL|MFB_TIP ) )
	{
		return;
	}
	SendRedPacketComm();
}

BOOL CMortgageTardDlg::AddListaddrDataBox(){

	map<int,uistruct::COMMONLISTADDR_t> m_mapCommonAddrInfo;
	string conditon =strprintf("app_id ='%s'",theApp.m_redPacketScriptid);
	theApp.m_SqliteDeal.GetCommonWalletAddressList(conditon, &m_mapCommonAddrInfo);

	//map<string,uistruct::LISTADDR_t> m_mapAddrInfo;
	//theApp.m_SqliteDeal.GetWalletAddressList(_T(" sign=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapCommonAddrInfo.size() ) return FALSE ;

	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->ResetContent();
	//加载到ComBox控件
	int nItem = 0;
	std::map<int,uistruct::COMMONLISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapCommonAddrInfo.begin() ; const_it != m_mapCommonAddrInfo.end() ; const_it++ ) {

		((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->InsertString(nItem , const_it->second.reg_id.c_str() );
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->SetCurSel(0);

	CString address;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		//UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
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
		//UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	m_addrbook.GetLBText(sel,text);

	//m_addrbook.GetWindowText(text) ;
	if(text!=_T(""))
	{
		string strCommand,strShowData =("");

		string strCond;
		strCommand =strprintf("%s %s %s",_T("getappaccinfo") , theApp.m_redPacketScriptid ,text);
		Json::Value root; 
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("UpdateAddressData rpccmd listaddr error");
			return;
		}
		strShowData = root.toStyledString();

		int pos = strShowData.find("FreeValues");
		INT64 nMoney = 0;
		if (pos >0)
		{
			nMoney = root["FreeValues"].asInt64() ;
		}
		double money = (nMoney*1.0/COIN);
		string strText= _T("");
		//strText = strprintf("%.4f",money);
		strText = strprintf("%.8f",money);
		strText = strText.substr(0,strText.length()-4);
		//// 应用账户余额
		((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(strText.c_str());

		
		pos = strShowData.find("vFreezedFund");
		if (pos >0)
		{
			Json::Value valuearray = root["vFreezedFund"]; 

			double vFreeAmount = 0.0;
			for(unsigned int i =0;i<valuearray.size();i++)
			{
				nMoney = valuearray[i]["value"].asInt64() ;
				vFreeAmount += (nMoney*1.0/COIN);
			}
			if(valuearray.size() >0)
			{
				strShowData = strprintf("%.4f",vFreeAmount);
				//// 应用冻结账户余额
				((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(strShowData.c_str());
			}else{
				((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(_T("0.0"));
			}
		}
		uistruct::LISTADDR_t pAddr;
		string condon;
		condon = strprintf("reg_id = '%s'",text);
		theApp.m_SqliteDeal.GetWalletAddressItem(condon,&pAddr);
		condon = strprintf("%.3f",pAddr.fMoney);
		((CStatic*)GetDlgItem(IDC_STATIC_MONEY4))->SetWindowText(condon.c_str());

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

bool CMortgageTardDlg::CheckBalance(CString strshow)
{
	OnCbnSelchangeComboAddres();
	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double money =strtod(strMoney,NULL);
	if (money == 0.0)
	{
		if (strshow == "")
		{
			UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_ACCOUNT_RECHARGE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage),MFB_OK|MFB_TIP );
		}else{
			UiFun::MessageBoxEx(strshow ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		}
		
		return false;
	}
	return true;
}
void CMortgageTardDlg::OnBnClickedButtonWithd()
{
	if (strcmp(theApp.m_redPacketScriptid.c_str(),theApp.m_neststcriptid.strNewSrcriptRedPacektid.c_str()) == 0)
	{
		GetAppAccountSomeMoney();
		return;
	}
	// TODO: 在此添加控件通知处理程序代码

	if (!theApp.IsSyncBlock )
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_UNFINISHED_BUSINESS" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_ADDRESS_NOT_NULL" ,theApp.gsLanguage), UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	if (!CheckBalance("账户金额为零,不能提现"))
	{
		return;
	}

	if ( IDNO == UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_CONFIRM_WITHDRAWALS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_YESNO|MFB_TIP ) )
		return;

	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;

	string strShowData = _T("");

	CString Money = _T("");
	GetDlgItem(IDC_STATIC_BALANCE)->GetWindowText(Money);

	double dmoney = strtod(Money,NULL);

	string strContractData = m_P2PBetHelp.GetAppAccountMoneyContract(addr.GetString(),1,1,REAL_MONEY(dmoney));

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_TIP_INSU" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}


	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid,addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData =="")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash =root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜提现成功!\n%s") , root["hash"].asCString() ) ;
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_WITHDRAWALS_SUCCESS" ,theApp.gsLanguage);
	}else{
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_FAIL" ,theApp.gsLanguage) ;
	}
	UiFun::MessageBoxEx(strTip.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
}


void CMortgageTardDlg::OnBnClickedButtonRech()
{
	// TODO: 在此添加控件通知处理程序代码
	string strshow= "红包已经升级,请到菜单栏中选择恢复默认设置";
	if (!UiFun::IsCurrentAppId(theApp.m_redPacketScriptid.c_str(),theApp.m_neststcriptid.strNewSrcriptRedPacektid.c_str(),strshow))
	{
		return;
	}

	if (!theApp.IsSyncBlock )
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_UNFINISHED_BUSINESS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	CReCharge outdlg;
	if ( IDOK != outdlg.DoModal()){
		return;
	}

	if (!CheckRegIDValid( theApp.m_redPacketScriptid )) return ;

	if (theApp.m_strAddress == _T(""))
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_MONEY_NOT_NULL" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	string strShowData = _T("");
	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_ADDRESS_NOT_NULL" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	string strCondition;
	strCondition = strprintf("reg_id = '%s'",addr);
	uistruct::LISTADDR_t pAddr;
	theApp.m_SqliteDeal.GetWalletAddressItem(strCondition,&pAddr);
	double sub = pAddr.fMoney - strtod(theApp.m_strAddress,NULL);
	if (sub <1.0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_TRDING_TIPS" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}


	string strContractData = m_P2PBetHelp.GetReChangContract();

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_TIP_INSU" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid,addr.GetString(),strContractData,0,strTxFee,REAL_MONEY(strtod(theApp.m_strAddress,NULL) ));
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash =root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜充值成功!\n%s") , root["hash"].asCString() ) ;
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_RECHARGE_SUCCESS" ,theApp.gsLanguage) ;
	}else{
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_RECHARGE_FAIL" ,theApp.gsLanguage) ;
	}
	UiFun::MessageBoxEx(strTip.c_str()  , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage),MFB_OK|MFB_TIP );
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
		m_GrabCommRedPacket.ReadComRedPacketPool();
	}else if (seltab == 1)
	{
		m_GrabSpecailRedPacket.ReadSpecailRedPacketPool();
		//m_SendRecord.Showlistbox(addr);
	}
}

LRESULT CMortgageTardDlg::OnShowListCtrol( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_NEWADDRESS:
		{
			OnUpAddressCombo();
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
	
		}  
	}  
	return CDialogBar::PreTranslateMessage(pMsg);
}


void CMortgageTardDlg::OnBnClickedButtonRefresh1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowListPoolItem(m_selPooltab);
}


void CMortgageTardDlg::OnBnClickedButtonRefresh2()
{
	// TODO: 在此添加控件通知处理程序代码
	 ShowListItem(m_seltab);
	 ShowAllSpecailWinAndLoss();
}

void   CMortgageTardDlg::SendRedPackeSpecail(){
	if (!theApp.IsSyncBlock )
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_UNFINISHED_BUSINESS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;

	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;

	CString addr;
	int sel = m_addrbook.GetCurSel();

	if (sel <0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("P2P_MODULE" , "P2P_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);


	CString strrednum;
	int redNum = 0;
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(strrednum) ;
	redNum = atoi(strrednum);

	string strContractData;
	double money = strtod(strTxMoney,NULL);
	CString nTemp;
	nTemp.Format(_T("%.8f"),money);
	strContractData = m_RedPacketHelp.PacketSendSecpailContract((INT64)REAL_MONEY(strtod(nTemp,NULL)),redNum,_T(""));

	INT64 strTxFee = theApp.m_RedPacketCfg.SendRedPacketSpecailFee;
	if (  strTxFee < 10000  ) {
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_TIP_INSU" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	string strShowData =_T("");
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid,addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData == "")
	{
		return;
	}

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash =  root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
		strTip.Format(  UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_SEND_SUCCESS" ,theApp.gsLanguage) ) ;
	}else{
		strTip.Format( UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_SENDRED_FAIL" ,theApp.gsLanguage) ) ;
	}

	//保存到数据库
	if ( bRes ) {

		string txhash = root["hash"].asString();
		double amount = strtod(nTemp,NULL);
		//插入到数据库
		string strSourceData;
		strSourceData = strprintf("'%s','%s','%lf','%d' , '%s' , '%d' , '%d'" , \
			txhash ,0 , amount ,redNum , addr , 0 ,2);

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData;
		pDatabase.strTabName =  _T("t_red_packets_send");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();

		postmsg.SetData(strTemp);
		theApp.m_MsgQueue.push(postmsg);
	}
	UiFun::MessageBoxEx(strTip ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
}
void CMortgageTardDlg::OnBnClickedButtonSpecailred()
{
	// TODO: 在此添加控件通知处理程序代码

	string strshow= UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_UPDATATIP" ,theApp.gsLanguage);
	if (!UiFun::IsCurrentAppId(theApp.m_redPacketScriptid.c_str(),theApp.m_neststcriptid.strNewSrcriptRedPacektid.c_str(),strshow))
	{
		return;
	}

	if (!CheckBalance())
	{
		return;
	}

	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double balance =strtod(strMoney,NULL);

	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		UiFun::MessageBoxEx( UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_MONEY_NOT_NULL" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	if (strtod(strTxMoney,NULL) > balance)
	{
		UiFun::MessageBoxEx( UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_REDMONEY_MORE_BALANCE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	if (strtod(strTxMoney,NULL)<10.0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_REDMONEY_MORE_10" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );

		return ;
	}

	CString strrednum;
	int redNum = 0;
	GetDlgItem(IDC_EDIT_NUM)->GetWindowText(strrednum) ;
	redNum = atoi(strrednum);

	if (redNum < 2 || redNum >20)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_REDMONEY_COUNT" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	double minamout = strtod(strTxMoney,NULL)/redNum;
	if (minamout < 1.0) /// 平均每个热你的红包吧不能少于1
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_AGERED_COUNT_1" ,theApp.gsLanguage), UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_SELECT_ADDRESS" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	CString strDisplay;
	strDisplay.Format(_T("%s:%s"), UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_GRADNUM_MONEY" ,theApp.gsLanguage) ,strTxMoney);

	if (IDCANCEL == UiFun::MessageBoxEx(strDisplay , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_OKCANCEL|MFB_TIP ) )
	{
		return;
	}
	SendRedPackeSpecail();
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

void  CMortgageTardDlg::onShowLink()
{
	v_linkCtrl->SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_HELP" ,theApp.gsLanguage));
	v_linkCtrl->SetURL("http://www.dacrs.com/forum.php?mod=viewthread&tid=3503&extra=page%3D1");
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
	string show;
	show = strprintf("%s:%.4f",UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_SOLITAIRETOTAL_PROFIT_LOSS" ,theApp.gsLanguage) ,result);
	m_rBtnWinerloser.SetWindowText(show.c_str());
	m_rBtnWinerloser.Invalidate();
}
void   CMortgageTardDlg::ShowAddressSpecailWinAndLoss(CString addr)
{
	string condtion;
	condtion =strprintf("grab_acct_id = '%s'and packet_type = 2 ",addr);

	uistruct::REDPACKETGRABLIST  RedPackeGrabRecordList;
	theApp.m_SqliteDeal.GetRedPacketGrabRecordList(condtion, &RedPackeGrabRecordList);
	double result = ComputeSpecailRedPacket(RedPackeGrabRecordList);
	string show;
	show = strprintf("%s:%.4f",UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_TOTAL_PROFIT" ,theApp.gsLanguage),result);
	m_rBtnAddrWinerloser.SetWindowText(show.c_str());
	m_rBtnAddrWinerloser.Invalidate();
}

bool CMortgageTardDlg::CheckBalance(double dmoney)
{
	OnCbnSelchangeComboAddres();
	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double money =strtod(strMoney,NULL);
	if (money == 0.0)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_ACCOUNT_RECHARGE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return false;
	}

	if (dmoney >money)
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_MORE_BALANCE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return false;
	}
	return true;
}
void  CMortgageTardDlg::GetAppAccountSomeMoney()
{

	if (!theApp.IsSyncBlock )
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_UNFINISHED_BUSINESS" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}


	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	if (!CheckBalance(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_NOTWIHTDRAW" ,theApp.gsLanguage)))
	{
		return;
	}
	CString balance ="";

	GetDlgItem(IDC_STATIC_BALANCE)->GetWindowText(balance);
	CReCharge outdlg(NULL,UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS1" ,theApp.gsLanguage),UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_MONEY" ,theApp.gsLanguage),"",balance);
	if ( IDOK != outdlg.DoModal()){
		return;
	}

	if (theApp.m_strAddress == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	double money = strtod(theApp.m_strAddress,NULL);
	if (money <0.0000000000000001)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		return ;
	}

	if (!CheckBalance(money))
	{
		return;
	}
	string show = strprintf("%s:%lf",UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_OK_WITHDRAWALS" ,theApp.gsLanguage) ,money);
	if ( IDNO == UiFun::MessageBoxEx(show.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage), MFB_YESNO|MFB_TIP ) )
		return;

	string strShowData ="";


	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_ADDRESS_NOT_NULL" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}


	string strContractData = m_P2PBetHelp.GetAppAccountSomeMoneyContract(addr.GetString(),3,1,REAL_MONEY(money));

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_TIP_INSU" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}


	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid,addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash =  root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜提现成功!\n%s") , root["hash"].asCString() ) ;
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_WITHDRAWALS_SUCCESS" ,theApp.gsLanguage) ;
	}else{
		strTip = UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_WITHDRAWALS_FAIL" ,theApp.gsLanguage) ;
	}
	
	UiFun::MessageBoxEx(strTip.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
}


void CMortgageTardDlg::OnBnClickedButtonSetaddr()
{
	// TODO: 在此添加控件通知处理程序代码
	CCommonAddr addDlg(UI_READPACKET_RECORD);
	addDlg.DoModal() ;
}
void CMortgageTardDlg::OnUpAddressCombo() 
{
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->ResetContent(); //清除ComBox控件
	map<int,uistruct::COMMONLISTADDR_t> m_mapCommonAddrInfo;
	string conditon =strprintf("app_id ='%s'",theApp.m_redPacketScriptid);
	theApp.m_SqliteDeal.GetCommonWalletAddressList(conditon, &m_mapCommonAddrInfo);

	if ( 0 == m_mapCommonAddrInfo.size() ) return  ;

	int nItem = 0;  //加载到ComBox控件
	std::map<int,uistruct::COMMONLISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapCommonAddrInfo.begin() ; const_it != m_mapCommonAddrInfo.end() ; const_it++ ) {

		((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->InsertString(nItem , const_it->second.reg_id.c_str() );
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->SetCurSel(0);
	return  ;
}