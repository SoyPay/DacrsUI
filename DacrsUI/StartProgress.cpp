// StartProgress.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "StartProgress.h"
#include "afxdialogex.h"


// CStartProgress 对话框

IMPLEMENT_DYNAMIC(CStartProgress, CDialogEx)

CStartProgress::CStartProgress(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartProgress::IDD, pParent)
{
	m_nBmpIndex = 0 ;
	m_ProgressWnd = NULL ; 
	mprocessindex = false;
}

CStartProgress::~CStartProgress()
{
	if ( NULL != m_ProgressWnd ) {
		delete m_ProgressWnd ;
		m_ProgressWnd = NULL ;
	}
}

void CStartProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_INIT, m_progress);
}


BEGIN_MESSAGE_MAP(CStartProgress, CDialogEx)
	ON_WM_PAINT()
	ON_MESSAGE(MSG_USER_STARTPROCESS_UI , &CStartProgress::OnShowProgressCtrl  )
END_MESSAGE_MAP()


// CStartProgress 消息处理程序


BOOL CStartProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(NULL , x/2 - 175 , y/2 - 50  , 350 ,100 , SWP_SHOWWINDOW) ;

	if ( NULL == m_ProgressWnd ) {
		m_ProgressWnd = new CGIFControl ;
		m_ProgressWnd->Create(_T("") , WS_CHILD | SS_OWNERDRAW | WS_VISIBLE | SS_NOTIFY , CRect(0,0,48,34) , this, 121 ) ;
		//m_ProgressWnd->SetWindowPos(NULL , x/2 - 175 + 250 , y/2+20 , 0 ,0 , SWP_NOSIZE) ;
	}

	m_ProgressBmp[0].LoadBitmap(IDB_BITMAP_PROG1);  
	m_ProgressBmp[1].LoadBitmap(IDB_BITMAP_PROG2);  
	m_ProgressBmp[2].LoadBitmap(IDB_BITMAP_PROG3);  
	m_ProgressBmp[3].LoadBitmap(IDB_BITMAP_PROG4);  

	m_progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(66, 65, 63));//背景色
	m_progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(254, 153, 0));//前景色

	m_progress.SetRange(0,4);
	mprocessindex = true;
	LoadGifing(TRUE);

	theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_STARTPROCESS_UI ) ;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CStartProgress::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CDC memDC;  
	memDC.CreateCompatibleDC(&dc);  
	CRect rc;  
	GetClientRect(&rc);  

	HBITMAP hOldbmp = (HBITMAP)memDC.SelectObject(m_ProgressBmp[m_nBmpIndex]); 
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);  
	memDC.SelectObject(hOldbmp);  
	memDC.DeleteDC(); 
}
void CStartProgress::LoadGifing( BOOL bState )
{
	if( NULL != m_ProgressWnd ) {
		if( m_ProgressWnd->GetSafeHwnd() ) {
			if( TRUE == bState ) {
				CString gifpath;
				gifpath.Format(_T("%s\\gif\\init.gif\0"),theApp.str_InsPath.c_str());
				if( TRUE == ((CGIFControl*)m_ProgressWnd)->Load(gifpath.GetBuffer())) {
					CRect rc ;
					GetClientRect( rc ) ;
					Invalidate() ;
					m_ProgressWnd->SetWindowPos( NULL , (rc.Width()*3)/4+10 , (rc.Height()*9)/32 , 0 , 0 , \
						SWP_SHOWWINDOW|SWP_NOSIZE ) ;
					((CGIFControl*)m_ProgressWnd)->Play();
				}
			}else{
				((CGIFControl*)m_ProgressWnd)->Stop() ;	
			}
		}
	}
}
void CStartProgress::SwitchBmp(int nIndex)
{
	ASSERT(nIndex<4) ;
	m_nBmpIndex = nIndex ;
	Invalidate(); 
}
void CStartProgress::SetProgessRange(int pos){
	if (mprocessindex)
	{
		m_progress.SetPos(pos);
	}
	
}
LRESULT CStartProgress::OnShowProgressCtrl( WPARAM wParam, LPARAM lParam ) 
{
	if (mprocessindex)
	{
		int index =(int)wParam;
		m_progress.SetPos(index);
	}
	return 1;
}
