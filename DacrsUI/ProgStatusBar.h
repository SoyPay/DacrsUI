#pragma once
#include "RoundButton.h"
#include "StaticTrans.h"
#include "XProgressCtrl.h"

// CProgStatusBar �Ի���

class CProgStatusBar : public CDialogBar
{
	DECLARE_DYNAMIC(CProgStatusBar)

public:
	CProgStatusBar();   // ��׼���캯��
	virtual ~CProgStatusBar();

	// �Ի�������
	enum { IDD = IDD_DIALOG_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic		     *m_ProgressWnd;	//Progress����
	CStaticTrans     m_strNeting;		//����ͬ�� 
	CStaticTrans     m_strHeight;		//�߶� 
	CStaticTrans     m_strVersion;      //�汾
public:
	HBITMAP		     m_pBmp; 
	CBitmap          m_Sigbmp[4];  //�ź�ͼƬ
	CBitmap          m_Lockbmp[2];  //����ͼƬ
	int              m_nSigIndex;
	int              m_nLockIndex;
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	void             LoadGifing( BOOL bState );
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
public:
	afx_msg LRESULT OnShowProgressCtrl(  WPARAM wParam, LPARAM lParam ) ;
	int  ShowProgressCtrl();
	void ShowLockCtrl();
	void OnIniLockParam();
;public:
	BOOL        m_bProgressType;
	int         m_ProgressMax  ;
//	CProgressCtrl m_progress;
	CXProgressCtrl m_progress;
	afx_msg void OnPaint();
	CRect  m_bmpsig;
	bool   m_walletui;
	bool   m_prosshiden;
	CRect  m_bmplock;
	CString netStr;
};
