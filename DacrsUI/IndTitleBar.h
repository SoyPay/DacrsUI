#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"

// CIndTitleBar �Ի���

class CIndTitleBar : public CDialogBar
{
	DECLARE_DYNAMIC(CIndTitleBar)

public:
	CIndTitleBar();   // ��׼���캯��
	virtual ~CIndTitleBar();

	// �Ի�������
	enum { IDD = IDD_DIALOGBAR_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CRoundButton     m_rBtnMainUi;
	CRoundButton     m_rBtnSend;
	CRoundButton     m_rBtnRece;
	CRoundButton     m_rBtnTrad;
	CRoundButton     m_rBtnP2p;
	CRoundButton     m_rBtnMortgage ;
	CRoundButton     m_rBtnIpo;
public:
	CToolTipCtrl     m_tooltip;			// ��ʾ��Ϣ
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	UINT             nButtonID                     ;
	void             MobileTip(UINT nButtonID);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
