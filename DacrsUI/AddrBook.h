#pragma once


// CAddrBook �Ի���

class CAddrBook : public CDialogEx
{
	DECLARE_DYNAMIC(CAddrBook)

public:
	CAddrBook(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddrBook();

// �Ի�������
	enum { IDD = IDD_DIALOG_ADDRBOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
