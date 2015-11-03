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
	afx_msg void OnBnClickedButtonTxdetail();
	afx_msg LRESULT OnShowListCtrl(  WPARAM wParam, LPARAM lParam ) ;
	void    OninitializeList();
	void    ShowListCtrl(uistruct::TRANSRECORDLIST* pListInfo);
	int     m_nConut   ;     //翻页计数
	void    ShowPageDataInfo(int nConut);
private:
	CListCtrlCl        m_listCtrl;
	CShadeButtonST     m_rBtnTxdetail;
	CShadeButtonST     m_rBtnExportTx;
	CComboBox		   m_condition;
	CComboBox		   m_time;
	CComboBox		   m_pageitem;
	CEdit		       m_edit;
	COLORREF          m_colorEditText;
	bool              m_seteditcolor;
	uistruct::TRANSRECORDLIST m_pListInfo ;
public:
	CStaticTrans           m_sCountpage  ;
	CShadeButtonST         m_rBtnUp;
	CShadeButtonST         m_rBtnNext;
	int                    m_pagesize;
	int                    m_offset;
public:
	afx_msg void OnNMDblclkListListtx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InsertItemData();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedExportExel();
	afx_msg void OnCbnSelchangeCombo1();
	void OnShowListCtrl(uistruct::TRANSRECORDLIST pListInfo);
	bool  isMine(CString addr);
	afx_msg void OnCbnSelchangeComboTime();
	string GetConditonTime();
	string GetConditonTime(INT64 &maxtime,INT64 &mintime);
	string GetConditonTxType(int &operate);
	string Getaddr();
	void ShowAddrConditon();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	string GetConditonStr(int &operate);
	BOOL IsInsertTx(uistruct::REVTRANSACTION_t txdetail);
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedUpPage();
	afx_msg void OnBnClickedNextPage();
	void    ShowPageCotent(int pageIndex);
	void    ShowComboxCotent();
	afx_msg void OnCbnSelchangeComboPage();
	void GetExportCol(int index,map<int,string> &item,uistruct::REVTRANSACTION_t const_it);
};
