
// DacrsUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "SynchronousSocket.h"
#include <afxinet.h>
#include "Language.h"
#include "StartProgress.h"
#include "WalletPassPhrase.h"
#include "ChoseLanguage.h"

#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>

#include "md5.h"
#include <afxsock.h>
#include "rsa.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "GdiPlusInit.h"
#include "StartProgress.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
// CDacrsUIApp

BEGIN_MESSAGE_MAP(CDacrsUIApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CDacrsUIApp 构造

CDacrsUIApp::CDacrsUIApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_betScritptid = _T("");
	m_darkScritptid =_T("");
	m_ipoScritptid = _T("");
	m_redPacketScriptid = _T("");
	m_blockSock  = INVALID_SOCKET ;
	m_strServerCfgFileName = "dacrs.conf";
	isStartMainDlg = false;
	m_bServerState = false;
	m_rpcUser = _T("");
	m_rpcPassWord = _T("");
	progessPos = 0;
	m_strAddress = _T("");
	m_bOutApp = FALSE ;
	m_bReIndexServer = FALSE;
	blocktipheight = 0;
	IsSyncBlock = FALSE;
	IsSyncTx = FALSE;
	IsSyncAppTx = FALSE;
	IsWalletLocked = TRUE;
	HaveLocked = FALSE;
	netWork = 0;
	m_reminder = 0;
	m_passlock = TRUE;
	m_dlgCreatfinsh = FALSE;
	m_syncHight = 0;
	m_commitdb = FALSE;
	m_readQuizPool = FALSE;
	m_readReadCommPacketPool = FALSE;;        
	m_readReadSpecalPacketPool = FALSE;
	m_poptips = true;
	m_msgprocessexit = false;
	helpurlcn = "";
	helpurlen = "";
	m_sigdacrsclient = "";
	m_singdacrsui ="";
	m_singdacrs ="";
	m_singRunBat ="";
	m_lockstate = 0;
}


// 唯一的一个 CDacrsUIApp 对象

CDacrsUIApp theApp;

BOOL  EnableDebugPrivilege();
// CDacrsUIApp 初始化


BOOL CDacrsUIApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	GetMoFilename( str_InsPath , str_ModuleFilename ); //获取文件路径和文件名称

	OnitSetLaguage();

	if (!RunOnlyOneApp())
	{
		return FALSE;
	}

	//创建字体
	m_fontSong.CreateFont (15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "宋体");
	m_fontBlackbody.CreateFont(15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "黑体");
	

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	if (!AfxSocketInit()){   //初始化SOCKET
		return FALSE ;
	}
		
	m_blockAutoDelete = false;
	m_msgAutoDelete= false;
	CheckPathValid( str_InsPath );

	
	/// 加载配置文件
	ParseUIConfigFile(str_InsPath);
	//初始化日志配置参数
	InitLogCfg();

	LogPrint("INFO", "启动DacrsUI程序\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	if (OnCheckUpdataEnvirment())
	{
		return TRUE;
	}
	

	string temprpc = m_rpcport;
	string tempuiport = m_uirpcport;
	ProductHttpHead(str_InsPath ,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr,m_uirpcport,netWork);

	string dbpath = strprintf("%s\\db",str_InsPath);
	if (!PathIsDirectory(dbpath.c_str()))
	{
		::CreateDirectory(dbpath.c_str(), NULL);
	}
	//打开sqlite3数据库
	m_SqliteDeal.InitializationDB();


	if (strcmp(m_severip.c_str(),_T("127.0.0.1")))
	{
		m_rpcport = temprpc;
		m_uirpcport = tempuiport;
	}
	OnCheckSeverfile();

	//启动服务程序
	StartSeverProcess(str_InsPath);
	m_bServerState = true;
	
//	Sleep(1000);

	//连接block
	//连接到服务器
	CSynchronousSocket te;
	SOCKET nSocket = te.OnblockConnnect(m_severip.c_str(),atoi(m_uirpcport.c_str()) ) ;
	if ( INVALID_SOCKET != nSocket ){
		TRACE("nSocket OK\n");
		theApp.m_blockSock = nSocket ;
	}
	else
	{
		TRACE("nSocket Error\n");
	}
	theApp.StartblockThrd();  //开启Block线程
	//gif
	m_ProgressGifFile =   str_InsPath + "\\gif\\progress.gif\0" ;
	if (theApp.language() == 2)
	{
		m_ProgressOutGifFile =   str_InsPath + "\\gif\\exit_en.gif\0" ;
	}else
	{
		m_ProgressOutGifFile =   str_InsPath + "\\gif\\exit.gif\0" ;
	}
	
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Status state = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) ;
	if( Ok != state ) {
		switch( state )
		{
		case OutOfMemory :
			{

				UiFun::MessageBoxEx(_T("OutOfMemory") , _T("Error") ,MFB_OK|MFB_ERROR );
			}
			break;
		}
		return FALSE ;
	}

	pSplashThread = (CSplashThread*) AfxBeginThread(RUNTIME_CLASS(CSplashThread),THREAD_PRIORITY_NORMAL,0, CREATE_SUSPENDED); 
	ASSERT(pSplashThread->IsKindOf(RUNTIME_CLASS(CSplashThread)));
	pSplashThread->ResumeThread(); 
	Sleep(1); 

	int nCount(0);

	SYSTEMTIME WaitTimeLast ;
	memset( &WaitTimeLast , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &WaitTimeLast ) ;

	m_bReIndexServer = TRUE;
	while(1)
	{
		
		HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS,FALSE,sever_pi.dwProcessId);  
		if(NULL == processHandle)
		{
			int errorCode = GetLastError();
			TRACE("Error OpenProcess:%d " , errorCode );
			//UiFun::MessageBoxEx(_T("区块链数据库损坏，请双击运行钱包下clear.bat文件，再重新打开钱包\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
			if (IDYES==UiFun::MessageBoxEx(_T("区块链数据库损坏,是否清除区块链数据，重新同步") , _T("Error") ,MFB_YESNO|MFB_ERROR ))
			{
				string batpath =strprintf("%s\\clear.bat",str_InsPath);
				ShellExecute(NULL, "open",batpath.c_str(), NULL, NULL, SW_SHOW);
				//system(batpath.c_str());
			}
			exit(1);
		}

		CloseHandle(processHandle);
		if (isStartMainDlg)
		{
			break;
		}
		Sleep(100);
	}
	
	CDacrsUIDlg dlg;
	m_pMainWnd = &dlg;
	
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

bool CDacrsUIApp::OnCheckUpdataEnvirment()
{
	//检测自动升级
	Update();
	
	Sleep(1000);

	//CheckUpdatafile();

	if(CSoyPayHelp::getInstance()->IsOSVersionBelowXp()) {
		if(!EnableDebugPrivilege())
			TRACE(_T("Call EnableDebugPrivilege failed!"));
		//				AfxMessageBox(_T("Call EnableDebugPrivilege failed!"));
		//// 不更新直接退出
		exit(0); 
	}
	return false;
}
void CDacrsUIApp::OnitSetLaguage()
{
	gsLanguage = language();
	if (gsLanguage == 0)
	{
		gsLanguage = 1;
		CChoseLanguage chosedlg;
		chosedlg.DoModal();
		gsLanguage = language();
	}
	//设置升级软件的语言包
	SetUpdataLanguage();
}
void CDacrsUIApp::OnCheckSeverfile()
{
	string strSeverPath = strprintf("%s\\dacrs-d.exe",str_InsPath);

	if (PathIsDirectory(strSeverPath.c_str()))
	{
		::CreateDirectory(strSeverPath.c_str(), NULL);
		::RemoveDirectory(strSeverPath.c_str());
		if( (_access( strSeverPath.c_str(), 0 )) == -1 )
		{
			UiFun::MessageBoxEx(_T("dacrs-d 文件不存在,请重新启动钱包\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
			exit(1);
		}
	}

	/*strSeverPath = strprintf("%s\\temp\\dacrs-d.exe",str_InsPath);

	if (PathIsDirectory(strSeverPath.c_str()))
	{
		::CreateDirectory(strSeverPath.c_str(), NULL);
		::RemoveDirectory(strSeverPath.c_str());
	}
	/// 判断文件是否存在
	if( (_access( strSeverPath.c_str(), 0 )) == -1 )
	{
		UiFun::MessageBoxEx(_T("dacrs-d 文件不存在,请重新下载\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
		exit(1);
	}*/
}
/*
提升程序运行权限，解决xp系统下调用OpenProcess()API返回失败码：5的问题。
*/
BOOL  EnableDebugPrivilege()
{
	HANDLE hToken;
	BOOL fOk=FALSE;
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken)) //Get Token
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount=1;
		if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid))//Get Luid
			printf("Can't lookup privilege value.\n");
		tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;//这一句很关键，修改其属性为SE_PRIVILEGE_ENABLED
		if(!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL))//Adjust Token
			printf("Can't adjust privilege value.\n");
		fOk=(GetLastError()==ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOk;
}

