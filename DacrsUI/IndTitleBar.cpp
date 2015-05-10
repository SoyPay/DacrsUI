// IndTitleBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "IndTitleBar.h"


// CIndTitleBar �Ի���

IMPLEMENT_DYNAMIC(CIndTitleBar, CDialogBar)

	CIndTitleBar::CIndTitleBar()
{
	m_pBmp = NULL ;
	m_nButtonID = IDC_BUTTON_MAIN_UI ;
}

CIndTitleBar::~CIndTitleBar()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CIndTitleBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MAIN_UI	, m_rBtnMainUi );
	DDX_Control(pDX, IDC_BUTTON_SEND	, m_rBtnSend );
	DDX_Control(pDX, IDC_BUTTON_RECE	, m_rBtnRece );
	DDX_Control(pDX, IDC_BUTTON_TRAD_INFO	, m_rBtnTrad );
	DDX_Control(pDX, IDC_BUTTON_P2P	, m_rBtnP2p );
	DDX_Control(pDX, IDC_BUTTON_MORTGAGE	, m_rBtnMortgage );
	DDX_Control(pDX, IDC_BUTTON_IPO	, m_rBtnIpo );
	DDX_Control(pDX, IDC_BUTTON_ADDAPP	, m_rBtnAddApp );

	DDX_Control(pDX, IDC_BUTTON_CLOSE	, m_rBtnClose );
	DDX_Control(pDX, IDC_BUTTON_MIN	, m_rBtnMin );
}


BEGIN_MESSAGE_MAP(CIndTitleBar, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CIndTitleBar ��Ϣ�������
void CIndTitleBar::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CIndTitleBar::OnEraseBkgnd(CDC* pDC)
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

	return 1;
}

int CIndTitleBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid( IDB_BITMAP_BAR ) ;
	ModifyStyle(WS_BORDER,   0); 
	ModifyStyleEx(WS_EX_WINDOWEDGE,   0); 

	return 0;
}

void CIndTitleBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;
		CRect rc  ;
		GetClientRect( rc ) ;
		CButton *p_Button = (CButton*)GetDlgItem(IDC_BUTTON_MAIN_UI);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_SEND);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 80 , 15 , 0 , 0 , SWP_NOSIZE);
		}
		//
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_RECE);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 2*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_TRAD_INFO);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 3*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_P2P);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 4*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_MORTGAGE);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 5*80, 15, 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_IPO);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 6*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_ADDAPP);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 7*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		CRect rect ; 
		this->GetWindowRect(rect);
		//�ر�
		CButton *p_Close = (CButton*)GetDlgItem(IDC_BUTTON_CLOSE);
		if( NULL != p_Close ) {	
			CRect m_BtnRc ;
			p_Close->GetWindowRect(&m_BtnRc);
			p_Close->SetWindowPos(NULL ,rect.Width() - m_BtnRc.Width() - 5 /*rect.right-1*m_BtnRc.Width()-10*/ , 0 , 0 , 0 , SWP_NOSIZE);
		}
		CButton * p_Min = (CButton*)GetDlgItem(IDC_BUTTON_MIN);
		if( NULL != p_Min ) {	
			CRect m_BtnRc ;
			p_Min->GetWindowRect(&m_BtnRc);
			p_Min->SetWindowPos(NULL ,rect.Width() - 2*m_BtnRc.Width() - 8 /*rect.right-1*m_BtnRc.Width()-10*/ , 0 , 0 , 0 , SWP_NOSIZE);
		}
		CStatic * pImage = (CStatic*)GetDlgItem(IDC_STATIC_LOGO);
		if( NULL != pImage ) {	
			CRect m_BtnRc ;
			pImage->GetWindowRect(&m_BtnRc);
			pImage->SetWindowPos(NULL ,0 , -2 , 0 , 0 , SWP_NOSIZE);
		}
	}
}
void CIndTitleBar::MobileTip(UINT nButtonID , BOOL bFlag )
{
	if ( bFlag ) {   //��һ��
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI_2 , IDB_BITMAP_MAINUI_2 ,IDB_BITMAP_MAINUI_2);
	}
	if ( m_nButtonID == nButtonID  && !bFlag ) {
		return ;
	}
	switch(m_nButtonID)
	{
	case IDC_BUTTON_MAIN_UI:
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI_1,IDB_BITMAP_MAINUI_3,IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI_1);
		break;
	case IDC_BUTTON_SEND:
		m_rBtnSend.LoadBitmaps(IDB_BITMAP_SEND_1,IDB_BITMAP_SEND_3,IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_1);
		break;
	case IDC_BUTTON_RECE:
		m_rBtnRece.LoadBitmaps(IDB_BITMAP_REV_1,IDB_BITMAP_REV_3,IDB_BITMAP_REV_2,IDB_BITMAP_REV_1);
		break;
	case IDC_BUTTON_TRAD_INFO:
		m_rBtnTrad.LoadBitmaps(IDB_BITMAP_TRAD_1,IDB_BITMAP_TRAD_3,IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_1);
		break;
	case IDC_BUTTON_P2P:
		m_rBtnP2p.LoadBitmaps(IDB_BITMAP_YOUR_1,IDB_BITMAP_YOUR_3,IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_1);
		break;
	case IDC_BUTTON_MORTGAGE:
		m_rBtnMortgage.LoadBitmaps(IDB_BITMAP_DIYA_1,IDB_BITMAP_DIYA_3,IDB_BITMAP_DIYA_2,IDB_BITMAP_DIYA_1);
		break;
	case IDC_BUTTON_IPO:
		m_rBtnIpo.LoadBitmaps(IDB_BITMAP_IPO_1,IDB_BITMAP_IPO_3,IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_1);
		break;
	case IDC_BUTTON_ADDAPP:
		m_rBtnAddApp.LoadBitmaps(IDB_BITMAP_ADDAPP_1 , IDB_BITMAP_ADDAPP_3 ,IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_1);
		break;
	}
	switch(nButtonID)
	{
	case IDC_BUTTON_MAIN_UI:
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI_2 , IDB_BITMAP_MAINUI_2 ,IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI_2);
		break;
	case IDC_BUTTON_SEND:
		m_rBtnSend.LoadBitmaps(IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_2);
		break;
	case IDC_BUTTON_RECE:
		m_rBtnRece.LoadBitmaps(IDB_BITMAP_REV_2,IDB_BITMAP_REV_2,IDB_BITMAP_REV_2,IDB_BITMAP_REV_2);
		break;
	case IDC_BUTTON_TRAD_INFO:
		m_rBtnTrad.LoadBitmaps(IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_2);
		break;
	case IDC_BUTTON_P2P:
		m_rBtnP2p.LoadBitmaps(IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_2);
		break;
	case IDC_BUTTON_MORTGAGE:
		m_rBtnMortgage.LoadBitmaps(IDB_BITMAP_DIYA_2,IDB_BITMAP_DIYA_2,IDB_BITMAP_DIYA_2,IDB_BITMAP_DIYA_2);
		break;
	case IDC_BUTTON_IPO:
		m_rBtnIpo.LoadBitmaps(IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_2);
		break;
	case IDC_BUTTON_ADDAPP:
		m_rBtnAddApp.LoadBitmaps(IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_2);
		break;
	}
	m_nButtonID = nButtonID ;
	Invalidate();
}
BOOL CIndTitleBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI_1,IDB_BITMAP_MAINUI_3,IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI_1);
		m_rBtnSend.LoadBitmaps(IDB_BITMAP_SEND_1,IDB_BITMAP_SEND_3,IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_1);
		m_rBtnRece.LoadBitmaps(IDB_BITMAP_REV_1,IDB_BITMAP_REV_3,IDB_BITMAP_REV_2,IDB_BITMAP_REV_1);
		m_rBtnTrad.LoadBitmaps(IDB_BITMAP_TRAD_1,IDB_BITMAP_TRAD_3,IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_1);
		m_rBtnP2p.LoadBitmaps(IDB_BITMAP_YOUR_1,IDB_BITMAP_YOUR_3,IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_1);
		m_rBtnMortgage.LoadBitmaps(IDB_BITMAP_DIYA_1,IDB_BITMAP_DIYA_3,IDB_BITMAP_DIYA_2,IDB_BITMAP_DIYA_1);
		m_rBtnIpo.LoadBitmaps(IDB_BITMAP_IPO_1,IDB_BITMAP_IPO_3,IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_1);
		m_rBtnAddApp.LoadBitmaps(IDB_BITMAP_ADDAPP_1 , IDB_BITMAP_ADDAPP_3 ,IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_1);

		m_rBtnClose.LoadBitmaps(IDB_BITMAP_CLOSE , IDB_BITMAP_CLOSE2 ,IDB_BITMAP_CLOSE3,IDB_BITMAP_CLOSE);
		m_rBtnMin.LoadBitmaps(IDB_BITMAP_MIN , IDB_BITMAP_MIN2 ,IDB_BITMAP_MIN3,IDB_BITMAP_MIN);

		UpdateData(0);

		m_tooltip.Create(this); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_MAIN_UI), _T("������")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_SEND), _T("����")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_RECE), _T("����")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_TRAD_INFO), _T("��������")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_P2P), _T("P2P��ɫ��")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_MORTGAGE), _T("��Ѻ����")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_IPO), _T("IPO���")); 

		MobileTip(IDC_BUTTON_MAIN_UI , TRUE );
	}
	return bRes ;
}

BOOL CIndTitleBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(pMsg->message) 
	{ 
	case   WM_MOUSEMOVE: 
		m_tooltip.RelayEvent(pMsg); 
	} 
	return CDialogBar::PreTranslateMessage(pMsg);
}
void CIndTitleBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetParent()->PostMessage(MSG_TITLE_LBUTTON_DOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialogBar::OnLButtonDown(nFlags, point);
}
