#pragma once


// COutGifDlg �Ի���

class COutGifDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COutGifDlg)

public:
	COutGifDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COutGifDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_OUTGIF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic		     *m_ProgressWnd ;	//Progress����
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	void             LoadGifing( BOOL bState );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
};
