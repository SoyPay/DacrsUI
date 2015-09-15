#pragma once


// CAddApp 对话框

class CAddApp : public CDialogBar
{
	DECLARE_DYNAMIC(CAddApp)

public:
	CAddApp();   // 标准构造函数
	virtual ~CAddApp();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDAPP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
};
