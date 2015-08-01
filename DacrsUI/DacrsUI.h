
// DacrsUI.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
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
// 有关此类的实现，请参阅 DacrsUI.cpp
//
#define		MSG_TITLE_LBUTTON_DOWN		(WM_USER + 0x0001)

#define strprintf tfm::format

class CDacrsUIApp : public CWinApp
{
public:
	CDacrsUIApp();

// 重写
public:
	virtual BOOL InitInstance();
public:
	UINT          gsLanguage            ;   //语言类型
	string		  str_InsPath	        ;   
	string		  str_ModuleFilename	;    
	string		  m_ProgressGifFile	;		//进度GIF文件
	string		  m_ProgressOutGifFile	;	//进度GIF文件
	void          GetMoFilename( string & path , string & filename ) ;// 获取文件名和路径
	int           language()    ;           //读取设置语言
	CString       m_strAddress  ;
	BOOL          m_bReIndexServer;         //重启打开服务程序进程
	BOOL          m_bOutApp     ;     //退出软件
public:
	CSqliteDeal   m_SqliteDeal  ;     //Sqlite3数据库操作

// 实现

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
	bool            m_blockAutoDelete;
	bool            m_msgAutoDelete;
	void            StartblockThrd();  
	static UINT __stdcall blockProc(LPVOID pParam);
	static UINT __stdcall ProcessMsg(LPVOID pParam);
	static UINT __stdcall ProcessNoUiMsg(LPVOID pParam);
	static UINT __stdcall ProcessAppTx(LPVOID pParam);
public:
	CMyMutex		cs_MsgDisptch ;    //消息订阅临界
	CMyMutex        cs_SqlData ;       //数据列表临界
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
		void	*relMsgMem	;	//回接内存地址
	} MSG_NOTIFY ;

	std::vector<MSG_NOTIFY> sSubscribeList ;
	//线程组
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
public:
	void         OnInitList();   //初始化list
	int          SendPostThread(DWORD msgtype);
	void         ParseUIConfigFile(const string& strExeDir);
public:
	void StartSeverProcess(const string& strdir);
	void CloseProcess(const string& exename);
	void CheckUpdate();
	int Update();
	bool GetVersionFromServer(string& strVersion);
	BOOL					m_bStartServer;
	PROCESS_INFORMATION		sever_pi; 
	string				m_rpcport;
	string					m_strServerCfgFileName;
	string				m_sendPreHeadstr;
	string				m_sendendHeadstr;
	string                m_uirpcport;
	string                 m_severip;
	string                m_rpcUser;
	string                m_rpcPassWord;
public:
	CMyQueue m_MsgQueue;
	CMyQueue m_UimsgQueue;
	CMyQueue m_LockmsgQueue;
	CMyQueue m_UiManDlgQueue;
	CMyQueue m_UiReciveDlgQueue;
	CMyQueue m_UiSendDlgQueue;
	CMyQueue m_UiTxDetailQueue;
	CMyQueue m_UiP2pDlgQueue;
	CMyQueue m_UiRedPacketDlgQueue;
	CMyQueue m_NetConnetCountQueue;
	void UpdateRedPacketPoolData();
	void UpdateQuizPoolData();
	void UpdateAddressData();
	void InsertTransaction(string hash);
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
	void ClearTransaction();
	void GetMainDlgStruct();
	BOOL RunOnlyOneApp();
	void SendUIMsg(int message,string jsonaddr);
	void SendP2pMsg(int message,string jsonaddr);
	void CheckPathValid(const string& strDir);

	void AcceptRePacketCommtRecord(vector<unsigned char> acceptRedPacket,uistruct::REVTRANSACTION_t transcion);
	void SendRePacketCommtRecord(vector<unsigned char> sendRedPacket,uistruct::REVTRANSACTION_t transcion);

	void AcceptRePacketSpecailRecord(vector<unsigned char> acceptRedPacket,uistruct::REVTRANSACTION_t transcion);
	void SendRePacketSpecailRecord(vector<unsigned char> sendRedPacket,uistruct::REVTRANSACTION_t transcion);

	bool IsLuckyRedPacket(CString acceptregid,uistruct::RED_DATA redPacket);
	INT64 GetRedPacketAmount(CString acceptregid,uistruct::RED_DATA redPacket);
	bool  isMine(CString addr);
	bool IsLockWallet();
	
public:
	bool isStartMainDlg;
	CNoUiMsgBuffer m_noUiMsgBuffer;
	bool m_bServerState;         //服务程序状态 false:关闭 true:运行
	int progessPos;

	CSplashThread* pSplashThread;
	int blocktipheight;
	bool IsSyncBlock;
	bool IsSyncTx;
	bool IsSyncAppTx;
	bool IsWalletLocked; //钱包锁定状态 TRUE : 不锁定状态 FALSE
	bool HaveLocked; // 有锁TRUE : 无锁 FALSE
	int  netWork;  /// 0 是main网络(正式网络)  1 regtest网络(局域网络) 2testnet(测试网络) 
	int m_reminder;    //是否要弹出提示框，关闭的时候 false不弹出 true弹出
	BOOL m_passlock;    //是否要弹出提示框，关闭的时候 false不弹出，存在解锁框 true弹出,不存在解锁框
	BOOL m_dlgCreatfinsh;   // 主对话框框的所有dlg是否创建完
};


extern CDacrsUIApp theApp;