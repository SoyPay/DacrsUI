#pragma once


// CSendDlg �Ի���

class CSendDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CSendDlg)

public:
	CSendDlg();   // ��׼���캯��
	virtual ~CSendDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