void CDacrsUIApp::GetMoFilename( string & path , string & filename ) 
{
	char strPath[MAX_PATH] ;
	GetModuleFileName( AfxGetApp()->m_hInstance , strPath , MAX_PATH );

	string strTemp=strPath;
	int i = strTemp.rfind('\\');
	path		= strTemp.substr(0,i);
	filename	= strPath;
}
int CDacrsUIApp::language()
{
	// 取默认语言索引
	char szText[256];
	string  strG;
	string strAppIni = str_InsPath;
	strAppIni += LANGUAGE_FILE;
	::GetPrivateProfileString("Default", "Index", "1", szText, 256,	(LPCTSTR)strAppIni.c_str());

	strG +=strprintf("Language%d", atoi(szText));
	::GetPrivateProfileString(strG.c_str(), "gsLanguage", "1", szText, 256, (LPCTSTR)strAppIni.c_str());
	return atoi(szText);
}
BOOL CDacrsUIApp::CreateMaintainThrd() 
{
	hMtThrd		= NULL ;
	nMtThrdID	= 0  ;
	hMtStartEvent = ::CreateEvent(0,FALSE,FALSE,0) ; 

	hMtThrd = (HANDLE)_beginthreadex( NULL, 0, &MtProc, this, 0, &nMtThrdID ) ;
	if( INVALID_HANDLE_VALUE == hMtThrd ) {
		UiFun::MessageBoxEx(_T("维护线程创建失败!") , _T("出错") ,MFB_OK|MFB_ERROR );
		CloseHandle( hMtStartEvent ) ;
		hMtStartEvent = NULL ;
		return  FALSE;
	}
	::WaitForSingleObject( hMtStartEvent , 100000 ) ;
	CloseHandle( hMtStartEvent ) ;
	hMtStartEvent = NULL ;

	return TRUE ;	
}
UINT __stdcall CDacrsUIApp::MtProc(LPVOID pParam)
{
	MSG	msg , msgcpy ;
	//PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE) ;

	if(!SetEvent( ((CDacrsUIApp*)pParam)->hMtStartEvent ) ) {
		return 1;
	}

	while ( 1 )
	{	
		BOOL MsgReturn	= GetMessage( &msg , NULL , WM_USER, MSG_USER_END);
		memset( &msgcpy , 0 , sizeof(MSG) ) ; 
		memcpy( &msgcpy , &msg , sizeof(MSG) );
		if ( MsgReturn )
		{
			switch ( msg.message )
			{
			case MSG_USER_OUT:
				{
					while(1)
					{
						if(((CDacrsUIApp*)pParam)->m_bOutApp){
							CDacrsUIDlg* pMainWnd = (CDacrsUIDlg*)AfxGetApp()->m_pMainWnd ;
							if ( NULL != pMainWnd ) {
								pMainWnd->CloseApp(); //关闭对话框
							}
							break;
						}
					}
				}
				break;
			case MSG_USER_QUITTHREAD:
				{
					std::vector< sThrd >::iterator it ;
					CDacrsUIApp* pApp = ((CDacrsUIApp*)pParam) ;
					for( it = pApp->v_ProcSubThrd.begin() ; it != pApp->v_ProcSubThrd.end() ; it++ ) {
						::PostThreadMessage( it->hThrdId , MSG_USER_QUITTHREAD , 0 , 0 ) ;
						DWORD exc = 0xffff ;
						while( ::GetExitCodeThread( it->hThrd , &exc ) ) {
							if( STILL_ACTIVE == exc ) {
								;
							}else {
								TRACE( "EXC = %d \n" , exc ) ;
								break;
							}
						}
					}
					pApp->v_ProcSubThrd.clear() ;
					_endthreadex( 0 ) ; 
				}
				break;
			default:
				break;
			}
			((CDacrsUIApp*)pParam)->DispatchMsg( ((CDacrsUIApp*)pParam)->GetMtHthrdId() , msgcpy.message , msgcpy.wParam , msgcpy.lParam ) ;
		}
	}
}
UINT __stdcall CDacrsUIApp::UpdateProcess(LPVOID lpParam){
	while(TRUE)
	{
		///一分钟
		static int nCount = 0;
		Sleep(1);
		if (theApp.m_msgAutoDelete)
		{
			return 1;
		}
		nCount++;
		if(nCount == 30000 ){
				nCount =0;

			CDacrsUIApp * pUiDemeDlg  = (CDacrsUIApp*)lpParam ;
			CString sPath;
			sPath.Format(_T("%s\\update.exe"),pUiDemeDlg->str_InsPath.c_str());
			sPath.Replace("\\\\","\\");
			CFileFind find;
			if(!find.FindFile(sPath)) return false;
			SHELLEXECUTEINFO ShRun = {0}; 
			ShRun.cbSize = sizeof(SHELLEXECUTEINFO); 
			ShRun.fMask = SEE_MASK_NOCLOSEPROCESS; 
			ShRun.hwnd = NULL; 
			ShRun.lpVerb = NULL; 
			ShRun.lpFile = sPath;
			ShRun.lpParameters = _T("-checkforupdates"); 
			ShRun.nShow = SW_HIDE; 
			ShRun.hInstApp = NULL; 
			ShellExecuteEx(&ShRun); 
			WaitForSingleObject(ShRun.hProcess, 120000); 
			ULONG lResult = 0; 
			if (!GetExitCodeProcess(ShRun.hProcess, &lResult)) return false;
			if (lResult == 0) continue;
			CString sMsg;
			if (theApp.language() == 1)
			{
				sMsg.Format(_T("检查有%d文件需要更新,现在是否要更新?"),lResult);
			}else{
				sMsg.Format(_T("%d updates found at the website,now to upgrade?"),lResult);

			}
		
			if ( IDYES != UiFun::MessageBoxEx(sMsg, UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_YESNO|MFB_TIP ) ){
				((CDacrsUIDlg*)(pUiDemeDlg->m_pMainWnd))->ClosWalletWind();
				return false;
			}

			SendMessage(((CDacrsUIDlg*)(pUiDemeDlg->m_pMainWnd))->m_hWnd,MSG_USER_CLOSEPROCESS,NULL,NULL);
			pUiDemeDlg->CloseProcess("dacrs-d.exe");
			DWORD exc;
			pUiDemeDlg->m_msgAutoDelete= true;
			pUiDemeDlg->m_blockAutoDelete = true;
			closesocket(theApp.m_blockSock);
			/// 等待处理消息进程退出
			while( ::GetExitCodeThread( theApp.m_msgThread , &exc ) ) {

				if( STILL_ACTIVE == exc ) {
					;
				}else {
					TRACE( "EXC = %d \n" , exc ) ;
					break;
				}
				Sleep(100);
			}
			ShRun.lpParameters = NULL; 
			ShellExecuteEx(&ShRun);
			pUiDemeDlg->CloseProcess("DacrsUI.exe");
		}
	}
	return true;
}
void CDacrsUIApp::StartblockThrd()
{
	m_hblockThread = (HANDLE)_beginthreadex(NULL,	0,	blockProc, this, 0, &ublockThreadId);	
	m_msgThread = (HANDLE)_beginthreadex(NULL,	0,	ProcessMsg, this, 0, &umsgThreadId);
	m_hProcessNoUiMsgThread = (HANDLE)_beginthreadex(NULL,	0,	ProcessNoUiMsg, this, 0, &m_uProNoUiMsgThreadId);
	m_hProcessAppTxThread = (HANDLE)_beginthreadex(NULL,	0,	ProcessAppTx, this, 0, &m_uProAppTxThreadId);	
	m_hProcessBet = (HANDLE)_beginthreadex(NULL,	0,	ProcessBetAcceptTx, this, 0, &m_uProBetxThreadId);	

	m_hProcessUpadata = (HANDLE)_beginthreadex(NULL,0,UpdateProcess, this, 0, &m_uProUpadataThreadId);	

	return ;
}

