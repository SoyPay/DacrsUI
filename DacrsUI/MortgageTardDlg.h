#pragma once


// CMortgageTardDlg �Ի���

class CMortgageTardDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CMortgageTardDlg)

public:
	CMortgageTardDlg();   // ��׼���캯��
	virtual ~CMortgageTardDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MORTGAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
