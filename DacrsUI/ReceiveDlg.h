#pragma once


// CReceiveDlg 对话框

class CReceiveDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CReceiveDlg)

public:
	CReceiveDlg();   // 标准构造函数
	virtual ~CReceiveDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECEIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
