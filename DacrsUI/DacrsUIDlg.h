
// DacrsUIDlg.h : ͷ�ļ�
//

#pragma once
#include "IndTitleBar.h"
#include "ProgStatusBar.h"
#include "MainDlg.h"
#include "SendDlg.h"
#include "ReceiveDlg.h"
#include "TradDlg.h"
#include "P2PDlg.h"
#include "MortgageTardDlg.h"
#include "IpoDlg.h"
#include "AddApp.h"
#include "OutGifDlg.h"
#include "RPCDlg.h"
#include "IpoCoin.h"
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
	CSendDlg            *m_pSendDlg  ;//����dlg
	CReceiveDlg         *m_pReceiveDlg;//����
	CTradDlg            *m_pTradDlg  ; //����
	CP2PDlg             *m_pP2PDlg    ;//P2P
	CMortgageTardDlg    *m_pMortgageTardDlg;// ��Ѻ
	CIpoCoin             *m_pIpoCoinDlg;    //IPO
	CAddApp             *m_pAddApp;    //���Ӧ��
	//
	COutGifDlg          *m_pOutGifDlg;
	CRPCDlg*				m_pRPCDlg;
public:
	std::map< UINT , CDialogBar * >  m_dlgMap ; 
	void       ShowDialog(UINT dlgid)  ;
	CDialogBar *p_CurSelDlg   ;   //��ǰѡ��Ի���ָ��
	int        dlgType;
	void       ShowStateTip(UINT nButtonID);
	void       DestroyDlg();
public:
	afx_msg void OnBnClickedButtonMainUI();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonRece();
	afx_msg void OnBnClickedButtonTrad();
	afx_msg void OnBnClickedButtonP2P();
	afx_msg void OnBnClickedButtonMortgage();
	afx_msg void OnBnClickedButtonDeals();
	afx_msg void OnBnClickedButtonAddApp();

	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonMin();
	bool         IsP2pBetFinsh();
	void         ClosWallet();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnBeginMoveWnd(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	void       InitialRpcCmd();
	void       LoadListDataInfo();         //�����ݿ��м����б�����
	void       SyncAddrInfo();
	void       CloseThread();
	void       StopSever();
public:
	void       Close(); 
	void       CloseApp();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnOk();

	afx_msg void BakWallet();
	afx_msg void SetAppFee();
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam) ; 
	void ToTray();
	void DeleteTray();
	afx_msg void SetAppID();
	afx_msg void RestoreDefault();
	afx_msg void encryptwallet();
	afx_msg void walletpassphrasechange();
	afx_msg void LockWallet();
	afx_msg void ExportPriveKey();
	afx_msg void ImportPrvieKey();
	afx_msg void OnUpdataState(CCmdUI *pCmdUI);
	void ClosWalletWind();
	bool GetFileName(CString &fileName,CString strexe );
	void AddImportWalletAndBookAddr(CString fileName);
	void WriteExportWalletAndBookAddr(CString fileName);
    /// �Զ���رճ�����Ϣ
	afx_msg void OnCloseApp() ; 
};
