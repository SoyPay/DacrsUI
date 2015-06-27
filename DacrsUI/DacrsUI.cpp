
// DacrsUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "SynchronousSocket.h"
#include <afxinet.h>
#include "Language.h"
#include "StartProgress.h"

#include <afxsock.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "GdiPlusInit.h"
#include "StartProgress.h"
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

	if (!RunOnlyOneApp())
	{
		return FALSE;
	}

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
	GetMoFilename( str_InsPath , str_ModuleFilename ); //获取文件路径和文件名称

	CheckPathValid( str_InsPath );
	////创建维护线程
	if( !CreateMaintainThrd() ) {
		return FALSE ;
	}

	/// 加载配置文件
	ParseUIConfigFile(str_InsPath);
	//初始化日志配置参数
	InitLogCfg();
	//检测自动升级
	int nResult = Update();
	if(-1 == nResult) {
		nResult = Update();
	}
	if (nResult == 1)
	{
		return TRUE;
	}else if(nResult == 0){
		//// 不更新直接退出
		exit(0); 
	}

	if(CSoyPayHelp::getInstance()->IsOSVersionBelowXp()) {
		if(!EnableDebugPrivilege())
			TRACE(_T("Call EnableDebugPrivilege failed!"));
		//				AfxMessageBox(_T("Call EnableDebugPrivilege failed!"));
		//// 不更新直接退出
		exit(0); 
	}

	CheckUpdate();

	gsLanguage = language();


	//打开sqlite3数据库
	m_SqliteDeal.InitializationDB();
	
	/// 清空交易记录
	ClearTransaction();

	CString temprpc = m_rpcport;
	CString tempuiport = m_uirpcport;
	ProductHttpHead(str_InsPath ,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr,m_uirpcport);

	if (strcmp(m_severip,_T("127.0.0.1")))
	{
		m_rpcport = temprpc;
		m_uirpcport = tempuiport;
	}

	//启动服务程序
	StartSeverProcess(str_InsPath);
	m_bServerState = true;
//	Sleep(1000);

	//连接block
	//连接到服务器
	CSynchronousSocket te;
	SOCKET nSocket = te.OnblockConnnect(m_severip,atoi(m_uirpcport) ) ;
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
	m_ProgressGifFile =   str_InsPath + _T("\\gif\\progress.gif\0") ;
	m_ProgressOutGifFile =   str_InsPath + _T("\\gif\\exit.gif\0") ;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Status state = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) ;
	if( Ok != state ) {
		switch( state )
		{
		case OutOfMemory :
			{
				::MessageBox( NULL , "OutOfMemory" , "Error" , MB_ICONERROR) ;
			}
			break;
		}
		return FALSE ;
	}
	
	//CStartProgress  progdlg ;
	//progdlg.DoModal();
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
			if(m_bReIndexServer) {
				StartSeverProcess(str_InsPath);
				m_bReIndexServer = FALSE;
				continue;
			}
			int errorCode = GetLastError();
			TRACE("Error OpenProcess:%d " , errorCode );
			::MessageBox( NULL , _T("加载钱包失败\r\n") , "Error" , MB_ICONERROR) ;
			//AfxMessageBox(_T(errorCode));
			exit(1);
		}
		CloseHandle(processHandle);
		//TRACE("detect count:%d\n", ++nCount);
		//pSplashThread->SetDlgPos(progessPos);
		//TRACE("index:%d\r\n",progessPos);
		if (isStartMainDlg)
		{
			break;
		}
		Sleep(100);

	/*	SYSTEMTIME CurTimeLast ;
		memset( &CurTimeLast , 0 , sizeof(SYSTEMTIME) ) ;
		GetLocalTime( &CurTimeLast ) ;
		if ((CurTimeLast.wMinute - WaitTimeLast.wMinute) > 2)
		{
			::MessageBox( NULL , _T("加载钱包失败\r\n") , "Error" , MB_ICONERROR) ;
			exit(0);
		}*/
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

