#pragma once


// CWalletPassPhrase �Ի���

class CWalletPassPhrase : public CDialogEx
{
	DECLARE_DYNAMIC(CWalletPassPhrase)

public:
	CWalletPassPhrase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWalletPassPhrase();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETWALLETENTRP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};
