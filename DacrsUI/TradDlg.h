#pragma once
#include "RoundButton.h"
#include "ListCtrlCl.h"
#include "ShadeButtonST.h"

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
