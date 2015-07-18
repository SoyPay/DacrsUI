#pragma once
#include "RedPacketPoolListBox.h"

// CSendRedPacketRecord 对话框

class CSendRedPacketRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CSendRedPacketRecord)

public:
	CSendRedPacketRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSendRedPacketRecord();

// 对话框数据
	enum { IDD = IDD_DIALOG_SENDREDPACKERECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	CStaticTrans           m_sCountpage  ;
	CShadeButtonST         m_rBtnUp;
	CShadeButtonST         m_rBtnNext;
	CRedPacketPoolListBox         m_listBox ;
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	void Showlistbox(CString address);
private:
	CString          m_addr;
	CP2PBetHelp		                    m_P2PBetHelp;
private:
	int                              m_pagecount;
	int                              m_curpage;
	unsigned int                              m_pagesize;
	uistruct::REDPACKETSENDLIST       m_SendRedPacketList;
	HBRUSH                          m_hbrush;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void      OnShowPagePool(int page);
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnLbnDblclkListBox();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
