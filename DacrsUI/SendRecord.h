#pragma once
#include "SendListBox.h"

// CSendRecord �Ի���

class CSendRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CSendRecord)

public:
	CSendRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSendRecord();

// �Ի�������
	enum { IDD = IDD_DIALOG_SENDRECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
