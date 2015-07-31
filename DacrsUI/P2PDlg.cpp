// P2PDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "P2PDlg.h"
#include "afxdialogex.h"
#include "Out.h"
#include "ReCharge.h"
#include "GuessNum.h"
#include "RpcCmd.h"

#define OUT_HEIGHT  60

// CP2PDlg �Ի���

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

	v_linkCtrl.InternalRelease();
	v_linkCtrl.ExternalRelease();
	v_linkCtrl.OnFinalRelease();
	v_linkCtrl.DestroyWindow();
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
	DDX_Control(pDX ,IDC_STATIC_MONEY ,m_money ) ;
	DDX_Control(pDX, IDC_MFCLINK1, v_linkCtrl);

	DDX_Control(pDX, IDC_WINERLOSER, m_rBtnWinerloser);
	DDX_Control(pDX, IDC_ONEWINER, m_rBtnAddrWinerloser);

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
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST_BONUS, &CP2PDlg::OnLbnDblclkListBonus)
END_MESSAGE_MAP()


// CP2PDlg ��Ϣ�������
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

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid(IDB_BITMAP_P2P_BJ);
	return 0;
}


BOOL CP2PDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnMale.LoadBitmaps(IDB_BITMAP_SISTER_1,IDB_BITMAP_SISTER_3,IDB_BITMAP_SISTER_2,IDB_BITMAP_SISTER_1);
		m_rBtnWoman.LoadBitmaps(IDB_BITMAP_BOTHER_1,IDB_BITMAP_BOTHER_3,IDB_BITMAP_BOTHER_2,IDB_BITMAP_BOTHER_1);
		UpdateData(0);
		m_Balance.SetFont(120, _T("����"));				//������ʾ����ʹ�С
		m_Balance.SetTextColor(RGB(0,0,0));			    //������ɫ	
		m_Balance.SetWindowText(_T(""));

		m_NotDraw.SetFont(120, _T("����"));				//������ʾ����ʹ�С
		m_NotDraw.SetTextColor(RGB(0,0,0));			    //������ɫ	
		m_NotDraw.SetWindowText(_T(""));

		m_Dw.SetFont(120, _T("����"));				//������ʾ����ʹ�С
		m_Dw.SetTextColor(RGB(0,0,0));			    //������ɫ	
		m_Dw.SetWindowText(_T("SMC"));

		m_VS.SetFont(120, _T("����"));				//������ʾ����ʹ�С
		m_VS.SetTextColor(RGB(0,0,0));			    //������ɫ	
		m_VS.SetWindowText(_T(" VS"));

		m_sCountpage.SetFont(90, _T("����"));				//������ʾ����ʹ�С
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //������ɫ	
		m_sCountpage.SetWindowText(_T("��:"));

		m_rBtnWithd.SetBitmaps( IDB_BITMAP_P2PBUTTON_1 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_1 , RGB(255, 255, 255) );
		m_rBtnWithd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWithd.SetWindowText("����") ;
		m_rBtnWithd.SetFontEx(20 , _T("΢���ź�"));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnWithd.SizeToContent();

		m_rBtnRech.SetBitmaps( IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 255) );
		m_rBtnRech.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRech.SetWindowText("��ֵ") ;
		m_rBtnRech.SetFontEx(20 , _T("΢���ź�"));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRech.SizeToContent();

		m_rBtnRefresh1.SetBitmaps( IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 255) );
		m_rBtnRefresh1.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh1.SetWindowText("ˢ��") ;
		m_rBtnRefresh1.SetFontEx(20 , _T("΢���ź�"));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRefresh1.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRefresh1.SizeToContent();

		m_rBtnRefresh2.SetBitmaps( IDB_BITMAP_BUTTON3 , RGB(255, 255, 0) , IDB_BITMAP_BUTTON3 , RGB(255, 255, 255) );
		m_rBtnRefresh2.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRefresh2.SetWindowText("ˢ��") ;
		m_rBtnRefresh2.SetFontEx(20 , _T("΢���ź�"));
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

		m_money.SetFont(120, _T("����"));				//������ʾ����ʹ�С
		m_money.SetTextColor(RGB(0,0,0));			    //������ɫ	
		m_money.SetWindowText(_T(""));
		

		m_tab.InsertItem(0,_T("�ӵ���¼"));  //��Ӳ���һѡ� 
		m_tab.InsertItem(1,_T("�����¼"));  //��Ӳ�����ѡ� 

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

		onShowLink();


		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_P2P_UI ) ;

		/// ���ö�ʱ��ˢ�½��� һ����
		//SetTimer(1,30000,NULL);
		SetTimer(1,60000,NULL);
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

	// TODO: �ڴ˴������Ϣ����������
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

		pst = GetDlgItem( IDC_WINERLOSER ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,550 , 270 , 0 ,0, SWP_NOSIZE ) ; 
		}

		pst = GetDlgItem( IDC_ONEWINER ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,670 , 270 , 0 ,0, SWP_NOSIZE ) ; 
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
		
		pst = GetDlgItem( IDC_STATIC_MONEY ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*32, (rc.Height()/100)*20 ,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
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

		pst = GetDlgItem( IDC_MFCLINK1 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*51 ,  (rc.Height()/100)*59,  rect.Width() , rect.Height() , SWP_SHOWWINDOW ) ; 
		}

		

	}
}


