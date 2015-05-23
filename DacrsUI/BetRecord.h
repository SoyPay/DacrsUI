#pragma once
#include "RecordListBox.h"

// CBetRecord 对话框

class CBetRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CBetRecord)

public:
	CBetRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBetRecord();

// 对话框数据
	enum { IDD = IDD_DIALOG_BETRECORD };

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
	CRecordListBox            m_ListBox;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Showlistbox(CString address);
};
