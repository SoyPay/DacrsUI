#pragma once

#include "CommonStruct.h"
// CTransfer 对话框

class CTransfer : public CDialogEx
{
	DECLARE_DYNAMIC(CTransfer)

public:
	CTransfer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTransfer();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRANSFER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendtrnsfer();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg LRESULT OnShowListaddrData(  WPARAM wParam, LPARAM lParam ) ;
private:
	uistruct::LISTADDRLIST m_pListaddrInfo;
public:
	BOOL AddListaddrDataBox();
};
