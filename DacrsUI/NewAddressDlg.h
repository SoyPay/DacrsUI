#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CNewAddressDlg 对话框

class CNewAddressDlg : public CDialogBase
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
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnNewAdd;
	CStaticTrans     m_label   ;  //
	CStaticTrans     m_code   ;  //
	CStaticTrans     m_headText;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonScdz();
};
