#pragma once
#include "ShadeButtonST.h"
#include "StaticTrans.h"
#include "CommonStruct.h"

// CMainDlg �Ի���

class CMainDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg();   // ��׼���캯��
	virtual ~CMainDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStaticTrans     m_strTrading  ;   
	CStaticTrans     m_strTrading2  ;   
	CStaticTrans     m_strTrading3  ;   
	CStaticTrans     m_strTrading4  ;   
	CStaticTrans     m_strTrading5  ;   

	CShadeButtonST     m_rBtnAllTxdetail;
public:
	afx_msg void OnBnClickedAlltxdetail();
	afx_msg LRESULT OnShowListCtorl(  WPARAM wParam, LPARAM lParam ) ;
public:
	void SetCtrlText();
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
};
