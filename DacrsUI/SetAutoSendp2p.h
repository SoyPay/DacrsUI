#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"
#include "afxwin.h"

// CSetAutoSendp2p 对话框

class CSetAutoSendp2p : public CDialogBase
{
	DECLARE_DYNAMIC(CSetAutoSendp2p)

public:
	CSetAutoSendp2p(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetAutoSendp2p();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETAUTOSEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CStaticTrans     m_tips;
	CStaticTrans     m_set;
	CStaticTrans     m_per;
	CStaticTrans     m_amount;
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
	CString          m_strtips;
	CString          m_money;
	int              m_comper;
	CEdit            m_Edit;
	CString          m_percent;
public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnUpdatePercent();
	afx_msg void OnEnKillfocusSendmoney();
	bool IsAllDigtal(const char* pData);
	void GetSendBetParam(string  &sendbetmoney,  string &accept_pecent);
};
