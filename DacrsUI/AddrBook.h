#pragma once
#include "ListCtrlCl.h"
#include "RoundButton.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"
// CAddrBook 对话框

class CAddrBook : public CDialogBase
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
	map<string,uistruct::ADDRBOOK_t> m_mapAddrInfo;
	uistruct::ADDRBOOK_t m_selectAddr;
public:
	void GetAddrbook(uistruct::ADDRBOOK_t &addr);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDeleitem();
	//afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CStaticTrans     m_Text;

	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnAdd;
	CShadeButtonST   m_rBtnDelete;
	CShadeButtonST   m_rBtnOk;

	int                hitRow;
	int                hitCol;
	CEdit              editItem;
	CString            m_prehittex;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonGb();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult);
};