UINT __stdcall CDacrsUIApp::ProcessBetAcceptTx(LPVOID pParam)
{
	CDacrsUIApp * pUiDemeDlg  = (CDacrsUIApp*)pParam ;
	while ( true)
	{

		if (theApp.m_msgAutoDelete)
		{
			return 1;
		}
		static int timelag = 0;
		SYSTEMTIME curTime ;
		memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
		GetLocalTime( &curTime ) ;
		int Curtimelag = UiFun::SystemTimeToTimet(curTime);
		/// 同步以后更新数据库表
		if (timelag == 0 || (Curtimelag-timelag) >=180)  /// 时间间隔3分
		{
			pUiDemeDlg->ScanQUIZNotAcceptBet();
			timelag = Curtimelag;
		}

		Sleep(100); 
	}
	return 1 ;
}
UINT __stdcall CDacrsUIApp::ProcessAppTx(LPVOID pParam)
{
	CDacrsUIApp * pUiDemeDlg  = (CDacrsUIApp*)pParam ;
	while ( true)
	{

		if (theApp.m_msgAutoDelete)
		{
			return 1;
		}
		/// 同步以后更新数据库表
		if (theApp.IsSyncAppTx && theApp.m_commitdb)
		{
			theApp.m_SqliteDeal.UpdataAllTableData();
			return 1;
		}
		Sleep(100); 
	}
	return 1 ;
}
UINT __stdcall CDacrsUIApp::ProcessMsg(LPVOID pParam) {
	CDacrsUIApp * pUiDemeDlg  = (CDacrsUIApp*)pParam ;
	
	while ( true)
	{
		CPostMsg Postmsg ;
		/// 消息处理完 才能够退出
		if (theApp.m_msgAutoDelete && !pUiDemeDlg->m_MsgQueue.pop(Postmsg))
		{
			pUiDemeDlg->m_msgprocessexit = true;
			return 1;
		}

		if(!pUiDemeDlg->m_MsgQueue.pop(Postmsg)){
			Sleep(100); 
			continue;
		}
		CDacrsUIDlg *pDlg = (CDacrsUIDlg*)(((CDacrsUIApp*)pParam)->m_pMainWnd) ;
		if ((pDlg == NULL || !theApp.m_dlgCreatfinsh) && Postmsg.GetUItype() != MSG_USER_STARTPROCESS_UI&&Postmsg.GetDatatype() !=WM_SYNC_TRANSACTION){
			pUiDemeDlg->m_MsgQueue.push(Postmsg);
			Sleep(100); 
			//TRACE("push message:MSG_USER_STARTPROCESS_UI\n");
			continue;
		}
		//TRACE("meassage wwwwwwwwwwwwwww\n");
		switch (Postmsg.GetUItype() )
		{
		case MSG_USER_STARTPROCESS_UI:
			{
				LogPrint("PROCESSMSG", "MSG_USER_STARTPROCESS_UI 第:%d\n",Postmsg.GetDatatype());
				theApp.DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_STARTPROCESS_UI ,Postmsg.GetDatatype(),0);
				if (Postmsg.GetDatatype() == 4)
				{
					theApp.isStartMainDlg = true;
				}
				if(theApp.IsSyncTx){
					theApp.IsSyncTx = FALSE;
					theApp.m_commitdb =theApp.m_SqliteDeal.CommitDbTransaction();
					TRACE("Sync Tx commit transaction\n");
					LogPrint("INFO", "Sync Tx commit transaction\n");
				}
			}
			break;
		case MSG_USER_GET_UPDATABASE:
			{
				//EnterCriticalSection( &theApp.cs_UpDatabasech ) ;
				switch (Postmsg.GetDatatype())
				{
				case WM_UP_ADDRESS:
					{
						LogPrint("PROCESSMSG", "WM_UP_ADDRESS\n");
						//更新钱包地址数据库
						((CDacrsUIApp*)pParam)->UpdateAddressData();
					}
					break;
				case WM_SYNC_TRANSACTION:
					{
						
						if(!theApp.IsSyncTx) {
							theApp.IsSyncTx = TRUE;
							theApp.m_SqliteDeal.BeginDBTransaction();
						}
						string txData = Postmsg.GetData();
						if ( "" != txData ) {
							((CDacrsUIApp*)pParam)->SyncTransaction(txData) ;
						}
						LogPrint("PROCESSMSG", "WM_SYNC_TRANSACTION 启动同步交易:%s\n",txData.c_str());
					}
					break;
				case WM_REVTRANSACTION:
					{

						//更新历史交易记录数据库
						string pHash = Postmsg.GetData();
						if ( "" != pHash ) {
							string strCondition("");
							strCondition = strprintf(" hash = '%s' ", pHash.c_str());
							int nItem =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.GetTableCountItem(_T("t_transaction") ,strCondition);
							if (nItem == 0)
							{
								((CDacrsUIApp*)pParam)->InsertTransaction(pHash) ;
							}else{
								((CDacrsUIApp*)pParam)->UpdateTransaction(pHash);
							}
						}
						LogPrint("PROCESSMSG", "WM_REVTRANSACTION 收取跟钱包有关的交易:%s\n",pHash.c_str());
					}
					break;
				case WM_APP_TRANSATION:
					{
						string txDetail = Postmsg.GetData();
						if ( "" != txDetail ) {
							((CDacrsUIApp*)pParam)->UpdateAppRecord(txDetail);
							((CDacrsUIApp*)pParam)->InsertAppTransaction(txDetail);
						}
						LogPrint("PROCESSMSG", "WM_APP_TRANSATION 收取跟钱包有关的应用交易:%s\n",txDetail.c_str());
					}
					break;
				case WM_P2P_BET_RECORD:
					{
						//更新数据库赌约数据库列表
						//((CDacrsUIApp*)pParam)->UpdateP2pBetRecord(); 
					}
					break;
				case WM_UP_BETPOOL:
					{
						LogPrint("PROCESSMSG", "WM_UP_BETPOOL 更新赌约池\n");
						/// 赌约池数据库列表
						BOOL flag =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_quiz_pool"));
						if (flag ) 
						{
							((CDacrsUIApp*)pParam)->UpdateQuizPoolData();
						}
					}
					break;
				case WM_REDPACKETPOOL:
					{
						LogPrint("PROCESSMSG", "WM_REDPACKETPOOL 更新红包池\n");
						/// 红包池
						BOOL flag =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_red_packets_pool"));
						if (flag ) 
						{
							((CDacrsUIApp*)pParam)->UpdateRedPacketPoolData();
						}
					}
				case WM_UP_BlLOCKTIP:
					{
						LogPrint("PROCESSMSG", "WM_UP_BlLOCKTIP 更新block最新高度\n");
						//更新最新blocktip数据库
						string data = Postmsg.GetData();
						if (data == "")
						{
							break;
						}
						if ( ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_chain_tip") ) ) {

							
							uistruct::BLOCKCHANGED_t block;
							if (block.JsonToStruct(data))
							{
								string strinsert;
								strinsert = strprintf("'%s','%d','%d','%d','%d'",block.hash,block.high,block.time,block.fuelrate,((CDacrsUIApp*)pParam)->m_lockstate);
								if (!((CDacrsUIApp*)pParam)->m_SqliteDeal.InsertTableItem(_T("t_chain_tip") ,strinsert ))
								{
									TRACE("insert into table t_chain_tip error");
								}
						
							
							}
							
						}
					}
					break;
				case WM_UP_UPDATABlLOCK:
					{
						uistruct::CHAIN_TIP_T chainblock;
						((CDacrsUIApp*)pParam)->m_SqliteDeal.GetTipBlockHash(&chainblock);
						string strWhere =strprintf("block_hash='%s'",chainblock.blockhash);
						string updatastr = strprintf("lock=%d",((CDacrsUIApp*)pParam)->m_lockstate);
						if (!((CDacrsUIApp*)pParam)->m_SqliteDeal.UpdateTableItem(_T("t_chain_tip") ,updatastr,strWhere ))
						{
							TRACE("insert into table t_chain_tip error");
						}
					}
					break;
				case WM_UP_ADDRBOOK:
					{
						LogPrint("PROCESSMSG", "WM_UP_ADDRBOOK 更新地址簿数据库\n");
						string josnaddr = Postmsg.GetData();
						uistruct::ADDRBOOK_t addr;
						if (addr.JsonToStruct(josnaddr))
						{
							string strCond;
							strCond = strprintf(" address='%s' ", addr.address.c_str());
							
							int nItem =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.GetTableCountItem(_T("t_address_book"), strCond);
							if (0 == nItem)
							{
								((CDacrsUIApp*)pParam)->InsertAddbook(addr) ;
							}else{
								((CDacrsUIApp*)pParam)->UpdateAddbook(addr);
							}
						}
					}
					break;
				case WM_UP_DELETERBOOK:
					{
						LogPrint("PROCESSMSG", "WM_UP_DELETERBOOK 删除地址簿数据库\n");
						string josnaddr = Postmsg.GetData();
						uistruct::ADDRBOOK_t addr;
						if (addr.JsonToStruct(josnaddr))
						{
							string strCond;
							strCond = strprintf(" address='%s' ", addr.address.c_str());
							((CDacrsUIApp*)pParam)->m_SqliteDeal.DeleteTableItem(_T("t_address_book"),strCond);
						}
					}
					break;
				case WM_RELEASETX:
					{
						LogPrint("PROCESSMSG", "WM_RELEASETX 收到交易重新放到mempool\n");
						//更新历史交易记录数据库
						string pHash = Postmsg.GetData();
						if ( _T("") != pHash ) {
		
							string strCondition("");
							strCondition = strprintf(" hash = '%s' ", pHash.c_str());
							int nItem =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.GetTableCountItem(_T("t_transaction") ,strCondition);
							if (nItem != 0)
							{
								string updataValue="confirm_height=0,confirmed_time=0,block_hash=''";
								LogPrint("INFO","WM_RELEASETX:%s",pHash.c_str());
								((CDacrsUIApp*)pParam)->m_SqliteDeal.UpdateTableItem(_T("t_transaction") , strCondition , updataValue ) ;
								nItem =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.GetTableCountItem(_T("t_app_transaction") ,strCondition);
								if (nItem != 0)
								{
									((CDacrsUIApp*)pParam)->m_SqliteDeal.UpdateTableItem(_T("t_app_transaction") , strCondition , updataValue ) ;
								}
								theApp.m_SqliteDeal.UpdataAllTableData();   /// 更新应用表格
							}
						}
					}
					break;
				case WM_REMOVETX:
					{
						string pHash = Postmsg.GetData();
						if ( _T("") != pHash ) {

							string strCondition(_T(""));
							strCondition = strprintf(" hash = '%s' ", pHash.c_str());
							int nItem =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.GetTableCountItem(_T("t_transaction") ,strCondition);
							if (nItem != 0)
							{
								LogPrint("INFO","WM_REMOVETX:%s",pHash.c_str());
								((CDacrsUIApp*)pParam)->m_SqliteDeal.DeleteTableItem(_T("t_transaction"),strCondition);
								///删除跟app相关的交易
								((CDacrsUIApp*)pParam)->m_SqliteDeal.DeleteTableItem(_T("t_app_transaction"),strCondition);
								theApp.m_SqliteDeal.UpdataAllTableData();   /// 更新应用表格
							}
						}
						/// 通知显示交易界面
						CPostMsg Postmsg(MSG_USER_TRANSRECORD_UI,WM_REMOVETX);
						pUiDemeDlg->m_MsgQueue.push(Postmsg);
						LogPrint("PROCESSMSG", "WM_REMOVETX 删除交易:%s\n",pHash.c_str());
					}
					break;
				default:
					break;
				}
				//	LeaveCriticalSection (&theApp.cs_UpDatabasech ) ;
			}
			break;
		case MSG_USER_UP_PROGRESS:
			{
				//string strTemp = Postmsg.GetData();
				//uistruct::BLOCKCHANGED_t      m_Blockchanged;
				//m_Blockchanged.JsonToStruct(strTemp.c_str());
				/*string strCommand1 = strprintf("%s", "listunconfirmedtx");
				Json::Value root1;
				if(!CSoyPayHelp::getInstance()->SendRpc(strCommand1,root1))
				{
					TRACE("OnBnClickedSendtrnsfer rpccmd sendtoaddress error");
					break;
				}

				string strShowData1 = root1.toStyledString();
				int pos1 = strShowData1.find("hash");*/

				switch(Postmsg.GetDatatype())
				{
				case WM_LOCKSTATE:
					{
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS WM_LOCKSTATE 锁的消息\n");
						pUiDemeDlg->m_LockmsgQueue.push(Postmsg);
					}
					break;
				case WM_CONNECTNET:
					{
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS WM_CONNECTNET 网络连接消息\n");
						pUiDemeDlg->m_NetConnetCountQueue.push(Postmsg);
					}
					break;
				default:
					{
						TRACE("change:%s\r\n","MSG_USER_UP_PROGRESS");

						//listunconfirmedtx
						//pUiDemeDlg->m_MsgQueue.ClearMessageType(MSG_USER_UP_PROGRESS);
						pUiDemeDlg->m_UimsgQueue.push(Postmsg);
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS  更新进度条消息:%s\n",Postmsg.GetData().c_str());
					}
					break;
				}
				//theApp.DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_UP_PROGRESS , 0,0);					
			}
			break;
		case MSG_USER_INSERT_DATA:   //// 插入数据
			{
				LogPrint("PROCESSMSG", "MSG_USER_INSERT_DATA 插入数据消息\n");
				uistruct::DATABASEINFO_t   pDatabase; // = (uistruct::DATABASEINFO_t *)Postmsg.GetStrPoint();
				string strTemp = Postmsg.GetData();
				pDatabase.JsonToStruct(strTemp.c_str());
				((CDacrsUIApp*)pParam)->m_SqliteDeal.InsertTableItem(pDatabase.strTabName.c_str() ,pDatabase.strSource.c_str()) ;
				if ( !strcmp(pDatabase.strTabName.c_str() , _T("t_p2p_quiz")) ){
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_P2P_BET_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("t_dark_record")) )
				{
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_DARK_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("t_wallet_address")))
				{
					uistruct::LISTADDR_t addr; 
					addr.JsonToStruct(pDatabase.strcutjson.c_str());
					string Temp = addr.ToJson();
					((CDacrsUIApp*)pParam)->SendUIMsg((int)WM_UP_NEWADDRESS,Temp.c_str());
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_UP_ADDRESS);
				}

			}
			break;
		case MSG_USER_UPDATA_DATA:   /// 更新数据库
			{
				LogPrint("PROCESSMSG", "MSG_USER_UPDATA_DATA 更新数据消息\n");
				uistruct::DATABASEINFO_t pDatabase;// = (uistruct::DATABASEINFO_t *)Postmsg.GetStrPoint();
				string strTemp = Postmsg.GetData();
				pDatabase.JsonToStruct(strTemp);

				if ( !((CDacrsUIApp*)pParam)->m_SqliteDeal.UpdateTableItem(pDatabase.strTabName.c_str() , pDatabase.strSource.c_str() , pDatabase.strWhere.c_str() ) ){
					TRACE(_T("t_p2p_quiz数据更新失败!") );
				}

				if ( !strcmp(pDatabase.strTabName.c_str() , _T("t_p2p_quiz")) ){
					Postmsg.SetType(MSG_USER_UPDATA_DATA,WM_P2P_BET_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("t_dark_record")) )
				{
					Postmsg.SetType(MSG_USER_UPDATA_DATA,WM_DARK_RECORD);
				}

			}
			break;
		default:
			break;
		}
		if (theApp.SendPostThread(Postmsg.GetDatatype()) == 0)
		{
			((CDacrsUIApp*)pParam)->DispatchMsg( ((CDacrsUIApp*)pParam)->GetMtHthrdId() , Postmsg.GetUItype(), Postmsg.GetDatatype() , 0) ;
		}
		
	}
	return 1 ;
}
#define  ININTAL_TYPE          1
#define  BLOCK_CHANGE_TYPE     2
#define  REV_TRANSATION_TYPE   3
#define  APP_TRANSATION_TYPE   4
#define  SERVER_NOTIYF_TYPE    5
#define  SERVER_SYNC_TX        6
#define  WALLET_LOCK           7
#define  WALLET_UNLOCK         8
#define  RELEASE_TX            9
#define  REMOVE_TX            10
#define  CONNECTON_NET        11
#define  SYSTX_HEIGHT         12
int GetMsgType(string const strData,Json::Value &root)
{
	string strType;
	int pos = strData.find("type");
	if ( pos >=0 ){
		strType =root["type"].asString();// strprintf("%s" ,  root["type"].asCString() ) ;
		if ( !strcmp(strType.c_str() ,_T("init") ) ) 
		{
			return ININTAL_TYPE;
		}
		if ( !strcmp(strType.c_str() ,_T("revtransaction") ) )
		{
			return REV_TRANSATION_TYPE;
		}
		if ( !strcmp(strType.c_str() ,_T("blockchanged") ) )
		{
			return BLOCK_CHANGE_TYPE;
		}
		if ( !strcmp(strType.c_str() ,_T("rev_app_transaction") ) )
		{
			return APP_TRANSATION_TYPE;
		}
		if(!strcmp(strType.c_str(), _T("notify"))) 
		{
			return SERVER_NOTIYF_TYPE;
		}
		if(!strcmp(strType.c_str(), _T("SyncTx"))) 
		{
			return SERVER_SYNC_TX;
		}
		if(!strcmp(strType.c_str(), _T("releasetx"))) 
		{
			return RELEASE_TX;
		}
		if(!strcmp(strType.c_str(), _T("rmtx"))) 
		{
			return REMOVE_TX;
		}
		if(!strcmp(strType.c_str(), _T("SyncTxHight"))) 
		{
			return SYSTX_HEIGHT;
		}
	}
	return  -1;
}
bool JsonCheck(string strjson){

	strjson =  UiFun::trimleft(strjson);
	strjson =  UiFun::trimright(strjson);
	if (strjson.at(0) != '{')
	{
		return false;
	}
	int len = strjson.length();
	if (strjson.at(len-1) != '}')
	{
		return false;
	}
	return true;
}


