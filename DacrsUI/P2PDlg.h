#pragma once
#include "RoundButton.h"
#include "UseListBox.h"
#include "CommonStruct.h"
#include "TabCtrl.h"
#include "BetRecord.h"
#include "SendRecord.h"
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
	CStaticTrans           m_money  ;

	CShadeButtonST         m_rBtnWithd;
	CShadeButtonST         m_rBtnRech;
	CRoundButton           m_rBtnMale;
	CRoundButton           m_rBtnWoman;
	CShadeButtonST         m_rBtnRefresh1;
	CShadeButtonST         m_rBtnRefresh2;

	CStaticTrans           m_sCountpage  ;
	CShadeButtonST         m_rBtnUp;
	CShadeButtonST         m_rBtnNext;

	CUseListBox            m_BonusListBox;
	//CRecordListBox         m_RecordListBox;
	CComboBox			   m_addrbook;
	CP2PBetHelp		       m_P2PBetHelp;
	CShadeButtonST         m_rBtnWinerloser;
public:
	CBetRecord            m_BetRecord     ;
	CSendRecord           m_SendRecord    ;
	int                   m_seltab;

	std::vector<CDialog*> m_pDialog;
	void      OnSelectShowWin(int nCurSelTab);
	void      OnShowPagePool(int page);
public:
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShowListCtrol(  WPARAM wParam, LPARAM lParam ) ;
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboAddres();
	void InsertComboxIitem();
	BOOL AddListaddrDataBox();
	void QueryNotDrawBalance(CString addr);
	afx_msg void OnBnClickedButtonWithd();
	afx_msg void OnBnClickedButtonRech();
	afx_msg void OnBnClickedButtonMale();
public:
	void SendBet(int rewardnum);
	afx_msg void OnBnClickedButtonWoman();
	void OnListPool();
	void AcceptBet(CString hash,CString money,CString sendaddr,int timeout);
	bool CheckBalance();
	CCTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	void   ShowListItem(int seltab);
	afx_msg void OnBnClickedButtonRefresh2();
	afx_msg void OnBnClickedButtonRefresh1();
	afx_msg void OnNMThemeChangedListBonus(NMHDR *pNMHDR, LRESULT *pResult);
	void ComPuteBetWinAandLoser();
private:
	int                   m_pagecount;
	int                   m_curpage;
	int                   m_pagesize;
	uistruct::P2PLIST     m_PoolList;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void       onShowLink();
	CMFCLinkCtrl v_linkCtrl;
};
