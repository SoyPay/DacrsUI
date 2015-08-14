#pragma once
#include "DialogBase.h"
#include "ShadeButtonST.h"
#include "StaticTrans.h"
// CMessageBoxEx 对话框


#define MAX_WIDTH 480 //最大宽度480个像素
class CMessageBoxEx : public CDialogBase
{
	DECLARE_DYNAMIC(CMessageBoxEx)

public:
	//CMessageBoxEx(CWnd* pParent = NULL);   // 标准构造函数
	CMessageBoxEx(CString strText,CString strTip ,UINT uType ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMessageBoxEx();

// 对话框数据
	enum { IDD = IDD_DIALOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_Text;
	CString m_Tip;
	int  m_nType;
	UINT m_uType ;

	CShadeButtonST m_quit;    //关闭
	CShadeButtonST m_OK;      //确认
	CShadeButtonST m_cancel;  //取消
	CStaticTrans   m_strTip   ;  //提示信息
	CStaticTrans   m_strText ;//显示内容
	CFont  m_fontGrid;//字体变量
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
