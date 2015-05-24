
// DacrsUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <vector>
#include <map>
#include <list>
#include "SqliteDeal.h"
#include "JsonConfigHelp.h"
#include "MyQueue.h"
#include "GIFControl.h"
#include "NoUiMsgBuffer.h"
#include "SplashThread.h"
#include "StartProgress.h"
#include "Log.h"
using namespace std;
// CDacrsUIApp:
// �йش����ʵ�֣������ DacrsUI.cpp
//
#define		MSG_TITLE_LBUTTON_DOWN		(WM_USER + 0x0001)

class CDacrsUIApp : public CWinApp
{
public:
	CDacrsUIApp();

// ��д
public:
	virtual BOOL InitInstance();
public:
	UINT          gsLanguage            ;   //��������
	CString		  str_InsPath	        ;   
	CString		  str_ModuleFilename	;    
	CString		  m_ProgressGifFile	;		//����GIF�ļ�
	CString		  m_ProgressOutGifFile	;	//����GIF�ļ�
	void          GetMoFilename( CString & path , CString & filename ) ;// ��ȡ�ļ�����·��
	int           language()    ;           //��ȡ��������
	CString       m_strAddress  ;
	BOOL          m_bReIndexServer;         //�����򿪷���������
	BOOL          m_bOutApp     ;     //�˳����
public:
	CSqliteDeal   m_SqliteDeal  ;     //Sqlite3���ݿ����

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	SOCKET          m_blockSock   ;  //block Socket
	HANDLE          m_hblockThread;
	UINT            ublockThreadId;
	HANDLE          m_msgThread;
	UINT            umsgThreadId;
	HANDLE          m_hProcessNoUiMsgThread;
	UINT            m_uProNoUiMsgThreadId;
	bool            m_blockAutoDelete;
	bool            m_msgAutoDelete;
	void            StartblockThrd();  
	static UINT __stdcall blockProc(LPVOID pParam);
	static UINT __stdcall ProcessMsg(LPVOID pParam);
	static UINT __stdcall ProcessNoUiMsg(LPVOID pParam);
public:
	CMyMutex		cs_MsgDisptch ;    //��Ϣ�����ٽ�
	CMyMutex        cs_SqlData ;       //�����б��ٽ�
public:
	HANDLE	        GetMtHthrd() const { return hMtThrd	       ; }  
	unsigned int	GetMtHthrdId() const { return nMtThrdID	   ; }  
public:
	HANDLE		 hMtThrd				;	
	HANDLE		 hMtStartEvent			;	
	unsigned int nMtThrdID				;	
	BOOL		 CreateMaintainThrd()	;	
	static UINT __stdcall MtProc(LPVOID pParam) ;
public:
	typedef struct tagMSG_NOTIFY {
		DWORD	nThreadId	;
		HWND	hSubWnd		;
		UINT	msg			;
		void	*relMsgMem	;	//�ؽ��ڴ��ַ
	} MSG_NOTIFY ;

	std::vector<MSG_NOTIFY> sSubscribeList ;
	//�߳���
	typedef struct tagsThrd{
		HANDLE hThrd		 ;
		unsigned int hThrdId ;
	} sThrd ;
	std::vector< sThrd > v_ProcSubThrd ;
public:
	void         RegThrdToMtThrd(  HANDLE hThread , unsigned int hThrdId ) ;	
	void         UnRegThrdToMtThrd(HANDLE hThread , unsigned int hThrdId ) ;
	void		 SubscribeMsg( DWORD nThreadId , HWND hWnd , UINT msg , void * relMsgMem = NULL )	;	
	void		 UnSubscribeMsg( DWORD , HWND , UINT )	;	
	void		 DispatchMsg( unsigned int threadID , UINT msg , WPARAM wParam , LPARAM lParam ) ;
public:
	CString      m_betScritptid;
	CString      m_ipoScritptid;
	CString      m_darkScritptid;
	CDarkTxCfg		m_DarkCfg;
	CP2PBetCfg		m_P2PBetCfg;
public:
	void         OnInitList();   //��ʼ��list
	int          SendPostThread(DWORD msgtype);
	void         ParseUIConfigFile(const CStringA& strExeDir);
public:
	void StartSeverProcess(const CStringA& strdir);
	void CloseProcess(const string& exename);
	void CheckUpdate();
	bool Update();
	bool GetVersionFromServer(string& strVersion);
	BOOL					m_bStartServer;
	PROCESS_INFORMATION		sever_pi; 
	CStringA				m_rpcport;
	string					m_strServerCfgFileName;
	CStringA				m_sendPreHeadstr;
	CStringA				m_sendendHeadstr;
	CStringA                m_uirpcport;
	CString                 m_severip;
	CStringA                m_rpcUser;
	CStringA                m_rpcPassWord;
	void UpdataUIData();
public:
	CMyQueue m_MsgQueue;
	CMyQueue m_UimsgQueue;
	CMyQueue m_UiManDlgQueue;
	CMyQueue m_UiReciveDlgQueue;
	CMyQueue m_UiSendDlgQueue;
	CMyQueue m_UiTxDetailQueue;
	CMyQueue m_UiP2pDlgQueue;
	void UpdateQuizPoolData();
	void UpdateAddressData();
	void InsertTransaction(string hash);
	void UpdateTransaction(string hash);
	void SyncTransaction(string obj);
	void UpdateAppRecord(string txdetail);
	void InsertAddbook(uistruct::ADDRBOOK_t );
	void UpdateAddbook(uistruct::ADDRBOOK_t );
	BOOL UpdateP2pBetRecord();
	void OpenBetRecord(vector<unsigned char> openbet,uistruct::REVTRANSACTION_t transcion);
	void AcceptBetRecord(vector<unsigned char> acceptbet,uistruct::REVTRANSACTION_t transcion);
	void ClearTransaction();
	void GetMainDlgStruct();
	BOOL RunOnlyOneApp();
	void SendUIMsg(int message,CString jsonaddr);
	void CheckPathValid(const CStringA& strDir);
public:
	bool isStartMainDlg;
	CNoUiMsgBuffer m_noUiMsgBuffer;
	bool m_bServerState;         //�������״̬ false:�ر� true:����
	int progessPos;

	CSplashThread* pSplashThread;
	int blocktipheight;
	bool IsSyncBlock;
	bool IsSyncTx;
};


extern CDacrsUIApp theApp;