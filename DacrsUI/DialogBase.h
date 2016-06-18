#pragma once


// CDialogBase �Ի���

class CDialogBase : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogBase)

public:
	CDialogBase(UINT ID,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogBase();

// �Ի�������
	enum { IDD = IDD_DIALOGBASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	CFont            m_fontGrid;//�������
	HBITMAP		     m_HeadBmp; 
	HBITMAP           m_pBmp;
	void		     SetBkBmpNid( UINT nBitmapIn ,HBITMAP &bmp) ;
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
