#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CSignAccountsDlg 对话框

class CSignAccountsDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CSignAccountsDlg)

public:
	CSignAccountsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSignAccountsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SIGN_ACCOUNTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont            m_fontGrid;//字体变量
public:
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnSend;
	CStaticTrans     m_addr   ;  //
	CStaticTrans     m_fee   ;  //
	CStaticTrans     m_headText;
public:
	afx_msg void OnBnClickedButtonSend();
	void SetShowAddr(CString addr);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonClose();
};