void CDacrsUIApp::GetMoFilename( CString & path , CString & filename ) 
{
	char strPath[MAX_PATH] ;
	GetModuleFileName( AfxGetApp()->m_hInstance , strPath , MAX_PATH );
	CString str=strPath;
	int i=str.ReverseFind('\\');
	path		= str.Left(i);
	filename	= strPath;	
}
int CDacrsUIApp::language()
{
	// 取默认语言索引
	char szText[256];
	CString strG;
	CString strAppIni = str_InsPath + (CString)LANGUAGE_FILE;
	::GetPrivateProfileString("Default", "Index", "1", szText, 256,	(LPCTSTR)strAppIni);

	strG.Format("Language%d", atoi(szText));
	::GetPrivateProfileString(strG, "gsLanguage", "1", szText, 256, (LPCTSTR)strAppIni);
	return atoi(szText);
}
BOOL CDacrsUIApp::CreateMaintainThrd() 
{
	hMtThrd		= NULL ;
	nMtThrdID	= 0  ;
	hMtStartEvent = ::CreateEvent(0,FALSE,FALSE,0) ; 

	hMtThrd = (HANDLE)_beginthreadex( NULL, 0, &MtProc, this, 0, &nMtThrdID ) ;
	if( INVALID_HANDLE_VALUE == hMtThrd ) {
		::MessageBox( NULL , "维护线程创建失败!" , "出错" , MB_ICONERROR ) ;
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

void CDacrsUIApp::StartblockThrd()
{
	m_hblockThread = (HANDLE)_beginthreadex(NULL,	0,	blockProc, this, 0, &ublockThreadId);	
	m_msgThread = (HANDLE)_beginthreadex(NULL,	0,	ProcessMsg, this, 0, &umsgThreadId);
	m_hProcessNoUiMsgThread = (HANDLE)_beginthreadex(NULL,	0,	ProcessNoUiMsg, this, 0, &m_uProNoUiMsgThreadId);
//	m_hProcessAppTxThread = (HANDLE)_beginthreadex(NULL,	0,	ProcessAppTx, this, 0, &m_uProAppTxThreadId);	
	return ;
}

UINT __stdcall CDacrsUIApp::ProcessAppTx(LPVOID pParam)
{
	CDacrsUIApp * pUiDemeDlg  = (CDacrsUIApp*)pParam ;
	CPostMsg Postmsg ;
	while ( true)
	{

		if (theApp.m_msgAutoDelete)
		{
			return 1;
		}
		/// 同步以后更新数据库表
		if (theApp.IsSyncBlock )
		{
			theApp.m_SqliteDeal.UpdataAllTableData();
			return 1;
		}
	}
	return 1 ;
}
UINT __stdcall CDacrsUIApp::ProcessMsg(LPVOID pParam) {
	CDacrsUIApp * pUiDemeDlg  = (CDacrsUIApp*)pParam ;
	CPostMsg Postmsg ;
	while ( true)
	{
		
		if (theApp.m_msgAutoDelete)
		{
			return 1;
		}

		if(!pUiDemeDlg->m_MsgQueue.pop(Postmsg)){
			Sleep(100); 
			continue;
		}
		CDacrsUIDlg *pDlg = (CDacrsUIDlg*)(((CDacrsUIApp*)pParam)->m_pMainWnd) ;
		if (pDlg == NULL && Postmsg.GetUItype() != MSG_USER_STARTPROCESS_UI&&Postmsg.GetDatatype() !=WM_SYNC_TRANSACTION){
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
				theApp.DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_STARTPROCESS_UI ,Postmsg.GetDatatype(),0);
				if (Postmsg.GetDatatype() == 4)
				{
					theApp.isStartMainDlg = true;
				}
				if(theApp.IsSyncTx){
					theApp.IsSyncTx = FALSE;
					theApp.m_SqliteDeal.CommitDbTransaction();
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
						CString txData = Postmsg.GetData();
						if ( _T("") != txData ) {
							((CDacrsUIApp*)pParam)->SyncTransaction(txData.GetString()) ;
						}
					}
					break;
				case WM_REVTRANSACTION:
					{
						//更新历史交易记录数据库
						CString pHash = Postmsg.GetData();
						if ( _T("") != pHash ) {
							CString strHash;
							strHash.Format("%s",pHash);
							strHash.TrimLeft("'");
							strHash.TrimRight("'");
							CString strCondition(_T(""));
							strCondition.Format(" hash = '%s' ", strHash);
							int nItem =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.GetTableCountItem(_T("t_transaction") ,strCondition);
							if (nItem == 0)
							{
								((CDacrsUIApp*)pParam)->InsertTransaction(strHash.GetString() ) ;
							}else{
								((CDacrsUIApp*)pParam)->UpdateTransaction(strHash.GetString() );
							}
						}
					}
					break;
				case WM_APP_TRANSATION:
					{
						CString txDetail = Postmsg.GetData();
						if ( _T("") != txDetail ) {
							((CDacrsUIApp*)pParam)->UpdateAppRecord(txDetail.GetString());
						}
					}
					break;
				case WM_P2P_BET_RECORD:
					{
						//更新数据库赌约数据库列表
						//((CDacrsUIApp*)pParam)->UpdateP2pBetRecord(); 
					}
					break;
				case WM_DARK_RECORD:
					{
						//更新数据库抵押数据库列表
						((CDacrsUIApp*)pParam)->m_SqliteDeal.UpdateDarkRecord(); 
					}
					break;
				case WM_UP_BETPOOL:
					{
						/// 赌约池数据库列表
						bool flag =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_quiz_pool"));
						if (flag ) 
						{
							((CDacrsUIApp*)pParam)->UpdateQuizPoolData();
						}
					}
					break;
				case WM_REDPACKETPOOL:
					{
						/// 红包池
						bool flag =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_red_packets_pool"));
						if (flag ) 
						{
							((CDacrsUIApp*)pParam)->UpdateRedPacketPoolData();
						}
					}
				case WM_UP_BlLOCKTIP:
					{
						//更新最新blocktip数据库
						if ( ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_chain_tip") ) ) {

							CString pHash = Postmsg.GetData();
							if ( _T("") != pHash ) {
								CString strinsert;
								strinsert.Format("'%s'",pHash);
								((CDacrsUIApp*)pParam)->m_SqliteDeal.InsertTableItem(_T("t_chain_tip") ,strinsert ) ;
							}
						}
					}
					break;
				case WM_UP_ADDRBOOK:
					{
						CString josnaddr = Postmsg.GetData();
						uistruct::ADDRBOOK_t addr;
						if (addr.JsonToStruct(josnaddr.GetString()))
						{
							CString strCond;
							strCond.Format(_T(" address='%s' "), addr.address);
							
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
						CString josnaddr = Postmsg.GetData();
						uistruct::ADDRBOOK_t addr;
						if (addr.JsonToStruct(josnaddr.GetString()))
						{
							CString strCond;
							strCond.Format(_T(" address='%s' "), addr.address);
							((CDacrsUIApp*)pParam)->m_SqliteDeal.DeleteTableItem(_T("t_address_book"),strCond);
						}
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
				TRACE("change:%s\r\n","MSG_USER_UP_PROGRESS");
				pUiDemeDlg->m_UimsgQueue.push(Postmsg);
				//theApp.DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_UP_PROGRESS , 0,0);					
			}
			break;
		case MSG_USER_INSERT_DATA:   //// 插入数据
			{
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
				uistruct::DATABASEINFO_t pDatabase;// = (uistruct::DATABASEINFO_t *)Postmsg.GetStrPoint();
				string strTemp = Postmsg.GetData();
				pDatabase.JsonToStruct(strTemp.c_str());

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
		case MSG_USER_UPDATA_UI:
			{
				theApp.UpdataUIData();
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

int GetMsgType(CString const strData,Json::Value &root)
{
	CString strType;
	int pos = strData.Find("type");
	if ( pos >=0 ){
		strType.Format(_T("%s") ,  root["type"].asCString() ) ;
		if ( !strcmp(strType ,_T("init") ) ) 
		{
			return ININTAL_TYPE;
		}
		if ( !strcmp(strType ,_T("revtransaction") ) )
		{
			return REV_TRANSATION_TYPE;
		}
		if ( !strcmp(strType ,_T("blockchanged") ) )
		{
			return BLOCK_CHANGE_TYPE;
		}
		if ( !strcmp(strType ,_T("rev_app_transaction") ) )
		{
			return APP_TRANSATION_TYPE;
		}
		if(!strcmp(strType, _T("notify"))) 
		{
			return SERVER_NOTIYF_TYPE;
		}
		if(!strcmp(strType, _T("SyncTx"))) 
		{
			return SERVER_SYNC_TX;
		}
	}
	return  -1;
}
bool JsonCheck(CString strjson){
	strjson.TrimLeft(" ");
	strjson.TrimRight(" ");
	if (strjson.GetAt(0) != '{')
	{
		return false;
	}
	int len = strjson.GetLength();
	if (strjson.GetAt(len-1) != '}')
	{
		return false;
	}
	return true;
}


bool ProcessMsgJson(Json::Value &msgValue, CDacrsUIApp* pApp) 
{
	string objstr = msgValue.toStyledString();
	int type = GetMsgType(objstr.c_str(),msgValue);

	switch(type)
	{
	case ININTAL_TYPE:
		{
//			CPostMsg postmsg(MSG_USER_SHOW_INIT,0);
//			postmsg.SetStrType(msgValue["type"].asCString());
			CString msg = msgValue["msg"].asCString();
			TRACE("MEST:%s\r\n",msg);
			if (!strcmp(msg,"Verifying blocks..."))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,1);
				pApp->m_MsgQueue.push(postmsg);
			}
			else if (!strcmp(msg,"Verifying Finished"))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,2);
				pApp->m_MsgQueue.push(postmsg);
			}
			else if (!strcmp(msg,"Loading addresses..."))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,3);
				pApp->m_MsgQueue.push(postmsg);
			}
			else if (!strcmp(msg,"initialize end"))
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
			CString txData ;
			txData.Format(_T("%s") , obj.toStyledString().c_str() );
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_SYNC_TRANSACTION);
			postmsg.SetData(txData);
			pApp->m_MsgQueue.push(postmsg);
			
		}
		break;
	case REV_TRANSATION_TYPE:
		{

			SYSTEMTIME curTime ;
			memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			GetLocalTime( &curTime ) ;
			static int RecivetxtxTimeLast =0;
			int tempTimemsg= UiFun::SystemTimeToTimet(curTime);
			/// 更新钱包
			CPostMsg postuimsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRESS);
			if ((tempTimemsg - RecivetxtxTimeLast)>10 || RecivetxtxTimeLast == 0)
			{	
				pApp->m_MsgQueue.push(postuimsg);
				RecivetxtxTimeLast = tempTimemsg;
			}

			const Json::Value& txArray = msgValue["transation"]; 
			//插入到数据库
			CString strHash ;
			strHash.Format(_T("'%s'") , txArray["hash"].asCString() );
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
	case BLOCK_CHANGE_TYPE:
		{
			SYSTEMTIME curTime ;
			memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			GetLocalTime( &curTime ) ;
			static int RecivetxMsgTimeLast =0;
			int tempTimemsg= UiFun::SystemTimeToTimet(curTime);
		
			TRACE("change:%s\r\n","blockchanged");
			uistruct::BLOCKCHANGED_t      m_Blockchanged ;
			m_Blockchanged.type = msgValue["type"].asString();
			m_Blockchanged.tips = msgValue["tips"].asInt();
			m_Blockchanged.high = msgValue["high"].asInt64() ;
			m_Blockchanged.hash = msgValue["hash"].asString();
			m_Blockchanged.connections = msgValue["connections"].asInt();

			string strJson = m_Blockchanged.ToJson();
			CPostMsg postmsg(MSG_USER_UP_PROGRESS,0);
			postmsg.SetData(strJson.c_str());

			pApp->m_MsgQueue.push(postmsg);
			
			
		//	if ((tempTimemsg - RecivetxMsgTimeLast)>10 || RecivetxMsgTimeLast == 0)
			if(m_Blockchanged.high == m_Blockchanged.tips)
			{	
				/// 更新tipblock hash
				CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_BlLOCKTIP);
				CString msg = msgValue["hash"].asCString();
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
			CPostMsg postmsg(MSG_USER_SHOW_INIT,0);
			postmsg.SetStrType(msgValue["type"].asCString());
			CString msg = msgValue["msg"].asCString();
			TRACE("MEST:%s\r\n",msg);
			if (!strcmp(msg,"server closed"))
			{
				theApp.m_bServerState = false;
			}
			postmsg.SetData(msg);
			pApp->m_MsgQueue.push(postmsg);
			TRACE("type: %s   mag: %s\r\n" , postmsg.GetStrType() ,msg);
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
			if (theApp.m_blockAutoDelete)
			{
				return 1;
			}
			while(pUiDemeDlg->m_noUiMsgBuffer.HaveNoUiMsg()) {
				CString strMsg;
				pUiDemeDlg->m_noUiMsgBuffer.GetNoUiMsg(strMsg);
			//	TRACE("recv msg:%s\n", strMsg.GetString());
				Json::Reader reader;  
				Json::Value jsonValue; 
				if (!JsonCheck(strMsg))
				{
					TRACE("JsonCheck noui msg error,msg content:%s\n", strMsg.GetString());
					continue;
				}
				if (!reader.parse(strMsg.GetString(), jsonValue)) 
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
	CString RevData ;
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
				SOCKET nSocket = te.OnblockConnnect(pUiDemeDlg->m_severip,atoi(pUiDemeDlg->m_uirpcport));
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
	//case WM_UP_ADDRESS:
	//	{
			/*if(pDlg->dlgType == CSendDlg::IDD)
				DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_SEND_UI , WM_UP_ADDRESS,0);*/
			/*if(pDlg->dlgType == CReceiveDlg::IDD)
				DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_RECIVE_UI , 0,0);*/

	//}
	//	break;
	case WM_REVTRANSACTION:
		{
			//SYSTEMTIME curTime ;
			//memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			//GetLocalTime( &curTime ) ;
			//static int UpdatManUiTimeLast =0;
			//int tempTimemsg= UiFun::SystemTimeToTimet(curTime);

			//if ((tempTimemsg - UpdatManUiTimeLast)>10|| UpdatManUiTimeLast == 0)
			//{	
			//	if(pDlg->dlgType == CMainDlg::IDD)
			//	{
					GetMainDlgStruct();
					DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI , 0,0);
				//}
				//UpdatManUiTimeLast = tempTimemsg;
			//}

		}
		break;
	case WM_UPWALLET:
		{
			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI ,WM_UPWALLET,0);
		}
		break;
	//case WM_P2P_BET_RECORD:
	//	{
	//		if(pDlg->dlgType == DIALOG_ACCEPTBET_RECORD)
	//			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_ACCEPTRECOD_UI,0,0);
	//		if(pDlg->dlgType == DIALOG_SEND_RECORD)
	//			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_SENDRECOD_UI,0,0);
	//	}
	//	break;
	//case WM_DARK_RECORD:
	//	{
	//		if(pDlg->dlgType == DIALOG_PRESS)
	//			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_DARK_UI,0,0);
	//	}
	//	break;
	//case WM_UP_BETPOOL:
	//	{
	//		if(pDlg->dlgType == DIALOG_P2P_BET)
	//			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_BETPOOL_UI,0,0);
	//	}
	//	break;
	default:
		return 0;
	}
	return 1 ;
}
void  CDacrsUIApp::ParseUIConfigFile(const CStringA& strExeDir){
	CStringA configpath = "";
	configpath.AppendFormat("%s",strExeDir);
	configpath.AppendFormat("\\%s","dacrsclient.conf");

	if (PathFileExistsA(configpath))
	{
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
		CNetParamCfg netParm;
		CJsonConfigHelp::getInstance()->GetNetParamCfgData(netParm);
		m_severip = netParm.server_ip;
		m_uirpcport = netParm.server_ui_port;
		m_rpcport = netParm.rpc_port;
		m_rpcUser = netParm.rpc_user;
		m_rpcPassWord = netParm.rpc_password;
	}
}
void CDacrsUIApp::StartSeverProcess(const CStringA& strdir){
	///// 启动前先关闭系统中dacrs-d.exe进程
	CloseProcess("dacrs-d.exe");
	LogPrint("INFO", "关闭服务端成功\n");
	
	STARTUPINFOA si; 
	memset(&si, 0, sizeof(STARTUPINFO));  
	si.cb = sizeof(STARTUPINFO);  
	si.dwFlags = STARTF_USESHOWWINDOW;  
	si.wShowWindow =SW_HIDE;//SW_HIDE; //SW_SHOW;  

	CString str = _T("dacrs-d.exe -datadir=");
	str.AppendFormat("%s",strdir);
	if(m_bReIndexServer)
		str.Append(_T(" -reindex=1"));
	//if (!m_bStartServer)
	//{
	//	return ;
	//}

	if(!CreateProcessA(NULL,(LPSTR)str.GetString(),NULL,NULL,FALSE,0,NULL,NULL,&si,&sever_pi))   
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
				char* pData = str.GetBuffer();
				strlwr(pData);  
				for(int i=0;i<3;i++)  
					if(!strcmp(strlwr((LPSTR)exename.c_str()),pData))   
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
	_splitpath(theApp.m_pszHelpFilePath,drive,dir,fname,ext);
	CString csDir;
	csDir =  CString(drive);
	csDir += CString(dir);
	__apppath=csDir;
	return csDir;
}
int CDacrsUIApp::Update()
{
	CString sPath=str_InsPath+"\\qupdater.exe";
	LogPrint("INFO","Updata:%s\r\n",sPath);
	sPath.Replace("\\\\","\\");
	LogPrint("INFO","Updata:%s\r\n",sPath);
	CFileFind find;
	if(!find.FindFile(sPath)) return false;
	LogPrint("INFO","Updata start:%s\r\n",sPath);
	SHELLEXECUTEINFO ShRun = {0}; 
	ShRun.cbSize = sizeof(SHELLEXECUTEINFO); 
	ShRun.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ShRun.hwnd = NULL; 
	ShRun.lpVerb = NULL; 
	ShRun.lpFile = sPath;
	ShRun.lpParameters = _T("-checkforupdates"); 
	ShRun.nShow = SW_SHOW; 
	ShRun.hInstApp = NULL; 
	ShellExecuteEx(&ShRun); 
	WaitForSingleObject(ShRun.hProcess, 120000); 
	ULONG lResult = 0; 
	LogPrint("INFO","Updata OpenProcess:%0x\r\n",ShRun.hProcess);
	if (!GetExitCodeProcess(ShRun.hProcess, &lResult)) return false;
	LogPrint("INFO","Updata GetExitCodeProcess:%s\r\n",sPath);
	if (lResult == 0) return -1;
	LogPrint("INFO","Updata lResult:%s\r\n",sPath);
	CString sMsg;
	//sMsg.Format(CLanguage::TranLanguage("MAIN","%d updates found at the website,now to upgrade?"),lResult);
	sMsg.Format(CLanguage::TranLanguage("MAIN","检查有%d文件需要更新,现在是否要更新?"),lResult);
	if (AfxMessageBox(sMsg, MB_ICONQUESTION | MB_YESNO) != IDYES) return 0;
	ShRun.lpParameters = NULL; 
	ShellExecuteEx(&ShRun); 
	return 1;
}
DWORD WINAPI Update1(LPVOID lpParam){
	CString sPath= GetAppPath()+"\\qupdater.exe";
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
	ShRun.nShow = SW_SHOW; 
	ShRun.hInstApp = NULL; 
	ShellExecuteEx(&ShRun); 
	WaitForSingleObject(ShRun.hProcess, 120000); 
	ULONG lResult = 0; 
	if (!GetExitCodeProcess(ShRun.hProcess, &lResult)) return false;
	if (lResult == 0) return false;
	CString sMsg;
	sMsg.Format(CLanguage::TranLanguage("MAIN","%d updates found at the website,now to upgrade?"),lResult);
	if (AfxMessageBox(sMsg, MB_ICONQUESTION | MB_YESNO) != IDYES) return false;
	ShRun.lpParameters = NULL; 
	ShellExecuteEx(&ShRun); 
	return true;
}
void  CDacrsUIApp::CheckUpdate(){
	CloseHandle(::CreateThread(NULL,0,Update1,this,0,NULL));

}
void CDacrsUIApp::UpdataUIData(){
	CDacrsUIDlg *pDlg = (CDacrsUIDlg*)(((CDacrsUIApp*)this)->m_pMainWnd) ;
	if (NULL != pDlg ) {
		int DType = pDlg->dlgType;
		switch(DType){
		//case DIALOG_P2P_BET:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_BETPOOL_UI,0,0);
		//	break;
		//case DIALOG_TRANSFER_RECORD:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_TRANSRECORD_UI,0,0);
		//	break;
		//case DIALOG_PRESS:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_DARK_UI,0,0);
		//	break;
		//case DIALOG_SEND_RECORD:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_SENDRECOD_UI,0,0);
		//	break;
		//case DIALOG_ACCEPTBET_RECORD:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_ACCEPTRECOD_UI,0,0);
		//	break;
		case CMainDlg::IDD:
			GetMainDlgStruct();
			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI , 0,0);
			break;
		//case DIALOG_SIGN_ACCOUNTS:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MYWALLET_UI , WM_UP_ADDRESS,0);
		//	break;
		//case CReceiveDlg::IDD:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_RECIVE_UI , 0,0);
		//	break;
		//case CSendDlg::IDD:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_SEND_UI , WM_UP_ADDRESS,0);
		//	break;
		}
	}
}