bool ProcessMsgJson(Json::Value &msgValue, CDacrsUIApp* pApp) 
{
	string objstr = msgValue.toStyledString();
	int type = GetMsgType(objstr.c_str(),msgValue);
	LogPrint("RECIVEUI", "MESG:%s\n",objstr.c_str());
	switch(type)
	{
	case ININTAL_TYPE:
		{
//			CPostMsg postmsg(MSG_USER_SHOW_INIT,0);
//			postmsg.SetStrType(msgValue["type"].asCString());
			string msg = msgValue["msg"].asString();
			TRACE("MEST:%s\r\n",msg.c_str());
			if (!strcmp(msg.c_str(),"Verifying blocks..."))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,1);
				pApp->m_MsgQueue.push(postmsg);
			}
			else if (!strcmp(msg.c_str(),"Verifying Finished"))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,2);
				pApp->m_MsgQueue.push(postmsg);
			}
			else if (!strcmp(msg.c_str(),"Loading addresses..."))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,3);
				pApp->m_MsgQueue.push(postmsg);
			}
			else if (!strcmp(msg.c_str(),"initialize end"))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,4);
				pApp->m_MsgQueue.push(postmsg);				
			}
//			postmsg.SetData(msg);
//			pApp->m_MsgQueue.push(postmsg);
//			TRACE("type: %s   mag: %s\r\n" , postmsg.GetStrType() ,msg);
			break;
		}
	case SERVER_SYNC_TX:
		{

			Json::Value obj =msgValue["msg"]; 
			//插入到数据库
			string txData ;
			txData =strprintf("%s" , obj.toStyledString().c_str() );
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_SYNC_TRANSACTION);
			postmsg.SetData(txData);
			pApp->m_MsgQueue.push(postmsg);
			
		}
		break;
	case SYSTX_HEIGHT:
		{
			Json::Value obj =msgValue["msg"]; 
			//插入到数据库
			if (!obj.isNull())
			{
				int height = obj["syncheight"].asInt();
				pApp->m_syncHight = height;
				pApp->DeleteItemTransaction(height);
			}
		}
		break;
	case REV_TRANSATION_TYPE:
		{

			SYSTEMTIME curTime ;
			memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			GetLocalTime( &curTime ) ;
			static int RecivetxtxTimeLast =0;
			int tempTimemsg= (int)UiFun::SystemTimeToTimet(curTime);
			/// 更新钱包
			CPostMsg postuimsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRESS);
			if ((tempTimemsg - RecivetxtxTimeLast)>10 || RecivetxtxTimeLast == 0)
			{	
				pApp->m_MsgQueue.push(postuimsg);
				RecivetxtxTimeLast = tempTimemsg;
			}

		//	LogPrint("INFO", "REV_TRANSATION %s\n",msgValue.toStyledString().c_str());
			const Json::Value& txArray = msgValue["transation"]; 
			//插入到数据库
			string strHash =txArray["hash"].asString();
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
			postmsg.SetData(strHash);
			pApp->m_MsgQueue.push(postmsg);

			}
			break;
		case APP_TRANSATION_TYPE:
		 {
			 Json::Value obj =msgValue["transation"]; 
			 CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_APP_TRANSATION);
			 postmsg.SetData(obj.toStyledString().c_str());
			 pApp->m_MsgQueue.push(postmsg);
		 }
		  break;
	case RELEASE_TX:
		{
			//插入到数据库
			string strHash =  msgValue["hash"].asString() ;
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_RELEASETX);
			postmsg.SetData(strHash);
			pApp->m_MsgQueue.push(postmsg);
		}
		break;
	case REMOVE_TX:
		{
			//插入到数据库
			string strHash = msgValue["hash"].asString() ;
	
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REMOVETX);
			postmsg.SetData(strHash);
			pApp->m_MsgQueue.push(postmsg);
		}
		break;
	case BLOCK_CHANGE_TYPE:
		{
			SYSTEMTIME curTime ;
			memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			GetLocalTime( &curTime ) ;
			static int RecivetxMsgTimeLast =0;
			int tempTimemsg=(unsigned int) UiFun::SystemTimeToTimet(curTime);
		
			TRACE("change:%s\r\n","blockchanged");
			uistruct::BLOCKCHANGED_t      m_Blockchanged ;
			m_Blockchanged.type = msgValue["type"].asString();
			m_Blockchanged.tips = msgValue["tips"].asInt();
			m_Blockchanged.high = msgValue["high"].asInt64() ;
			m_Blockchanged.hash = msgValue["hash"].asString();
			m_Blockchanged.connections = msgValue["connections"].asInt();
			m_Blockchanged.fuelrate = msgValue["fuelrate"].asInt();

			string strJson = m_Blockchanged.ToJson();
			CPostMsg postmsg(MSG_USER_UP_PROGRESS,0);
			postmsg.SetData(strJson.c_str());

			pApp->m_MsgQueue.push(postmsg);
			
			
		//	if ((tempTimemsg - RecivetxMsgTimeLast)>10 || RecivetxMsgTimeLast == 0)
			if(m_Blockchanged.high == m_Blockchanged.tips)
			{	
				/// 更新tipblock hash
				CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_BlLOCKTIP);
				string msg = msgValue.toStyledString();
				postblockmsg.SetData(msg);
				pApp->m_MsgQueue.push(postblockmsg);  

			
				/// 更新钱包地址
				CPostMsg postuimsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRESS);
				pApp->m_MsgQueue.push(postuimsg);

				// 更新赌约池
				CPostMsg postpoolmsg(MSG_USER_GET_UPDATABASE,WM_UP_BETPOOL);
				pApp->m_MsgQueue.push(postpoolmsg);

				// 更新红包池
				CPostMsg postredpoolmsg(MSG_USER_GET_UPDATABASE,WM_REDPACKETPOOL);
					pApp->m_MsgQueue.push(postredpoolmsg);
				/// 赌约界面地址列表
				CPostMsg postp2pmsg(MSG_USER_P2P_UI,WM_UP_ADDRESS);
				pApp->m_MsgQueue.push(postp2pmsg);

				/// 红包界面地址列表
				CPostMsg postredmsg(MSG_USER_REDPACKET_UI,WM_UP_ADDRESS);
				pApp->m_MsgQueue.push(postredmsg);

				RecivetxMsgTimeLast = tempTimemsg;
			}
			break;
		}
	case SERVER_NOTIYF_TYPE:
		{
			string msg = msgValue["msg"].asString();
			TRACE("MEST:%s\r\n",msg.c_str());
			if (!strcmp(msg.c_str(),"server closed"))
			{
				theApp.m_bServerState = false;
			}else if (!strcmp(msg.c_str(),"Lock"))
			{
				CPostMsg postmsg(MSG_USER_UP_PROGRESS,WM_LOCKSTATE);
				postmsg.SetStrType(msg);
				pApp->m_MsgQueue.push(postmsg);
				pApp->IsWalletLocked = TRUE;
				pApp->m_lockstate =2;

				//// 更新block
				CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_UPDATABlLOCK);
				pApp->m_MsgQueue.push(postblockmsg);  
			}else if (!strcmp(msg.c_str(),"UnLock"))
			{
				CPostMsg postmsg(MSG_USER_UP_PROGRESS,WM_LOCKSTATE);
				postmsg.SetStrType(msg);
				pApp->m_MsgQueue.push(postmsg);
				pApp->IsWalletLocked = FALSE;
				pApp->m_lockstate =1;

				//// 更新block
				CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_UPDATABlLOCK);
				pApp->m_MsgQueue.push(postblockmsg);  
			}else if (msg.find("connections")>=0)
			{
				CPostMsg postmsg(MSG_USER_UP_PROGRESS,WM_CONNECTNET);
				postmsg.SetStrType(msg);
				pApp->m_MsgQueue.push(postmsg);
			}
			//TRACE("type: %s   mag: %s\r\n" , postmsg.GetStrType() ,msg);
			break;
		}
	default:
		break;
	}
	return true;
}

