#pragma once


// CChangPassWord 对话框

class CChangPassWord : public CDialogEx
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
};
