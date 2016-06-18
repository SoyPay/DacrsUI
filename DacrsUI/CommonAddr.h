#pragma once
#include "DialogBase.h"
#include "ShadeButtonST.h"
#include "StaticTrans.h"
#include "afxcmn.h"
#include "afxwin.h"

// CCommonAddr �Ի���

class CCommonAddr : public CDialogBase
{
	DECLARE_DYNAMIC(CCommonAddr)

public:
	CCommonAddr(UiType uType , CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCommonAddr();

// �Ի�������
	enum { IDD = IDD_DIALOG_COMMONADDR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CShadeButtonST m_ButClose;    //�ر�
	CStaticTrans   m_head  ;
public:
	map<string,uistruct::LISTADDR_t> m_mapAddrInfo ; //ȫ����ַ
	map<int,uistruct::COMMONLISTADDR_t> m_mapCommonAddrInfo; //���õ�ַ
	BOOL AddListaddrDataBox();
	void UpdataCommonAddrList(map<int,uistruct::COMMONLISTADDR_t> *pListInfo);
public:
	UiType     m_uAddrType   ;    //���õ�ַ����
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonClose();
	CListBox m_AllAddrlistBox;
	CListBox m_CommonAddrListBox;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnLbnSelchangeListAlladdr();
	afx_msg void OnLbnSelchangeListCommonaddr();
	afx_msg void OnBnClickedButtonAddrok();
};
