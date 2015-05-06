#pragma once
#include "SignAccountsDlg.h"
#include "NewAddressDlg.h"
// CReceiveDlg 对话框

class CReceiveDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CReceiveDlg)

public:
	CReceiveDlg();   // 标准构造函数
	virtual ~CReceiveDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECEIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
