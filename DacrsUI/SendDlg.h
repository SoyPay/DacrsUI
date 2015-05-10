#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "ShadeButtonST.h"

// CSendDlg �Ի���

class CSendDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CSendDlg)

public:
	CSendDlg();   // ��׼���캯��
	virtual ~CSendDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendtrnsfer();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg LRESULT OnShowListaddrData(  WPARAM wParam, LPARAM lParam ) ;
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
public:
	CRoundButton     m_rBtnAddbook;
private:
	uistruct::LISTADDRLIST m_pListaddrInfo;
	CShadeButtonST         m_rBtnSend;
	CStaticTrans     m_strTx1  ;  
public:
	BOOL AddListaddrDataBox();
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
