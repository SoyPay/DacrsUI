#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "CommonStruct.h"
#include <map>
// CMainDlg 对话框

class CMainDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg();   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
public:
	CStaticTrans     m_strTrading  ;   
	CStaticTrans     m_strTrading2  ;   
	CStaticTrans     m_strTrading3  ;   
	CStaticTrans     m_strTrading4  ;   
	CStaticTrans     m_strTrading5  ;   

	CStaticTrans     m_strTx1  ;  
	CStaticTrans     m_strTx2  ;  
	CStaticTrans     m_strTx3  ;  
	CStaticTrans     m_strTx4  ;  
	CStaticTrans     m_strTx5  ;  

	CStaticTrans     m_strOver      ;
	CStaticTrans     m_strOking     ;
	CStaticTrans     m_strTranNum   ;

	CRoundButton     m_rBtnAllTxdetail;
	CRoundButton     m_rBtnImportWallet;
	CRoundButton     m_rBtnDumpWallet;
public:
	afx_msg void OnBnClickedAlltxdetail();
	afx_msg LRESULT OnShowListCtorl(  WPARAM wParam, LPARAM lParam ) ;
public:
	void SetCtrlText();
	void ClearCtrlText();
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	bool GetUrlServer();
	map<CString,CString> m_url;
	CMFCLinkCtrl v_linkCtrl;
	CMFCLinkCtrl v_linkCtrl1;
	CMFCLinkCtrl v_linkCtrl2;
	CMFCLinkCtrl v_linkCtrl3;
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void OnnitCtrlText();
	void onnitLinkText();
	afx_msg void OnBnClickedButtonImportwallet();
	afx_msg void OnBnClickedButtonDumpwallet();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetShowCtrol();
};
