#pragma once
#include "afxcmn.h"


// CListAddr �Ի���

class CListAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CListAddr)

public:
	CListAddr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CListAddr();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	virtual BOOL OnInitDialog();
	void ShowListInfo();
};
