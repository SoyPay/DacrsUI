
// DacrsUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "SynchronousSocket.h"
#include <afxinet.h>
#include "Language.h"
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
	m_blockSock  = INVALID_SOCKET ;
	m_strServerCfgFileName = "dacrs.conf";
	isStartMainDlg = false;
	m_bServerState = false;
	m_rpcUser = _T("");
	m_rpcPassWord = _T("");
}


// 唯一的一个 CDacrsUIApp 对象

CDacrsUIApp theApp;


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
	//检测自动升级
	if (Update())
	{
		return TRUE;
	}
	CheckUpdate();

	/// 加载配置文件
	ParseUIConfigFile(str_InsPath);
	gsLanguage = language();

	OnInitList() ;
	//打开sqlite3数据库
	m_SqliteDeal.OpenSqlite(str_InsPath) ;

	CString temprpc = m_rpcport;
	CString tempuiport = m_uirpcport;
	ProductHttpHead(str_InsPath ,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr,m_uirpcport);

	if (strcmp(m_severip,_T("127.0.0.1")))
	{
		m_rpcport = temprpc;
		m_uirpcport = tempuiport;
	}

	/// 关闭系统中dacrs-d.exe进程
	//CloseProcess("dacrs-d.exe");
	////启动服务程序
	//StartSeverProcess(str_InsPath);
	m_bServerState = true;
	Sleep(1000);

	//连接block
	//连接到服务器
	CSynchronousSocket te;
	//SOCKET nSocket = te.OnblockConnnect(_T("127.0.0.1"),atoi(m_uirpcport) ) ;
	SOCKET nSocket = te.OnblockConnnect(m_severip,atoi(m_uirpcport) ) ;
	if ( INVALID_SOCKET != nSocket ){
		theApp.m_blockSock = nSocket ;
		theApp.StartblockThrd();  //开启Block线程
	}
	//gif
	m_ProgressGifFile =   str_InsPath + _T("\\progress.gif\0") ;
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

	while(1)
	{
		if (isStartMainDlg)
		{
			break;
		}
	}
	////
//	CStartProgress  progdlg ;
//	progdlg.DoModal();


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
			CPostMsg postmsg(msg.message,msg.wParam);
			if(!(msg.message == MSG_USER_UP_PROGRESS || msg.message == MSG_USER_BLOCKSTATE_UI))
			{

				char * data = (char *)msg.lParam;
				if(data != NULL)
				//	postmsg.SetData(data,strlen(data)+1);
				switch ( msg.message )
				{
				case MSG_USER_INSERT_DATA:
				case MSG_USER_UPDATA_DATA:
					{
						//EnterCriticalSection( &theApp.cs_UpDatabasech ) ;
						//LeaveCriticalSection (&theApp.cs_UpDatabasech ) ;
					}
					break;
				default:
					break;
				}
				//((CUIDemoApp*)pParam)->InsertMsg(postmsg);
				((CDacrsUIApp*)pParam)->m_MsgQueue.push(postmsg);
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
	return ;
}

