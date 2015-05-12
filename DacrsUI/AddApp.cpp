// AddApp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "AddApp.h"
#include "afxdialogex.h"


// CAddApp �Ի���

IMPLEMENT_DYNAMIC(CAddApp, CDialogBar)

CAddApp::CAddApp()
{
	m_pBmp = NULL ;
}

CAddApp::~CAddApp()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	v_linkCtrl1.InternalRelease();
	v_linkCtrl1.ExternalRelease();
	v_linkCtrl1.OnFinalRelease();
	v_linkCtrl1.DestroyWindow();

	v_linkCtrl2.InternalRelease();
	v_linkCtrl2.ExternalRelease();
	v_linkCtrl2.OnFinalRelease();
	v_linkCtrl2.DestroyWindow();
}

void CAddApp::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, v_linkCtrl1);
	DDX_Control(pDX, IDC_MFCLINK2, v_linkCtrl2);
}


BEGIN_MESSAGE_MAP(CAddApp, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CAddApp ��Ϣ�������

void CAddApp::SetBkBmpNid( UINT nBitmapIn ) 
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
int CAddApp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid(IDB_BITMAP_ADDAPP_BJ);

	return 0;
}


BOOL CAddApp::OnEraseBkgnd(CDC* pDC)
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


BOOL CAddApp::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(0);
		v_linkCtrl1.SetWindowText(_T(""));
		v_linkCtrl2.SetWindowText(_T(""));

		v_linkCtrl1.SetWindowText(_T("��ע��������"));
		v_linkCtrl2.SetWindowText(_T("QQȺ"));

		v_linkCtrl1.SetURL(_T("http://www.dacrs.com/forum.php?mod=viewthread&tid=2598"));
		v_linkCtrl2.SetURL(_T("http://jq.qq.com/?_wv=1027&k=T5mlmd"));
	}
	return bRes ;
}
