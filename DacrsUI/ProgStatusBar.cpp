// ProgStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ProgStatusBar.h"
#include "afxdialogex.h"


// CProgStatusBar 对话框

IMPLEMENT_DYNAMIC(CProgStatusBar, CDialogBar)

	CProgStatusBar::CProgStatusBar()
{
	m_pBmp = NULL ;
	m_bProgressType = false;
	m_ProgressWnd = NULL ;
	m_nSigIndex = 0 ;
	m_walletui = false;
	memset(m_bmpsig , 0 , sizeof(CRect)) ;
}

CProgStatusBar::~CProgStatusBar()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	if ( NULL != m_ProgressWnd ) {
		delete m_ProgressWnd ;
		m_ProgressWnd = NULL ;
	}
}

void CProgStatusBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}


BEGIN_MESSAGE_MAP(CProgStatusBar, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(MSG_USER_UP_PROGRESS , &CProgStatusBar::OnShowProgressCtrl  )
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CProgStatusBar 消息处理程序
void CProgStatusBar::SetBkBmpNid( UINT nBitmapIn ) 
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
void CProgStatusBar::LoadGifing( BOOL bState )
{
	if( NULL != m_ProgressWnd ) {
		if( m_ProgressWnd->GetSafeHwnd() ) {
			if( TRUE == bState ) {
				if( TRUE == ((CGIFControl*)m_ProgressWnd)->Load(theApp.m_ProgressGifFile.GetBuffer()) ) {
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

BOOL CProgStatusBar::OnEraseBkgnd(CDC* pDC)
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

	return 1;
}


int CProgStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid( IDB_BITMAP_BAR3 ) ;
	ModifyStyle(WS_BORDER,   0); 
	ModifyStyleEx(WS_EX_WINDOWEDGE,   0); 

	return 0;
}


void CProgStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


BOOL CProgStatusBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(0);

		if ( NULL == m_ProgressWnd ) {
			m_ProgressWnd = new CGIFControl ;
			m_ProgressWnd->Create(_T("") , WS_CHILD | SS_OWNERDRAW | WS_VISIBLE | SS_NOTIFY , \
				CRect(20,20,36,36) , this, 111 ) ;
		}
		
		m_Sigbmp[0].LoadBitmap(IDB_BITMAP_SIG0);  
		m_Sigbmp[1].LoadBitmap(IDB_BITMAP_SIG1);  
		m_Sigbmp[2].LoadBitmap(IDB_BITMAP_SIG2);  
		m_Sigbmp[3].LoadBitmap(IDB_BITMAP_SIG3);  

		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_UP_PROGRESS ) ;

		m_progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(66, 65, 63));//背景色
		m_progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(254, 153, 0));//前景色

		//CPostMsg postmsg(MSG_USER_UP_PROGRESS,0);
		//theApp.m_MsgQueue.pushFront(postmsg);
	}
	return bRes ;
}

LRESULT CProgStatusBar::OnShowProgressCtrl( WPARAM wParam, LPARAM lParam ) 
{
	TRACE("OnShowProgressCtrl:%s\r\n","OnShowProgressCtrl");
	CPostMsg postmsg;
	if (!theApp.m_UimsgQueue.pop(postmsg))
	{
		return 1;
	}

	uistruct::BLOCKCHANGED_t pBlockchanged; 
	string strTemp = postmsg.GetData();
	pBlockchanged.JsonToStruct(strTemp.c_str());

	if (!m_bProgressType)
	{
		
			m_progress.SetRange32( 0 , 100); 
			int  setpos = (pBlockchanged.high*1.0/pBlockchanged.tips)*100;
			setpos = setpos>100?100:setpos;
			//设置进度条的值
			m_progress.SetPos(setpos);

			m_bProgressType = TRUE;
			m_nSigIndex =pBlockchanged.connections>3?3:pBlockchanged.connections;

			if ((pBlockchanged.tips-pBlockchanged.high)<10 && !m_walletui)
			{
				TRACE("ok:%s\r\n","OnShowProgressCtrl");
				//// 发送钱包同步完毕
				CPostMsg postblockmsg(MSG_USER_MAIN_UI,WM_UPWALLET);
				theApp.m_MsgQueue.pushFront(postblockmsg); 
				LoadGifing(false);
				m_walletui = true;
			}
			//Invalidate(); 
			InvalidateRect(m_bmpsig);
		return 1;
	}

	m_nSigIndex = pBlockchanged.connections>3?3:pBlockchanged.connections;
	int  setpos = (pBlockchanged.high*1.0/pBlockchanged.tips)*100 ;
	setpos = setpos>100?100:setpos;
	//设置进度条的值
	m_progress.SetPos(setpos);
	
	if ((pBlockchanged.tips-pBlockchanged.high)<10&& !m_walletui)
	{
		TRACE("ok:%s\r\n","OnShowProgressCtrl");
		//// 发送钱包同步完毕
		CPostMsg postblockmsg(MSG_USER_MAIN_UI,WM_UPWALLET);
		theApp.m_MsgQueue.pushFront(postblockmsg); 
		LoadGifing(false);
		m_walletui = true;
	}
	//Invalidate(); 
	InvalidateRect(m_bmpsig);
	return 1;
}
//Invalidate(); 
void CProgStatusBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogBar::OnPaint()
	CDC memDC;  
	memDC.CreateCompatibleDC(&dc);  
	CRect rc;  
	GetClientRect(&rc);  

	HBITMAP hOldbmp = (HBITMAP)memDC.SelectObject(m_Sigbmp[m_nSigIndex]); 
	dc.BitBlt(900-60, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);  
	CRect rc1(900-60, 0, rc.Width(), rc.Height());
	m_bmpsig = rc1;
	memDC.SelectObject(hOldbmp);  
	memDC.DeleteDC();  
}