UINT __stdcall CDacrsUIApp::ProcessNoUiMsg(LPVOID pParam)
{
	CDacrsUIApp * pUiDemeDlg  = (CDacrsUIApp*)pParam;
	if ( NULL != pUiDemeDlg ) {
		while (true)
		{
		    Sleep(100); 
			if (theApp.m_blockAutoDelete && !pUiDemeDlg->m_noUiMsgBuffer.HaveNoUiMsg())
			{
				return 1;
			}
			while(pUiDemeDlg->m_noUiMsgBuffer.HaveNoUiMsg()) {
				string strMsg;
				pUiDemeDlg->m_noUiMsgBuffer.GetNoUiMsg(strMsg);
			//	TRACE("recv msg:%s\n", strMsg.GetString());
				Json::Reader reader;  
				Json::Value jsonValue; 
				if (!JsonCheck(strMsg))
				{
					TRACE("JsonCheck noui msg error,msg content:%s\n", strMsg.c_str());
					continue;
				}
				if (strMsg == _T(""))
				{
					continue;
				}
				if (!reader.parse(strMsg, jsonValue)) 
					continue;
				ProcessMsgJson(jsonValue, pUiDemeDlg);
			}
		}
	}
	return 1;
}

UINT __stdcall CDacrsUIApp::blockProc(LPVOID pParam)
{
	bool bReConnect(false);
	int nMaxbufferLen(10*1024-1);
	CDacrsUIApp * pUiDemeDlg  = (CDacrsUIApp*)pParam ;
	if ( NULL != pUiDemeDlg ) {
		while (true)
		{
			Sleep(100);
			if (theApp.m_blockAutoDelete)
			{
				return 1;
			}
			if(bReConnect) {

				CSynchronousSocket te;
				SOCKET nSocket = te.OnblockConnnect((char*)pUiDemeDlg->m_severip.c_str(),atoi(pUiDemeDlg->m_uirpcport.c_str()));
				if( INVALID_SOCKET != nSocket) {
					pUiDemeDlg->m_blockSock = nSocket;
					bReConnect = false;
					TRACE("retry nSocket OK\n");
				}
				else
				{
				TRACE("retry nSocket Error\n");
				}
			}
			else {
				if ( INVALID_SOCKET != pUiDemeDlg->m_blockSock ) {
					char  cRecvbuffer[10*1024];
					int nRecLen = recv( pUiDemeDlg->m_blockSock , cRecvbuffer , nMaxbufferLen , 0);

					if ( nRecLen > 0 ) {
						/*TRACE("recv len %d:",nRecLen);
						for(int i=0; i< nRecLen; ++i) {
							TRACE("%02X",cRecvbuffer[i]);
						}
						TRACE("\n");*/
						if(!pUiDemeDlg->m_noUiMsgBuffer.AddBytesToBuffer(cRecvbuffer, nRecLen)) {
							if (INVALID_SOCKET != pUiDemeDlg->m_blockSock)
							{
								closesocket(pUiDemeDlg->m_blockSock);
								pUiDemeDlg->m_blockSock = INVALID_SOCKET;
							}
							pUiDemeDlg->m_noUiMsgBuffer.ClearBuffer();
							bReConnect = true;
						}
					}
					else if(nRecLen == 0) 
					{
						TRACE0("noui socket has been closed\n");
						if (INVALID_SOCKET != pUiDemeDlg->m_blockSock)
						{
							closesocket(pUiDemeDlg->m_blockSock);
							pUiDemeDlg->m_blockSock = INVALID_SOCKET;
						}
						bReConnect = true;
					}
					else if(nRecLen < 0) 
					{
						 TRACE1("recv failed: %d\n", WSAGetLastError());
						 if (INVALID_SOCKET != pUiDemeDlg->m_blockSock)
						 {
							 closesocket(pUiDemeDlg->m_blockSock);
							 pUiDemeDlg->m_blockSock = INVALID_SOCKET;
						 }
						 bReConnect = true;
					}
				}
				else
				{
						bReConnect = true;
				}
			}
		}
	}
	return 1 ;
}

