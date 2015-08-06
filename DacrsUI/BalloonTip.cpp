// BalloonTip.cpp : 实现文件
//

#include "stdafx.h"
#include "BalloonTip.h"
#include "DacrsUI.h"


// CBalloonTip

#define  ID_TIMER 100

BEGIN_MESSAGE_MAP(CBalloonTip, CFrameWnd)
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBalloonTip 消息处理程序
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::CBalloonTip()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Constructor
 
 Parameters :
    1. strMessage -> Message to be shown in the balloon
    2. lf         -> LOGFONT structure from which the the message font
                     will be created.
    3. bBalloonUp -> Is the balloon up or upside down?
     
 Return Value : none
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
CBalloonTip::CBalloonTip(CString strMessage, LOGFONT lf, BOOL bBalloonUp,BOOL Bpic)
{
 
    m_strMessage       = strMessage;
    m_bBalloonUp       = bBalloonUp;
	m_showpic          = Bpic;
     
    VERIFY(m_fontBalloonText.CreateFontIndirect(&lf));
 
}
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::~CBalloonTip()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Destructor
 
 Parameters : none
     
 Return Value : none
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
CBalloonTip::~CBalloonTip()
{
 
 
}
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::Show()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Creates the CBalloonTip on the heap, forces heap creation
 
 Parameters :
    1. pt -> Point of the balloon tip
    2. size -> Size of the window(width and height)
    3. strMessage -> Message to be shown in the balloon
    4. nSecs -> Seconds for which the balloon will be shown
    5. lf -> LOGFONT structure from which the the message font
             will be created.
    6. bBalloonUp -> Is balloon up or upside down?
 
 Return Value : void
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 static   long rwith = 0;
CBalloonTip* CBalloonTip::Show(CPoint pt, CSize size, CString strMessage, LOGFONT lf, UINT nSecs, BOOL bBalloonUp,BOOL Bpic)
{
 
	if( CBalloonTip::nBalloonInstances != 0) // No window creation if already one instance of the class
	{
 
		return NULL;
     
	}
 
	CBalloonTip* pBalloonTip = new CBalloonTip(strMessage, lf, bBalloonUp,Bpic);
     
	CBalloonTip::nBalloonInstances = 1; // Only one instance of window possible.
         
	UINT nRectLeft;
	UINT nRectRight;
	UINT nRectTop; 
	UINT nRectBottom;
 
	rwith = size.cx;

    // Rectangle co-ordinates w.r.t balloon tip point
// Balloon tip at CPoint pt shold remain there without change in window size
    if (bBalloonUp)
    {
 
		nRectLeft   = (UINT)(pt.x - (size.cx * 0.65));
		nRectRight  = (UINT)(pt.x + (size.cx * 0.35));
		nRectTop    = pt.y - size.cy;
		nRectBottom = pt.y;
     
	}
    else
    {
 
		nRectLeft   = (UINT)(pt.x - (size.cx * 0.35));
		nRectRight  = (UINT)(pt.x + (size.cx * 0.65));
		nRectTop    = pt.y;
		nRectBottom = pt.y + (size.cy);
     
	}
         
	pBalloonTip->Create(CRect(nRectLeft, nRectTop, nRectRight, nRectBottom));   
	pBalloonTip->MakeVisisble(nSecs);
 
    return pBalloonTip;
 
}
 
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::MakeVisible()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Shows the balloon and sets the timer for it's eventual destruction.
 
 Parameters :
    1. nSecs -> Seconds for the balloon to be shown
     
 Return Value : none
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
void CBalloonTip::MakeVisisble(UINT nSecs)
{
 
    ASSERT(nSecs > 0);
 
    SetTimer(ID_TIMER, (nSecs * 1000), NULL);
         
    CRect rect;
    GetWindowRect(&rect);
 
    // Caption bar height to offeset when the balloon is shown
    int nCaptionBarSize = ::GetSystemMetrics(SM_CYCAPTION);
    int nVerticalBorderSize = ::GetSystemMetrics(SM_CYSIZEFRAME);
 
    if(m_bBalloonUp)
    {
 
        // Account for the missing title bar and border thickness in the 3rd parameter
        // as the balloon shifts up by the height of title bar and window tickness
        SetWindowPos(
            &wndTopMost,
            m_rectWindow.left,
            (m_rectWindow.top + nCaptionBarSize + (2 * nVerticalBorderSize)),
            m_rectWindow.right,
            m_rectWindow.bottom,
            SWP_SHOWWINDOW | SWP_NOACTIVATE
        );
     
}
    else
    {
 
        // Account for the missing title bar and border thickness in the 3rd parameter
        // as the balloon shifts up by the height of window tickness.
        SetWindowPos(
            &wndTopMost,
            m_rectWindow.left,
            m_rectWindow.top - nVerticalBorderSize,
            m_rectWindow.right,
            m_rectWindow.bottom,
            SWP_SHOWWINDOW | SWP_NOACTIVATE
        );
     
}
 
}
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::OnTimer()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Gets called by MFC for an WM_TIMER message
 
 Parameters :
        1. nIDEvent -> Timer ID   
 
 Return Value : void
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
void CBalloonTip::OnTimer(UINT nIDEvent)
{
 
    KillTimer(ID_TIMER);
    DestroyWindow();
 
}
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::OnLButtonDown()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Gets called by MFC for an WM_LBUTTONDOWN message
 
 Parameters :
        See MFC documentation
 
 Return Value : void
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
void CBalloonTip::OnLButtonDown(UINT nFlags, CPoint point)
{
    
	if (point.x >= m_close.left && point.x <= m_close.right && point.y >= m_close.top && point.y <= m_close.bottom && m_showpic)
	{
		KillTimer(ID_TIMER);
		DestroyWindow();
	}

 
}
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::OnRButtonDown()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Gets called by MFC for an WM_RBUTTONDOWN message
 
 Parameters :
        See MFC documentation
 
 Return Value : void
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
void CBalloonTip::OnRButtonDown(UINT nFlags, CPoint point)
{
 
  //  KillTimer(ID_TIMER);
  //  DestroyWindow();
 
}
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::OnCreate()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Gets called by MFC for an WM_CREATE message
 
 Parameters :
        See MFC documentation
 
 Return Value : int
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
int CBalloonTip::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
 
		return -1;
     
	}
 
	// Remove caption and thick frame for proper shading
	ModifyStyle(WS_CAPTION | WS_THICKFRAME, 0);
 
	CRect t_Rect;
	GetClientRect(&t_Rect);  // Get the client rectangle to set the balloon region
 
	if (m_bBalloonUp)
	{
 
		// Calculation of Text window, where to write text in the balloon
		m_rectText.left   = 10;//(UINT)(t_Rect.Width() * 0.10);
		//m_rectText.right  = (UINT)(t_Rect.Width() * 0.90);
		m_rectText.right  = (UINT)(t_Rect.Width() );
		m_rectText.top    = (UINT)(t_Rect.Height() * 0.10);
		m_rectText.bottom = (UINT)(t_Rect.Height()* 0.70);
             
		// Create an elliptical region out of the client rectangle
		m_rgnRoundRect.CreateRoundRectRgn(
			t_Rect.left,
			t_Rect.top,
			t_Rect.right,
			(UINT)(t_Rect.bottom - (t_Rect.Height() * 0.25)),
			(UINT)(abs(t_Rect.left - t_Rect.right) * 0.18),
			(UINT)(t_Rect.Height() * 0.25)
		);
    
		// Start the process of creating the balloon tip
		CPoint ptTri[4];
		ptTri[0].x =(UINT)( (t_Rect.left) + (t_Rect.Width() * 0.93));
		ptTri[0].y = (UINT)((t_Rect.bottom) - (t_Rect.Height() * 0.40));
     
		ptTri[1].x = (UINT)((t_Rect.left) + (t_Rect.Width() * 0.65));
		ptTri[1].y = t_Rect.bottom;
 
		ptTri[2].x = (UINT)((t_Rect.left) + (t_Rect.Width() * 0.70));
		ptTri[2].y = (UINT)((t_Rect.bottom) - (t_Rect.Height() * 0.40));
    
		ptTri[3].x = (UINT)((t_Rect.left) + (t_Rect.Width() * 0.93));
		ptTri[3].y = (UINT)((t_Rect.bottom) - (t_Rect.Height() * 0.40));
    
		m_rgnTip.CreatePolygonRgn(ptTri,2, ALTERNATE);
     
	}
	else
	{
 
		// Calculation of Text window, where to write text in the balloon
			m_rectText.left   =(UINT) (t_Rect.Width() * 0.10);
			m_rectText.right  = (UINT)(t_Rect.Width() * 0.90);
			m_rectText.top    = (UINT)(t_Rect.Height() * 0.30);
			m_rectText.bottom = (UINT)(t_Rect.Height() * 0.90);
         
			// Create an elliptical region out of the client rectangle
		m_rgnRoundRect.CreateRoundRectRgn(
				t_Rect.left,
				(UINT)(t_Rect.bottom - (t_Rect.Height() * 0.75)),
				t_Rect.right,
				t_Rect.bottom,
				(UINT)(abs(t_Rect.left - t_Rect.right) * 0.18),
				(UINT)(t_Rect.Height() * 0.25)
			);
 
			// Start the process of creating the balloon tip
		CPoint ptTri[4];
		ptTri[0].x = (UINT)((t_Rect.left) + (t_Rect.Width() * 0.07));
		ptTri[0].y = (UINT)((t_Rect.Height() * 0.40));
             
		ptTri[1].x = (UINT)((t_Rect.left) + (t_Rect.Width() * 0.35));
		ptTri[1].y = t_Rect.bottom - t_Rect.Height();
 
		ptTri[2].x = (UINT)((t_Rect.left) + (t_Rect.Width() * 0.30));
		ptTri[2].y = (UINT)(t_Rect.Height() * 0.40);
            
		ptTri[3].x = (UINT)((t_Rect.left) + (t_Rect.Width() * 0.07));
		ptTri[3].y = (UINT)(t_Rect.Height() * 0.40);
            
		m_rgnTip.CreatePolygonRgn(ptTri, 3, ALTERNATE);
     
	}
     
    // Create the combined region with ellipse and tip
	CRgn rgnComb;
     
	rgnComb.CreateRectRgn(t_Rect.left, t_Rect.top, t_Rect.right, t_Rect.bottom);
 
	int iRetComb = rgnComb.CombineRgn(&m_rgnTip, &m_rgnRoundRect, RGN_OR);
 
	if (iRetComb == ERROR)
	{
 
		::MessageBox( NULL,_T("ERROR in Combining Region"),_T("提示"),MB_ICONINFORMATION );
		return -1;
 
	}
    
	SetWindowRgn(rgnComb.operator HRGN(), TRUE);
 
