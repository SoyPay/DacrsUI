
// DacrsUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "Out.h"
#include "afxdialogex.h"
#include "ChangeDPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDacrsUIDlg 对话框




CDacrsUIDlg::CDacrsUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDacrsUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	p_CurSelDlg = NULL  ;
	m_pTitleBar = NULL  ;
	m_pStatusBar = NULL ;
	m_pMainDlg  = NULL  ;
	m_pSendDlg  = NULL  ;
	m_pReceiveDlg  = NULL  ;
	m_pTradDlg  = NULL  ;
	m_pP2PDlg  = NULL  ;
	m_pMortgageTardDlg  = NULL  ;
	m_pIpoDlg  = NULL  ;
	m_pAddApp  = NULL  ;
	dlgType = 0;
	m_pOutGifDlg =  NULL ;
}

void CDacrsUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDacrsUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_MESSAGE(MSG_TITLE_LBUTTON_DOWN, &CDacrsUIDlg::OnBeginMoveWnd)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_UI, &CDacrsUIDlg::OnBnClickedButtonMainUI)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CDacrsUIDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECE, &CDacrsUIDlg::OnBnClickedButtonRece)
	ON_BN_CLICKED(IDC_BUTTON_TRAD_INFO, &CDacrsUIDlg::OnBnClickedButtonTrad)
	ON_BN_CLICKED(IDC_BUTTON_P2P, &CDacrsUIDlg::OnBnClickedButtonP2P)
	ON_BN_CLICKED(IDC_BUTTON_MORTGAGE, &CDacrsUIDlg::OnBnClickedButtonMortgage)
	ON_BN_CLICKED(IDC_BUTTON_IPO, &CDacrsUIDlg::OnBnClickedButtonDeals)  
	ON_BN_CLICKED(IDC_BUTTON_ADDAPP, &CDacrsUIDlg::OnBnClickedButtonAddApp)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDacrsUIDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CDacrsUIDlg::OnBnClickedButtonMin)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDacrsUIDlg 消息处理程序

BOOL CDacrsUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if ( ((CDacrsUIApp*)AfxGetApp())->pSplashThread != NULL)
		((CDacrsUIApp*)AfxGetApp())->pSplashThread->HideSplash();   

	CDC* dc = GetDC();
	int dpiX = GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSX);
	int dpiY = GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSY);
	SetDPI(dpiX, dpiY);
	ReleaseDC(dc);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDacrsUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDacrsUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDacrsUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CDacrsUIDlg::OnBeginMoveWnd(WPARAM wParam, LPARAM lParam)
{
	PostMessage(WM_NCLBUTTONDOWN,wParam,lParam);
	return 1 ;
}

int CDacrsUIDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetWindowPos(NULL, 0, 0, 900/*1024*/, 600/*768*/, 0);

	//SetWindowLong(m_hWnd,   GWL_STYLE,GetWindowLong(m_hWnd,   GWL_STYLE)   &   
	//	(~(WS_CAPTION   |   WS_BORDER)));  
	CRect rect ;
	GetClientRect( rect ) ;

	//检测EXE文件所在路径是否包含中文或者空格
	//CheckPathValid( theApp.str_InsPath );

	//初始化RPC Cmd
	InitialRpcCmd();
	LoadListDataInfo();//加载数据库信息

	//TRACE("wwwwwLoadListDataInfo\r\n");
	//TOP
	if( NULL == m_pTitleBar ){
		m_pTitleBar = new CIndTitleBar ;
		m_pTitleBar->Create(this, IDD_DIALOGBAR_TOOL,CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,1) ;
		m_pTitleBar->SetWindowPos(NULL , 0 , 0  , rect.Width() ,72 , SWP_SHOWWINDOW) ;
		m_pTitleBar->ShowWindow(SW_SHOW) ;
	}
	//TRACE("wwwwwCIndTitleBar\r\n");
	//BOTTOM
	if( NULL == m_pStatusBar ){
		m_pStatusBar = new CProgStatusBar ;
		m_pStatusBar->Create(this, IDD_DIALOG_STATUS,CBRS_ALIGN_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,2) ;
		m_pStatusBar->SetWindowPos(NULL , 0 , rect.Height() - 32  , rect.Width() ,32 , SWP_SHOWWINDOW) ;
		m_pStatusBar->ShowWindow(SW_SHOW) ;
	}
	//TRACE("wwwwwCProgStatusBar\r\n");
	if( NULL == m_pMainDlg ){
		m_pMainDlg = new CMainDlg ;
		m_pMainDlg->Create(this, CMainDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,3) ;
		m_pMainDlg->ShowWindow(SW_HIDE) ;
	}
	//TRACE("wwwwwCMainDlg\r\n");
	if( NULL == m_pSendDlg ){
		m_pSendDlg = new CSendDlg ;
		m_pSendDlg->Create(this, CSendDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,4) ;
		m_pSendDlg->ShowWindow(SW_HIDE) ;
	}
	//TRACE("wwwwwCSendDlg\r\n");
	//
	if( NULL == m_pReceiveDlg ){
		m_pReceiveDlg = new CReceiveDlg ;
		m_pReceiveDlg->Create(this, CReceiveDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,5) ;
		m_pReceiveDlg->ShowWindow(SW_HIDE) ;
	}
	//TRACE("wwwwwCReceiveDlg\r\n");
	if( NULL == m_pTradDlg ){
		m_pTradDlg = new CTradDlg ;
		m_pTradDlg->Create(this, CTradDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,6) ;
		m_pTradDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pP2PDlg ){
		m_pP2PDlg = new CP2PDlg ;
		m_pP2PDlg->Create(this, CP2PDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,7) ;
		m_pP2PDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pMortgageTardDlg ){
		m_pMortgageTardDlg = new CMortgageTardDlg ;
		m_pMortgageTardDlg->Create(this, CMortgageTardDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,8) ;
		m_pMortgageTardDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pIpoDlg ){
		m_pIpoDlg = new CIpoDlg ;
		m_pIpoDlg->Create(this, CIpoDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,9) ;
		m_pIpoDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pAddApp ){
		m_pAddApp = new CAddApp ;
		m_pAddApp->Create(this, CAddApp::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,10) ;
		m_pAddApp->ShowWindow(SW_HIDE) ;
	}

	if( NULL == m_pOutGifDlg ){
		m_pOutGifDlg = new COutGifDlg ;
		m_pOutGifDlg->Create(COutGifDlg::IDD , this) ;
		m_pOutGifDlg->ShowWindow(SW_HIDE) ;
	}

	//加入到m_dlgMap
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMainDlg::IDD , m_pMainDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CSendDlg::IDD , m_pSendDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CReceiveDlg::IDD , m_pReceiveDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CTradDlg::IDD , m_pTradDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CP2PDlg::IDD , m_pP2PDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMortgageTardDlg::IDD , m_pMortgageTardDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CIpoDlg::IDD , m_pIpoDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CAddApp::IDD , m_pAddApp)) ;

	//显示主界面
	ShowDialog(CMainDlg::IDD) ;

	return 0;
}
void CDacrsUIDlg::ShowDialog(UINT dlgid) 
{
	CRect rc , rt;
	GetClientRect(&rc);
	std::map< UINT , CDialogBar * >::iterator it ;
	for( it = m_dlgMap.begin() ; it != m_dlgMap.end() ; it++ ) {
		if ( NULL != it->second ) {
			if ( dlgid == it->first ) {
				(it->second)->SetWindowPos(NULL , rc.left , 72  , rc.Width() ,rc.Height() - 72- 32 , SWP_SHOWWINDOW) ;
				(it->second)->ShowWindow( SW_SHOW ) ;
			}else{
				(it->second)->ShowWindow( SW_HIDE ) ;
			}
		}
	}
	p_CurSelDlg = m_dlgMap[dlgid] ;  //当前选择对话框指针
	dlgType = dlgid;

	CPostMsg postkmsg(MSG_USER_UPDATA_UI,0);
	theApp.m_MsgQueue.push(postkmsg); 
}
void CDacrsUIDlg::ShowStateTip(UINT nButtonID)
{
	if ( NULL == m_pTitleBar  ) return ;
	m_pTitleBar->MobileTip(nButtonID , FALSE );
}
void CDacrsUIDlg::DestroyDlg()
{
	if ( NULL != m_pTitleBar ) {
		delete m_pTitleBar ;
		m_pTitleBar = NULL ;
	}
	if ( NULL != m_pStatusBar ) {
		delete m_pStatusBar ;
		m_pStatusBar = NULL ;
	}
	if ( NULL != m_pMainDlg ) {
		delete m_pMainDlg ;
		m_pMainDlg = NULL ;
	}
	if ( NULL != m_pSendDlg ) {
		delete m_pSendDlg ;
		m_pSendDlg = NULL ;
	}
	if ( NULL != m_pReceiveDlg ) {
		delete m_pReceiveDlg ;
		m_pReceiveDlg = NULL ;
	}
	if ( NULL != m_pTradDlg ) {
		delete m_pTradDlg ;
		m_pTradDlg = NULL ;
	}
	if ( NULL != m_pP2PDlg ) {
		delete m_pP2PDlg ;
		m_pP2PDlg = NULL ;
	}
	if ( NULL != m_pMortgageTardDlg ) {
		delete m_pMortgageTardDlg ;
		m_pMortgageTardDlg = NULL ;
	}
	if ( NULL != m_pIpoDlg ) {
		delete m_pIpoDlg ;
		m_pIpoDlg = NULL ;
	}
	if ( NULL != m_pAddApp ) {
		delete m_pAddApp ;
		m_pAddApp = NULL ;
	}
}
//主界面
void CDacrsUIDlg::OnBnClickedButtonMainUI()
{
	ShowDialog(CMainDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_MAIN_UI);
}
//发送
void CDacrsUIDlg::OnBnClickedButtonSend()
{
	ShowDialog(CSendDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_SEND);
}
//接收
void CDacrsUIDlg::OnBnClickedButtonRece()
{
	ShowDialog(CReceiveDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_RECE);
}
//交易详情
void CDacrsUIDlg::OnBnClickedButtonTrad()
{
	ShowDialog(CTradDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_TRAD_INFO);
}
//P2P掷色子
void CDacrsUIDlg::OnBnClickedButtonP2P()
{
	ShowDialog(CP2PDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_P2P);
}
//抵押交易
void CDacrsUIDlg::OnBnClickedButtonMortgage()
{
	ShowDialog(CMortgageTardDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_MORTGAGE);
}
//IPO领币
void CDacrsUIDlg::OnBnClickedButtonDeals()
{
	ShowDialog(CIpoDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_IPO);
}
//添加应用
void CDacrsUIDlg::OnBnClickedButtonAddApp()
{
	ShowDialog(CAddApp::IDD) ;
	ShowStateTip(IDC_BUTTON_ADDAPP);
}

