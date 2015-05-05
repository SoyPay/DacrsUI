#pragma once
#include "afxcmn.h"


// CListAddr 对话框

class CListAddr : public CDialogEx
{
	DECLARE_DYNAMIC(CListAddr)

public:
	CListAddr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListAddr();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	virtual BOOL OnInitDialog();
	void ShowListInfo();
};
