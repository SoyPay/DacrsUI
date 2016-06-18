#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"

// CSetAppId �Ի���

class CSetAppId : public CDialogBase
{
	DECLARE_DYNAMIC(CSetAppId)

public:
	CSetAppId(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetAppId();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETAPPID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetDaluft();
	virtual BOOL OnInitDialog();
	bool IsRegId(const char* pData);
public:
	CStaticTrans     m_P2Pid;
	CStaticTrans     m_redpackeid;
	CStaticTrans     m_ipoid;
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnCancel;
	CShadeButtonST   m_rBtnOk;
	afx_msg void OnBnClickedClose();
};
