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
}


BEGIN_MESSAGE_MAP(CIndTitleBar, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
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
	SetBkBmpNid( IDB_BITMAP_WNDTITLEBK ) ;
	ModifyStyle(WS_BORDER,   0); 
	ModifyStyleEx(WS_EX_WINDOWEDGE,   0); 

	return 0;
}

void CIndTitleBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect ; 
	this->GetWindowRect(rect);

	//�ر�
	CButton *p_Close = (CButton*)GetDlgItem(IDC_BUTTON_CLOSE);
	if( NULL != p_Close ) {	
		CRect m_BtnRc ;
		p_Close->GetWindowRect(&m_BtnRc);
		p_Close->SetWindowPos(NULL ,rect.Width() - m_BtnRc.Width() - 5 /*rect.right-1*m_BtnRc.Width()-10*/ , 9 , 0 , 0 , SWP_NOSIZE);
	}
	CButton * p_Min = (CButton*)GetDlgItem(IDC_BUTTON_MIN);
	if( NULL != p_Min ) {	
		CRect m_BtnRc ;
		p_Min->GetWindowRect(&m_BtnRc);
		p_Min->SetWindowPos(NULL ,rect.Width() - 2*m_BtnRc.Width() - 8 /*rect.right-1*m_BtnRc.Width()-10*/ , 9 , 0 , 0 , SWP_NOSIZE);
	}
}

BOOL CIndTitleBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI,IDB_BITMAP_MAINUI_1,IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI);
		m_rBtnSend.LoadBitmaps(IDB_BITMAP_SEND,IDB_BITMAP_SEND,IDB_BITMAP_SEND,IDB_BITMAP_SEND);
		m_rBtnRece.LoadBitmaps(IDB_BITMAP_RECEIVE,IDB_BITMAP_RECEIVE,IDB_BITMAP_RECEIVE,IDB_BITMAP_RECEIVE);
		m_rBtnTrad.LoadBitmaps(IDB_BITMAP_DEALS,IDB_BITMAP_DEALS,IDB_BITMAP_DEALS,IDB_BITMAP_DEALS);
		m_rBtnP2p.LoadBitmaps(IDB_BITMAP_P2P,IDB_BITMAP_P2P,IDB_BITMAP_P2P,IDB_BITMAP_P2P);
		m_rBtnMortgage.LoadBitmaps(IDB_BITMAP_MORTGAGE,IDB_BITMAP_MORTGAGE,IDB_BITMAP_MORTGAGE,IDB_BITMAP_MORTGAGE);
		m_rBtnIpo.LoadBitmaps(IDB_BITMAP_IPO,IDB_BITMAP_IPO,IDB_BITMAP_IPO,IDB_BITMAP_IPO);

		UpdateData(0);

		m_tooltip.Create(this); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_MAIN_UI), _T("������")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_SEND), _T("����")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_RECE), _T("����")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_TRAD_INFO), _T("��������")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_P2P), _T("P2P��ɫ��")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_MORTGAGE), _T("��Ѻ����")); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_IPO), _T("IPO���")); 
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