void CDacrsUIApp::UpdataBetPoolData()
{
	Json::Reader reader;  
	Json::Value root; 
	Json::Value root1;

	if(theApp.m_betScritptid != _T(""))
	{
		CString strCommand;
		strCommand.Format(_T("%s %s %s %s"),_T("getscriptdata"),theApp.m_betScritptid.GetBuffer(),_T("100"),_T("1"));
		CStringA strShowData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		//m_AppID
		if (strShowData.Find("key") < 0)
		{
			return;
		}
		if (!reader.parse(strShowData.GetString(), root)) 
			return;
		int size = root.size();
		for ( int index =0; index < size; ++index )
		{
			CString txhash = root[index]["key"].asCString();
			CString nValue = root[index]["value"].asCString();
			uistruct::DBBET_DATA DBbet;
			memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue.GetString());

			memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

			std::vector<unsigned char> vnTemp = CSoyPayHelp::getInstance()->ParseHex(txhash);
			reverse(vnTemp.begin(),vnTemp.end());
			string strTemp = CSoyPayHelp::getInstance()->HexStr(vnTemp);

			/// 查找数据库中此赌约是否正在接赌
			uistruct::P2P_BET_RECORD_t  betrecord;
			CString findhash;
			findhash.Format(_T("%s"),strTemp.c_str());
			theApp.cs_SqlData.Lock();
			int nItem =  theApp.m_SqliteDeal.FindDB(_T("p2p_bet_record") , findhash ,_T("tx_hash"),&betrecord ) ;
			theApp.cs_SqlData.Unlock();
			if (nItem != 0 && betrecord.state == 4) ////此赌约正在接赌，只是在block中没有确认
			{
				continue;
			}

			strCommand.Format(_T("%s %s"),_T("gettxdetail"),strTemp.c_str());
			CStringA strShowData;
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (!reader.parse(strShowData.GetString(), root1)) 
				return;
			int confirheight =root1["confirmHeight"].asInt();

			strCommand.Format(_T("%s"),_T("getinfo"));
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (!reader.parse(strShowData.GetString(), root1)) 
				return;
			int curheight =root1["blocks"].asInt();
			if(curheight >=(confirheight+DBbet.hight+500))
				continue;

			if (DBbet.betstate == 0x00)
			{
				CString strSourceData;
				strSourceData.Format(_T("'%s' , '%s'") ,txhash,nValue) ;
				theApp.cs_SqlData.Lock();
				m_SqliteDeal.InsertData(_T("p2ppool") ,strSourceData ) ;
				theApp.cs_SqlData.Unlock();

			}
		}
	}
}
void CDacrsUIApp::UpdataAddressData(){
	CString strCommand;
	strCommand.Format(_T("%s"),_T("listaddr"));
	CStringA strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	//更新数据库
	/*theApp.cs_SqlData.Lock();
	if (!m_SqliteDeal.EmptyTabData(_T("MYWALLET")))
	{
		theApp.cs_SqlData.Unlock();
		return;
	}
	theApp.cs_SqlData.Unlock();*/
	uistruct::LISTADDR_t listaddr;
	for(int i = 0; i < root.size(); ++i){
		memset(&listaddr , 0 , sizeof(uistruct::LISTADDR_t));
		//address
		CString strData;
		strData.Format( _T("%s") , root[i]["addr"].asCString() ) ;
		strncpy(listaddr.address  , strData , strlen(strData) > sizeof(listaddr.address) ? sizeof(listaddr.address):strlen(strData));
		//RegID
		strData.Format( _T("%s") , root[i]["regid"].asCString() ) ;
		strncpy(listaddr.RegID  , strData , strlen(strData) > sizeof(listaddr.RegID) ? sizeof(listaddr.RegID):strlen(strData));
		//金额
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//是否支持冷挖矿
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//是否注册GetLength();
		CString strSign ;
		strSign.Format(_T("%s") ,listaddr.RegID ) ;
		if ( 3 <= strSign.GetLength() ) {
			listaddr.bSign    = 1 ;
		}else{
			listaddr.bSign   = 0 ;
		}
		CString strSourceData,feild;
		feild.Format(_T("address"));
		strSourceData.Format(_T("%s"),listaddr.address);

		theApp.cs_SqlData.Lock();
		int item = m_SqliteDeal.FindDB(_T("MYWALLET") ,strSourceData, feild) ;
		theApp.cs_SqlData.Unlock();

		if (item == 0 )
		{
			strSourceData.Format(_T("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'") , listaddr.address ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Lebel ) ;
			theApp.cs_SqlData.Lock();
			m_SqliteDeal.InsertData(_T("MYWALLET") ,strSourceData ) ;
			theApp.cs_SqlData.Unlock();
		}else{
			CString strSourceData,strWhere;
			strSourceData.Format(_T("regid = '%s', money = %.8f ,coldig =%d,sign =%d") ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
			strWhere.Format(_T("address = '%s'") , listaddr.address  ) ;
			theApp.cs_SqlData.Lock();
			if ( !m_SqliteDeal.Updatabase(_T("MYWALLET") , strSourceData , strWhere ) ){
				TRACE(_T("MYWALLET数据更新失败!") );
			}
			theApp.cs_SqlData.Unlock();
		}
	}
}

void CDacrsUIApp::UpdatarevtransactionData(string hash){

	CString strCommand,strShowData;
	strCommand.Format(_T("%s %s"),_T("gettxdetail") ,hash.c_str() );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData.Find("hash") < 0){		
		return;
	}

	//解析Json数据
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) {		
		return;
	}

	if (strShowData.Find("blockhash") >= 0)
	{
		uistruct::REVTRANSACTION_t transcion;
		if (transcion.JsonToStruct(root.toStyledString()))
		{
			CString strSourceData,strWhere;
			strSourceData.Format(_T("confirmHeight = %d , confirmedtime = '%d' ,blockhash ='%s',") ,transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str() ) ;
			strWhere.Format(_T("tx_hash = '%s'") , hash ) ;
			theApp.cs_SqlData.Lock();
			if ( !m_SqliteDeal.Updatabase(_T("revtransaction") , strSourceData , strWhere ) ){
				TRACE(_T("revtransaction数据更新失败!") );
			}
			theApp.cs_SqlData.Unlock();
		}
	}
}

