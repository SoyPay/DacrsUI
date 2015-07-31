#pragma once

#include "RoundButton.h"
#include "RedPacketPoolListBox.h"
#include "CommonStruct.h"
#include "AcceptRedPacketRecord.h"
#include "SendRedPacketRecord.h"
#include "TabCtrl.h"
#include "GrabCommRedPacket.h"
#include "GrabSpecalRedPacket.h"
// CMortgageTardDlg 对话框

class CMortgageTardDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CMortgageTardDlg)

public:
	CMortgageTardDlg();   // 标准构造函数
	virtual ~CMortgageTardDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MORTGAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CStaticTrans           m_Balance  ; 
	CStaticTrans           m_NotDraw  ;
	CStaticTrans           m_Dw  ;
//	CStaticTrans           m_VS  ;
	CStaticTrans           m_money  ;

	CShadeButtonST         m_rBtnWithd;
	CShadeButtonST         m_rBtnRech;
	CShadeButtonST         m_rBtnMale;
	CShadeButtonST         m_rBtnWoman;
	//CRoundButton           m_rBtnMale;
	//CRoundButton           m_rBtnWoman;
	CShadeButtonST         m_rBtnRefresh1;
	CShadeButtonST         m_rBtnRefresh2;

	//CStaticTrans           m_sCountpage  ;
	//CShadeButtonST         m_rBtnUp;
	//CShadeButtonST         m_rBtnNext;

//	CRedPacketPoolListBox  m_BonusListBox;
	//CRecordListBox         m_RecordListBox;
	CComboBox			   m_addrbook;
	CRedPacketHelp		   m_RedPacketHelp;
	CP2PBetHelp		                    m_P2PBetHelp;
	CShadeButtonST         m_rBtnWinerloser;
	CShadeButtonST         m_rBtnAddrWinerloser;
public:
	BOOL AddListaddrDataBox();
	bool CheckBalance();
	bool CheckBalance(double dmoney);
	afx_msg void OnBnClickedButtonWithd();
	afx_msg void OnBnClickedButtonRech();
	afx_msg void OnBnClickedButtonCommred();
	afx_msg void OnCbnSelchangeComboAddres();
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnShowListCtrol(  WPARAM wParam, LPARAM lParam ) ;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	double    ComputeSpecailRedPacket(uistruct::REDPACKETGRABLIST  RedPackeGrabRecordList);
	void   ShowAllSpecailWinAndLoss();
	void   ShowAddressSpecailWinAndLoss(CString addr);
public:
	CAcceptRedPacketRecord            m_BetRecord     ;
	CSendRedPacketRecord              m_SendRecord    ;
	CGrabCommRedPacket                m_GrabCommRedPacket;
	CGrabSpecalRedPacket              m_GrabSpecailRedPacket;
	int                               m_seltab;
	int                               m_selPooltab;
	CCTabCtrl                         m_tab;
	CCTabCtrl                         m_tabpool;
	std::vector<CDialog*>            m_pDialog;
	std::vector<CDialog*>            m_poopDialog;
	void      OnSelectShowWin(int nCurSelTab);
	void      OnSelectShowPoolWin(int nCurSelTab);
	void      OnShowPagePool(int page);
	void      InsertComboxIitem();
	void      ShowListItem(int seltab);
	void      ShowListPoolItem(int seltab);
	void      OnListPool();
	void      SendRedPacketComm();
	void      SendRedPackeSpecail();
	void      AcceptRedPacketComm(CString sendhash,uistruct::REDPACKETPOOL_t pPoolList);
	void      AcceptRedPackeSpecail(CString sendhash,uistruct::REDPACKETPOOL_t pPoolList);
	bool      IsAcceptRedPacket(CString account,uistruct::REDPACKETPOOL_t pPoolList);
private:
	int                             m_pagecount;
	int                             m_curpage;
	int                             m_pagesize;
	uistruct::REDPACKETPOOLLIST     m_PoolList;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnBnClickedButtonUp();
	//afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonRefresh1();
	afx_msg void OnBnClickedButtonRefresh2();
	afx_msg void OnBnClickedButtonSpecailred();
	void SetGrabParam();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void       onShowLink();
	CMFCLinkCtrl v_linkCtrl;
	void GetAppAccountSomeMoney();

};