void CP2PDlg::OnCbnSelchangeComboAddres()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		string strCommand,strShowData =_T("");

		string strCond;
		strCommand = strprintf("%s %s %s","getappaccinfo" , theApp.m_betScritptid ,text);
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(strShowData, root)) 
			return  ;

		int pos = strShowData.find("FreeValues");
		INT64 nMoney = 0;
		if (pos >0)
		{
			nMoney = root["FreeValues"].asInt64() ;
		}
		double money = (nMoney*1.0/COIN);
		strShowData = strprintf("%.4f",money);

		((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(strShowData.c_str());

		uistruct::LISTADDR_t pAddr;
		string condon;
		condon = strprintf("reg_id = '%s'",text);
		theApp.m_SqliteDeal.GetWalletAddressItem(condon,&pAddr);
		condon = strprintf("%.3f",pAddr.fMoney);
		((CStatic*)GetDlgItem(IDC_STATIC_MONEY))->SetWindowText(condon.c_str());

		Invalidate();
	}

	QueryNotDrawBalance(text);          /// blockchanged ˢ��Ϊ����������
	ShowListItem(0);
	ShowListItem(1);
	ShowAllBetWinAndLoss();
	ShowAddressBetWinAndLoss(text);
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

	string addressd;
	addressd =strprintf("%s",addr.RegID);

	int item = m_addrbook.GetCount();
	m_addrbook.InsertString(item,addressd.c_str());
}

