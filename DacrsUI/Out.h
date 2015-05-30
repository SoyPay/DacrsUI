#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// COut �Ի���

class COut : public CDialogEx
{
	DECLARE_DYNAMIC(COut)

public:
	COut(CWnd* pParent = NULL, CString strDisplay=_T(""), int nFontSize=120,CString strok=_T(""),CString strNo = _T(""));   // ��׼���캯��
	virtual ~COut();

// �Ի�������
	enum { IDD = IDD_DIALOG_OUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFont            m_fontGrid;//�������
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn );
	CString          m_strDisplay;
	CString          m_strok;
	CString          m_strno;
	int              m_nFontSize;//�����С
	CStaticTrans     m_Text;
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
