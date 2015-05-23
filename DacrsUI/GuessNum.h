#pragma once


// CGuessNum 对话框

class CGuessNum : public CDialogEx
{
	DECLARE_DYNAMIC(CGuessNum)

public:
	CGuessNum(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGuessNum();

// 对话框数据
	enum { IDD = IDD_DIALOG_GUESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
