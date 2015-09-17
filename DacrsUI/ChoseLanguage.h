#pragma once
#include "afxwin.h"


// CChoseLanguage 对话框

class CChoseLanguage : public CDialogBase
{
	DECLARE_DYNAMIC(CChoseLanguage)

public:
	CChoseLanguage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChoseLanguage();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHOSELANGUAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_language;
	afx_msg void OnCbnSelchangeCombolanguage();
	virtual BOOL OnInitDialog();
	CComboBox m_laguabox;
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
	afx_msg void OnBnClickedClose();
};