BOOL CP2PDlg::AddListaddrDataBox(){

	map<string,uistruct::LISTADDR_t> m_mapAddrInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" sign=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	//���ComBox�ؼ�
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->ResetContent();
	//���ص�ComBox�ؼ�
	int nItem = 0;
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {

		((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->InsertString(nItem , const_it->second.RegID.c_str() );
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
	//��������
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
	string strCond;
	strCond = strprintf(" (state != 2) and (left_addr ='%s' or right_addr = '%s') ",addr,addr);

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
		if (theApp.IsSyncBlock)
		{
			if (const_it->state ==0 &&(500 + const_it->height)< theApp.blocktipheight || const_it->state ==3
				|| ((const_it->state ==1 || const_it->state ==4) &&(const_it->time_out + const_it->height)< theApp.blocktipheight))
			{
				continue;;
			}
		}
		money += const_it->amount; 
	}

	string srtShow ="";
	srtShow = strprintf("%.3lf",money);

	((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(srtShow.c_str());

	Invalidate();

}

void CP2PDlg::OnBnClickedButtonWithd()
{
	GetAppAccountSomeMoney();
	return;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( IDNO == ::MessageBox( this->GetSafeHwnd() ,_T("�Ƿ�ȷ��Ҫ����") , _T("��ʾ") , MB_YESNO|MB_ICONINFORMATION ) )
		return;
	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!CheckBalance())
	{
		return;
	}

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

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
		::MessageBox( this->GetSafeHwnd() ,_T("��ַ����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	CString Money = _T("");
	GetDlgItem(IDC_STATIC_BALANCE)->GetWindowText(Money);

	double dmoney = strtod(Money,NULL);

	string strContractData = m_P2PBetHelp.GetAppAccountMoneyContract(addr.GetString(),1,1,REAL_MONEY(dmoney));

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("С�Ѳ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}

	
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,0);
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
		//���뵽���׼�¼���ݿ�
		string strHash =  root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("��ϲ���ֳɹ�!\n%s") , root["hash"].asCString() ) ;
		strTip = "��ϲ���ֳɹ�����ȴ�1-2����ȷ�Ͻ���\n" ;
	}else{
		strTip = "����ʧ��!" ;
	}
	::MessageBox( this->GetSafeHwnd() ,strTip.c_str() , _T("��ʾ") , MB_ICONINFORMATION ) ;
}


void CP2PDlg::OnBnClickedButtonRech()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	CReCharge outdlg;
	if ( IDOK != outdlg.DoModal()){
		return;
	}

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	if (theApp.m_strAddress == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}

	string strShowData ="" ;
	CString addr;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return ;
	}
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("��ַ����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	string strCondition;
	strCondition = strprintf("reg_id = '%s'",addr);
	uistruct::LISTADDR_t pAddr;
	theApp.m_SqliteDeal.GetWalletAddressItem(strCondition,&pAddr);
	double sub = pAddr.fMoney - atof(theApp.m_strAddress);
	if (sub <1.0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ϵͳ�˻��������1smc,��Ϊ������Լ����С��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}


	string strContractData = m_P2PBetHelp.GetReChangContract();

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("С�Ѳ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,REAL_MONEY(atof(theApp.m_strAddress) ));
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
		//���뵽���׼�¼���ݿ�
		string strHash = root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("��ϲ��ֵ�ɹ�!\n%s") , root["hash"].asCString() ) ;
		strTip = "��ϲ��ֵ�ɹ�����ȴ�1-2����ȷ�Ͻ���\n" ;
	}else{
		strTip = "��ֵʧ��!";
	}
	::MessageBox( this->GetSafeHwnd() ,strTip.c_str() , _T("��ʾ") , MB_ICONINFORMATION ) ;
}

void CP2PDlg::SendBet(int rewardnum)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������  PacketP2PSendContract

	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
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
		::MessageBox( this->GetSafeHwnd() ,_T("����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}

	if (atof(strTxMoney) > balance)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("Ͷע�������˻����") , _T("��ʾ") , MB_ICONINFORMATION ) ;
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
		::MessageBox( this->GetSafeHwnd() ,_T("��ַ����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	//// ��ѯ��ַ�Ƿ񼤻�
	CString strCond;
	/*strCond.Format(_T(" address = '%s' "), addr);
	uistruct::LISTADDR_t addrsql;
	int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;
	if (!addrsql.bSign)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("�˻�δ����ܷ�����Լ����") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}*/
	char strTemp[34];
	memset(strTemp , 0 , 34 );
	memcpy(strTemp , UiFun::Rnd32() , 32 );
	strTemp[32] =rewardnum ;


	string temp(strTemp,strTemp+33);
	int aa = temp.length() ;
	string strCommand;
	strCommand = strprintf("%s %s","gethash" , strTemp );
	string strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	int pos = strShowData.find("hash");
	if ( pos < 0 ) return ;

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;

	string  strHash = root["hash"].asString() ;
	//strHash.Format(_T("%s") ,  root["hash"].asCString() ) ;

	TRACE(_T("contect:%s\r\n"),strTemp);
	TRACE(_T("sendhash:%s"),strHash.c_str());
	string strContractData;
	string strRamdHash = CSoyPayHelp::getInstance()->GetReverseHash(strHash);

	double money = atof(strTxMoney);
	CString nTemp;
	nTemp.Format(_T("%.8f"),money);
	strContractData = m_P2PBetHelp.PacketP2PSendContract((INT64)REAL_MONEY(atof(nTemp)),OUT_HEIGHT ,strRamdHash );

	INT64 strTxFee = theApp.m_P2PBetCfg.SendBetFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("С�Ѳ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}
	
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,0);
	strShowData = "";
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (strShowData == "")
	{
		return;
	}
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//���뵽���׼�¼���ݿ�
		string strHash = root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("��ϲ���Ͷ�Լ�ɹ�!\n%s") , root["hash"].asCString() ) ;
		strTip.Format( _T("��ϲ���ͳɹ�����ȴ�1-2����ȷ�Ͻ���\n")) ;
	}else{
		strTip.Format( _T("���Ͷ�Լʧ��!") ) ;
	}

	//���浽���ݿ�
	if ( bRes ) {
		uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
		memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));
		SYSTEMTIME curTime ;
		memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
		GetLocalTime( &curTime ) ;
		string strSendTime;
		strSendTime= strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);
		p2pbetrecord.time_out  = OUT_HEIGHT ;
		p2pbetrecord.tx_hash = root["hash"].asString();
		p2pbetrecord.left_addr = strprintf("%s",addr);
		p2pbetrecord.amount = atof(strTxMoney) ;
		memcpy(p2pbetrecord.content ,strTemp , sizeof(p2pbetrecord.content));

		p2pbetrecord.actor  = 0 ;
		p2pbetrecord.state  = 0 ;
		//���뵽���ݿ�
		string strSourceData;
		strSourceData = strprintf("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'" , \
			strSendTime.c_str() , _T("") , p2pbetrecord.time_out , \
			p2pbetrecord.tx_hash.c_str() ,  p2pbetrecord.left_addr.c_str() , p2pbetrecord.right_addr.c_str() ,p2pbetrecord.amount);

		strSourceData += strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
			p2pbetrecord.relate_hash.c_str() ,p2pbetrecord.guess_num ) ;

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData;
		pDatabase.strTabName =  _T("t_p2p_quiz");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();

		postmsg.SetData(strTemp);
		theApp.m_MsgQueue.push(postmsg);
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("��ʾ") , MB_ICONINFORMATION ) ;
}

