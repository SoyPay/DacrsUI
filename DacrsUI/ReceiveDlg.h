#pragma once


// CReceiveDlg �Ի���

class CReceiveDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CReceiveDlg)

public:
	CReceiveDlg();   // ��׼���캯��
	virtual ~CReceiveDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECEIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
