#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CSetAppFee 对话框

class CSetAppFee : public CDialogBase
{
	DECLARE_DYNAMIC(CSetAppFee)

public:
	CSetAppFee(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetAppFee();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETFEE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool IsAllDigtal(const char* pData);
	bool IsRegId(const char* pData);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnBnClickedButtonClose();
	void SetDaluft();
public:
	CStaticTrans     m_strTrading  ;   
	CStaticTrans     m_strTrading2  ;   
	CStaticTrans     m_strTrading3  ;   
	CStaticTrans     m_strTrading4  ;   
	CStaticTrans     m_strTrading5  ;   

	CStaticTrans     m_strTx1  ;  
	CStaticTrans     m_strTx2  ;  
	CStaticTrans     m_strTx3  ;  
	CStaticTrans     m_strTx4  ;  
	CStaticTrans     m_strTx5  ;  

	CStaticTrans     m_strDesTx1  ;  


	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
	afx_msg void OnBnClickedClose();
};