void CDacrsUIApp::RegThrdToMtThrd( HANDLE hThread , unsigned int hThrdId )
{
	if( NULL == hThrdId || NULL == hThread ) {
		return ;
	}
	sThrd sthrd ;
	memset(  &sthrd , 0 , sizeof( sThrd ) ) ;
	sthrd.hThrd = hThread ;
	sthrd.hThrdId = hThrdId ;

	UnRegThrdToMtThrd( sthrd.hThrd , sthrd.hThrdId ) ;
	v_ProcSubThrd.push_back( sthrd ) ;
}
void CDacrsUIApp::UnRegThrdToMtThrd( HANDLE hThread , unsigned int hThrdId ) 
{
	if( NULL == hThrdId || NULL == hThread ) {
		return ;
	}
	std::vector< sThrd >::iterator it ;
	for( it = v_ProcSubThrd.begin() ; it != v_ProcSubThrd.end() ; it++ ) {
		if( hThrdId == it->hThrdId || 
			hThread == it->hThrd ) {
				TRACE("The thread id=%d UnRegThrdToMtThrd\n" , it->hThrdId ) ;
				v_ProcSubThrd.erase( it );
				break ;
		}
	}
}
void CDacrsUIApp::SubscribeMsg( DWORD nThreadId , HWND hWnd, UINT msg , void * relMsgMem )
{
	if( NULL == hWnd ) {
		return ;
	}
	UnSubscribeMsg( nThreadId , hWnd , msg ) ;
	//
	MSG_NOTIFY msgNotify ;
	memset(  &msgNotify , 0 , sizeof( MSG_NOTIFY ) ) ;
	msgNotify.nThreadId = nThreadId ;
	msgNotify.hSubWnd   = hWnd		;
	msgNotify.msg		= msg		;
	msgNotify.relMsgMem	= relMsgMem	;

	theApp.cs_MsgDisptch.Lock();
	sSubscribeList.push_back( msgNotify ) ;
	theApp.cs_MsgDisptch.Unlock();
}
void CDacrsUIApp::UnSubscribeMsg( DWORD nThreadId , HWND hWnd, UINT msg )
{
	if( NULL == hWnd ) {
		return ;
	}
	std::vector<MSG_NOTIFY>::iterator it ;
	theApp.cs_MsgDisptch.Lock();
	for( it = sSubscribeList.begin() ; it != sSubscribeList.end() ; it++ ) {
		if( nThreadId == it->nThreadId && \
			hWnd == it->hSubWnd && \
			msg == it->msg ) {
				sSubscribeList.erase( it );
				break ;
		}
	}
	theApp.cs_MsgDisptch.Unlock();
}
void CDacrsUIApp::DispatchMsg( unsigned int threadID , UINT msg , WPARAM wParam , LPARAM lParam ) 
{
	BOOL flag = FALSE ;
	std::vector<MSG_NOTIFY>::iterator it ;

	cs_MsgDisptch.Lock();
	for( it = sSubscribeList.begin() ; it != sSubscribeList.end() ; it++ ) {
		if( threadID == it->nThreadId && msg == it->msg ) {
			if( NULL != it->relMsgMem ) {
				flag = TRUE ;
				::PostMessage( it->hSubWnd , msg , wParam , (LPARAM)it->relMsgMem ) ;
				//::SendMessage( it->hSubWnd , msg , wParam , (LPARAM)it->relMsgMem ) ;
			} else {
				::PostMessage( it->hSubWnd , msg , wParam , lParam ) ;
				//::SendMessage( it->hSubWnd , msg , wParam , lParam ) ;
			}
		}
	}
	cs_MsgDisptch.Unlock();

	if( flag ) {
		UiFun::ReleaseP( (void*)lParam );
	}
}
void CDacrsUIApp::OnInitList()
{
	//m_listAddr.clear() ;
}

