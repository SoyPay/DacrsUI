#pragma once


// CSendDlg 对话框

class CSendDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CSendDlg)

public:
	CSendDlg();   // 标准构造函数
	virtual ~CSendDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
