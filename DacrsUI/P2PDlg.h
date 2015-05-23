#pragma once
#include "RoundButton.h"
#include "UseListBox.h"
#include "CommonStruct.h"
#include "RecordListBox.h"
// CP2PDlg 对话框

class CP2PDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CP2PDlg)

public:
	CP2PDlg();   // 标准构造函数
	virtual ~CP2PDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_P2P };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int      OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL     OnEraseBkgnd(CDC* pDC);
public:
	CStaticTrans           m_Balance  ; 
	CStaticTrans           m_NotDraw  ;
	CStaticTrans           m_Dw  ;
	CStaticTrans           m_VS  ;

	CShadeButtonST         m_rBtnWithd;
	CShadeButtonST         m_rBtnRech;
	CRoundButton           m_rBtnMale;
	CRoundButton           m_rBtnWoman;

	CUseListBox            m_BonusListBox;
	//CRecordListBox         m_RecordListBox;
	CComboBox			   m_addrbook;
	CP2PBetHelp		                    m_P2PBetHelp;
public:
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShowListCtrol(  WPARAM wParam, LPARAM lParam ) ;
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboAddres();
	void InsertComboxIitem();
	BOOL AddListaddrDataBox();
	void QueryNotDrawBalance();
	afx_msg void OnBnClickedButtonWithd();
	afx_msg void OnBnClickedButtonRech();
	afx_msg void OnBnClickedButtonMale();
public:
	void SendBet(int rewardnum);
	afx_msg void OnBnClickedButtonWoman();
	void OnListPool();
	void AcceptBet(CString hash,CString money);
	bool CheckBalance();
};