int CDacrsUIApp::SendPostThread(DWORD msgtype)
{
	CDacrsUIDlg *pDlg = (CDacrsUIDlg*)(((CDacrsUIApp*)this)->m_pMainWnd) ;
	if (pDlg == NULL)
	{
		return 0 ;
	}

	switch (msgtype)
	{
	case WM_REVTRANSACTION:
		{
			GetMainDlgStruct();
			//DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI , 0,0);
		}
		break;
	case WM_UPWALLET:
		{
			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI ,WM_UPWALLET,0);
		}
		break;
	default:
		return 0;
	}
	return 1 ;
}
void  CDacrsUIApp::ParseUIConfigFile(const string& strExeDir){
	string configpath = "";
	configpath += strprintf("%s",strExeDir);
	configpath+= strprintf("\\%s","dacrsclient.conf");

	if (PathFileExistsA(configpath.c_str()))
	{
		CJsonConfigHelp::getInstance()->Init();
		CJsonConfigHelp::getInstance()->ReadJsonConfig(configpath);
		CScriptCfg scriptCfg;
		CJsonConfigHelp::getInstance()->GetScriptCfgData(scriptCfg);
		m_betScritptid = scriptCfg.strScriptBetid;
		m_darkScritptid= scriptCfg.strSrcriptDarkid;
		m_ipoScritptid = scriptCfg.m_ipoScritptid;
		m_redPacketScriptid = scriptCfg.m_redpacketScriptid;
		CJsonConfigHelp::getInstance()->GetDarkCfgData(m_DarkCfg);
		CJsonConfigHelp::getInstance()->GetP2PBetCfgData(m_P2PBetCfg);
		CJsonConfigHelp::getInstance()->GetRedPacketCfgData(m_RedPacketCfg);
		CJsonConfigHelp::getInstance()->GetClosConfig(m_reminder);
		CJsonConfigHelp::getInstance()->GetPopTipsConfig(m_poptips);
		CJsonConfigHelp::getInstance()->GetNewestScriptData(m_neststcriptid);
		CJsonConfigHelp::getInstance()->GetP2pBetStep(m_p2pbetstep);
		CJsonConfigHelp::getInstance()->GetRedPacketStep(m_redPackestep);
		CJsonConfigHelp::getInstance()->GetListAppConfig(m_listapp);
		CJsonConfigHelp::getInstance()->GetHelpUrl(helpurlen,helpurlcn);
		CNetParamCfg netParm;
		CJsonConfigHelp::getInstance()->GetNetParamCfgData(netParm);
		CJsonConfigHelp::getInstance()->GetInsertAppId(m_listappid);

		m_severip = netParm.server_ip;
		m_uirpcport = netParm.server_ui_port;
		m_rpcport = netParm.rpc_port;
		m_rpcUser = netParm.rpc_user;
		m_rpcPassWord = netParm.rpc_password;
	}
}
void CDacrsUIApp::StartSeverProcess(const string& strdir){
	///// 启动前先关闭系统中dacrs-d.exe进程
	CloseProcess("dacrs-d.exe");
	LogPrint("INFO", "关闭服务端成功\n");
	
	STARTUPINFOA si; 
	memset(&si, 0, sizeof(STARTUPINFO));  
	si.cb = sizeof(STARTUPINFO);  
	si.dwFlags = STARTF_USESHOWWINDOW;  
	si.wShowWindow =SW_HIDE;//SW_HIDE; //SW_SHOW;  

	string str = "dacrs-d.exe -datadir=";
	str +=strprintf("%s",strdir);
	str+=strprintf(" %s",_T("-ui=1"));
	if(m_bReIndexServer)
		str+=" -reindex=1";
	//if (!m_bStartServer)
	//{
	//	return ;
	//}

	if(!CreateProcessA(NULL,(LPSTR)str.c_str(),NULL,NULL,FALSE,0,NULL,NULL,&si,&sever_pi))   
	{  
		int n = GetLastError();
		AfxMessageBox(_T("CreateProcessA sever error!"));
		LogPrint("INFO", "开启服务端程序失败\n");
		exit(1);  
	}  
	CloseHandle(sever_pi.hProcess);
	CloseHandle(sever_pi.hThread);
	LogPrint("INFO", "开启服务端程序成功\n");
}
bool CDacrsUIApp::IsExistProcess(const string& exename){
	HANDLE SnapShot,ProcessHandle;  
	SHFILEINFO shSmall;  
	PROCESSENTRY32 ProcessInfo;   
	int count  = 0;
	while(1)  
	{  
		SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
		if(SnapShot!=NULL)   
		{  
			ProcessInfo.dwSize=sizeof(ProcessInfo);
			BOOL Status=Process32First(SnapShot,&ProcessInfo);  
			while(Status)  
			{    
				SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,  
					sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);
				USES_CONVERSION;
				CString str(ProcessInfo.szExeFile);
				char* pData = str.GetBuffer(str.GetLength());
				str.ReleaseBuffer();
				strlwr(pData);
				for(int i=0;i<3;i++)  
					if(!strcmp(strlwr((LPSTR)exename.c_str()),str))   
					{  
						return true;  
					}  
					Status=Process32Next(SnapShot,&ProcessInfo);  
			}  
			break;
		}  

		Sleep(1000);  
	} 
	return false;  
}
void CDacrsUIApp::CloseProcess(const string& exename){
	HANDLE SnapShot,ProcessHandle;  
	SHFILEINFO shSmall;  
	PROCESSENTRY32 ProcessInfo;   
	int count  = 0;
	while(1)  
	{  
		SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
		if(SnapShot!=NULL)   
		{  
			ProcessInfo.dwSize=sizeof(ProcessInfo);
			BOOL Status=Process32First(SnapShot,&ProcessInfo);  
			while(Status)  
			{    
				SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,  
					sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);
				USES_CONVERSION;
				CString str(ProcessInfo.szExeFile);
				char* pData = str.GetBuffer(str.GetLength());
				str.ReleaseBuffer();
				strlwr(pData);
				for(int i=0;i<3;i++)  
					if(!strcmp(strlwr((LPSTR)exename.c_str()),str))   
					{  

						ProcessHandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,ProcessInfo.th32ProcessID);  
						TerminateProcess(ProcessHandle,0);  
						return;  
					}  
					Status=Process32Next(SnapShot,&ProcessInfo);  
			}  
			break;
		}  

		Sleep(1000);  
	}  
}


CString GetAppPath()
{
	static CString __apppath;
	if(!__apppath.IsEmpty())return __apppath;
	char drive[_MAX_DRIVE],dir[_MAX_DIR],fname[_MAX_FNAME],ext[_MAX_EXT];
	_splitpath_s(theApp.m_pszHelpFilePath,drive,dir,fname,ext);
	CString csDir;
	csDir =  CString(drive);
	csDir += CString(dir);
	__apppath=csDir;
	return csDir;
}

int CDacrsUIApp::Update()
{
	CString sPath;
	sPath.Format(_T("%s\\update.exe"),str_InsPath.c_str());
	LogPrint("INFO","Updata:%s\n",sPath);
	sPath.Replace("\\\\","\\");
	LogPrint("INFO","Updata:%s\n",sPath);
	CFileFind find;
	if(!find.FindFile(sPath)) {
		return false;
	}
	LogPrint("INFO","Updata start:%s\n",sPath);
	SHELLEXECUTEINFO ShRun = {0}; 
	ShRun.cbSize = sizeof(SHELLEXECUTEINFO); 
	ShRun.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ShRun.hwnd = NULL; 
	ShRun.lpVerb = NULL; 
	ShRun.lpFile = sPath;
	ShRun.lpParameters = _T("-checkforupdates"); 
	ShRun.nShow = SW_HIDE; 
	ShRun.hInstApp = NULL; 
	ShellExecuteEx(&ShRun); 
	WaitForSingleObject(ShRun.hProcess, 120000); 
	ULONG lResult = 0; 
	LogPrint("INFO","Updata OpenProcess:%0x\n",ShRun.hProcess);
	if (!GetExitCodeProcess(ShRun.hProcess, &lResult)) {
		CloseHandle(ShRun.hProcess);
		return false;
	}
	LogPrint("INFO","Updata GetExitCodeProcess:%s\n",sPath);
	if (lResult == 0 || lResult == 0xFFFFFFFF) {
		CloseHandle(ShRun.hProcess);
		return false;
	}
	LogPrint("INFO","Updata lResult:%s\n",sPath);
	CString sMsg;
	if (theApp.language() == 1)
	{
		sMsg.Format(_T("检查有%d文件需要更新,现在是否要更新?"),lResult);
	}else{
		sMsg.Format(_T("%d updates found at the website,now to upgrade?"),lResult);

	}

	/*if ( IDYES != UiFun::MessageBoxEx(sMsg, UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_YESNO|MFB_TIP ) ) {
	return 0;
	}*/

	CloseProcess("dacrs-d.exe");
	DWORD exc;

	while (::GetExitCodeThread(theApp.m_msgThread, &exc)) {
		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	
	ShRun.lpParameters = NULL; 
	ShRun.nShow = SW_SHOW;
	ShellExecuteEx(&ShRun); 
	CloseHandle(ShRun.hProcess);

	CloseProcess("DacrsUI.exe");

	return true;
}


void CDacrsUIApp::GetMainDlgStruct()
{
	uistruct::MINDLG_T maindlg;
	string strCommand,strShowData;
	strCommand = "0";
//	theApp.cs_SqlData.Lock();
	double nmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_wallet_address") ,_T("money"), _T(" 1=1 "));
//	theApp.cs_SqlData.Unlock();
	if (nmoney < 0)
	{
		maindlg.money = "0.0";
	}else{
		string strmoney;
		strmoney = strprintf("%.3lf",nmoney);
		maindlg.money = strmoney;
	}


	//在这里进行判断是接收确认还是发送确认
	string strCond;

	strCond = " confirm_height = 0 and state = 2";
	double nmoneyRecv =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_transaction") , _T("money") , strCond) ;
	if (nmoneyRecv <0)
	{
		//maindlg.unconfirmrecvmoney = _T("0.0");
		nmoneyRecv = 0.0;
	}

	strCond = " confirm_height = 0 and state = 1";					//加入状态判断
	double nmoneySend =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_transaction") , _T("money") , strCond) ;

	if (nmoney <0)
	{
		nmoneySend = 0.0;
	}

	CString strmoney;
	strmoney.Format(_T("%.3lf"),nmoneyRecv - nmoneySend);
	maindlg.unconfirmmoney = strmoney;

	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction"), _T(" 1=1 "));

	strCommand = strprintf("%d",nItem);
	maindlg.itemcount = strCommand;

	uistruct::TRANSRECORDLIST pTransaction;
	strCond = " tx_type='COMMON_TX' order by confirmed_time desc limit 5";
	theApp.m_SqliteDeal.GetTransactionList(strCond, &pTransaction);


	int i = 1;
	if (pTransaction.size() != 0  ) {
		std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
		for (const_it = pTransaction.begin(); const_it != pTransaction.end()&&i<6 ; const_it++ ) {
			uistruct::REVTRANSACTION_t temp = *const_it;
			if(i == 1)
			maindlg.addr1 = temp.ToJson();
			if(i == 2)
				maindlg.addr2 = temp.ToJson();
			if(i == 3)
				maindlg.addr3 = temp.ToJson();
			if(i ==4)
				maindlg.addr4 = temp.ToJson();
			if(i == 5)
				maindlg.addr5 = temp.ToJson();

			i++;
		}
	}

	//m_UiManDlgQueue.clear();
	CPostMsg Postmsg(MSG_USER_MAIN_UI,0);
	string msg =maindlg.ToJson();
	Postmsg.SetData(msg.c_str());	
	m_UiManDlgQueue.push(Postmsg);

	CPostMsg msg1(MSG_USER_MAIN_UI,0);
	m_MsgQueue.push(msg1);
}

BOOL CDacrsUIApp::RunOnlyOneApp()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, _T("DacrsUI"));
	// 检查错误代码
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		CloseHandle(hMutex);
		AfxMessageBox(UiFun::UI_LoadString("DACRSU" , "DACRSU_PROJECTRUN" ,theApp.gsLanguage) ,MB_OK);
		exit(1);  

		return FALSE;
	}

	return TRUE;
}
//// 通知发送界面或者接受界面地址的内容改变了获取要插入新地址
void CDacrsUIApp::SendUIMsg(int message,string jsonaddr){

	CPostMsg Postmsg(MSG_USER_MAIN_UI,message);
	Postmsg.SetData(jsonaddr);	
	m_UiReciveDlgQueue.push(Postmsg);

	CPostMsg msg(MSG_USER_RECIVE_UI,message);
	m_MsgQueue.push(msg);


	m_UiSendDlgQueue.push(Postmsg);
	CPostMsg msg1(MSG_USER_SEND_UI,message);
	m_MsgQueue.push(msg1);

}

