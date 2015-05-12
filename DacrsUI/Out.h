#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// COut 对话框

class COut : public CDialogEx
{
	DECLARE_DYNAMIC(COut)

public:
	COut(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COut();

// 对话框数据
	enum { IDD = IDD_DIALOG_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont            m_fontGrid;//字体变量
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	CStaticTrans     m_Text  ;
	CShadeButtonST   m_rBtnOK;
	CShadeButtonST   m_rBtnNO;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonNo();
	afx_msg void OnBnClickedButtonOk();
};