void CDacrsUIApp::GetMainDlgStruct()
{
	uistruct::MINDLG_T maindlg;
	CString strCommand,strShowData;
	strCommand.Format(_T("0"));
//	theApp.cs_SqlData.Lock();
	double nmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_wallet_address") ,_T("money"), _T(" 1=1 "));
//	theApp.cs_SqlData.Unlock();
	if (nmoney < 0)
	{
		maindlg.money = "0.0";
	}else{
		CString strmoney;
		strmoney.Format(_T("%.3lf"),nmoney);
		maindlg.money = strmoney;
	}

	CString strCond;
	strCond.Format(_T(" confirm_height = 0 "));

	nmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_transaction") , _T("money") , strCond) ;

	if (nmoney <0)
	{
		maindlg.unconfirmmoney = _T("0.0");
	}else{
		CString strmoney;
		strmoney.Format(_T("%.3lf"),nmoney);
		maindlg.unconfirmmoney = strmoney;
	}

	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction"), _T(" 1=1 "));

	strCommand.Format(_T("%d"),nItem);
	maindlg.itemcount = strCommand.GetString();

	uistruct::TRANSRECORDLIST pTransaction;
	strCond.Format(_T(" tx_type='COMMON_TX' order by confirmed_time desc limit 5"));
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

	m_UiManDlgQueue.clear();
	CPostMsg Postmsg(MSG_USER_MAIN_UI,0);
	string msg =maindlg.ToJson();
	Postmsg.SetData(msg.c_str());	
	m_UiManDlgQueue.push(Postmsg);
}

