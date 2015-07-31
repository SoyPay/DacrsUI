#pragma once


// CSetAppId 对话框

class CSetAppId : public CDialogEx
{
	DECLARE_DYNAMIC(CSetAppId)

public:
	CSetAppId(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetAppId();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETAPPID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetDaluft();
	virtual BOOL OnInitDialog();
	bool IsRegId(const char* pData);
};
