#pragma once


// CDialogBase 对话框

class CDialogBase : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogBase)

public:
	CDialogBase(UINT ID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogBase();

// 对话框数据
	enum { IDD = IDD_DIALOGBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	CFont            m_fontGrid;//字体变量
	HBITMAP		     m_HeadBmp; 
	HBITMAP           m_pBmp;
	void		     SetBkBmpNid( UINT nBitmapIn ,HBITMAP &bmp) ;
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
