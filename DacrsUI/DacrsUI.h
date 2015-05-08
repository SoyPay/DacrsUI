
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

using namespace std;
// CDacrsUIApp:
// 有关此类的实现，请参阅 DacrsUI.cpp
//
#define		MSG_TITLE_LBUTTON_DOWN		(WM_USER + 0x0001)

class CDacrsUIApp : public CWinApp
{
public:
	CDacrsUIApp();

// 重写
public:
	virtual BOOL InitInstance();
public:
	UINT          gsLanguage            ;   //语言类型
	CString		  str_InsPath	        ;   
	CString		  str_ModuleFilename	;    
	CString		  m_ProgressGifFile	;		//进度GIF文件
	void          GetMoFilename( CString & path , CString & filename ) ;// 获取文件名和路径
	int           language()    ;     //读取设置语言
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
	bool            m_blockAutoDelete;
	bool            m_msgAutoDelete;
	void            StartblockThrd();  
	static UINT __stdcall blockProc(LPVOID pParam);
	static UINT __stdcall ProcessMsg(LPVOID pParam);
	static UINT __stdcall ProcessNoUiMsg(LPVOID pParam);
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
	CString      m_betScritptid;
	CString      m_darkScritptid;
	CDarkTxCfg		m_DarkCfg;
	CP2PBetCfg		m_P2PBetCfg;
public:
	void         OnInitList();   //初始化list
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
	void UpdataUIData();
public:
	CMyQueue m_MsgQueue;
	CMyQueue m_UimsgQueue;
	void UpdataBetPoolData();
	void UpdataAddressData();
	void InsertarevtransactionData(string hash);
	void UpdatarevtransactionData(string hash);
	void UpdatarevAppRecord(string txdetail);
public:
	bool isStartMainDlg;
	CNoUiMsgBuffer m_noUiMsgBuffer;
	int progessPos;
};


extern CDacrsUIApp theApp;