void CDacrsUIApp::CheckPathValid(const string& strDir)
{
	BOOL bExist = FALSE;
	for(int i = 0;i < (int)strDir.length();i++)
	{
		BYTE bchar = (BYTE)strDir.at(i);
		if(bchar == ' ')
		{
			bExist = TRUE;
			break;
		}
	}

	if (bExist)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("DACRSU" , "DACRSU_FILEPATH" ,theApp.gsLanguage)+"\r\n" , _T("Error") ,MFB_OK|MFB_ERROR );
		exit(0);
	}
}
bool CDacrsUIApp::IsLockWallet(){
	
	if (m_passlock)
	{
		while(IsWalletLocked)
		{
			CWalletPassPhrase walletpassdlg;
			if (walletpassdlg.DoModal() == IDCANCEL)
			{
				break;
			}
			Sleep(100);
		}
	}

	return IsWalletLocked;
}
void CDacrsUIApp::SetUpdataLanguage()
{
	string strAppIni = theApp.str_InsPath;
	strAppIni += "\\qupdater.ini";
	if (language() == 2)
	{
		::WritePrivateProfileString("QUpdater","Langage","QUpdateren.lan",(LPCTSTR)strAppIni.c_str());
	}else{
		::WritePrivateProfileString("QUpdater","Langage","QUpdatercn.lan",(LPCTSTR)strAppIni.c_str());
	}
}

bool CDacrsUIApp::Verify(char *publickey,char *publicmod,char *output, unsigned int *outputlen, char *input, unsigned int inputlen,char *vermessage)
{
	RSADecrypt(publickey,publicmod,output,outputlen,input,inputlen);
	if (memcmp(output,vermessage,strlen(vermessage)+1)==0)
	{
		return true;
	}
	return false;
}
int CDacrsUIApp::InitEncrypt()
{
	char szText[400] ={0};

	string strAppIni = str_InsPath;// + (CString)LANGUAGE_FILE;
	strAppIni += "\\encrypt.ini";
	::GetPrivateProfileString("sigmessage", "sigclienconf", "NOT FOUNT", szText, 400,	(LPCTSTR)strAppIni.c_str());
	m_sigdacrsclient =strprintf("%s",szText);

	memset(szText,0,400);
	::GetPrivateProfileString("sigmessage", "sigdacrsui", "NOT FOUNT", szText, 400,	(LPCTSTR)strAppIni.c_str());
	m_singdacrsui =strprintf("%s",szText);

	memset(szText,0,400);
	::GetPrivateProfileString("sigmessage", "sigdacrs", "NOT FOUNT", szText, 400,	(LPCTSTR)strAppIni.c_str());
	m_singdacrs =strprintf("%s",szText);

	memset(szText,0,400);
	::GetPrivateProfileString("sigmessage", "sigrunbat", "NOT FOUNT", szText, 400,	(LPCTSTR)strAppIni.c_str());
	m_singRunBat =strprintf("%s",szText);

	memset(szText,0,400);
	::GetPrivateProfileString("sigmessage", "index", "NOT FOUNT", szText, 400,	(LPCTSTR)strAppIni.c_str());

	return atoi(szText);
}
bool CDacrsUIApp::CheckUpdatafile()
{
	int index =InitEncrypt();
	if (index != 1)
	{
		return true;
	}
	string clienconf =theApp.str_InsPath+"\\temp\\"+"dacrsclient.conf";
	string dacrsui =theApp.str_InsPath+"\\temp\\"+"DacrsUI.exe";
	string dacrssever =theApp.str_InsPath+"\\temp\\"+"dacrs-d.exe";
	string batfile =theApp.str_InsPath+"\\"+"run.bat";
	if (_access(clienconf.c_str(),0) == -1||_access(dacrsui.c_str(),0) == -1
       ||_access(dacrssever.c_str(),0) == -1||_access(batfile.c_str(),0) == -1)
	{
		return false;
	}
	
	MD5 md5;
	md5.update(ifstream(clienconf));
	string strmd5 =md5.toString();

	char *pubmod ="6EC64CD17DD5206F3B224BC4E3CE455A81F7DC4F59E1AFF04D0CF5C7AB7313F5C133F020BF2D2D6B2D1306A9A87E0BFE04324179088B3F6AC5BF9C802D8CF9F1";
	char *pritekey ="4B54B325EF183B2B9FB7D1AB7D55CE6E7FD849B35AD1BD53ACC394638D4F4517904A46A511C7B2A6CAB4162F525557230573D43B5595D530FDFAC00A8751EBD";
	char decrypt_text[300]; // 输出的解密文   
	unsigned int decrypt_len;  

	string Errorfile = "";
	bool flagconf=Verify(pritekey,pubmod,decrypt_text,&decrypt_len,(char*)m_sigdacrsclient.c_str(),m_sigdacrsclient.length(),(char*)strmd5.c_str());
	
	if (!flagconf)
	{
		Errorfile = strprintf("Verify file:%s error\r\n",clienconf);
	}
	md5.reset();
	md5.update(ifstream(dacrsui));
	strmd5 =md5.toString();

	bool flagui=Verify(pritekey,pubmod,decrypt_text,&decrypt_len,(char*)m_singdacrsui.c_str(),m_singdacrsui.length(),(char*)strmd5.c_str());
	
	if (!flagui)
	{
		Errorfile += strprintf("Verify file:%s error\r\n",dacrsui);
	}

	md5.reset();
	md5.update(ifstream(dacrssever));
	strmd5 =md5.toString();

	bool flagsever=Verify(pritekey,pubmod,decrypt_text,&decrypt_len,(char*)m_singdacrs.c_str(),m_singdacrs.length(),(char*)strmd5.c_str());
	
	if (!flagsever)
	{
		Errorfile += strprintf("Verify file:%s error\r\n",dacrssever);
	}
	md5.reset();
	md5.update(ifstream(batfile));
	strmd5 =md5.toString();
	bool flagbat=Verify(pritekey,pubmod,decrypt_text,&decrypt_len,(char*)m_singRunBat.c_str(),m_singRunBat.length(),(char*)strmd5.c_str());

	if (!flagbat)
	{
		Errorfile += strprintf("Verify file:%s error\r\n",batfile);
	}
	if (!flagconf || !flagsever || !flagui || !flagbat)
	{
		AfxMessageBox(Errorfile.c_str());
		return false;
	}else{
		string desfile =theApp.str_InsPath+"\\"+"dacrsclient.conf";
		::CopyFile(clienconf.c_str(),desfile.c_str(),false);
		desfile =theApp.str_InsPath+"\\"+"dacrs-d.exe";
		CloseProcess("dacrs-d.exe");
		::CopyFile(dacrssever.c_str(),desfile.c_str(),false);
		desfile =theApp.str_InsPath+"\\"+"DacrsUI.exe";
		::CopyFile(dacrsui.c_str(),desfile.c_str(),false);
		//CloseProcess("DacrsUI.exe");
		string batpath =strprintf("%s\\run.bat",str_InsPath);
		::CopyFile(dacrsui.c_str(),desfile.c_str(),false);
		AfxMessageBox(_T("钱包已成功更新到最新版本,请重启!"),MB_ICONASTERISK|MB_OK);
		ShellExecute(NULL, "open",batpath.c_str(), NULL, NULL, SW_SHOW);
		//exit(0);
	}
	if (index == 1)
	{
		char szText[400] ={0};

		string strAppIni = str_InsPath;// + (CString)LANGUAGE_FILE;
		strAppIni += "\\encrypt.ini";
		memset(szText,0,400);
		::GetPrivateProfileString("sigmessage", "index", "NOT FOUNT", szText, 400,	(LPCTSTR)strAppIni.c_str());

		int index =atoi(szText);
		string writestr=strprintf("%d",index+1);
		::WritePrivateProfileString("sigmessage","index",writestr.c_str(),(LPCTSTR)strAppIni.c_str());
	}
	return true;
}

int CDacrsUIApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	 CMFCVisualManager::DestroyInstance();
	return CWinApp::ExitInstance();
}
