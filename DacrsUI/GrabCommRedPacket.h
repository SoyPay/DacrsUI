#pragma once
#include "UseListBox.h"

// CGrabCommRedPacket 对话框

class CGrabCommRedPacket : public CDialogEx
{
	DECLARE_DYNAMIC(CGrabCommRedPacket)

public:
	CGrabCommRedPacket(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGrabCommRedPacket();

// 对话框数据
	enum { IDD = IDD_DIALOG_GRB_COMM_REDPACKE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
public:
	CShadeButtonST         m_rBtnUp;
	CShadeButtonST         m_rBtnNext;
	CStaticTrans           m_sCountpage  ;

	CUseListBox            m_BonusListBox;
	CString                   m_addr;
	CString                   m_walletmoney;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Showlistbox();
private:
	int                              m_pagecount;
	int                              m_curpage;
	unsigned int                              m_pagesize;
	uistruct::REDPACKETPOOLLIST     m_PoolList;
	CRedPacketHelp		   m_RedPacketHelp;
	HBRUSH                          m_hbrush;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void      OnShowPagePool(int page);
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonNext();
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
	void      AcceptRedPacketComm(CString sendhash,uistruct::REDPACKETPOOL_t pPoolList);
	bool      IsAcceptRedPacket(CString account,uistruct::REDPACKETPOOL_t pPoolList);
	void      SetTxt(CString addr,CString strwallet);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
