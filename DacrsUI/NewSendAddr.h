#pragma once


// CNewSendAddr �Ի���

class CNewSendAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CNewSendAddr)

public:
	CNewSendAddr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewSendAddr();

// �Ի�������
	enum { IDD = IDD_DIALOG_NEWSENDADDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	uistruct::ADDRBOOK_t m_selectAddr;
public:
	void GetAddrbook(uistruct::ADDRBOOK_t &addr);
};
