#pragma once


// CAddrBook 对话框

class CAddrBook : public CDialogEx
{
	DECLARE_DYNAMIC(CAddrBook)

public:
	CAddrBook(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddrBook();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDRBOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
