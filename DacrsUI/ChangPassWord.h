#pragma once


// CChangPassWord �Ի���

class CChangPassWord : public CDialogEx
{
	DECLARE_DYNAMIC(CChangPassWord)

public:
	CChangPassWord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangPassWord();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
