#pragma once


// CEncryptWallet �Ի���

class CEncryptWallet : public CDialogEx
{
	DECLARE_DYNAMIC(CEncryptWallet)

public:
	CEncryptWallet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEncryptWallet();

// �Ի�������
	enum { IDD = IDD_DIALOG_ENCRYPTWALLET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
