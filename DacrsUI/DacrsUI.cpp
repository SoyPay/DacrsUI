
// DacrsUI.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "SynchronousSocket.h"
#include <afxinet.h>
#include "Language.h"
#include "StartProgress.h"
#include "WalletPassPhrase.h"

#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>

#include <afxsock.h>
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


// CDacrsUIApp ����

CDacrsUIApp::CDacrsUIApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
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
}


// Ψһ��һ�� CDacrsUIApp ����

CDacrsUIApp theApp;

BOOL  EnableDebugPrivilege();
// CDacrsUIApp ��ʼ��


BOOL CDacrsUIApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!RunOnlyOneApp())
	{
		return FALSE;
	}

	//��������
	m_fontSong.CreateFont (15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "����");
	m_fontBlackbody.CreateFont(15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "����");
		

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	if (!AfxSocketInit()){   //��ʼ��SOCKET
		return FALSE ;
	}


	m_blockAutoDelete = false;
	m_msgAutoDelete= false;
	GetMoFilename( str_InsPath , str_ModuleFilename ); //��ȡ�ļ�·�����ļ�����

	CheckPathValid( str_InsPath );
	////����ά���߳�
	if( !CreateMaintainThrd() ) {
		return FALSE ;
	}

	/// ���������ļ�
	ParseUIConfigFile(str_InsPath);
	//��ʼ����־���ò���
	InitLogCfg();

	LogPrint("INFO", "����DacrsUI����\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	//����Զ�����
	int nResult = Update();
	if(-1 == nResult) {
		nResult = Update();
	}
	if (nResult == 1)
	{
		return TRUE;
	}else if(nResult == 0){
		//// ������ֱ���˳�
		exit(0); 
	}

	if(CSoyPayHelp::getInstance()->IsOSVersionBelowXp()) {
		if(!EnableDebugPrivilege())
			TRACE(_T("Call EnableDebugPrivilege failed!"));
		//				AfxMessageBox(_T("Call EnableDebugPrivilege failed!"));
		//// ������ֱ���˳�
		exit(0); 
	}

	//CheckUpdate();

	gsLanguage = language();


	string temprpc = m_rpcport;
	string tempuiport = m_uirpcport;
	ProductHttpHead(str_InsPath ,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr,m_uirpcport,netWork);

	string dbpath = strprintf("%s\\db",str_InsPath);
	if (!PathIsDirectory(dbpath.c_str()))
	{
		::CreateDirectory(dbpath.c_str(), NULL);
	}
	//��sqlite3���ݿ�
	m_SqliteDeal.InitializationDB();

	/// ��ս��׼�¼
	ClearTransaction();


	if (strcmp(m_severip.c_str(),_T("127.0.0.1")))
	{
		m_rpcport = temprpc;
		m_uirpcport = tempuiport;
	}

	string strSeverPath = strprintf("%s\\dacrs-d.exe",str_InsPath);

	if (PathIsDirectory(strSeverPath.c_str()))
	{
		::CreateDirectory(strSeverPath.c_str(), NULL);
		::RemoveDirectory(strSeverPath.c_str());
		if( (_access( strSeverPath.c_str(), 0 )) == -1 )
		{
			UiFun::MessageBoxEx(_T("dacrs-d �ļ�������,����������Ǯ��\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
			exit(1);
		}
	}
	/// �ж��ļ��Ƿ����
	 if( (_access( strSeverPath.c_str(), 0 )) == -1 )
	{
		UiFun::MessageBoxEx(_T("dacrs-d �ļ�������,����������\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
		exit(1);
	}
	//�����������
	StartSeverProcess(str_InsPath);
	m_bServerState = true;

//	Sleep(1000);

	//����block
	//���ӵ�������
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
	theApp.StartblockThrd();  //����Block�߳�
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

				UiFun::MessageBoxEx(_T("OutOfMemory") , _T("Error") ,MFB_OK|MFB_ERROR );
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
			/*
			if(m_bReIndexServer) {
				StartSeverProcess(str_InsPath);
				m_bReIndexServer = FALSE;
				continue;
			}
			*/
			int errorCode = GetLastError();
			TRACE("Error OpenProcess:%d " , errorCode );
			UiFun::MessageBoxEx(_T("���������ݿ��𻵣���˫������Ǯ����clear.bat�ļ��������´�Ǯ��\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
			if (IDYES==UiFun::MessageBoxEx(_T("�Ƿ�������������ݣ�����ͬ��") , _T("Error") ,MFB_YESNO|MFB_ERROR ))
			{
				string batpath =strprintf("%s\\clear.bat",str_InsPath);
				ShellExecute(NULL, "open",batpath.c_str(), NULL, NULL, SW_SHOW);
				//system(batpath.c_str());
			}
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
			::MessageBox( NULL , _T("����Ǯ��ʧ��\r\n") , "Error" , MB_ICONERROR) ;
			exit(0);
		}*/
	}

	CDacrsUIDlg dlg;
	m_pMainWnd = &dlg;
	
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
/*
������������Ȩ�ޣ����xpϵͳ�µ���OpenProcess()API����ʧ���룺5�����⡣
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
		tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;//��һ��ܹؼ����޸�������ΪSE_PRIVILEGE_ENABLED
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
	// ȡĬ����������
	char szText[256];
	string  strG;
	string strAppIni = str_InsPath;// + (CString)LANGUAGE_FILE;
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
		UiFun::MessageBoxEx(_T("ά���̴߳���ʧ��!") , _T("����") ,MFB_OK|MFB_ERROR );
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
								pMainWnd->CloseApp(); //�رնԻ���
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
	m_hProcessAppTxThread = (HANDLE)_beginthreadex(NULL,	0,	ProcessAppTx, this, 0, &m_uProAppTxThreadId);	
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
		/// ͬ���Ժ�������ݿ��
		if (theApp.IsSyncAppTx )
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
		/// ��Ϣ������ ���ܹ��˳�
		if (theApp.m_msgAutoDelete && !pUiDemeDlg->m_MsgQueue.pop(Postmsg))
		{
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
				LogPrint("PROCESSMSG", "MSG_USER_STARTPROCESS_UI ��:%d\n",Postmsg.GetDatatype());
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
						LogPrint("PROCESSMSG", "WM_UP_ADDRESS\n");
						//����Ǯ����ַ���ݿ�
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
						LogPrint("PROCESSMSG", "WM_SYNC_TRANSACTION ����ͬ������:%s\n",txData.c_str());
					}
					break;
				case WM_REVTRANSACTION:
					{

						//������ʷ���׼�¼���ݿ�
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
						LogPrint("PROCESSMSG", "WM_REVTRANSACTION ��ȡ��Ǯ���йصĽ���:%s\n",pHash.c_str());
					}
					break;
				case WM_APP_TRANSATION:
					{
						string txDetail = Postmsg.GetData();
						if ( "" != txDetail ) {
							((CDacrsUIApp*)pParam)->UpdateAppRecord(txDetail);
						}
						LogPrint("PROCESSMSG", "WM_APP_TRANSATION ��ȡ��Ǯ���йص�Ӧ�ý���:%s\n",txDetail.c_str());
					}
					break;
				case WM_P2P_BET_RECORD:
					{
						//�������ݿ��Լ���ݿ��б�
						//((CDacrsUIApp*)pParam)->UpdateP2pBetRecord(); 
					}
					break;
				case WM_UP_BETPOOL:
					{
						LogPrint("PROCESSMSG", "WM_UP_BETPOOL ���¶�Լ��\n");
						/// ��Լ�����ݿ��б�
						BOOL flag =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_quiz_pool"));
						if (flag ) 
						{
							((CDacrsUIApp*)pParam)->UpdateQuizPoolData();
						}
					}
					break;
				case WM_REDPACKETPOOL:
					{
						LogPrint("PROCESSMSG", "WM_REDPACKETPOOL ���º����\n");
						/// �����
						BOOL flag =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_red_packets_pool"));
						if (flag ) 
						{
							((CDacrsUIApp*)pParam)->UpdateRedPacketPoolData();
						}
					}
				case WM_UP_BlLOCKTIP:
					{
						LogPrint("PROCESSMSG", "WM_UP_BlLOCKTIP ����block���¸߶�\n");
						//��������blocktip���ݿ�
						if ( ((CDacrsUIApp*)pParam)->m_SqliteDeal.ClearTableData(_T("t_chain_tip") ) ) {

							string pHash = Postmsg.GetData();
							if ( _T("") != pHash ) {
								string strinsert;
								strinsert = strprintf("'%s'",pHash);
								((CDacrsUIApp*)pParam)->m_SqliteDeal.InsertTableItem(_T("t_chain_tip") ,strinsert ) ;
							}
						}
					}
					break;
				case WM_UP_ADDRBOOK:
					{
						LogPrint("PROCESSMSG", "WM_UP_ADDRBOOK ���µ�ַ�����ݿ�\n");
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
						LogPrint("PROCESSMSG", "WM_UP_DELETERBOOK ɾ����ַ�����ݿ�\n");
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
						LogPrint("PROCESSMSG", "WM_RELEASETX �յ��������·ŵ�mempool\n");
						//������ʷ���׼�¼���ݿ�
						string pHash = Postmsg.GetData();
						if ( _T("") != pHash ) {
		
							string strCondition("");
							strCondition = strprintf(" hash = '%s' ", pHash.c_str());
							int nItem =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.GetTableCountItem(_T("t_transaction") ,strCondition);
							if (nItem != 0)
							{
								LogPrint("INFO","WM_RELEASETX:%s",pHash.c_str());
								((CDacrsUIApp*)pParam)->InsertTransaction(pHash ) ;
								theApp.m_SqliteDeal.UpdataAllTableData();   /// ����Ӧ�ñ��
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
								theApp.m_SqliteDeal.UpdataAllTableData();   /// ����Ӧ�ñ��
							}
						}
							LogPrint("PROCESSMSG", "WM_REMOVETX ɾ������:%s\n",pHash.c_str());
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
				switch(Postmsg.GetDatatype())
				{
				case WM_LOCKSTATE:
					{
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS WM_LOCKSTATE ������Ϣ\n");
						pUiDemeDlg->m_LockmsgQueue.push(Postmsg);
					}
					break;
				case WM_CONNECTNET:
					{
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS WM_CONNECTNET ����������Ϣ\n");
						pUiDemeDlg->m_NetConnetCountQueue.push(Postmsg);
					}
					break;
				default:
					{
						TRACE("change:%s\r\n","MSG_USER_UP_PROGRESS");
						//pUiDemeDlg->m_MsgQueue.ClearMessageType(MSG_USER_UP_PROGRESS);
						pUiDemeDlg->m_UimsgQueue.push(Postmsg);
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS  ���½�������Ϣ:%s\n",Postmsg.GetData().c_str());
					}
					break;
				}
				//theApp.DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_UP_PROGRESS , 0,0);					
			}
			break;
		case MSG_USER_INSERT_DATA:   //// ��������
			{
				LogPrint("PROCESSMSG", "MSG_USER_INSERT_DATA ����������Ϣ\n");
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
		case MSG_USER_UPDATA_DATA:   /// �������ݿ�
			{
					LogPrint("PROCESSMSG", "MSG_USER_UPDATA_DATA ����������Ϣ\n");
				uistruct::DATABASEINFO_t pDatabase;// = (uistruct::DATABASEINFO_t *)Postmsg.GetStrPoint();
				string strTemp = Postmsg.GetData();
				pDatabase.JsonToStruct(strTemp);

				if ( !((CDacrsUIApp*)pParam)->m_SqliteDeal.UpdateTableItem(pDatabase.strTabName.c_str() , pDatabase.strSource.c_str() , pDatabase.strWhere.c_str() ) ){
					TRACE(_T("t_p2p_quiz���ݸ���ʧ��!") );
				}

				if ( !strcmp(pDatabase.strTabName.c_str() , _T("t_p2p_quiz")) ){
					Postmsg.SetType(MSG_USER_UPDATA_DATA,WM_P2P_BET_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("t_dark_record")) )
				{
					Postmsg.SetType(MSG_USER_UPDATA_DATA,WM_DARK_RECORD);
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
			//���뵽���ݿ�
			string txData ;
			txData =strprintf("%s" , obj.toStyledString().c_str() );
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
			int tempTimemsg= (int)UiFun::SystemTimeToTimet(curTime);
			/// ����Ǯ��
			CPostMsg postuimsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRESS);
			if ((tempTimemsg - RecivetxtxTimeLast)>10 || RecivetxtxTimeLast == 0)
			{	
				pApp->m_MsgQueue.push(postuimsg);
				RecivetxtxTimeLast = tempTimemsg;
			}

		//	LogPrint("INFO", "REV_TRANSATION %s\n",msgValue.toStyledString().c_str());
			const Json::Value& txArray = msgValue["transation"]; 
			//���뵽���ݿ�
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
			//���뵽���ݿ�
			string strHash =  msgValue["hash"].asString() ;
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_RELEASETX);
			postmsg.SetData(strHash);
			pApp->m_MsgQueue.push(postmsg);
		}
		break;
	case REMOVE_TX:
		{
			//���뵽���ݿ�
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
				/// ����tipblock hash
				CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_BlLOCKTIP);
				string msg = msgValue["hash"].asString();
				postblockmsg.SetData(msg);
				pApp->m_MsgQueue.push(postblockmsg);  

			
				/// ����Ǯ����ַ
				CPostMsg postuimsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRESS);
				pApp->m_MsgQueue.push(postuimsg);

				// ���¶�Լ��
				CPostMsg postpoolmsg(MSG_USER_GET_UPDATABASE,WM_UP_BETPOOL);
				pApp->m_MsgQueue.push(postpoolmsg);

				// ���º����
				CPostMsg postredpoolmsg(MSG_USER_GET_UPDATABASE,WM_REDPACKETPOOL);
					pApp->m_MsgQueue.push(postredpoolmsg);
				/// ��Լ�����ַ�б�
				CPostMsg postp2pmsg(MSG_USER_P2P_UI,WM_UP_ADDRESS);
				pApp->m_MsgQueue.push(postp2pmsg);

				/// ��������ַ�б�
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
			}else if (!strcmp(msg.c_str(),"UnLock"))
			{
				CPostMsg postmsg(MSG_USER_UP_PROGRESS,WM_LOCKSTATE);
				postmsg.SetStrType(msg);
				pApp->m_MsgQueue.push(postmsg);
				pApp->IsWalletLocked = FALSE;
			}else if (!msg.find("connections")>=0)
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
		CJsonConfigHelp::getInstance()->GetNewestScriptData(m_neststcriptid);
		CJsonConfigHelp::getInstance()->GetP2pBetStep(m_p2pbetstep);
		CJsonConfigHelp::getInstance()->GetRedPacketStep(m_redPackestep);

		CNetParamCfg netParm;
		CJsonConfigHelp::getInstance()->GetNetParamCfgData(netParm);
		m_severip = netParm.server_ip;
		m_uirpcport = netParm.server_ui_port;
		m_rpcport = netParm.rpc_port;
		m_rpcUser = netParm.rpc_user;
		m_rpcPassWord = netParm.rpc_password;
	}
}
void CDacrsUIApp::StartSeverProcess(const string& strdir){
	///// ����ǰ�ȹر�ϵͳ��dacrs-d.exe����
	CloseProcess("dacrs-d.exe");
	LogPrint("INFO", "�رշ���˳ɹ�\n");
	
	STARTUPINFOA si; 
	memset(&si, 0, sizeof(STARTUPINFO));  
	si.cb = sizeof(STARTUPINFO);  
	si.dwFlags = STARTF_USESHOWWINDOW;  
	si.wShowWindow =SW_HIDE;//SW_HIDE; //SW_SHOW;  

	string str = _T("dacrs-d.exe -datadir=");
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
		LogPrint("INFO", "��������˳���ʧ��\n");
		exit(1);  
	}  
	CloseHandle(sever_pi.hProcess);
	CloseHandle(sever_pi.hThread);
	LogPrint("INFO", "��������˳���ɹ�\n");
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
	CString sPath;//=str_InsPath+"\\qupdater.exe";
	sPath.Format(_T("%s\\qupdater.exe"),str_InsPath.c_str());
	LogPrint("INFO","Updata:%s\n",sPath);
	sPath.Replace("\\\\","\\");
	LogPrint("INFO","Updata:%s\n",sPath);
	CFileFind find;
	if(!find.FindFile(sPath)) return false;
	LogPrint("INFO","Updata start:%s\n",sPath);
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
	LogPrint("INFO","Updata OpenProcess:%0x\n",ShRun.hProcess);
	if (!GetExitCodeProcess(ShRun.hProcess, &lResult)) return false;
	LogPrint("INFO","Updata GetExitCodeProcess:%s\n",sPath);
	if (lResult == 0) return -1;
	LogPrint("INFO","Updata lResult:%s\n",sPath);
	CString sMsg;
	//sMsg.Format(CLanguage::TranLanguage("MAIN","%d updates found at the website,now to upgrade?"),lResult);
	sMsg.Format(CLanguage::TranLanguage("MAIN","�����%d�ļ���Ҫ����,�����Ƿ�Ҫ����?"),lResult);
	//if (AfxMessageBox(sMsg, MB_ICONQUESTION | MB_YESNO) != IDYES) return 0;
	if ( IDYES != UiFun::MessageBoxEx(sMsg, _T("��ʾ") , MFB_YESNO|MFB_TIP ) )
		return 0;

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
	//if (AfxMessageBox(sMsg, MB_ICONQUESTION | MB_YESNO) != IDYES) return false;
	if ( IDYES != UiFun::MessageBoxEx(sMsg, _T("��ʾ") , MFB_YESNO|MFB_TIP ) )
		return false;
	ShRun.lpParameters = NULL; 
	ShellExecuteEx(&ShRun); 
	return true;
}
void  CDacrsUIApp::CheckUpdate(){
	CloseHandle(::CreateThread(NULL,0,Update1,this,0,NULL));

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

	string strCond;
	strCond = " confirm_height = 0 ";

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
	// ���������
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		CloseHandle(hMutex);
		AfxMessageBox(_T("�����Ѿ���������") ,MB_OK);
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
//// ֪ͨ���ͽ�����߽��ܽ����ַ�����ݸı��˻�ȡҪ�����µ�ַ
void CDacrsUIApp::SendUIMsg(int message,string jsonaddr){

	//m_UiReciveDlgQueue.clear();
	//m_MsgQueue
	CPostMsg Postmsg(MSG_USER_MAIN_UI,message);
	Postmsg.SetData(jsonaddr);	
	m_UiReciveDlgQueue.push(Postmsg);
	//while(!m_UiReciveDlgQueue.isEmpty())
	CPostMsg msg(MSG_USER_RECIVE_UI,message);
	m_MsgQueue.push(msg);
	//DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_RECIVE_UI ,message,0);

	m_UiSendDlgQueue.push(Postmsg);
	//while(!m_UiSendDlgQueue.isEmpty())
	CPostMsg msg1(MSG_USER_SEND_UI,message);
	m_MsgQueue.push(msg1);
	//DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_SEND_UI ,message,0);

}
void CDacrsUIApp::SendP2pMsg(int message,string jsonaddr)
{
	CPostMsg Postmsg(MSG_USER_MAIN_UI,message);
	Postmsg.SetData(jsonaddr);
	m_UiP2pDlgQueue.push(Postmsg);

	CPostMsg msg(MSG_USER_P2P_UI,message);
	m_MsgQueue.push(msg);
	//DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_P2P_UI ,message,0);

	m_UiRedPacketDlgQueue.push(Postmsg);
	CPostMsg msg1(MSG_USER_REDPACKET_UI,message);
	m_MsgQueue.push(msg1);
	//DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_REDPACKET_UI ,message,0);
}
void CDacrsUIApp::CheckPathValid(const string& strDir)
{
	BOOL bExist = FALSE;
	for(int i = 0;i < strDir.length();i++)
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
		UiFun::MessageBoxEx(_T("���򲻿��Է��ں��пո��Ŀ¼��\r\n!") , _T("Error") ,MFB_OK|MFB_ERROR );
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
