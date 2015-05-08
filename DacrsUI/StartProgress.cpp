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
		m_ProgressWnd->SetWindowPos(NULL , x/2 - 175 + 270 , y/2 - 50 + 50 , 0 ,0 , SWP_NOSIZE) ;
	}

	m_ProgressBmp[0].LoadBitmap(IDB_BITMAP_PROG1);  
	m_ProgressBmp[1].LoadBitmap(IDB_BITMAP_PROG2);  
	m_ProgressBmp[2].LoadBitmap(IDB_BITMAP_PROG3);  
	m_ProgressBmp[3].LoadBitmap(IDB_BITMAP_PROG4);  

	m_progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(66, 65, 63));//背景色
	m_progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(254, 153, 0));//前景色

	m_progress.SetRange(0,4);
	LoadGifing(TRUE);
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
				if( TRUE == ((CGIFControl*)m_ProgressWnd)->Load(_T("C:\\Users\\Administrator\\Desktop\\UI\\最后\\gif\\init.gif\0")) ) {
					CRect rc ;
					GetClientRect( rc ) ;
					Invalidate() ;
					m_ProgressWnd->SetWindowPos( NULL , rc.Width()- 18 , (rc.Height()/2)-8 , 0 , 0 , \
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
	m_progress.SetPos(pos);
}