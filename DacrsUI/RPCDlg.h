#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "AutoComplete.h"
#include "afxcmn.h"
#include "DialogBase.h"
#include "ShadeButtonST.h"
#include "StaticTrans.h"
// CRPCDlg �Ի���

class CRPCDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CRPCDlg)

public:
	CRPCDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRPCDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RPC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg LRESULT OnShowRecvData(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnShowSendData(WPARAM wparam,LPARAM lparam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnClear();
	//void Jiexi(Json::Value &obj, HTREEITEM hItemRoot,CTreeCtrl&m_rpccommand);
private:
	void UpdateEditContent(const CStringA& strNewData,const CString& strKeyWord);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CAutoComplete AutoCombo;
private:
	Json::Value mRoot; 
public:
	void ShowJson(string rpcname,string jsomstr);

	CTreeCtrl m_rpccommand;
	CComboBox m_comboxinput;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnClear;
	CStaticTrans     m_headText;
	afx_msg void OnBnClickedClose1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
