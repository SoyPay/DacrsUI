#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CP2pTip 对话框

class CP2pTip : public CDialogBase
{
	DECLARE_DYNAMIC(CP2pTip)

public:
	CP2pTip(CWnd* pParent = NULL,CString strtips="",CString stmoeny="");   // 标准构造函数
	virtual ~CP2pTip();

// 对话框数据
	enum { IDD = IDD_DIALOG_P2PTIP };

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
public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool IsAllDigtal(const char* pData);
	afx_msg void OnEnUpdatePercent();
};
