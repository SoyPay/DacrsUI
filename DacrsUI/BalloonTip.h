#pragma once


// CBalloonTip 框架

class CBalloonTip : public CFrameWnd
{
//protected:
//	CBalloonTip();           // 动态创建所使用的受保护的构造函数
//	virtual ~CBalloonTip();

protected:
	DECLARE_MESSAGE_MAP()
public:

	// Implementation

	// Creates a new CBalloonTip Object
	static CBalloonTip* Show(
		CPoint pt,         // Point where the balloon tip will be 
		CSize size,        // Size of the balloon
		CString strMessage,// Messsage to be shown
		LOGFONT lf, // A LOGFONT structure from which the message font will created 
		UINT nSecs, // Seconds for which the balloon will be shown
		BOOL bBalloonUp,    // Is balloon show up or upside down?
		BOOL Bpic = TRUE
		);

	// Makes the window invisible, destroys, and releases resources, if required to destroy before
	// the Timer ticks.
	static void Hide(CBalloonTip* pBalloonTip);
	static int nBalloonInstances;

protected:
	virtual void PostNcDestroy();

	// Implementation

	CBalloonTip(CString strMessage, LOGFONT lf, BOOL bBalloonUp,BOOL Bpic = TRUE);
	~CBalloonTip();
	BOOL Create(CRect rect);       // Create the Windows(R) window, HANDLE etc ...
	void MakeVisisble(UINT nSecs); // Show the balloon for nSecs Seconds

	// Overrides and messages
	//{
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnPaint();
		//
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Attributes
	BOOL    m_bBalloonUp;   // Is balloon up or upside down
	BOOL    m_showpic;
	CRect   m_rectText;     // Rectabgle where the text will be in the balloon
	CRect   m_rectWindow;   // Rectangle in which the balloon will be displayed in screen coordinates
	//CString m_strMessage;   // Message to be displayed in the balloon
	CFont   m_fontBalloonText; // Font of the message
	CRgn    m_rgnTip;          // The region of the tip
	CRgn    m_rgnRoundRect;   // The region of the round rectangle 
	CWnd    m_wndInvisibleParent; // Invisible parent of this window, this avoids the balloon's
public:
	CRect m_close;
	CString m_strMessage; 
	// appreance in the Taskbar
};