void CDacrsUIDlg::OnBnClickedButtonMin()
{
	ShowWindow(SW_SHOWMINIMIZED);
}

void CDacrsUIDlg::InitialRpcCmd()
{
	//ProductHttpHead(theApp.str_InsPath ,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr);
	CSoyPayHelp::getInstance()->InitialRpcCmd(theApp.m_severip,theApp.m_sendPreHeadstr,theApp.m_sendendHeadstr,theApp.m_rpcport);
}
void    CDacrsUIDlg::SyncAddrInfo()
{
	CString strCommand;
	strCommand.Format(_T("%s"),_T("listaddr"));
	CStringA strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	map<CString,uistruct::LISTADDR_t> pListInfo;
	theApp.m_SqliteDeal.GetListaddrData(&pListInfo);

	/// 数据库中没有的地址要插入
	map<CString,int> SListInfo;
	uistruct::LISTADDR_t listaddr;
	for(int i = 0; i < root.size(); ++i){
		//address
		CString address;
		address.Format( _T("%s") , root[i]["addr"].asCString() ) ;		
		SListInfo[address] = i;

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

		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.FindDB(_T("MYWALLET") ,strSourceData, feild,&addrsql) ;

		if (item == 0 )
		{
			strSourceData.Format(_T("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'") , listaddr.address ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Lebel ) ;
			if (!theApp.m_SqliteDeal.InsertData(_T("MYWALLET") ,strSourceData ) )
			{
				TRACE(_T("MYWALLET数据插入失败!") );
				TRACE("insert:%d\r\n",i);
			}
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("regid = '%s', money = %.8f ,coldig =%d,sign =%d") ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere.Format(_T("address = '%s'") , listaddr.address  ) ;
				if ( !theApp.m_SqliteDeal.Updatabase(_T("MYWALLET") , strSourceData , strWhere ) ){
					TRACE(_T("MYWALLET数据更新失败!") );
				}
			}
		}

	}

	////// 剔除数据库中钱包没有的地址
	map<CString,uistruct::LISTADDR_t>::const_iterator it;
	for (it= pListInfo.begin();it != pListInfo.end();it++)
	{
		if (SListInfo.count(it->first) <= 0)
		{
			CString strSourceData,feild;
			feild.Format(_T("address"));
			strSourceData.Format(_T("%s"),it->first);
			int item = theApp.m_SqliteDeal.DeleteData(_T("MYWALLET"),feild,strSourceData);
		}
	}
}
void  CDacrsUIDlg::LoadListDataInfo()
{
	//加载连表数据,没有的表创建
	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.UpdataAllTable();
	theApp.m_SqliteDeal.UpdataAllTableData(theApp.m_SqliteDeal.isinBlock());
	SyncAddrInfo();
	theApp.cs_SqlData.Unlock();
}
void CDacrsUIDlg::CloseThread()
{
	DWORD exc;
	theApp.m_msgAutoDelete= true;
	theApp.m_blockAutoDelete = true;

	closesocket(theApp.m_blockSock);

	while( ::GetExitCodeThread( theApp.m_msgThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}

	while( ::GetExitCodeThread( theApp.m_hProcessNoUiMsgThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}

	while( ::GetExitCodeThread( theApp.m_hblockThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}

	//delete theApp.pSplashThread;
}

void  CDacrsUIDlg::StopSever()
{
	CString strCommand;
	strCommand.Format(_T("%s"),_T("stop"));
	CStringA strSendData;
	CString strret = _T("Dacrsd server stopping");

	SYSTEMTIME curTime ;
	memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &curTime ) ;
	int RecivetxTimestart =0;
	RecivetxTimestart= UiFun::SystemTimeToTimet(curTime);
	bool nRecStopCmd = false;
	while(TRUE){
		if(!nRecStopCmd) 
		{
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);
		}
		if (strSendData.Find(strret) >=0)
		{
			nRecStopCmd = true;
		}
		GetLocalTime( &curTime ) ;
		int RecivetxTimeLast =0;
		RecivetxTimeLast= UiFun::SystemTimeToTimet(curTime);
		if(!theApp.m_bServerState)
		{
			return;
		}
		if ((RecivetxTimeLast - RecivetxTimestart) > 5)
		{
			return;
		}
		Sleep(5);
	}

}
void CDacrsUIDlg::CloseApp()
{
	EndWaitCursor();
	PostMessage( WM_QUIT ) ; 
	PostMessage( WM_CLOSE ); 	
	//DestroyWindow();
	Sleep(200);
}
void CDacrsUIDlg::OnBnClickedButtonClose()
{
	COut outdlg;
	if ( IDOK == outdlg.DoModal()){
		BeginWaitCursor();
		if ( NULL != m_pOutGifDlg ) {
			CRect rc;
			GetWindowRect(&rc);	
			m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
			m_pOutGifDlg->ShowWindow(SW_SHOW) ;
		}
		::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
		SetTimer( 0x10 , 2000 , NULL ) ; 
	}else{
		;
	}
}

void CDacrsUIDlg::Close() 
{
	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 );
	SetTimer( 0x10 , 3000 , NULL); 
}
void CDacrsUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( 0x10 == nIDEvent ) {
		KillTimer(0x10);
		StopSever();
		CloseThread();

		DestroyDlg();

		if ( NULL != m_pOutGifDlg ) {
			m_pOutGifDlg->ShowWindow(SW_HIDE) ;
			m_pOutGifDlg->LoadGifing(FALSE);
			delete m_pOutGifDlg ;
			m_pOutGifDlg = NULL ;
		}
		theApp.m_bOutApp = TRUE ;
	}
	CDialogEx::OnTimer(nIDEvent);
}
