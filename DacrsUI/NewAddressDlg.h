#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CNewAddressDlg �Ի���

class CNewAddressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewAddressDlg)

public:
	CNewAddressDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewAddressDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GENERATE_NEWADDRES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFont            m_fontGrid;//�������
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
public:
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnNewAdd;
	CStaticTrans     m_label   ;  //
	CStaticTrans     m_code   ;  //
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButtonScdz();
};
