#pragma once


// CMortgageTardDlg �Ի���

class CMortgageTardDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CMortgageTardDlg)

public:
	CMortgageTardDlg();   // ��׼���캯��
	virtual ~CMortgageTardDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MORTGAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	CMFCLinkCtrl v_linkCtrl1;
	CMFCLinkCtrl v_linkCtrl2;
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
};
