#pragma once


// CSetAppFee �Ի���

class CSetAppFee : public CDialogEx
{
	DECLARE_DYNAMIC(CSetAppFee)

public:
	CSetAppFee(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetAppFee();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETFEE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool IsAllDigtal(const char* pData);
	bool IsRegId(const char* pData);
};
