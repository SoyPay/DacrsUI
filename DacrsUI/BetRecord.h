#pragma once
#include "RecordListBox.h"

// CBetRecord �Ի���

class CBetRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CBetRecord)

public:
	CBetRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBetRecord();

// �Ի�������
	enum { IDD = IDD_DIALOG_BETRECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
