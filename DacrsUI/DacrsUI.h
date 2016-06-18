
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
#include "MessageBoxEx.h"
using namespace std;
// CDacrsUIApp:
// �йش����ʵ�֣������ DacrsUI.cpp
//
#define		MSG_TITLE_LBUTTON_DOWN		(WM_USER + 0x0001)

#define strprintf tfm::format

class CDacrsUIApp : public CWinApp
{
public:
	CDacrsUIApp();

// ��д
public:
	virtual BOOL InitInstance();
public:
	UINT          gsLanguage            ;   //��������
	string		  str_InsPath	        ;	//Ӧ�ó���ĵ�ǰ·�� 
	string		  str_ModuleFilename	;   //Ӧ�ó�����ļ���
	string		  m_ProgressGifFile	;		//����GIF�ļ�
	string		  m_ProgressOutGifFile	;	//����GIF�ļ�
	void          GetMoFilename( string & path , string & filename ) ;// ��ȡ�ļ�����·��
	int           language()    ;           //��ȡ��������
	CString       m_strAddress  ;
	BOOL          m_bReIndexServer;         //�����򿪷���������
	BOOL          m_bOutApp     ;     //�˳�����
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
	HANDLE          m_hProcessAppTxThread;
	UINT            m_uProAppTxThreadId;
	HANDLE          m_hProcessBet;
	UINT            m_uProBetxThreadId;
	HANDLE          m_hProcessUpadata;
	UINT            m_uProUpadataThreadId;
	bool            m_blockAutoDelete;
	bool            m_msgAutoDelete;
	bool            m_msgprocessexit;   //true ������Ϣ�Ľ����˳��� false û���˳�
	void            StartblockThrd();  
	static UINT __stdcall blockProc(LPVOID pParam);
	static UINT __stdcall ProcessMsg(LPVOID pParam);
	static UINT __stdcall ProcessNoUiMsg(LPVOID pParam);
	static UINT __stdcall ProcessAppTx(LPVOID pParam);
	static UINT __stdcall ProcessBetAcceptTx(LPVOID pParam);
	static UINT __stdcall  UpdateProcess(LPVOID lpParam);
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
	string      m_betScritptid;
	string      m_ipoScritptid;
	string      m_darkScritptid;
	string      m_redPacketScriptid;
	CDarkTxCfg		m_DarkCfg;
	CP2PBetCfg		m_P2PBetCfg;
	CRedPacketCfg   m_RedPacketCfg;
	CNewestScriptCfg m_neststcriptid;

	CRedPacketStepCfg m_redPackestep;
	CP2PBetStepCfg m_p2pbetstep;
	
public:
	void         OnInitList();   //��ʼ��list
	int          SendPostThread(DWORD msgtype);
	void         ParseUIConfigFile(const string& strExeDir);
public:
	void StartSeverProcess(const string& strdir);
	void CloseProcess(const string& exename);
	bool IsExistProcess(const string& exename);
	int Update();
	bool GetVersionFromServer(string& strVersion);
	BOOL					m_bStartServer;
	PROCESS_INFORMATION		sever_pi; 
	string				m_rpcport;
	string				m_strServerCfgFileName;
	string				m_sendPreHeadstr;
	string				m_sendendHeadstr;
	string              m_uirpcport;
	string              m_severip;
	string              m_rpcUser;
	string              m_rpcPassWord;
public:
	CMyQueue m_MsgQueue;
	CMyQueue m_UimsgQueue;
	CMyQueue m_LockmsgQueue;
	CMyQueue m_UiManDlgQueue;
	CMyQueue m_UiReciveDlgQueue;
	CMyQueue m_UiSendDlgQueue;
	CMyQueue m_UiTxDetailQueue;
	CMyQueue m_NetConnetCountQueue;
	void UpdateRedPacketPoolData();
	void UpdateQuizPoolData();
	void UpdateAddressData();
	void InsertTransaction(string hash);
	void InsertAppTransaction(string txdetail);
	void UpdateTransaction(string hash);
	void SyncTransaction(string obj);
	void UpdateAppRecord(string txdetail);
	void InsertAddbook(uistruct::ADDRBOOK_t );
	void UpdateAddbook(uistruct::ADDRBOOK_t );
	//BOOL UpdateP2pBetRecord();
	void OpenBetRecord(vector<unsigned char> openbet,uistruct::REVTRANSACTION_t transcion);
	void AcceptBetRecord(vector<unsigned char> acceptbet,uistruct::REVTRANSACTION_t transcion);
	void SendBetRecord(vector<unsigned char> sendbet,uistruct::REVTRANSACTION_t transcion);
	void OpenBet(CString txhash,BOOL Flag = TRUE);
	void DeleteItemTransaction(int height);
	void GetMainDlgStruct();
	BOOL RunOnlyOneApp();
	void SendUIMsg(int message,string jsonaddr);
	void CheckPathValid(const string& strDir);

