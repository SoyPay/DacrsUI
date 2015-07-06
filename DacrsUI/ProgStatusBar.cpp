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
	m_prosshiden = false;
	m_ProgressWnd = NULL ;
	m_nSigIndex = 0 ;
	m_nLockIndex = 0;
	m_walletui = false;
	memset(m_bmpsig , 0 , sizeof(CRect));
	memset(m_bmplock , 0 , sizeof(CRect));
	m_progress.ShowPercent(FALSE);
	m_progress.ShowDefineText(TRUE);
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
	DDX_Control(pDX, IDC_STATIC_NET_TB, m_strNeting);
	DDX_Control(pDX, IDC_STATIC_HEIGHT, m_strHeight);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_strVersion);
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
					m_ProgressWnd->SetWindowPos( NULL , rc.Width()+880 , rc.Height()+8 , 0 , 0 , \
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
	OnIniLockParam();         //设置锁的变量
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

		m_strNeting.SetFont(90, _T("宋体"));				//设置显示字体和大小
		m_strNeting.SetTextColor(RGB(255,255,255));			    //字体颜色
//		m_strNeting.SetWindowText(_T("网络同步中..."));
		m_strNeting.SetWindowText(_T("获取网络连接")) ;

		m_strHeight.SetFont(90, _T("宋体"));				//设置显示字体和大小
		m_strHeight.SetTextColor(RGB(255,255,255));			    //字体颜色
		m_strHeight.SetWindowText(_T("高度:")) ;
		m_strHeight.ShowWindow(SW_HIDE) ;

		m_strVersion.SetFont(90, _T("宋体"));				//设置显示字体和大小
		m_strVersion.SetTextColor(RGB(255,255,255));	    //字体颜色
		m_strVersion.SetWindowText(_T("版本:V1.0.0.14beta")) ;


		if ( NULL == m_ProgressWnd ) {
			m_ProgressWnd = new CGIFControl ;
			m_ProgressWnd->Create(_T("") , WS_CHILD | SS_OWNERDRAW | WS_VISIBLE | SS_NOTIFY , \
				CRect(20,20,36,36) , this, 111 ) ;
		}
		
		m_Sigbmp[0].LoadBitmap(IDB_BITMAP_SIG0);  
		m_Sigbmp[1].LoadBitmap(IDB_BITMAP_SIG1);  
		m_Sigbmp[2].LoadBitmap(IDB_BITMAP_SIG2);  
		m_Sigbmp[3].LoadBitmap(IDB_BITMAP_SIG3);  

		m_Lockbmp[0].LoadBitmap(IDB_BITMAP_LOCK); 
		m_Lockbmp[1].LoadBitmap(IDB_BITMAP_UNLOCK); 

		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_UP_PROGRESS ) ;

		m_progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(66, 65, 63));//背景色
		m_progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(254, 153, 0));//前景色

		LoadGifing(TRUE);
		//CPostMsg postmsg(MSG_USER_UP_PROGRESS,0);
		//theApp.m_MsgQueue.pushFront(postmsg);
	}
	return bRes ;
}
int CProgStatusBar::ShowProgressCtrl(){
	CPostMsg postmsg;
	if (!theApp.m_UimsgQueue.pop(postmsg))
	{
		return 1;
	}

	uistruct::BLOCKCHANGED_t pBlockchanged; 
	string strTemp = postmsg.GetData();
	pBlockchanged.JsonToStruct(strTemp.c_str());
	if (pBlockchanged.tips <= 0)
	{
		return 1;
	}

	//// blocktip高度
	theApp.blocktipheight = pBlockchanged.tips ;
	if (!m_bProgressType)
	{
		m_strNeting.SetWindowText(_T("网络同步中..."));
		m_strNeting.ShowWindow(SW_HIDE);
		m_strNeting.ShowWindow(SW_SHOW);

		m_progress.SetRange32( 0 , 100); 
		int  setpos = (pBlockchanged.high*1.0/pBlockchanged.tips)*100;
		setpos = setpos>100?100:setpos;
		//设置进度条的值
		m_progress.SetPos(setpos);
		CString strText;
		strText.AppendFormat("剩余 ~%d 块没有同步到本地", pBlockchanged.tips-pBlockchanged.high);
		m_progress.SetDefinedStr(strText);
		m_bProgressType = TRUE;
		m_nSigIndex =pBlockchanged.connections>3?3:pBlockchanged.connections;

		if (pBlockchanged.tips==pBlockchanged.high)
		{
			theApp.IsSyncAppTx = TRUE;             /// 同步app交易
		}

		if ((pBlockchanged.tips-pBlockchanged.high)<10 && !m_walletui)
		{
			TRACE("ok:%s\r\n","OnShowProgressCtrl");
			//// 发送钱包同步完毕
			CPostMsg postblockmsg(MSG_USER_MAIN_UI,WM_UPWALLET);
			theApp.m_MsgQueue.pushFront(postblockmsg); 
			LoadGifing(false);
			m_walletui = true;
			theApp.IsSyncBlock = true;
		}
		Invalidate(); 
		//InvalidateRect(m_bmpsig);
		//		return 1;
	}

	m_nSigIndex = pBlockchanged.connections>3?3:pBlockchanged.connections;
	int  setpos = (pBlockchanged.high*1.0/pBlockchanged.tips)*100 ;
	setpos = setpos>100?100:setpos;
	//设置进度条的值
	m_progress.SetPos(setpos);
	CString strText;
	strText.AppendFormat("剩余 ~%d 块没有同步到本地", pBlockchanged.tips-pBlockchanged.high);
	m_progress.SetDefinedStr(strText);
	m_progress.Invalidate();

	if (pBlockchanged.tips==pBlockchanged.high)
	{
		theApp.IsSyncAppTx = TRUE;             /// 同步app交易
	}
	if ((pBlockchanged.tips-pBlockchanged.high)<10&& !m_walletui)
	{
		TRACE("ok:%s\r\n","OnShowProgressCtrl");
		//// 发送钱包同步完毕
		CPostMsg postblockmsg(MSG_USER_MAIN_UI,WM_UPWALLET);
		theApp.m_MsgQueue.pushFront(postblockmsg); 
		LoadGifing(false);
		m_walletui = true;
		theApp.IsSyncBlock = true;
	}
	if ( m_walletui && !m_prosshiden) {
		m_strNeting.SetWindowText(_T("网络已同步")) ;
		m_strNeting.ShowWindow(SW_HIDE);
		m_strNeting.ShowWindow(SW_SHOW);

		m_progress.ShowWindow(SW_HIDE);
		if ( NULL != m_ProgressWnd ) {
			m_ProgressWnd->ShowWindow(SW_HIDE) ;
		}
		m_prosshiden = !m_prosshiden ;
	}

	if (m_walletui && m_prosshiden)
	{
		CString strTips;
		strTips.Format(_T("当前高度:%d") ,pBlockchanged.tips ) ;
		m_strHeight.SetWindowText(strTips) ;
		m_strHeight.ShowWindow(SW_HIDE);
		m_strHeight.ShowWindow(SW_SHOW);
	}
	InvalidateRect(m_bmpsig);
}
void CProgStatusBar::ShowLockCtrl()
{
	CPostMsg postmsg;
	if (!theApp.m_LockmsgQueue.pop(postmsg))
	{
		return ;
	}
	CString strTemp = postmsg.GetStrType();
	if (!strcmp(strTemp,"Lock"))
	{
		m_nLockIndex = 0;
	}else if(!strcmp(strTemp,"UnLock")){
		m_nLockIndex = 1;
	}
	InvalidateRect(m_bmplock);
}
LRESULT CProgStatusBar::OnShowProgressCtrl( WPARAM wParam, LPARAM lParam ) 
{
//	TRACE("OnShowProgressCtrl:%s\r\n","OnShowProgressCtrl");
	if (wParam == WM_LOCKSTATE)
	{
		ShowLockCtrl();
	}else{
		ShowProgressCtrl();
	}
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
//	memDC.DeleteDC();  

	HBITMAP hlockbmp = (HBITMAP)memDC.SelectObject(m_Lockbmp[m_nLockIndex]); 
	dc.BitBlt(900-60-30, 3, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);  
	CRect rc2(900-60-30, 3, rc.Width(), rc.Height());
	m_bmplock = rc2;
	memDC.SelectObject(hlockbmp);  
	memDC.DeleteDC();  
}
void CProgStatusBar::OnIniLockParam()
{
	CString strCommand;
	strCommand.Format(_T("%s"),_T("islocked"));
	CStringA strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return ;

	if (strShowData.Find("islock") > 0)
	{
		theApp.IsWalletLocked = root["islock"].asBool();
		if (!theApp.IsWalletLocked)
		{
			m_nLockIndex = 1;
		}
	}

}