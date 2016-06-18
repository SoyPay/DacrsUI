#pragma once
#include "StaticTrans.h"
#include "CommonStruct.h"
#include "ListCtrlCl.h"
#include "ShadeButtonST.h"
// CIpoDlg 对话框

class CIpoDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CIpoDlg)

public:
	CIpoDlg();   // 标准构造函数
	virtual ~CIpoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_IPO };

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
	BOOL AddListaddrDataBox();
	afx_msg void OnBnClickedButtonDrawal();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CStaticTrans           m_strTx1  ; 
	CStaticTrans           m_strTx2  ;
	CStaticTrans           m_strTx3 ;
	CShadeButtonST         m_rBtnSend;
	CShadeButtonST         m_rQueryAmout;
	void OnShowListCtrol(CString addr);
private:
		CListCtrlCl                          m_listCtrl;
		CP2PBetHelp		                    m_P2PBetHelp;
public:
	afx_msg void OnBnClickedButtonQuery();
};