	void AcceptRePacketCommtRecord(vector<unsigned char> acceptRedPacket,uistruct::REVTRANSACTION_t transcion);
	void SendRePacketCommtRecord(vector<unsigned char> sendRedPacket,uistruct::REVTRANSACTION_t transcion);

	void AcceptRePacketSpecailRecord(vector<unsigned char> acceptRedPacket,uistruct::REVTRANSACTION_t transcion);
	void SendRePacketSpecailRecord(vector<unsigned char> sendRedPacket,uistruct::REVTRANSACTION_t transcion);

	bool IsLuckyRedPacket(CString acceptregid,uistruct::RED_DATA redPacket);
	INT64 GetRedPacketAmount(CString acceptregid,uistruct::RED_DATA redPacket);
	bool  isMine(CString addr);
	bool IsLockWallet();
	void PopupCommBalloonTip(string hash);
	void PopupContactBalloonTip(uistruct::REVTRANSACTION_t,int apptype,int txtype);
	void ScanQUIZNotAcceptBet();
	void SetUpdataLanguage();
	bool CheckUpdatafile();
	int InitEncrypt();
	bool Verify(char *publickey,char *publicmod,char *output, unsigned int *outputlen, char *input, unsigned int inputlen,char *vermessage);
	bool OnCheckUpdataEnvirment();
	void OnitSetLaguage();
	void OnCheckSeverfile();
public:
	bool isStartMainDlg;
	CNoUiMsgBuffer m_noUiMsgBuffer;
	bool m_bServerState;         //�������״̬ false:�ر� true:����
	int progessPos;

	CSplashThread* pSplashThread;
	int blocktipheight;
	bool IsSyncBlock;
	bool IsSyncTx;
	bool IsSyncAppTx;
	bool IsWalletLocked; //Ǯ������״̬ TRUE : ������״̬ FALSE
	bool HaveLocked; // ����TRUE : ���� FALSE
	int  netWork;  /// 0 ��main����(��ʽ����)  1 regtest����(��������) 2testnet(��������) 
	int m_reminder;    //�Ƿ�Ҫ������ʾ��0 ����Ĭ����С�� 1���»�������   2 �رյ���  3�ر� ������
	bool m_poptips;
	BOOL m_passlock;    //�Ƿ�Ҫ������ʾ�򣬹رյ�ʱ�� false�����������ڽ����� true����,�����ڽ�����
	BOOL m_dlgCreatfinsh;   // ���Ի���������dlg�Ƿ񴴽���

	CFont m_fontSong;         //��������
	CFont m_fontBlackbody;    //��������
	int   m_syncHight;
	bool  m_commitdb;
	bool  m_readQuizPool;         /// TURE ���Դ����ݿ��ж� ��Լ�ص����� FALSE �����Զ�
	bool  m_readReadCommPacketPool;         /// TURE ���Դ����ݿ��ж� ����ص����� FALSE �����Զ�
	bool  m_readReadSpecalPacketPool;         /// TURE ���Դ����ݿ��ж� ����ص����� FALSE �����Զ�
	map<string,CONFIG_APP_DATA> m_listapp;
	string helpurlcn;
	string helpurlen;
	int      m_lockstate;
private:
	string  m_sigdacrsclient;				//encrypt.ini�������ļ�����
	string  m_singdacrsui;
	string  m_singdacrs;
	string  m_singRunBat;
	map<string,string>  m_listappid;

public:
	virtual int ExitInstance();
};


extern CDacrsUIApp theApp;