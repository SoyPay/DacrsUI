#pragma once
#include "RoundButton.h"
#include "ListCtrlCl.h"
#include "ShadeButtonST.h"
// CReceiveDlg �Ի���

class CReceiveDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CReceiveDlg)

public:
	CReceiveDlg();   // ��׼���캯��
	virtual ~CReceiveDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECEIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void ShowListInfo();
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnBnClickedCopyaddress();
	afx_msg LRESULT OnShowListCtrl(  WPARAM wParam, LPARAM lParam ) ;
private:
	CListCtrlCl m_listCtrl;
	map<CString,uistruct::LISTADDR_t> m_MapAddrInfo;
	CShadeButtonST     m_rBtnAcitve;
	CShadeButtonST     m_rBtnNewaddr;
	CShadeButtonST     m_rBtnCopyaddr;
	int                hitRow;
	int                hitCol;
	CEdit              editItem;
	CString            m_prehittex;
public:
	afx_msg void OnBnClickedButtonSignAccount();
	afx_msg void OnBnClickedButtonNewaddress();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void    ModifyListCtrlItem();
	void    InsertListCtrlItem();
	afx_msg void OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListShow(NMHDR *pNMHDR, LRESULT *pResult);
};