void CP2PDlg::OnBnClickedButtonMale()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}

	if (atof(strTxMoney)<0.0001)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("Ͷע���������0.0001") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}

	CString strDisplay;
	strDisplay.Format(_T("ȷ��Ҫ������--->���:%s"), strTxMoney);
	COut outdlg(NULL, strDisplay,100);
	if ( IDOK != outdlg.DoModal()){
		return;
	}
	SendBet(1);
}


void CP2PDlg::OnBnClickedButtonWoman()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}

	if (atof(strTxMoney)<0.0001)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("Ͷע���������0.0001") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}

	CString strDisplay;
	strDisplay.Format(_T("ȷ��Ҫ���͸�--->���:%s"), strTxMoney);
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
	
	string temp;
	temp =strprintf("��:%d",m_pagecount);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_BonusListBox.DeleteAllIndex();
	OnShowPagePool(1);
}
 LRESULT CP2PDlg::onBnCLick( WPARAM wParam, LPARAM lParam )
 {
	List_AppendData* pinf = m_BonusListBox.GetAppendDataInfo((int)wParam);
	if ( NULL != pinf ) { 
		CString hash = pinf->pstr;
		string conditon;
		conditon = strprintf("hash='%s'",hash);
		uistruct::LISTP2POOL_T pPoolList;
		theApp.m_SqliteDeal.GetP2PQuizPoolItem(conditon, &pPoolList);
		if (pPoolList.hash.length() != 0)
		{
			uistruct::DBBET_DATA DBbet;
			memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(pPoolList.data);
			if (vTemp.size() <=0)
			{
				return 0;
			}
			memcpy(&DBbet, &vTemp[0], sizeof(DBbet));
			CString money,adddr;
			money = pinf->pstr1;
			//pinf->pSta2->GetWindowText(money);
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
		 ::MessageBox( this->GetSafeHwnd() ,_T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		 return;
	 }
	 if (!CheckBalance())
	 {
		 return;
	 }
	 if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	 CString strTxMoney;
	 GetDlgItem(IDC_STATIC_BALANCE)->GetWindowText(strTxMoney) ;

	 if (atof(strTxMoney) < atof(money))
	 {
		 ::MessageBox( this->GetSafeHwnd() ,_T("�ӵ��������˻����") , _T("��ʾ") , MB_ICONINFORMATION ) ;
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
		 ::MessageBox( this->GetSafeHwnd() ,_T("��ַ����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		 return;
	 }
	 //// ��ѯ��ַ�Ƿ񼤻�
	 //CString strCond;
	 //strCond.Format(_T(" address = '%s' "), addr);
	 //uistruct::LISTADDR_t addrsql;
	 //int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;
	 //if (!addrsql.bSign)
	 //{
		// ::MessageBox( this->GetSafeHwnd() ,_T("�˻�δ����ܷ�����Լ����") , _T("��ʾ") , MB_ICONINFORMATION ) ;
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

	 string strShowData ="";
	 string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,theApp.m_P2PBetCfg.AcceptBetnFee,0);
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
		 //���뵽���׼�¼���ݿ�
		 string strHash= root["hash"].asString() ;
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	 }
	 if ( pos >=0 ) {
		 bRes = TRUE ;
		 //strTip.Format( _T("��ϲ�Ӷĳɹ�!\n%s") , root["hash"].asCString() ) ;
		 strTip="��ϲ�ӵ��ɹ�����ȴ�1-2����ȷ�Ͻ���\n";
	 }else{
		 strTip="�˵��Ѿ�����!" ;
	 }

	 //���浽���ݿ�
	 if ( bRes ) {

		 //���뵽���׼�¼���ݿ�

		 //// �������ݿ����Ƿ���ڴ˼�¼
		 string conditon;
		 conditon = strprintf("tx_hash ='%s'", hash );
		 uistruct::P2P_QUIZ_RECORD_t pPoolItem;
		 int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
		 if (pPoolItem.tx_hash == "") ///�˼�¼������,�����¼
		 {
			 uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
			 memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));
			 SYSTEMTIME curTime ;
			 memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			 GetLocalTime( &curTime ) ;
			 string strSendTime;
			 strSendTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			 p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);

			p2pbetrecord.tx_hash = strprintf("%s",hash );

			 p2pbetrecord.relate_hash=root["hash"].asString();
		

			 p2pbetrecord.right_addr = strprintf("%s",addr);
			 p2pbetrecord.left_addr = strprintf("%s",sendaddr);
			 p2pbetrecord.amount = atof(money);

			 p2pbetrecord.state = 4;
			 p2pbetrecord.actor  = 1 ;
			 p2pbetrecord.guess_num = (int)guess ;
			 //���뵽���ݿ�
			 string strSourceData ;
			 strSourceData = strprintf("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'" , \
				 strSendTime.c_str() , _T("") , timeout , \
				 p2pbetrecord.tx_hash ,  p2pbetrecord.left_addr , p2pbetrecord.right_addr ,p2pbetrecord.amount);

			 strSourceData+=strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
				 p2pbetrecord.relate_hash ,p2pbetrecord.guess_num ) ;

			 uistruct::DATABASEINFO_t   pDatabase;
			 pDatabase.strSource = strSourceData;
			 pDatabase.strTabName =  _T("t_p2p_quiz");
			 CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
			 string strTemp = pDatabase.ToJson();

			 postmsg.SetData(strTemp);
			 theApp.m_MsgQueue.push(postmsg);

		 }else{        ///���¼�¼
			 string txhash = root["hash"].asString();
			 string strSourceData  , strW ;
			 strSourceData = strprintf("actor = %d , relate_hash = '%s' ,right_addr ='%s',", 2 , txhash ,addr ) ;
			 strSourceData += strprintf(" guess_num = %d,state = %d", (int)guess,4);
			 strW = strprintf("tx_hash = '%s'" , hash ) ;

			 uistruct::DATABASEINFO_t DatabaseInfo;
			 DatabaseInfo.strSource = strSourceData;
			 DatabaseInfo.strWhere = strW ;
			 DatabaseInfo.strTabName = _T("t_p2p_quiz");
			 CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			 string strTemp = DatabaseInfo.ToJson();

			 postmsg.SetData(strTemp);
			 theApp.m_MsgQueue.push(postmsg);

		 }
	 }
	 ::MessageBox( this->GetSafeHwnd() ,strTip.c_str() , _T("��ʾ") , MB_ICONINFORMATION ) ;
 }
 bool CP2PDlg::CheckBalance()
 {
	 OnCbnSelchangeComboAddres();
	 CString strMoney;
	 ((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	 double money =atof(strMoney);
	if (money == 0.0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("�˻����Ϊ��,���ȳ�ֵ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return false;
	}
	return true;
 }
 void CP2PDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
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

 void  CP2PDlg::ShowAllBetWinAndLoss(){
	 uistruct::P2PBETRECORDLIST       m_P2pBetTxList;
	 theApp.m_SqliteDeal.GetP2PQuizRecordList(_T("1=1") ,&m_P2pBetTxList ) ;

	 double result = ComPuteBetWinAandLoser(m_P2pBetTxList);
	 string show;
	 show =strprintf("��ӯ��:%.4f",result);
	 m_rBtnWinerloser.SetWindowText(show.c_str());
	 m_rBtnWinerloser.Invalidate();
 }
 void   CP2PDlg::ShowAddressBetWinAndLoss(CString addr)
 {
	 uistruct::P2PBETRECORDLIST       m_P2pBetTxList;
	 string condtion;
	 condtion = strprintf("left_addr = '%s' or right_addr = '%s'",addr,addr);
	 theApp.m_SqliteDeal.GetP2PQuizRecordList(condtion ,&m_P2pBetTxList ) ;

	 double result = ComPuteAddrBetWinAandLoser(m_P2pBetTxList,addr);
	 string show;
	 show = strprintf("ӯ��:%.4f",result);
	 m_rBtnAddrWinerloser.SetWindowText(show.c_str());
	 m_rBtnAddrWinerloser.Invalidate();
 }
 double CP2PDlg::ComPuteBetWinAandLoser(uistruct::P2PBETRECORDLIST  m_P2pBetTxList)
 {

	 double winer = 0.0;
	 double loser = 0.0;

	 std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	 for (const_it = m_P2pBetTxList.begin() ; const_it != m_P2pBetTxList.end() ; const_it++ ) {
		if (const_it->actor == 0){         //// ����Լ�����
			if (const_it->state == 2){       
				if (const_it->guess_num == const_it->content[32])
				{
					loser += const_it->amount;
				}else
				{
					winer += const_it->amount;
				}
			}else if (const_it->state == 1){
				if(const_it->height != 0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight){
					loser += const_it->amount;
				}
			}
		}else if (const_it->actor == 1){ /// �Ӷ������
			if (const_it->state == 2)
			{
				int rewardnum = (int)const_it->content[32];
				if (const_it->guess_num == const_it->content[32])
				{
					winer += const_it->amount;
				}else
				{
					loser += const_it->amount;
				}

			}else{
				if (const_it->height>0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight)
				{
					winer += const_it->amount;
				}
			}
		}	 
	 }
	double result = winer-loser;
	return result;
 }
 double CP2PDlg::ComPuteAddrBetWinAandLoser(uistruct::P2PBETRECORDLIST  m_P2pBetTxList,CString addr)
 {
	 double winer = 0.0;
	 double loser = 0.0;

	 std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	 for (const_it = m_P2pBetTxList.begin() ; const_it != m_P2pBetTxList.end() ; const_it++ ) {
		 if (const_it->actor == 0){         //// ����Լ�����
			 if (const_it->state == 2){       
				 if (const_it->guess_num == const_it->content[32])
				 {
					 loser += const_it->amount;
				 }else
				 {
					 winer += const_it->amount;
				 }
			 }else if (const_it->state == 1){
				 if(const_it->height != 0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight){
					 loser += const_it->amount;
				 }
			 }
		 }else if (const_it->actor == 1){ /// �Ӷ������
			 if (const_it->state == 2)
			 {
				 int rewardnum = (int)const_it->content[32];
				 if (const_it->guess_num == const_it->content[32])
				 {
					 winer += const_it->amount;
				 }else
				 {
					 loser += const_it->amount;
				 }

			 }else{
				 if (const_it->height>0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight)
				 {
					 winer += const_it->amount;
				 }
			 }
		 }else if(strcmp(const_it->left_addr.c_str(),const_it->right_addr.c_str()) != 0){
			 if (strcmp(const_it->left_addr.c_str(),addr) == 0)
			 {
				 if (const_it->state == 2){       
					 if (const_it->guess_num == const_it->content[32])
					 {
						 loser += const_it->amount;
					 }else
					 {
						 winer += const_it->amount;
					 }
				 }else if (const_it->state == 1){
					 if(const_it->height != 0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight){
						 loser += const_it->amount;
					 }
				 }
			 }else if (strcmp(const_it->right_addr.c_str(),addr) == 0)
			 {
				 if (const_it->state == 2)
				 {
					 int rewardnum = (int)const_it->content[32];
					 if (const_it->guess_num == const_it->content[32])
					 {
						 winer += const_it->amount;
					 }else
					 {
						 loser += const_it->amount;
					 }

				 }else{
					 if (const_it->height>0 &&(const_it->time_out + const_it->height)< theApp.blocktipheight)
					 {
						 winer += const_it->amount;
					 }
				 }
			 }
		 }
	 }
		 
	 double result = winer-loser;
	 return result;
 }
 void CP2PDlg::OnBnClickedButtonRefresh2()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 ShowListItem(m_seltab);
	  ShowAllBetWinAndLoss();
 }


 void CP2PDlg::OnBnClickedButtonRefresh1()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 OnListPool();
 }


 void CP2PDlg::OnNMThemeChangedListBonus(NMHDR *pNMHDR, LRESULT *pResult)
 {
	 // �ù���Ҫ��ʹ�� Windows XP ����߰汾��
	 // ���� _WIN32_WINNT ���� >= 0x0501��
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 *pResult = 0;
 }
 void  CP2PDlg::OnShowPagePool(int page)
 {
	 if (page >m_pagecount || page == m_curpage || page <= 0)
	 {
		 return;
	 }


	 m_BonusListBox.DeleteAllIndex();
	 string strpage;
	 strpage = strprintf("%d",page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());
	 m_curpage = page;
	 int index = (page-1)*m_pagesize;
	 unsigned int count = (m_PoolList.size() -index)>=m_pagesize?m_pagesize:(m_PoolList.size() -index);
	 int i =0;

	 string strmoney;
	 string addr,money;
	 string txhash, line;

	 string temp = "";

	 char buffer[1024] = {0};
	 for (unsigned int k = index;k< (index+count)&& k<m_PoolList.size();k++)
	 {
		 uistruct::LISTP2POOL_T const_it = m_PoolList.at(k);
		 string nValue = const_it.data;
		 uistruct::DBBET_DATA DBbet;
		 memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
		 std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
		 if (vTemp.size() <=0)
		 {
			 continue;
		 }
		 memcpy(&DBbet, &vTemp[0], sizeof(DBbet));


		 std::vector<unsigned char> vSendid;
		 vSendid.assign(DBbet.sendbetid,DBbet.sendbetid+sizeof(DBbet.sendbetid));

		 string regid = CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);

		 double dmoney = (DBbet.money*1.0)/COIN;
		 sprintf_s(buffer,"%.4f",dmoney);
		 money = buffer;
		 memset(buffer,0,1024);
		 //money.Format(_T("%.4f"),dmoney);

		 sprintf_s(buffer,"%.8f",dmoney);
		 strmoney = buffer;
		 memset(buffer,0,1024);
		// strmoney.Format(_T("%.8f"),dmoney);

		 temp = const_it.hash.substr(0,6);
		 line = temp;

		// line.Format(_T("%d"),(i+1));
		// txhash.Format(_T("%s"),const_it.hash.c_str());
		 m_BonusListBox.InsertStr(i,this->GetSafeHwnd());
		 m_BonusListBox.SetIndexInage(i , IDB_BITMAP_P2P_LISTBOX_BUT);
		 m_BonusListBox.SetIndexString(i , line.c_str(),regid.c_str(), _T("��"), money.c_str(), const_it.hash.c_str(),strmoney.c_str());
		 i++;
	 }
 }

 BOOL CP2PDlg::PreTranslateMessage(MSG* pMsg)
 {
	 // TODO: �ڴ����ר�ô����/����û���
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
					::MessageBox( this->GetSafeHwnd() ,_T("��������,����������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
				 }
				 return TRUE;
			 }
		 }  
	 }  
	 return CDialogBar::PreTranslateMessage(pMsg);
 }


 void CP2PDlg::OnBnClickedButtonUp()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 OnShowPagePool((m_curpage-1));
 }


 void CP2PDlg::OnBnClickedButtonNext()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 OnShowPagePool((m_curpage+1));
 }


 void CP2PDlg::OnTimer(UINT_PTR nIDEvent)
 {
	 // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	 switch(nIDEvent)
	 {
	 case 1:
		 OnBnClickedButtonRefresh2();
		 OnBnClickedButtonRefresh1();
		 break;
	 default:
		 break;

	 }
	 CDialogBar::OnTimer(nIDEvent);
 }
 void  CP2PDlg::onShowLink()
 {
	 v_linkCtrl.SetWindowText(_T("����"));
	 v_linkCtrl.SetURL("http://www.dacrs.com/forum.php?mod=viewthread&tid=3487&extra=page%3D1");
 }

 void CP2PDlg::OnLbnDblclkListBonus()
 {
	 // TODO: �ڴ���ӿؼ�֪ͨ����������
	 int nSel=m_BonusListBox.GetCurSel(); 
	 int count = ((m_curpage -1)*m_pagesize) + nSel;
	 if (count <=(int)m_PoolList.size())
	 {
		 uistruct::LISTP2POOL_T const_it = m_PoolList.at(count);
		 string temp = _T("���½���ID: ");
	    temp +=const_it.hash ;
		 ::MessageBox( this->GetSafeHwnd() ,temp.c_str() , _T("��ʾ") , MB_ICONINFORMATION ) ;
	 }
 }
