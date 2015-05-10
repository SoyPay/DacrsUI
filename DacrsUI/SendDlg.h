#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "ShadeButtonST.h"

// CSendDlg 对话框

class CSendDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CSendDlg)

public:
	CSendDlg();   // 标准构造函数
	virtual ~CSendDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendtrnsfer();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg LRESULT OnShowListaddrData(  WPARAM wParam, LPARAM lParam ) ;
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
public:
	CRoundButton     m_rBtnAddbook;
private:
	uistruct::LISTADDRLIST m_pListaddrInfo;
	CShadeButtonST         m_rBtnSend;
	CStaticTrans     m_strTx1  ;  
public:
	BOOL AddListaddrDataBox();
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
