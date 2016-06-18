#pragma once
#include "StaticTrans.h"
#include "CommonStruct.h"
#include "ListCtrlCl.h"
#include "ShadeButtonST.h"

// CIpoCoin 对话框

class CIpoCoin : public CDialogBar
{
	DECLARE_DYNAMIC(CIpoCoin)

public:
	CIpoCoin();   // 标准构造函数
	virtual ~CIpoCoin();

// 对话框数据
	enum { IDD = IDD_DIALOG_GETIPO };

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
	CShadeButtonST         m_rBtnSend;
	CShadeButtonST         m_rQueryAmout;
	void OnShowListCtrol(CString addr);
	double GetFreeMoney(CString addr);
	CComboBox             m_listapp;
private:
	CListCtrlCl                          m_listCtrl;
	CP2PBetHelp		                    m_P2PBetHelp;
	string                              m_appid;
public:
	afx_msg void OnBnClickedButtonQuery();
	void  GetAppAccountSomeMoney();
	void CheckBalance(double dmoney);
	void InitCombox();
	string GetAppID(string AppName);
	afx_msg void OnCbnSelchangeComboappid();
};
