#pragma once


// CTradDlg �Ի���

class CTradDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CTradDlg)

public:
	CTradDlg();   // ��׼���캯��
	virtual ~CTradDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_TRAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