void CDacrsUIApp::UpdatarevAppRecord(string txdetail){
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(txdetail))
	{
		CString strCommand,strShowData;
		strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,theApp.m_betScritptid );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData  =_T("")){		
			return;
		}
		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(strShowData.GetString(), root)) {		
			return;
		}
		CString DesAddr = root["Address"].asCString();
		if (!strcmp(transcion.desaddr.c_str(),DesAddr))
		{
			string nValue = transcion.Contract;
			OPEN_DATA Openbet;
			memset(&Openbet , 0 , sizeof(OPEN_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
			if(vTemp.size()>0)
			memcpy(&Openbet, &vTemp[0], sizeof(OPEN_DATA));
			if (Openbet.type != TX_OPENBET)
			{
				return;
			}
			vector<unsigned char>vHash;
			vHash.assign(Openbet.txhash,Openbet.txhash+sizeof(Openbet.txhash));
			reverse(vHash.begin(),vHash.end());
			string hexHash = CSoyPayHelp::getInstance()->HexStr(vHash);
			theApp.cs_SqlData.Lock();
			int nItem =  theApp.m_SqliteDeal.FindDB(_T("p2p_bet_record") , hexHash.c_str() ,_T("tx_hash")) ;
			theApp.cs_SqlData.Unlock();
			if (nItem != 0)
			{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("recvtime = %d ,guessnum ='%s',") ,transcion.confirmedtime,(int)Openbet.dhash[32] ) ;
				strWhere.Format(_T("tx_hash = '%s'") , hexHash.c_str() ) ;
				theApp.cs_SqlData.Lock();
				if ( !m_SqliteDeal.Updatabase(_T("p2p_bet_record") , strSourceData , strWhere ) ){
					TRACE(_T("p2p_bet_record数据更新失败!") );
				}
				theApp.cs_SqlData.Unlock();
			}
		}
	}
}
void CDacrsUIApp::InsertarevtransactionData(string hash){

	CString strCommand,strShowData;
	strCommand.Format(_T("%s %s"),_T("gettxdetail") ,hash.c_str() );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData.Find("hash") < 0){		
		return;
	}

	//解析Json数据
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) {		
		return;
	}

	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(root.toStyledString()))
	{
		theApp.cs_SqlData.Lock();
		int nItem =  theApp.m_SqliteDeal.FindDB(_T("MYWALLET") , transcion.addr.c_str() ,_T("address")) ;
		int nItem1 =  theApp.m_SqliteDeal.FindDB(_T("MYWALLET") , transcion.desaddr.c_str() ,_T("address")) ;
		theApp.cs_SqlData.Unlock();
		if (nItem != 0)
		{
			transcion.state = 1;        //// 扣钱
		}else if (nItem1 != 0)
		{
			transcion.state = 2;        ////价钱
		}
		CString strSourceData;
		strSourceData.Format(_T("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d") , transcion.txhash ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		theApp.cs_SqlData.Lock();
		m_SqliteDeal.InsertData(_T("revtransaction") ,strSourceData ) ;
		theApp.cs_SqlData.Unlock();
	}
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

		if(!pUiDemeDlg->m_MsgQueue.pop(Postmsg))
			continue;
		CDacrsUIDlg *pDlg = (CDacrsUIDlg*)(((CDacrsUIApp*)pParam)->m_pMainWnd) ;
		if (pDlg == NULL)
			continue;
		switch (Postmsg.GetUItype() )
		{
		case MSG_USER_GET_UPDATABASE:
			{
				//EnterCriticalSection( &theApp.cs_UpDatabasech ) ;
				switch (Postmsg.GetDatatype())
				{
				case WM_UP_ADDRESS:
					{
						//更新钱包地址数据库
						((CDacrsUIApp*)pParam)->UpdataAddressData();
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
							theApp.cs_SqlData.Lock();
							int nItem =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.FindDB(_T("revtransaction") ,strHash,_T("hash") ) ;
							theApp.cs_SqlData.Unlock();
							if (nItem == 0)
							{
								((CDacrsUIApp*)pParam)->InsertarevtransactionData(strHash.GetString() ) ;
							}else{
								((CDacrsUIApp*)pParam)->UpdatarevtransactionData(strHash.GetString() );
							}
						}
					}
					break;
				case WM_APP_TRANSATION:
					{
						CString txDetail = Postmsg.GetData();
						if ( _T("") != txDetail ) {
							((CDacrsUIApp*)pParam)->UpdatarevAppRecord(txDetail.GetString());
						}
					}
				case WM_P2P_BET_RECORD:
					{
						//更新数据库赌约数据库列表
						theApp.cs_SqlData.Lock();
						((CDacrsUIApp*)pParam)->m_SqliteDeal.UpdataP2pBetRecord(); 
						theApp.cs_SqlData.Unlock();
					}
					break;
				case WM_DARK_RECORD:
					{
						//更新数据库抵押数据库列表
						theApp.cs_SqlData.Lock();
						((CDacrsUIApp*)pParam)->m_SqliteDeal.UpdataDarkRecord(); 
						theApp.cs_SqlData.Unlock();
					}
					break;
				case WM_UP_BETPOOL:
					{
						/// 赌约池数据库列表
						theApp.cs_SqlData.Lock();
						bool flag =  ((CDacrsUIApp*)pParam)->m_SqliteDeal.EmptyTabData(_T("p2ppool") );
						theApp.cs_SqlData.Unlock();
						if (flag ) 
						{
							((CDacrsUIApp*)pParam)->UpdataBetPoolData();
						}
					}
					break;
				case WM_UP_BlLOCKTIP:
					{
						//更新最新blocktip数据库
						theApp.cs_SqlData.Lock();
						if ( ((CDacrsUIApp*)pParam)->m_SqliteDeal.EmptyTabData(_T("tip_block") ) ) {

							CString pHash = Postmsg.GetData();
							if ( _T("") != pHash ) {
								CString strinsert;
								strinsert.Format("'%s'",pHash);
								((CDacrsUIApp*)pParam)->m_SqliteDeal.InsertData(_T("tip_block") ,strinsert ) ;
							}
						}
						theApp.cs_SqlData.Unlock();
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
				string strTemp = Postmsg.GetData();
				uistruct::BLOCKCHANGED_t      m_Blockchanged;
				m_Blockchanged.JsonToStruct(strTemp.c_str());
				pUiDemeDlg->m_UimsgQueue.push(Postmsg);
				theApp.DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_UP_PROGRESS , 0,0);					
			}
			break;
		case MSG_USER_BLOCKSTATE_UI:
			{
				//if (pDlg->dlgType == DIALOG_BLOCK_STATE)
				//{
				//	theApp.DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_BLOCKSTATE_UI , Postmsg.GetDatatype(),0);
				//	//::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_BLOCKSTATE_UI ,Postmsg.GetDatatype(), 0 ) ; 
				//}

			}
			break;
		case MSG_USER_INSERT_DATA:   //// 插入数据
			{
				uistruct::DATABASEINFO_t   pDatabase; // = (uistruct::DATABASEINFO_t *)Postmsg.GetStrPoint();
				string strTemp = Postmsg.GetData();
				pDatabase.JsonToStruct(strTemp.c_str());
				theApp.cs_SqlData.Lock();
				((CDacrsUIApp*)pParam)->m_SqliteDeal.InsertData(pDatabase.strTabName.c_str() ,pDatabase.strSource.c_str() ) ;
				theApp.cs_SqlData.Unlock();
				if ( !strcmp(pDatabase.strTabName.c_str() , _T("p2p_bet_record")) ){
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_P2P_BET_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("dark_record")) )
				{
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_DARK_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("MYWALLET")))
				{
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_UP_ADDRESS);
				}

			}
			break;
		case MSG_USER_UPDATA_DATA:   /// 更新数据库
			{
				uistruct::DATABASEINFO_t pDatabase;// = (uistruct::DATABASEINFO_t *)Postmsg.GetStrPoint();
				string strTemp = Postmsg.GetData();
				pDatabase.JsonToStruct(strTemp.c_str());
				theApp.cs_SqlData.Lock();
				if ( !((CDacrsUIApp*)pParam)->m_SqliteDeal.Updatabase(pDatabase.strTabName.c_str() , pDatabase.strSource.c_str() , pDatabase.strWhere.c_str() ) ){
					TRACE(_T("p2p_bet_record数据更新失败!") );
				}
				theApp.cs_SqlData.Unlock();
				if ( !strcmp(pDatabase.strTabName.c_str() , _T("p2p_bet_record")) ){
					Postmsg.SetType(MSG_USER_UPDATA_DATA,WM_P2P_BET_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("dark_record")) )
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
			CPostMsg postmsg(MSG_USER_SHOW_INIT,0);
			postmsg.SetStrType(msgValue["type"].asCString());
			CString msg = msgValue["msg"].asCString();
			TRACE("MEST:%s\r\n",msg);
			if (!strcmp(msg,"initialize end"))
			{
				theApp.isStartMainDlg = true;
			}
			postmsg.SetData(msg);
			pApp->m_MsgQueue.push(postmsg);
			TRACE("type: %s   mag: %s\r\n" , postmsg.GetStrType() ,msg);
			break;
		}
	case REV_TRANSATION_TYPE:
		{
			const Json::Value& txArray = msgValue["transation"]; 
			//插入到数据库
			CString strHash ;
			strHash.Format(_T("%s") , txArray["hash"].asCString() );
			theApp.cs_SqlData.Lock();
			int nItem =  pApp->m_SqliteDeal.FindDB(_T("revtransaction") ,strHash ,_T("hash") ) ;
			theApp.cs_SqlData.Unlock();
			strHash.Format(_T("'%s'") , txArray["hash"].asCString() );
			if ( 0 == nItem ) {
				CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
				postmsg.SetData(strHash);
				pApp->m_MsgQueue.push(postmsg);

			}
			break;
		}
	case BLOCK_CHANGE_TYPE:
		{

			TRACE("change:%s\r\n","blockchanged");
			uistruct::BLOCKCHANGED_t      m_Blockchanged ;
			memset(&m_Blockchanged , 0 , sizeof(uistruct::BLOCKCHANGED_t));
			m_Blockchanged.type = msgValue["type"].asString();
			m_Blockchanged.time = msgValue["time"].asInt();
			m_Blockchanged.high = msgValue["high"].asInt64() ;
			m_Blockchanged.hash = msgValue["hash"].asString();

			static int ReciveBlockTimeLast =0;
			int tempTime= m_Blockchanged.time;

			string strJson = m_Blockchanged.ToJson();
			ReciveBlockTimeLast = tempTime;
			CPostMsg postmsg(MSG_USER_UP_PROGRESS,0);
			postmsg.SetData(strJson.c_str());

			pApp->m_MsgQueue.pushFront(postmsg);
			/// 更新tipblock hash
			CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_BlLOCKTIP);
			CString msg = msgValue["hash"].asCString();
			postblockmsg.SetData(msg);
			pApp->m_MsgQueue.push(postblockmsg);  //.push(postblockmsg);

			SYSTEMTIME curTime ;
			memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			GetLocalTime( &curTime ) ;
			static int RecivetxMsgTimeLast =0;
			int tempTimemsg= UiFun::SystemTimeToTimet(curTime);
			/// 更新钱包
			CPostMsg postuimsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRESS);
			if ((tempTimemsg - RecivetxMsgTimeLast)>10 || RecivetxMsgTimeLast == 0)
			{	
				pApp->m_MsgQueue.push(postuimsg);
				postuimsg.SetType(MSG_USER_GET_UPDATABASE,WM_UP_BETPOOL);
				pApp->m_MsgQueue.push(postuimsg);

				postuimsg.SetType(MSG_USER_GET_UPDATABASE,WM_P2P_BET_RECORD);
				pApp->m_MsgQueue.push(postuimsg);

				postuimsg.SetType(MSG_USER_GET_UPDATABASE,WM_DARK_RECORD);
				pApp->m_MsgQueue.push(postuimsg);
				RecivetxMsgTimeLast = tempTimemsg;
			}

			///更新block状态
			postuimsg.SetType(MSG_USER_BLOCKSTATE_UI,m_Blockchanged.high);
			pApp->m_MsgQueue.push(postuimsg);
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
			Sleep(100); 
			
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
						TRACE0("noui socket has been closed");
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
			} else {
				::PostMessage( it->hSubWnd , msg , wParam , lParam ) ;
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
	case WM_UP_ADDRESS:
		{
			if(pDlg->dlgType == CSendDlg::IDD)
				DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_SEND_UI , WM_UP_ADDRESS,0);
			if(pDlg->dlgType == CReceiveDlg::IDD)
				DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_RECIVE_UI , 0,0);

		}
		break;
	case WM_REVTRANSACTION:
		{
			if(pDlg->dlgType == CMainDlg::IDD)
				DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI , 0,0);
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
		CJsonConfigHelp::getInstance()->GetDarkCfgData(m_DarkCfg);
		CJsonConfigHelp::getInstance()->GetP2PBetCfgData(m_P2PBetCfg);
		CNetParamCfg netParm;
		CJsonConfigHelp::getInstance()->GetNetParmCfgData(netParm);
		m_severip = netParm.server_ip;
		m_uirpcport = netParm.server_ui_port;
		m_rpcport = netParm.rpc_port;
		m_rpcUser = netParm.rpc_user;
		m_rpcPassWord = netParm.rpc_password;
	}
}
void CDacrsUIApp::StartSeverProcess(const CStringA& strdir){
	STARTUPINFOA si; 
	memset(&si, 0, sizeof(STARTUPINFO));  
	si.cb = sizeof(STARTUPINFO);  
	si.dwFlags = STARTF_USESHOWWINDOW;  
	si.wShowWindow =SW_HIDE;//SW_HIDE; //SW_SHOW;  

	CString str = _T("dacrs-d.exe -datadir=");
	str.AppendFormat("%s",strdir);

	//if (!m_bStartServer)
	//{
	//	return ;
	//}

	if(!CreateProcessA(NULL,(LPSTR)str.GetString(),NULL,NULL,FALSE,0,NULL,NULL,&si,&sever_pi))   
	{  
		int n = GetLastError();
		AfxMessageBox(_T("sever error!"));
		exit(1);  
	}  
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
bool CDacrsUIApp::Update()
{
	CString sPath=str_InsPath+"\\qupdater.exe";
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
			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI , 0,0);
			break;
		//case DIALOG_SIGN_ACCOUNTS:
		//	DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MYWALLET_UI , WM_UP_ADDRESS,0);
		//	break;
		case CReceiveDlg::IDD:
			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_RECIVE_UI , 0,0);
			break;
		case CSendDlg::IDD:
			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_SEND_UI , WM_UP_ADDRESS,0);
			break;
		}
	}
}