BOOL CDacrsUIApp::RunOnlyOneApp()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, _T("DacrsUI"));
	// 检查错误代码
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		CloseHandle(hMutex);
		AfxMessageBox(_T("程序已经在运行了") ,MB_OK);
		exit(1);  

		/*HWND hWndPrevious = ::GetWindow( ::GetDesktopWindow(), GW_CHILD );
		while(::IsWindow(hWndPrevious))
		{
			if (::GetProp(hWndPrevious, WINDOW_TAG))
			{
				if (::IsIconic(hWndPrevious))
				{
					::ShowWindow(hWndPrevious, SW_RESTORE);
					::SetForegroundWindow(hWndPrevious);
				}
				else
				{
					::SetForegroundWindow(::GetLastActivePopup(hWndPrevious));
				}
				return FALSE;
			}
			hWndPrevious = ::GetWindow( hWndPrevious, GW_HWNDNEXT );
		}*/

		return FALSE;
	}

	return TRUE;
}
//// 通知发送界面或者接受界面地址的内容改变了获取要插入新地址
void CDacrsUIApp::SendUIMsg(int message,CString jsonaddr){

	//m_UiReciveDlgQueue.clear();
	CPostMsg Postmsg(MSG_USER_MAIN_UI,message);
	Postmsg.SetData(jsonaddr);	
	m_UiReciveDlgQueue.push(Postmsg);
	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_RECIVE_UI ,message,0);

	m_UiSendDlgQueue.push(Postmsg);
	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_SEND_UI ,message,0);

}
void CDacrsUIApp::SendP2pMsg(int message,CString jsonaddr)
{
	CPostMsg Postmsg(MSG_USER_MAIN_UI,message);
	Postmsg.SetData(jsonaddr);
	m_UiP2pDlgQueue.push(Postmsg);
	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_P2P_UI ,message,0);

	m_UiRedPacketDlgQueue.push(Postmsg);
	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_REDPACKET_UI ,message,0);
}
void CDacrsUIApp::CheckPathValid(const CStringA& strDir)
{
	BOOL bExist = FALSE;
	for(int i = 0;i <= strDir.GetLength();i++)
	{
		BYTE bchar = (BYTE)strDir.GetAt(i);
		if(bchar == ' ')
		{
			bExist = TRUE;
			break;
		}
	}

	if (bExist)
	{
		::MessageBox( NULL , _T("程序不可以放在含有空格的目录下\r\n") , "Error" , MB_ICONERROR) ;
		exit(0);
	}
}