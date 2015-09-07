#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "NewMenu.h"
// CIndTitleBar 对话框

class CIndTitleBar : public CDialogBar
{
	DECLARE_DYNAMIC(CIndTitleBar)

public:
	CIndTitleBar();   // 标准构造函数
	virtual ~CIndTitleBar();

	// 对话框数据
	enum { IDD = IDD_DIALOGBAR_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CRoundButton     m_rBtnMainUi;
	CRoundButton     m_rBtnSend;
	CRoundButton     m_rBtnRece;
	CRoundButton     m_rBtnTrad;
	CRoundButton     m_rBtnP2p;
	CRoundButton     m_rBtnMortgage ;
	CRoundButton     m_rBtnIpo;
	CRoundButton     m_rBtnAddApp;

	CRoundButton     m_rBtnClose;
	CRoundButton     m_rBtnMin;
	CRoundButton     m_rBtnMainMenu;
	CNewMenu         newMenu;
	CRoundButton     m_rCnButton;
	CMenu         LanguageMenu;
public:
	CToolTipCtrl     m_tooltip;			// 提示信息
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	UINT             m_nButtonID                   ; //当前点击的button
	void             MobileTip(UINT nButtonID , BOOL bFlag );
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
	afx_msg void OnBnClickedMenubar();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedCn();
	afx_msg void OnChinese();
	afx_msg void OnEnglish();
	afx_msg void OnUpdateEn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCn(CCmdUI *pCmdUI);
	void Setlanguage(int index);
};
