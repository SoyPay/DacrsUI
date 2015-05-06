#pragma once


// CSignAccountsDlg 对话框

class CSignAccountsDlg : public CDialogEx
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
	afx_msg void OnBnClickedButtonSend();
		void SetShowAddr(CString addr);
};
