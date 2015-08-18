#pragma once
#include "SendListBox.h"

// CSendRecord 对话框

class CSendRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CSendRecord)

public:
	CSendRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSendRecord();

// 对话框数据
	enum { IDD = IDD_DIALOG_SENDRECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
public:
	CStaticTrans           m_sCountpage  ;
	CShadeButtonST         m_rBtnUp;
	CShadeButtonST         m_rBtnNext;
	CSendListBox    m_listBox ;
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	void Showlistbox(CString address);
	void OpenBet(CString txhash);
private:
	CString          m_addr;
	CP2PBetHelp		                    m_P2PBetHelp;
private:
	int                              m_pagecount;
	int                              m_curpage;
	unsigned int                              m_pagesize;
	uistruct::P2PBETRECORDLIST       m_PoolList;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void      OnShowPagePool(int page);
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnLbnDblclkListBox();
	void    OpenAcceptbet();
    void ExportSendBetRecordToexel();
	void GetCellName(int nRow, int nCol, CString &strName);
	void GetExportCol(map<int,string> &item,uistruct::P2P_QUIZ_RECORD_t const_it);
};
