#pragma once

#include "RecordListBox.h"
// CAcceptRedPacketRecord 对话框

class CAcceptRedPacketRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CAcceptRedPacketRecord)

public:
	CAcceptRedPacketRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAcceptRedPacketRecord();

// 对话框数据
	enum { IDD = IDD_DIALOG_ACCEPTREDPACKERECORD };

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

	CRecordListBox            m_ListBox;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Showlistbox(CString address);
private:
	int                              m_pagecount;
	int                              m_curpage;
	unsigned int                              m_pagesize;
	uistruct::REDPACKETGRABLIST       m_AcceptRedPacketList;
	HBRUSH                          m_hbrush;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void      OnShowPagePool(int page);
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnLbnDblclkListBox();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void ExportAcceptRedPacektToexel();
	void GetCellName(int nRow, int nCol, CString &strName);
	void GetExportCol(map<int,string> &item,uistruct::REDPACKETGRAB_t const_it);
};
