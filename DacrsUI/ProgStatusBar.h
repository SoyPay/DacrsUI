#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"

// CProgStatusBar �Ի���

class CProgStatusBar : public CDialogBar
{
	DECLARE_DYNAMIC(CProgStatusBar)

public:
	CProgStatusBar();   // ��׼���캯��
	virtual ~CProgStatusBar();

	// �Ի�������
	enum { IDD = IDD_DIALOG_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStaticTrans     m_strShowInfo  ;    
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	afx_msg LRESULT OnShowProgressCtrl(  WPARAM wParam, LPARAM lParam ) ;
public:
	BOOL        m_bProgressType;
	int         m_ProgressMax  ;
	CProgressCtrl m_progress;
};
