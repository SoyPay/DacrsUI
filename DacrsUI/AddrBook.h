#pragma once
#include "ListCtrlCl.h"
#include "RoundButton.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"
// CAddrBook 对话框

class CAddrBook : public CDialogEx
{
	DECLARE_DYNAMIC(CAddrBook)

public:
	CAddrBook(CWnd* pParent = NULL , CString strTip = _T(""));   // 标准构造函数
	virtual ~CAddrBook();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDRBOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddaddrbook();
public:
	CListCtrlCl m_listCtrl;
	virtual BOOL OnInitDialog();
private:
	bool LoadAddrBook();
private:
	CString    m_strTip      ;
	map<CString,uistruct::ADDRBOOK_t> m_mapAddrInfo;
	uistruct::ADDRBOOK_t m_selectAddr;
public:
	void GetAddrbook(uistruct::ADDRBOOK_t &addr);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDeleitem();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CStaticTrans     m_Text;

	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnAdd;
	CShadeButtonST   m_rBtnDelete;
	CShadeButtonST   m_rBtnOk;
public:
	CFont            m_fontGrid;//字体变量
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonGb();
};
