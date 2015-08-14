#pragma once
#include "DialogBase.h"
#include "ShadeButtonST.h"
#include "StaticTrans.h"
// CMessageBoxEx �Ի���


#define MAX_WIDTH 480 //�����480������
class CMessageBoxEx : public CDialogBase
{
	DECLARE_DYNAMIC(CMessageBoxEx)

public:
	//CMessageBoxEx(CWnd* pParent = NULL);   // ��׼���캯��
	CMessageBoxEx(CString strText,CString strTip ,UINT uType ,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMessageBoxEx();

// �Ի�������
	enum { IDD = IDD_DIALOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CString m_Text;
	CString m_Tip;
	int  m_nType;
	UINT m_uType ;

	CShadeButtonST m_quit;    //�ر�
	CShadeButtonST m_OK;      //ȷ��
	CShadeButtonST m_cancel;  //ȡ��
	CStaticTrans   m_strTip   ;  //��ʾ��Ϣ
	CStaticTrans   m_strText ;//��ʾ����
	CFont  m_fontGrid;//�������
	int m_select;

public:
	HBITMAP		     m_pBmp; 
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	void             SetBitmap(UINT uBmpResource)  ;
	int              GetSelect();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