return 0;
 
}
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::OnPaint()
 
 Created: Nov 1, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Gets called by MFC for a WM_PAINT message
 
 Parameters :
        See MFC documentation
 
 Return Value : void
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
void CBalloonTip::OnPaint()
{
 
	CPaintDC dc(this); // device context for painting
 
    CRect t_Rect;
    GetClientRect(&t_Rect);
 

    // Get the total balloon, inclding all cut out regions
	CRgn rgnComb;
    rgnComb.CreateRectRgn(t_Rect.left, t_Rect.top, t_Rect.right, t_Rect.bottom);
    
    // Create a balloon tip from this total region
    int iRetComb = rgnComb.CombineRgn(&m_rgnTip, &m_rgnRoundRect, RGN_OR );
    if (iRetComb == ERROR)
	{
		::MessageBox( NULL,_T("ERROR in Combining Region"),_T("提示"),MB_ICONINFORMATION );
		return;

	}
 
	CBrush brOutlineBrush;
    brOutlineBrush.CreateSolidBrush(RGB(0, 0, 0)); // blue
    
    CBrush brFillBrush;
   // brFillBrush.CreateSolidBrush(RGB(249, 254, 188)); // light yellow
//	brFillBrush.CreateSolidBrush(RGB(220, 220, 220)); // light yellow
	brFillBrush.CreateSolidBrush(RGB(249, 249, 249));
//	brFillBrush.CreateSolidBrush(RGB(255, 244, 244));
    
    
    dc.FillRgn(&rgnComb, &brFillBrush);  // Fill the balloon
    dc.FrameRgn(&rgnComb, &brOutlineBrush, 2, 1); // Outline the balloon
 
    int nBkMode          = dc.SetBkMode(TRANSPARENT);
    COLORREF clrPrevious =  dc.SetTextColor(RGB(105,105,105));
    CFont *pFont = dc.SelectObject(&m_fontBalloonText);
    
    CSize czTextWidth = dc.GetTextExtent(m_strMessage);
    
    if (czTextWidth.cx < m_rectText.Width())
    {
 
       dc.DrawText(m_strMessage, m_rectText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
     }
     else
    {
 
        dc.DrawText(m_strMessage, m_rectText, DT_LEFT | DT_WORDBREAK);//DT_CENTER
     }
     
	if (m_showpic)
	{
		CBitmap          m_bmp;
		m_bmp.LoadBitmap(IDB_BITMAP_TIPCLOSE);


		BITMAP bm1;
		m_bmp.GetObject(sizeof(BITMAP),&bm1);

		int width = bm1.bmWidth; //图片的宽度（逻辑单位）。 
		int height=	bm1.bmHeight; ////图片的高度（逻辑单位）。

		CDC memDC;  
		memDC.CreateCompatibleDC(&dc);  
		CRect rc;  
		GetClientRect(&rc);  

		HBITMAP hOldbmp = (HBITMAP)memDC.SelectObject(m_bmp); 
		dc.BitBlt(rwith -35, 5, width,height, &memDC, 0, 0, SRCCOPY); 
		CRect close(rwith -35,5, rc.Width(), rc.Height());
		m_close = close;
		memDC.SelectObject(hOldbmp); 
		memDC.DeleteDC(); 
	}

    // Restore the DC to its oroginal state
    dc.SelectObject(pFont);
    dc.SetBkColor(nBkMode);
    dc.SetTextColor(clrPrevious);
 
}
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::PreCreateWindow(CREATESTRUCT& cs)
 
 Created: Dec 07, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Called by MFC when a before WM_CREATE message is received by the
            window.
 
            We override this function so that the window so created is not
            visible in the Taskbar. We make this window the child of an
            invisible parent m_wndInvisibleParent.
 
 Parameters : Look in MFC documentation
 
 Return Value : BOOL
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
BOOL CBalloonTip::PreCreateWindow(CREATESTRUCT& cs)
{
 
if (!CWnd::PreCreateWindow(cs))
    {
 
return FALSE;
     
}
 
 // Create invisible parent window
 if (!::IsWindow(m_wndInvisibleParent.m_hWnd))
 {
 
        // Try creating the invisible parent window
        PCSTR pstrOwnerClass = (LPSTR)::AfxRegisterWndClass(0);
        
        BOOL bError = m_wndInvisibleParent.CreateEx(
                          0,
                          (LPCTSTR)pstrOwnerClass,
                          _T(""),
                          WS_POPUP,
          0,
                          0,
                          0,
                          0,
          NULL,
                          0
                      );
 
        if (bError == FALSE)
        {
 
return FALSE;
         
}
  
}
 
    // Set the invisible window as the parent of "this" window
cs.hwndParent = m_wndInvisibleParent.m_hWnd;
 
return TRUE;
 
}
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::Create()
 
 Created: Dec 07, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Called by the static CBalloonTip to create a Windows(R) window
 
 Parameters : rect-> a rectangle in screen co ordinates where the balloon
              will be displayed.
 
 Return Value : BOOL
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
BOOL CBalloonTip::Create(CRect rect)
{
 
    m_rectWindow = rect;
 
    PCSTR pstrOwnerClass = (LPSTR)::AfxRegisterWndClass(0);
    BOOL bResult = CFrameWnd::Create((LPCTSTR)pstrOwnerClass, NULL, WS_OVERLAPPED, m_rectWindow);
     
    return bResult;
 
}
 
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::Hide()
 
 Created: Dec 08, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Provided to destroy a balloon before it self destructs. If it is
            already hidden, it will have no effect.
             
 Parameters : CBalloonTip*
 
 Return Value : void
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
void CBalloonTip::Hide(CBalloonTip* pBalloonTip)
{
 
    if (CBalloonTip::nBalloonInstances != 0)
    {
 
        pBalloonTip->ShowWindow(SW_HIDE);
        pBalloonTip->KillTimer(ID_TIMER);
        pBalloonTip->DestroyWindow();
     
}
 
}
 
 
 
 
/*-----------------------------------------------------------------------------
  int CBalloonTip::nBalloonInstances;
 
  Abstract: Used for preventing more than one instance of the class CBalloonTip
-----------------------------------------------------------------------------*/
 
int CBalloonTip::nBalloonInstances = 0;
 
 
 
 
/*-----------------------------------------------------------------------------
 Function : CBalloonTip::PostNcDestroy()
 
 Created: Dec 08, 2001
 Author:  Prateek Kaul
 e-mail:  kaulpr@yahoo.com
 
 Abstract : Last function the window receives when it is destroyed.
            Used for resetting the counter CBalloonTip::nBalloonInstances to 0,
            so that a new instance of this class can be created.
             
 Parameters : void
 
 Return Value : void
 
 Exceptions : none
 
 Revisions : none
----------------------------------------------------------------------------*/
 
void CBalloonTip::PostNcDestroy()
{
 
    CBalloonTip::nBalloonInstances = 0;
    CFrameWnd::PostNcDestroy();
 
}