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

	CStaticTrans     m_strDesTx1  ;  
	CStaticTrans     m_strDesTx2  ;  
	CStaticTrans     m_strDesTx3  ;  
	CStaticTrans     m_strDesTx4  ;  
	CStaticTrans     m_strDesTx5  ;  

	CStaticTrans     m_strOver      ;
	CStaticTrans     m_strOking     ;
	CStaticTrans     m_strTranNum   ;
	//CStaticTrans	 m_strRecvOking;

	CRoundButton     m_rBtnAllTxdetail;
	CStatic          m_sysWallet;
public:
	afx_msg void OnBnClickedAlltxdetail();
	afx_msg LRESULT OnShowListCtorl(  WPARAM wParam, LPARAM lParam ) ;
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	void SetCtrlText();
	void ClearCtrlText();
	void OnnitCtrlText();
	void onnitLinkText();
	void SetShowCtrol();
private:
	bool GetUrlServer();
	map<CString,CString> m_url;
	CMFCLinkCtrl *v_linkCtrl;
	CMFCLinkCtrl *v_linkCtrl1;
	CMFCLinkCtrl *v_linkCtrl2;
	CMFCLinkCtrl *v_linkCtrl3;
	CMFCLinkCtrl *v_linkCtrlQQ;
	CMFCLinkCtrl *v_linkCtrlBlock;
public:
	// 接收金额确认add by lbf 2016-06-02
	
};
