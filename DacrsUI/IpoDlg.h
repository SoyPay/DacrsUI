#pragma once


// CIpoDlg �Ի���

class CIpoDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CIpoDlg)

public:
	CIpoDlg();   // ��׼���캯��
	virtual ~CIpoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_IPO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
