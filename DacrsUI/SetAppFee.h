#pragma once


// CSetAppFee 对话框

class CSetAppFee : public CDialogEx
{
	DECLARE_DYNAMIC(CSetAppFee)

public:
	CSetAppFee(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetAppFee();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETFEE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool IsAllDigtal(const char* pData);
	bool IsRegId(const char* pData);
};
