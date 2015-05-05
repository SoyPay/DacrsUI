#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"

// CMainDlg 对话框

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStaticTrans     m_strTrading  ;   
public:
	afx_msg void OnBnClickedAlltxdetail();
	virtual BOOL OnInitDialog();
public:
	void SetCtrlText();
};
