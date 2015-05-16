#pragma once
#include "RoundButton.h"
#include "ListCtrlCl.h"
#include "ShadeButtonST.h"

// CTradDlg 对话框

class CTradDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CTradDlg)

public:
	CTradDlg();   // 标准构造函数
	virtual ~CTradDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	//CMFCLinkCtrl v_linkCtrl1;
	//CMFCLinkCtrl v_linkCtrl2;
	afx_msg void OnBnClickedButtonTxdetail();
	void OninitializeList();
private:
	CListCtrlCl        m_listCtrl;
	CShadeButtonST     m_rBtnTxdetail;
public:
	afx_msg void OnNMDblclkListListtx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
