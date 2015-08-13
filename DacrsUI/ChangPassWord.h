#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"
// CChangPassWord �Ի���

class CChangPassWord : public CDialogBase
{
	DECLARE_DYNAMIC(CChangPassWord)

public:
	CChangPassWord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChangPassWord();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHANGEPASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedClose();
	virtual BOOL OnInitDialog();
public:
	CStaticTrans     m_oldpassword;
	CStaticTrans     m_newpassword;
	CStaticTrans     m_comfiredpassword;
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
};
