#pragma once


// CIpoDlg �Ի���

class CIpoDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CIpoDlg)

public:
	CIpoDlg();   // ��׼���캯��
	virtual ~CIpoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_IPO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	CMFCLinkCtrl v_linkCtrl1;
	CMFCLinkCtrl v_linkCtrl2;
};
