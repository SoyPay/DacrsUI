#pragma once


// CSignAccountsDlg �Ի���

class CSignAccountsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSignAccountsDlg)

public:
	CSignAccountsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSignAccountsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SIGN_ACCOUNTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
		void SetShowAddr(CString addr);
};
