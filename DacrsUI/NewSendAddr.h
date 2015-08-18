#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CNewSendAddr 对话框

class CNewSendAddr : public CDialogBase
{
	DECLARE_DYNAMIC(CNewSendAddr)

public:
	CNewSendAddr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewSendAddr();

// 对话框数据
	enum { IDD = IDD_DIALOG_NEWSENDADDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	uistruct::ADDRBOOK_t m_selectAddr;
public:
	void GetAddrbook(uistruct::ADDRBOOK_t &addr);
	virtual BOOL OnInitDialog();
public:
	CStaticTrans     m_label;
	CStaticTrans     m_addr;
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
	afx_msg void OnBnClickedClose();
};
