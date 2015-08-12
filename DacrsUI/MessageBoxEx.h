#pragma once
#include "ShadeButtonST.h"
#include "StaticTrans.h"
// CMessageBoxEx 对话框

class CMessageBoxEx : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageBoxEx)

public:
	//CMessageBoxEx(CWnd* pParent = NULL);   // 标准构造函数
	CMessageBoxEx(CString strText,int nType,UINT nImageType = IDB_BITMAP_MESSAGE_H ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMessageBoxEx();

// 对话框数据
	enum { IDD = IDD_DIALOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_Text;
	int m_nType;

	CShadeButtonST m_quit;    //关闭
	CShadeButtonST m_OK;      //确认
	CShadeButtonST m_cancel;  //取消
	CStaticTrans   m_strText ;//显示内容
	CFont  m_fontGrid;//字体变量
	int m_select;

public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	int       GetSelect();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
