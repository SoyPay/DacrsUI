#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"

// CMainDlg �Ի���

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStaticTrans     m_strTrading  ;   
public:
	afx_msg void OnBnClickedAlltxdetail();
	virtual BOOL OnInitDialog();
public:
	void SetCtrlText();
};
