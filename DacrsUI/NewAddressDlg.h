#pragma once
#include "CommonStruct.h"
// CNewAddressDlg 对话框

class CNewAddressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewAddressDlg)

public:
	CNewAddressDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewAddressDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GENERATE_NEWADDRES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg void OnPaint();
};
