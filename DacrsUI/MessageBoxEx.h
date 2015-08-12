#pragma once
#include "ShadeButtonST.h"
#include "StaticTrans.h"
// CMessageBoxEx �Ի���

class CMessageBoxEx : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageBoxEx)

public:
	//CMessageBoxEx(CWnd* pParent = NULL);   // ��׼���캯��
	CMessageBoxEx(CString strText,int nType,UINT nImageType = IDB_BITMAP_MESSAGE_H ,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMessageBoxEx();

// �Ի�������
	enum { IDD = IDD_DIALOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_Text;
	int m_nType;

	CShadeButtonST m_quit;    //�ر�
	CShadeButtonST m_OK;      //ȷ��
	CShadeButtonST m_cancel;  //ȡ��
	CStaticTrans   m_strText ;//��ʾ����
	CFont  m_fontGrid;//�������
	int m_select;

public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	int       GetSelect();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
