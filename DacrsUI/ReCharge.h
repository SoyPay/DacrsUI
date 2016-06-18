#pragma once
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CReCharge 对话框

class CReCharge : public CDialogBase
{
	DECLARE_DYNAMIC(CReCharge)

public:
	CReCharge(CWnd* pParent = NULL,CString text = _T(""),CString strsms = _T(""),CString strhead = _T(""),CString amount=_T(""));   // 标准构造函数
	virtual ~CReCharge();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
private:
	CStaticTrans     m_Text;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
	CString          m_strText;
	CString          m_strSms;
	CString          m_strHead;
	CString          m_edit;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonGb1();
};
