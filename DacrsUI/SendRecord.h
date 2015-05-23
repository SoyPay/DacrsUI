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
public:
	CSendListBox    m_listBox ;
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
};
