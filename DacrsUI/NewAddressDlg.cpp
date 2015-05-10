// NewAddressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "NewAddressDlg.h"
#include "afxdialogex.h"


// CNewAddressDlg �Ի���

IMPLEMENT_DYNAMIC(CNewAddressDlg, CDialogEx)

CNewAddressDlg::CNewAddressDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CNewAddressDlg::IDD, pParent)
{
	m_pBmp = NULL ;
}

CNewAddressDlg::~CNewAddressDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CNewAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDC_BUTTON_SCDZ, m_rBtnNewAdd);
}


BEGIN_MESSAGE_MAP(CNewAddressDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CNewAddressDlg::OnBnClickedButtonClose)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CNewAddressDlg ��Ϣ�������
BOOL CNewAddressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(24 , _T("΢���ź�"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	m_rBtnClose.SetWindowPos(NULL ,476-26 , 0 , 0 , 0 , SWP_NOSIZE);

	m_rBtnNewAdd.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	m_rBtnNewAdd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnNewAdd.SetWindowText("�����µ�ַ") ;
	m_rBtnNewAdd.SetFontEx(18 , _T("΢���ź�"));
	m_rBtnNewAdd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnNewAdd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnNewAdd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnNewAdd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnNewAdd.SizeToContent();

	SetBkBmpNid( IDB_BITMAP_DLG_BALCK ) ;
	m_fontGrid.CreatePointFont(100,_T("������"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CNewAddressDlg::SetBkBmpNid( UINT nBitmapIn ) 
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
void CNewAddressDlg::OnPaint()
{
	 CPaintDC dc(this); // device context for painting
	 // TODO: �ڴ˴������Ϣ����������
	 // ��Ϊ��ͼ��Ϣ���� CDialogBar::OnPaint()
}
HBRUSH CNewAddressDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(&m_fontGrid);
		hbr = (HBRUSH)CreateSolidBrush(RGB(249,249,249));
	}
	return hbr;
}

BOOL CNewAddressDlg::OnEraseBkgnd(CDC* pDC)
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
		pDC->StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


void CNewAddressDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

void CNewAddressDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(IDOK);
}


void CNewAddressDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);//����һ���ƶ�����Ϣ
	CDialogEx::OnLButtonDown(nFlags, point);
}


LRESULT CNewAddressDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	 UINT nResult = CDialog::OnNcHitTest(point);
	 return nResult  == HTCLIENT ? HTCAPTION : nResult;//���������ڿͻ���ô���ڵĻ��Ͱ������ɱ�����

	 return CDialogEx::OnNcHitTest(point);
}
