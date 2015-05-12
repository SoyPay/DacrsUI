#pragma once


// COutGifDlg 对话框

class COutGifDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COutGifDlg)

public:
	COutGifDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COutGifDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_OUTGIF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic		     *m_ProgressWnd ;	//Progress窗口
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	void             LoadGifing( BOOL bState );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
};
