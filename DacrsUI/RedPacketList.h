#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CRedPacketList �Ի���

class CRedPacketList : public CDialogBase
{
	DECLARE_DYNAMIC(CRedPacketList)

public:
	CRedPacketList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRedPacketList();

// �Ի�������
	enum { IDD = IDD_DIALOG_REDPACKETLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowTxDetail(CString txhash);
	CListCtrl   m_listCtrl;
	afx_msg void OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();
public:
	CStaticTrans     m_strTx1  ;  
	CStaticTrans     m_strTx2  ;  

	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnOk;
};
