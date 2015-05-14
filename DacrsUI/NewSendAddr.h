#pragma once


// CNewSendAddr 对话框

class CNewSendAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CNewSendAddr)

public:
	CNewSendAddr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewSendAddr();

// 对话框数据
	enum { IDD = IDD_DIALOG_NEWSENDADDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	uistruct::ADDRBOOK_t m_selectAddr;
public:
	void GetAddrbook(uistruct::ADDRBOOK_t &addr);
};
