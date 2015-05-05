
// DacrsUIDlg.h : ͷ�ļ�
//

#pragma once
#include "IndTitleBar.h"
#include "ProgStatusBar.h"
#include "MainDlg.h"

// CDacrsUIDlg �Ի���
class CDacrsUIDlg : public CDialogEx
{
// ����
public:
	CDacrsUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DACRSUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIndTitleBar		*m_pTitleBar;//*m_pIndToolbar;
	CProgStatusBar      *m_pStatusBar;//״̬��
	CMainDlg            *m_pMainDlg  ;//������
	
public:
	std::map< UINT , CDialog * >  m_dlgMap ; 
	void       ShowDialog(UINT dlgid)  ;
	CDialog			   *p_CurSelDlg   ;   //��ǰѡ��Ի���ָ��
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
