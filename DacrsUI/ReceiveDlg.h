#pragma once
#include "SignAccountsDlg.h"
#include "NewAddressDlg.h"
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
public:
	void ShowListInfo();
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnBnClickedCopyaddress();
	afx_msg LRESULT OnShowListCtrl(  WPARAM wParam, LPARAM lParam ) ;
public:
	CListCtrl m_listCtrl;
private:
	CSignAccountsDlg *m_accountDlg;
	CNewAddressDlg   *m_newaddrDlg;
public:
	afx_msg void OnBnClickedButtonSignAccount();
	afx_msg void OnBnClickedButtonNewaddress();
};
