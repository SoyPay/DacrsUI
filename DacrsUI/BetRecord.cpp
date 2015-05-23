// BetRecord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "BetRecord.h"
#include "afxdialogex.h"


// CBetRecord �Ի���

IMPLEMENT_DYNAMIC(CBetRecord, CDialogEx)

CBetRecord::CBetRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBetRecord::IDD, pParent)
{
	m_pBmp = NULL ;
}

CBetRecord::~CBetRecord()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CBetRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_ListBox);
}


BEGIN_MESSAGE_MAP(CBetRecord, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CBetRecord ��Ϣ�������
void CBetRecord::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CBetRecord::OnEraseBkgnd(CDC* pDC)
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


int CBetRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid(IDB_BITMAP_BET_RECORD);
	return 0;
}


BOOL CBetRecord::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ){
		UpdateData(0);
		m_ListBox.InsertStr(0,this->GetSafeHwnd());
		m_ListBox.SetIndexBackCol(0 , 0 , RGB(181,185,212));
		m_ListBox.SetIndexBackCol(0 , 1 , RGB(181,185,212));
		m_ListBox.SetIndexBackCol(0 , 2 , RGB(181,185,212));
		m_ListBox.SetIndexBackCol(0 , 3 , RGB(181,185,212));
		m_ListBox.SetIndexBackCol(0 , 4 , RGB(181,185,212));
		m_ListBox.SetIndexString(0 , _T("durCyWC8MTdQdpCo9QXZ5wxSyL9jtyDFri"), _T("+100"), _T("vbvvvvv"), _T("ddddddd") , _T("2015-01-01 01:02:03"));

		m_ListBox.InsertStr(1,this->GetSafeHwnd());
		m_ListBox.SetIndexBackCol(1 , 0,RGB(255,76,0));
		m_ListBox.SetIndexBackCol(1 , 1 , RGB(255,76,0));
		m_ListBox.SetIndexBackCol(1 , 2 , RGB(255,76,0));
		m_ListBox.SetIndexBackCol(1 , 3 , RGB(255,76,0));
		m_ListBox.SetIndexBackCol(1 , 4 , RGB(255,76,0));
		m_ListBox.SetIndexString(1 , _T("durCyWC8MTdQdpCo9QXZ5wxSyL9jtyDFri"), _T("+100"), _T("vbvvvvv"), _T("ddddddd") , _T("2015-01-01 01:02:03"));
	}
	return bRes;
}


void CBetRecord::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( NULL != GetSafeHwnd() ) {
		CWnd *pst = GetDlgItem( IDC_LIST_BOX ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,0 , 33 , 875, 185  ,SWP_SHOWWINDOW ) ; 
		}
	}
}
