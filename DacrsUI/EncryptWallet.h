#pragma once


// CEncryptWallet 对话框

class CEncryptWallet : public CDialogEx
{
	DECLARE_DYNAMIC(CEncryptWallet)

public:
	CEncryptWallet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEncryptWallet();

// 对话框数据
	enum { IDD = IDD_DIALOG_ENCRYPTWALLET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
