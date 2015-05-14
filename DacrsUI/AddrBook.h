#pragma once
#include "ListCtrlCl.h"

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
public:
	afx_msg void OnBnClickedButtonAddaddrbook();
public:
	CListCtrlCl m_listCtrl;
	virtual BOOL OnInitDialog();
private:
	bool LoadAddrBook();
private:
	map<CString,uistruct::ADDRBOOK_t> m_mapAddrInfo;
	uistruct::ADDRBOOK_t m_selectAddr;
public:
	void GetAddrbook(uistruct::ADDRBOOK_t &addr);
	afx_msg void OnBnClickedOk();
};
