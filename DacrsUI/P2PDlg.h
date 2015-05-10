#pragma once


// CP2PDlg 对话框

class CP2PDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CP2PDlg)

public:
	CP2PDlg();   // 标准构造函数
	virtual ~CP2PDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_P2P };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
