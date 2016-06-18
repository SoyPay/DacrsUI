#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"
// CEncryptWallet �Ի���

class CEncryptWallet : public CDialogBase
{
	DECLARE_DYNAMIC(CEncryptWallet)

public:
	CEncryptWallet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEncryptWallet();

// �Ի�������
	enum { IDD = IDD_DIALOG_ENCRYPTWALLET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();
	virtual BOOL OnInitDialog();
public:
	CStaticTrans     m_password;
	CStaticTrans     m_confiredpassword;
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;

};
