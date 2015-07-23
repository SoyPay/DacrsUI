#pragma once
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CReminderdlg 对话框

class CReminderdlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReminderdlg)

public:
	CReminderdlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReminderdlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_REMINDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
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
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