bool CP2PDlg::CheckBalance(double dmoney)
{
	OnCbnSelchangeComboAddres();
	CString strMoney;
	((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->GetWindowText(strMoney);
	double money =atof(strMoney);
	if (money == 0.0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("�˻����Ϊ��,���ȳ�ֵ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return false;
	}

	if (dmoney >money)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("���ֽ������˻��еĽ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return false;
	}
	return true;
}
void  CP2PDlg::GetAppAccountSomeMoney()
{
	if ( IDNO == ::MessageBox( this->GetSafeHwnd() ,_T("�Ƿ�ȷ��Ҫ����") , _T("��ʾ") , MB_YESNO|MB_ICONINFORMATION ) )
		return;
	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}


	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	CReCharge outdlg(NULL,"����:","���ֽ��");
	if ( IDOK != outdlg.DoModal()){
		return;
	}

	if (theApp.m_strAddress == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("���ֽ���Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}
	
	double money = strtod(theApp.m_strAddress,NULL);
	if (!CheckBalance())
	{
		return;
	}

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
		::MessageBox( this->GetSafeHwnd() ,_T("��ַ����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}


	string strContractData = m_P2PBetHelp.GetAppAccountSomeMoneyContract(addr.GetString(),3,1,REAL_MONEY(money));

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("С�Ѳ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}


	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,addr.GetString(),strContractData,0,strTxFee,0);
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
		//���뵽���׼�¼���ݿ�
		string strHash =  root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("��ϲ���ֳɹ�!\n%s") , root["hash"].asCString() ) ;
		strTip = "��ϲ���ֳɹ�����ȴ�1-2����ȷ�Ͻ���\n" ;
	}else{
		strTip = "����ʧ��!" ;
	}
	::MessageBox( this->GetSafeHwnd() ,strTip.c_str() , _T("��ʾ") , MB_ICONINFORMATION ) ;
}