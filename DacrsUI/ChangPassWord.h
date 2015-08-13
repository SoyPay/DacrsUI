#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"
// CChangPassWord 对话框

class CChangPassWord : public CDialogBase
{
	DECLARE_DYNAMIC(CChangPassWord)

public:
	CChangPassWord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChangPassWord();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();
	virtual BOOL OnInitDialog();
public:
	CStaticTrans     m_oldpassword;
	CStaticTrans     m_newpassword;
	CStaticTrans     m_comfiredpassword;
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
};
