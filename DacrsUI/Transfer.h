#pragma once

#include "CommonStruct.h"
// CTransfer �Ի���

class CTransfer : public CDialogEx
{
	DECLARE_DYNAMIC(CTransfer)

public:
	CTransfer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTransfer();

// �Ի�������
	enum { IDD = IDD_DIALOG_TRANSFER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
