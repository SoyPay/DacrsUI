#pragma once
#include "XProgressCtrl.h"

// CStartProgress �Ի���

class CStartProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CStartProgress)

public:
	CStartProgress(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartProgress();

// �Ի�������
	enum { IDD = IDD_DIALOG_INIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void             LoadGifing( BOOL bState );
public:
	CStatic		     *m_ProgressWnd ;	//Progress����
public:
	int              m_nBmpIndex;
	CBitmap          m_ProgressBmp[4];  //�ź�ͼƬ
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
public:
	afx_msg LRESULT OnShowProgressCtrl(  WPARAM wParam, LPARAM lParam ) ;
public:
	void    SwitchBmp(int nIndex);   
	void    SetProgessRange(int pos);
//	CProgressCtrl m_progress;
	CXProgressCtrl m_progress;
	bool mprocessindex;
};
