#pragma once
#include "afxwin.h"


// CChoseLanguage �Ի���

class CChoseLanguage : public CDialogBase
{
	DECLARE_DYNAMIC(CChoseLanguage)

public:
	CChoseLanguage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChoseLanguage();

// �Ի�������
	enum { IDD = IDD_DIALOG_CHOSELANGUAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_language;
	afx_msg void OnCbnSelchangeCombolanguage();
	virtual BOOL OnInitDialog();
	CComboBox m_laguabox;
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
	afx_msg void OnBnClickedClose();
};
