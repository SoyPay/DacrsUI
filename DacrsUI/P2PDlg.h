#pragma once


// CP2PDlg �Ի���

class CP2PDlg : public CDialogBar
{
	DECLARE_DYNAMIC(CP2PDlg)

public:
	CP2PDlg();   // ��׼���캯��
	virtual ~CP2PDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_P2P };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
