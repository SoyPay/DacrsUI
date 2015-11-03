#pragma once
#include "RoundButton.h"
#include "BetPoolListBox.h"
#include "CommonStruct.h"
#include "TabCtrl.h"
#include "BetRecord.h"
#include "SendRecord.h"

#include "BalloonTip.h"
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
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShowListCtrol(  WPARAM wParam, LPARAM lParam ) ;
	afx_msg LRESULT OnUpAddressCombo(  WPARAM wParam, LPARAM lParam ) ;
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeComboAddres();
	afx_msg void OnBnClickedButtonWithd();
	afx_msg void OnBnClickedButtonRech();
	afx_msg void OnBnClickedButtonMale();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonWoman();
	afx_msg void OnBnClickedButtonRefresh2();
	afx_msg void OnBnClickedButtonRefresh1();
	afx_msg void OnNMThemeChangedListBonus(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLbnDblclkListBonus();

	afx_msg void OnBnClickedCancelorde();
	afx_msg void OnBnClickedButtonSetaddr();
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
	CShadeButtonST         m_rbCancelOrder;

	CBetPoolListBox            m_BonusListBox;
	//CRecordListBox         m_RecordListBox;
	CComboBox			   m_addrbook;
	CP2PBetHelp		       m_P2PBetHelp;
	CShadeButtonST         m_rBtnWinerloser;
	CShadeButtonST         m_rBtnAddrWinerloser;

	CShadeButtonST         m_rBtnSetCommonAddr;

	CBetRecord            m_BetRecord     ;
	CSendRecord           m_SendRecord    ;
	int                   m_seltab;

	std::vector<CDialog*> m_pDialog;
public:	
	void      OnSelectShowWin(int nCurSelTab);
	void      OnShowPagePool(int page);
	BOOL AddListaddrDataBox();
	void QueryNotDrawBalance(CString addr);
	void SendBet(int rewardnum);
	void OnListPool();
	void AcceptBet(CString hash,INT64 money,CString sendaddr,int timeout,INT64 sendmoney);
	bool CheckBalance(CString strshow = _T(""));
	bool CheckBalance(double dmoney);
	void   ShowListItem(int seltab);
	double ComPuteBetWinAandLoser(uistruct::P2PBETRECORDLIST  m_P2pBetTxList);
	double ComPuteAddrBetWinAandLoser(uistruct::P2PBETRECORDLIST  m_P2pBetTxList,CString addr);
	void   ShowAllBetWinAndLoss();
	void   ShowAddressBetWinAndLoss(CString addr);
	void  GetAppAccountSomeMoney();
	void AutoSendBet();
	BOOL AcceptBet(string hash,double dmoney,string sendaddr,int timeout,string addr);
	void AKeyCancelTheOrder();
	void ReadP2pPoolFromDB();
	void ReadP2pPoolFromCmd(uistruct::P2PLIST &m_PoolList);
	void       onShowLink();
private:
	int                   m_pagecount;
	int                   m_curpage;
	unsigned int          m_pagesize;
	uistruct::P2PLIST     m_PoolList;
	CCTabCtrl             m_tab;
	CMFCLinkCtrl          *v_linkCtrl;

};
