
// DacrsUIDlg.h : 头文件
//

#pragma once
#include "IndTitleBar.h"
#include "ProgStatusBar.h"
#include "MainDlg.h"

// CDacrsUIDlg 对话框
class CDacrsUIDlg : public CDialogEx
{
// 构造
public:
	CDacrsUIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DACRSUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIndTitleBar		*m_pTitleBar;//*m_pIndToolbar;
	CProgStatusBar      *m_pStatusBar;//状态栏
	CMainDlg            *m_pMainDlg  ;//主界面
	
public:
	std::map< UINT , CDialog * >  m_dlgMap ; 
	void       ShowDialog(UINT dlgid)  ;
	CDialog			   *p_CurSelDlg   ;   //当前选择对话框指针
public:
	afx_msg void OnBnClickedButtonMainUI();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonRece();
	afx_msg void OnBnClickedButtonTrad();
	afx_msg void OnBnClickedButtonP2P();
	afx_msg void OnBnClickedButtonMortgage();
	afx_msg void OnBnClickedButtonDeals();

	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonMin();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
