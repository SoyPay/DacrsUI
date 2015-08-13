// DialogBase.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DialogBase.h"
#include "afxdialogex.h"


// CDialogBase 对话框

IMPLEMENT_DYNAMIC(CDialogBase, CDialogEx)

CDialogBase::CDialogBase(UINT ID,CWnd* pParent /*=NULL*/)
	: CDialogEx(ID, pParent)
{
	m_pBmp = NULL ;
	m_HeadBmp = NULL ;
}

CDialogBase::~CDialogBase()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	if (NULL != m_HeadBmp)
	{
		DeleteObject(m_HeadBmp) ;
		m_HeadBmp = NULL ;
	}
}

void CDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogBase, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDialogBase 消息处理程序

void CDialogBase::SetBkBmpNid( UINT nBitmapIn ,HBITMAP &bmp) 
{
	if( NULL != bmp ) {
		::DeleteObject( bmp ) ;
		bmp = NULL ;
	}
	bmp	= NULL ;
	HINSTANCE	hInstResource = NULL;	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);
	if( NULL != hInstResource ) {
		bmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
	}
}
BOOL CDialogBase::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect   rect; 
	GetClientRect(&rect); 

	if(m_pBmp   !=   NULL) { 
		BITMAP   bm; 
		CDC   dcMem; 

		::GetObject(m_HeadBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldhBitmap   =(HBITMAP   )   dcMem.SelectObject(m_HeadBmp); 
		int height = bm.bmHeight;
		pDC->StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 
		dcMem.SelectObject(pOldhBitmap);

		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
		pDC->StretchBlt(rect.left,rect.top-1+height,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 
		dcMem.SelectObject(pOldBitmap);

		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


BOOL CDialogBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBkBmpNid( IDB_BITMAP_CONTENT,m_pBmp ) ;
	SetBkBmpNid( IDB_BITMAP_TITLE,m_HeadBmp ) ;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDialogBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);//发送一个移动的消息
	CDialogEx::OnLButtonDown(nFlags, point);
}


LRESULT CDialogBase::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT nResult = CDialog::OnNcHitTest(point);
	return nResult  == HTCLIENT ? HTCAPTION : nResult;//鼠标的坐标在客户区么？在的话就把它当成标题栏

	return CDialogEx::OnNcHitTest(point);
}