#pragma once

#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"
// CGuessNum �Ի���

class CGuessNum : public CDialogEx
{
	DECLARE_DYNAMIC(CGuessNum)

public:
	CGuessNum(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGuessNum();

// �Ի�������
	enum { IDD = IDD_DIALOG_GUESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	void SetBkBmpNid( UINT nBitmapIn ) ;
private:
	CStaticTrans     m_Text;
	CShadeButtonST   m_rBtnClose;
	HBITMAP		     m_pBmp; 
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonClose();
};
