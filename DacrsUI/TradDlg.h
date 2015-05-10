#pragma once


// CTradDlg 对话框

class CTradDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CTradDlg)

public:
	CTradDlg();   // 标准构造函数
	virtual ~CTradDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
