#pragma once
#include "RoundButton.h"
#include "ListCtrlCl.h"
#include "ShadeButtonST.h"

// CTradDlg 对话框

class CTradDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CTradDlg)

public:
	CTradDlg();   // 标准构造函数
	virtual ~CTradDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	//CMFCLinkCtrl v_linkCtrl1;
	//CMFCLinkCtrl v_linkCtrl2;
	afx_msg void OnBnClickedButtonTxdetail();
	afx_msg LRESULT OnShowListCtrl(  WPARAM wParam, LPARAM lParam ) ;
	void OninitializeList();
private:
	CListCtrlCl        m_listCtrl;
	CShadeButtonST     m_rBtnTxdetail;
	CShadeButtonST     m_rBtnExportTx;
	CComboBox		   m_condition;
	CComboBox		   m_time;
	CEdit		       m_edit;
public:
	afx_msg void OnNMDblclkListListtx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InsertItemData();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedExportExel();
	void  GetCellName(int nRow, int nCol, CString &strName);
	afx_msg void OnCbnSelchangeCombo1();
	void OnShowListCtrl(uistruct::TRANSRECORDLIST pListInfo,int flag =0);
	bool  isMine(CString addr);
	afx_msg void OnCbnSelchangeComboTime();
	CString GetConditonTime();
	CString GetConditonTxType(int &operate);
	CString Getaddr();
	void ShowAddrConditon();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString GetConditonStr(int &operate);
	//afx_msg void OnEnChangeEditAddr();
	afx_msg void OnEnErrspaceEditAddr();
};
