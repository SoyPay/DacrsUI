#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "AutoComplete.h"
#include "afxcmn.h"
// CRPCDlg 对话框

class CRPCDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRPCDlg)

public:
	CRPCDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRPCDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RPC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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


};
