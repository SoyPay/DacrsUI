// P2PDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "P2PDlg.h"
#include "afxdialogex.h"


// CP2PDlg 对话框

IMPLEMENT_DYNAMIC(CP2PDlg, CDialogBar)

CP2PDlg::CP2PDlg()
{
	m_pBmp = NULL ;
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
}


BEGIN_MESSAGE_MAP(CP2PDlg, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
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
