#pragma once


// CMortgageTardDlg 对话框

class CMortgageTardDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CMortgageTardDlg)

public:
	CMortgageTardDlg();   // 标准构造函数
	virtual ~CMortgageTardDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MORTGAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
