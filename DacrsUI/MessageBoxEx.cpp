// MessageBoxEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "MessageBoxEx.h"
#include "afxdialogex.h"


// CMessageBoxEx �Ի���

IMPLEMENT_DYNAMIC(CMessageBoxEx, CDialogBase)

CMessageBoxEx::CMessageBoxEx(CString strText,CString strTip ,UINT uType ,CWnd* pParent /*=NULL*/)
	: CDialogBase(CMessageBoxEx::IDD, pParent)
{
	m_Text = strText;
	m_Tip  = strTip ;
	m_uType = uType ;
	m_pBmp = NULL;
	SetBkBmpNid( IDB_BITMAP_MESSAGE_H ) ;
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
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUIT, m_quit);
	DDX_Control(pDX, IDC_OK, m_OK);
	DDX_Control(pDX, IDC_CANCEL, m_cancel);
	DDX_Control(pDX, IDC_TEXT, m_strText);
	DDX_Control(pDX, IDC_TIP, m_strTip);
}


BEGIN_MESSAGE_MAP(CMessageBoxEx, CDialogBase)
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
void CMessageBoxEx::SetBitmap(UINT uBmpResource)
{
	HBITMAP hBitmap; //���meaasgebox�����е�ͼƬ 
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_IMAGE); 
	//����ͼƬID 
	hBitmap = (HBITMAP)LoadImage(    
	AfxGetInstanceHandle(), 
	MAKEINTRESOURCE(uBmpResource), 
	IMAGE_BITMAP, 
	0, 
	0, 
	LR_LOADMAP3DCOLORS); 
	pStatic->ModifyStyle(0xF, SS_BITMAP|SS_CENTERIMAGE); 
	pStatic->SetBitmap(hBitmap); 
}
BOOL CMessageBoxEx::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);

	CRect rect;
	GetWindowRect(&rect);
	ClientToScreen(rect);
	//MoveWindow((rect.left + rect.right)/2 - 381/2  ,(rect.top + rect.bottom)/2 - 145/2 ,381,145,FALSE);
	MoveWindow((rect.left + rect.right)/2 - 381/2  ,(rect.top + rect.bottom)/2 - 145/2 ,381,300,FALSE);
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
	//ͼƬ
	if ( (m_uType & MFB_TIP) == MFB_TIP ) {
		SetBitmap(IDB_BITMAP_TIP);
	}else if ( (m_uType & MFB_ERROR) == MFB_ERROR ) {
		SetBitmap(IDB_BITMAP_ERROR);
	}else if ( (m_uType & MFB_WARNING) == MFB_WARNING ) {
		SetBitmap(IDB_BITMAP_WARNING);
	}
	GetDlgItem(IDC_IMAGE)->MoveWindow(381/6 - 39/2 , (145 - 145/5 )/2 - 38/2 + 10 , 39 , 38);
	//�رհ�ťλ��
	m_quit.MoveWindow(381-25-2,2,26,22);

	//��ʾ��Ϣλ��
	m_strTip.SetFont(100, _T("΢���ź�"));				//������ʾ����ʹ�С
	m_strTip.SetTextColor(RGB(255,255,255));	
	m_strTip.MoveWindow(2,2,381/4,145/5);
	m_strTip.SetWindowText(m_Tip);

	//��ʾ�ı���λ��
	m_strText.SetFont(100, _T("����"));				//������ʾ����ʹ�С
	m_strText.SetTextColor(RGB(0,0,0));	
	m_strText.MoveWindow(381/6 + 39/2 + 20 ,(145 - 145/5 )/2 - 38/2 + 10,381-381/6 + 39/2,145-145/5-((145 - 145/5 )/2 - 38/2 + 10));
	m_strText.SetWindowText(m_Text);

	//��ť
	if ( (m_uType & MFB_OK) == MFB_OK ) {
		   m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_OK.SetWindowText("ȷ��") ;
		  // m_OK.SetFontEx(12 , _T("����"));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_OK.SizeToContent();
		   m_OK.MoveWindow(381/2 + 381/4 - 56/2,145-36-6,56,23);
		   m_cancel.ShowWindow(SW_HIDE);
	}else if ( (m_uType & MFB_OKCANCEL) == MFB_OKCANCEL ) {
		  m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_OK.SetWindowText("ȷ��") ;
		  // m_OK.SetFontEx(12 , _T("����"));
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
		   m_cancel.SetWindowText("ȡ��") ;
		  // m_cancel.SetFontEx(12 , _T("����"));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_cancel.SizeToContent();
		   //��ťλ��
		   m_cancel.MoveWindow(381/2 + 381/4 - 56/2,145-36-6,56,23);
	}else if ( (m_uType & MFB_YES) == MFB_YES ) {
		   m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_OK.SetWindowText("��") ;
		  // m_OK.SetFontEx(12 , _T("����"));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_OK.SizeToContent();
		   m_OK.MoveWindow(381/2 + 381/4 - 56/2,145-36-6,56,23);
		   m_cancel.ShowWindow(SW_HIDE);
	}else if ( (m_uType & MFB_YESNO) == MFB_YESNO ) {
		   m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_OK.SetWindowText("��") ;
		  // m_OK.SetFontEx(12 , _T("����"));
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
		  // m_cancel.SetFontEx(12 , _T("����"));
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
	CDialogBase::OnCancel();
}


void CMessageBoxEx::OnBnClickedQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_select = 0;
	CDialogBase::OnCancel();
}


void CMessageBoxEx::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_select = 0;
	CDialogBase::OnCancel();
}


BOOL CMessageBoxEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogBase::OnEraseBkgnd(pDC);
	/*CRect   rect; 
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
		CWnd::OnEraseBkgnd(pDC); */

	return TRUE;
}


HBRUSH CMessageBoxEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//if (nCtlColor == CTLCOLOR_STATIC)
	//{
	//	/*pDC->SetBkMode(TRANSPARENT);
	//	pDC->SelectObject(&m_fontGrid);
	//	hbr = (HBRUSH)CreateSolidBrush(RGB(224,224,224));*/
	//}
	return hbr;
}
int CMessageBoxEx::GetSelect()
{
	return m_select;
}

