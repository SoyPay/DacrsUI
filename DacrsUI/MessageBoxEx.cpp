// MessageBoxEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "MessageBoxEx.h"
#include "afxdialogex.h"


// CMessageBoxEx �Ի���

IMPLEMENT_DYNAMIC(CMessageBoxEx, CDialogEx)

CMessageBoxEx::CMessageBoxEx(CString strText,int nType,UINT nImageType ,CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageBoxEx::IDD, pParent)
{
	m_Text = strText;
	m_nType = nType;
	SetBkBmpNid( nImageType ) ;
}

CMessageBoxEx::~CMessageBoxEx()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CMessageBoxEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUIT, m_quit);
	DDX_Control(pDX, IDC_OK, m_OK);
	DDX_Control(pDX, IDC_CANCEL, m_cancel);
	DDX_Control(pDX, IDC_TEXT, m_strText);
}


BEGIN_MESSAGE_MAP(CMessageBoxEx, CDialogEx)
	ON_BN_CLICKED(IDC_OK, &CMessageBoxEx::OnBnClickedOk)
	ON_BN_CLICKED(IDC_QUIT, &CMessageBoxEx::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_CANCEL, &CMessageBoxEx::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMessageBoxEx ��Ϣ�������

void CMessageBoxEx::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CMessageBoxEx::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);

	CRect rect;
	GetWindowRect(&rect);
	ClientToScreen(rect);
	MoveWindow((rect.left + rect.right)/2 - 381  ,(rect.top + rect.bottom)/2 - 145/2 ,381,145,FALSE);
	m_fontGrid.CreatePointFont(100,_T("������"));

	m_quit.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_quit.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_quit.SetWindowText("") ;
	m_quit.SetFontEx(20 , _T("΢���ź�"));
	m_quit.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_quit.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_quit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_quit.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_quit.SizeToContent();
	//�رհ�ťλ��
	m_quit.MoveWindow(381-25-2,2,26,22);

	//��ʾ�ı���λ��
	m_strText.SetFont(100, _T("����"));				//������ʾ����ʹ�С
	m_strText.SetTextColor(RGB(0,0,0));	
	m_strText.MoveWindow(40,50,381-80,145-36-70);
	m_strText.SetWindowText(m_Text);

	if ( m_nType == 0 ) {
	   //ȷ����ť
	   m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	   m_OK.SetWindowText("ȷ��") ;
	   m_OK.SetFontEx(12 , _T("����"));
	   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	   m_OK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	   m_OK.SizeToContent();
	   m_OK.MoveWindow(381/2 + 381/4 - 56/2,145-36-6,56,23);
	   m_cancel.ShowWindow(SW_HIDE);
	}else if ( m_nType == 1 ) {
	   //�ǰ�ť
	   m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	   m_OK.SetWindowText("��") ;
	   m_OK.SetFontEx(12 , _T("����"));
	   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	   m_OK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	   m_OK.SizeToContent();
	   //�ǰ�ťλ��
	   m_OK.MoveWindow(381/2 - 381/4 - 56/2,145-36-6,56,23);
	   //��ť
	   m_cancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	   m_cancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	   m_cancel.SetWindowText("��") ;
	   m_cancel.SetFontEx(12 , _T("����"));
	   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	   m_cancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	   m_cancel.SizeToContent();
	   //��ťλ��
	   m_cancel.MoveWindow(381/2 + 381/4 - 56/2,145-36-6,56,23);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMessageBoxEx::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_select = 1;
	CDialogEx::OnCancel();
}


void CMessageBoxEx::OnBnClickedQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_select = 0;
	CDialogEx::OnCancel();
}


void CMessageBoxEx::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_select = 0;
	CDialogEx::OnCancel();
}


BOOL CMessageBoxEx::OnEraseBkgnd(CDC* pDC)
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


HBRUSH CMessageBoxEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		/*pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(&m_fontGrid);
		hbr = (HBRUSH)CreateSolidBrush(RGB(224,224,224));*/
	}
	return hbr;
}
int CMessageBoxEx::GetSelect()
{
	return m_select;
}

LRESULT CMessageBoxEx::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UINT nResult = CDialog::OnNcHitTest(point);
	return nResult  == HTCLIENT ? HTCAPTION : nResult;//���������ڿͻ���ô���ڵĻ��Ͱ������ɱ�����
	return CDialogEx::OnNcHitTest(point);
}


void CMessageBoxEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);//����һ���ƶ�����Ϣ
	CDialogEx::OnLButtonDown(nFlags, point);
	//CDialogEx::OnLButtonDown(nFlags, point);
}
