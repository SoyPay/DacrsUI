#pragma once


// CIpoDlg 对话框

class CIpoDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CIpoDlg)

public:
	CIpoDlg();   // 标准构造函数
	virtual ~CIpoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_IPO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
