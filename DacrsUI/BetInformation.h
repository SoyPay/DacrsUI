#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CBetInformation �Ի���

class CBetInformation : public CDialogBase
{
	DECLARE_DYNAMIC(CBetInformation)

public:
	CBetInformation(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBetInformation();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	void ShowBetRecordDetail(CString jsontx);
public:
	afx_msg void OnBnClickedClose();
public:
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnOk;
};
