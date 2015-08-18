// DialogBase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DialogBase.h"
#include "afxdialogex.h"


// CDialogBase �Ի���

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


// CDialogBase ��Ϣ�������

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBkBmpNid( IDB_BITMAP_CONTENT,m_pBmp ) ;
	SetBkBmpNid( IDB_BITMAP_TITLE,m_HeadBmp ) ;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CDialogBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);//����һ���ƶ�����Ϣ
	CDialogEx::OnLButtonDown(nFlags, point);
}


LRESULT CDialogBase::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	UINT nResult = CDialog::OnNcHitTest(point);
	return nResult  == HTCLIENT ? HTCAPTION : nResult;//���������ڿͻ���ô���ڵĻ��Ͱ������ɱ�����

	return CDialogEx::OnNcHitTest(point);
}