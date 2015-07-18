
// DacrsUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "Out.h"
#include "afxdialogex.h"
#include "ChangeDPI.h"
#include "SetAppFee.h"
#include "EncryptWallet.h"
#include "ChangPassWord.h"

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
	m_pAddApp  = NULL  ;
	dlgType = 0;
	m_pOutGifDlg =  NULL ;
	m_pRPCDlg = NULL;
	m_pIpoCoinDlg = NULL;
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
	ON_COMMAND(ID__BAKWALLET, &CDacrsUIDlg::BakWallet)
	ON_COMMAND(ID__SET, &CDacrsUIDlg::SetAppFee)
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_COMMAND(ID__ENCRYPTWALLET, &CDacrsUIDlg::encryptwallet)
	ON_COMMAND(ID_CHANGEPASSWORD, &CDacrsUIDlg::walletpassphrasechange)
	ON_COMMAND(ID__LOCK, &CDacrsUIDlg::LockWallet)
	ON_COMMAND(ID__EXPORTPRIVEKEY, &CDacrsUIDlg::ExportPriveKey)
	ON_COMMAND(ID__IMPORTPRIVEKEY, &CDacrsUIDlg::ImportPrvieKey)
	ON_UPDATE_COMMAND_UI(ID__SET, &CDacrsUIDlg::OnUpdataState)
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
	SetWindowText("智能坊");

	// TODO: 在此添加额外的初始化代码
	if ( ((CDacrsUIApp*)AfxGetApp())->pSplashThread != NULL)
		((CDacrsUIApp*)AfxGetApp())->pSplashThread->HideSplash();   

	CDC* dc = GetDC();
	int dpiX = GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSX);
	int dpiY = GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSY);
	SetDPI(dpiX, dpiY);
	ReleaseDC(dc);

	ToTray() ;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDacrsUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else if (nID==SC_MINIMIZE) 
	{	
		ToTray(); //最小化到托盘的函数 )
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

	if ( NULL == m_pRPCDlg ) {
		m_pRPCDlg = new CRPCDlg;
		m_pRPCDlg->Create(CRPCDlg::IDD,this);
		m_pRPCDlg->ShowWindow(SW_HIDE);
	}
	LogPrint("INFO","Create RpdDlg end\n");
	//TOP
	if( NULL == m_pTitleBar ){
		m_pTitleBar = new CIndTitleBar ;
		m_pTitleBar->Create(this, IDD_DIALOGBAR_TOOL,CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,1) ;
		m_pTitleBar->SetWindowPos(NULL , 0 , 0  , rect.Width() ,72 , SWP_SHOWWINDOW) ;
		m_pTitleBar->ShowWindow(SW_SHOW) ;
	}
		LogPrint("INFO","Create m_pTitleBar end\n");
	//TRACE("wwwwwCIndTitleBar\r\n");
	//BOTTOM
	if( NULL == m_pStatusBar ){
		m_pStatusBar = new CProgStatusBar ;
		m_pStatusBar->Create(this, IDD_DIALOG_STATUS,CBRS_ALIGN_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,2) ;
		m_pStatusBar->SetWindowPos(NULL , 0 , rect.Height() - 32  , rect.Width() ,32 , SWP_SHOWWINDOW) ;
		m_pStatusBar->ShowWindow(SW_SHOW) ;
	}
	LogPrint("INFO","Create m_pStatusBar end\n");
	//TRACE("wwwwwCProgStatusBar\r\n");
	if( NULL == m_pMainDlg ){
		m_pMainDlg = new CMainDlg ;
		m_pMainDlg->Create(this, CMainDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,3) ;
		m_pMainDlg->ShowWindow(SW_HIDE) ;
	}
		LogPrint("INFO","Create m_pMainDlg end\n");
	//TRACE("wwwwwCMainDlg\r\n");
	if( NULL == m_pSendDlg ){
		m_pSendDlg = new CSendDlg ;
		m_pSendDlg->Create(this, CSendDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,4) ;
		m_pSendDlg->ShowWindow(SW_HIDE) ;
	}
		LogPrint("INFO","Create m_pSendDlg end\n");
	//TRACE("wwwwwCSendDlg\r\n");
	//
	if( NULL == m_pReceiveDlg ){
		m_pReceiveDlg = new CReceiveDlg ;
		m_pReceiveDlg->Create(this, CReceiveDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,5) ;
		m_pReceiveDlg->ShowWindow(SW_HIDE) ;
	}
	LogPrint("INFO","Create m_pReceiveDlg end\n");
	//TRACE("wwwwwCReceiveDlg\r\n");
	if( NULL == m_pTradDlg ){
		m_pTradDlg = new CTradDlg ;
		m_pTradDlg->Create(this, CTradDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,6) ;
		m_pTradDlg->ShowWindow(SW_HIDE) ;
	}
	LogPrint("INFO","Create m_pTradDlg end\n");
	if( NULL == m_pP2PDlg ){
		m_pP2PDlg = new CP2PDlg ;
		m_pP2PDlg->Create(this, CP2PDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,7) ;
		m_pP2PDlg->ShowWindow(SW_HIDE) ;
	}
	LogPrint("INFO","Create m_pP2PDlg end\n");
	if( NULL == m_pMortgageTardDlg ){
		m_pMortgageTardDlg = new CMortgageTardDlg ;
		m_pMortgageTardDlg->Create(this, CMortgageTardDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,8) ;
		m_pMortgageTardDlg->ShowWindow(SW_HIDE) ;
	}
	LogPrint("INFO","Create m_pMortgageTardDlg end\n");
	//if( NULL == m_pIpoDlg ){
	//	m_pIpoDlg = new CIpoDlg ;
	//	m_pIpoDlg->Create(this, CIpoDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,9) ;
	//	m_pIpoDlg->ShowWindow(SW_HIDE) ;
	//}

	if( NULL == m_pIpoCoinDlg ){
		m_pIpoCoinDlg = new CIpoCoin ;
		m_pIpoCoinDlg->Create(this, CIpoCoin::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,11) ;
		m_pIpoCoinDlg->ShowWindow(SW_HIDE) ;
	}

	LogPrint("INFO","Create m_pIpoCoinDlg end\n");
	if( NULL == m_pAddApp ){
		m_pAddApp = new CAddApp ;
		m_pAddApp->Create(this, CAddApp::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,10) ;
		m_pAddApp->ShowWindow(SW_HIDE) ;
	}
	LogPrint("INFO","Create m_pAddApp end \n");
	if( NULL == m_pOutGifDlg ){
		m_pOutGifDlg = new COutGifDlg ;
		m_pOutGifDlg->Create(COutGifDlg::IDD , this) ;
		m_pOutGifDlg->ShowWindow(SW_HIDE) ;
	}
	LogPrint("INFO","Create m_pOutGifDlg end\n");
	//加入到m_dlgMap
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMainDlg::IDD , m_pMainDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CSendDlg::IDD , m_pSendDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CReceiveDlg::IDD , m_pReceiveDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CTradDlg::IDD , m_pTradDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CP2PDlg::IDD , m_pP2PDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMortgageTardDlg::IDD , m_pMortgageTardDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CIpoCoin::IDD , m_pIpoCoinDlg)) ;
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
	if ( NULL != m_pIpoCoinDlg ) {
		delete m_pIpoCoinDlg ;
		m_pIpoCoinDlg = NULL ;
	}
	if ( NULL != m_pAddApp ) {
		delete m_pAddApp ;
		m_pAddApp = NULL ;
	}
	if ( NULL != m_pRPCDlg ) {
		delete m_pRPCDlg ;
		m_pRPCDlg = NULL ;
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
	ShowDialog(CIpoCoin::IDD) ;
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
	//ShowWindow(SW_SHOWMINIMIZED);
	ToTray();
}

void CDacrsUIDlg::InitialRpcCmd()
{
	//ProductHttpHead(theApp.str_InsPath ,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr);
	CSoyPayHelp::getInstance()->InitialRpcCmd(theApp.m_severip,theApp.m_sendPreHeadstr,theApp.m_sendendHeadstr,theApp.m_rpcport);
}
void    CDacrsUIDlg::SyncAddrInfo()
{
	CString strCommand;
	strCommand.Format(_T("%s"),"listaddr");
	CStringA strShowData =_T("");

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	map<CString,uistruct::LISTADDR_t> pListInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), (&pListInfo));

	/// 数据库中没有的地址要插入
	map<CString,int> SListInfo;
	uistruct::LISTADDR_t listaddr;
	for(unsigned int i = 0; i < root.size(); ++i){
		//address
		CString address;
		address.Format( _T("%s") , root[i]["addr"].asCString() ) ;		
		SListInfo[address] = i;

		memset(&listaddr , 0 , sizeof(uistruct::LISTADDR_t));
		//address
		CString strData;
		strData.Format( _T("%s") , root[i]["addr"].asCString() ) ;
	//	strncpy(listaddr.address  , strData , strlen(strData) > sizeof(listaddr.address) ? sizeof(listaddr.address):strlen(strData));
		//strncpy_s((char*)listaddr.address  , strData.GetBuffer(strData.GetLength()) , strlen(strData) > sizeof(listaddr.address) ? sizeof(listaddr.address):strlen(strData));
		strncpy_s(listaddr.address  , strData , strlen(strData) > sizeof(listaddr.address) ? sizeof(listaddr.address):strlen(strData));
		//RegID
		strData.Format( _T("%s") , root[i]["regid"].asCString() ) ;
		strncpy_s(listaddr.RegID  , strData , strlen(strData) > sizeof(listaddr.RegID) ? sizeof(listaddr.RegID):strlen(strData));
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
	
		CString strCond;
		strCond.Format(_T(" address = '%s' "), listaddr.address);
		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		if (strlen(addrsql.address) == 0 )
		{
			CString strData;
			strData.Format(_T("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'") , listaddr.address ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Label ) ;
			if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strData ))
			{
				TRACE("INSERT t_wallet_address data failed!\n");
			}
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("reg_id = '%s', money = %.8f ,cold_dig =%d, sign =%d") ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere.Format(_T("address = '%s'") , listaddr.address  ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE("UPDATE t_wallet_address data failed!\n");
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
			CString strCond;
			strCond.Format(_T(" address='%s' "), it->first);
			int item = theApp.m_SqliteDeal.DeleteTableItem(_T("t_wallet_address"), strCond);
		}
	}
}
void  CDacrsUIDlg::LoadListDataInfo()
{
	//加载连表数据,没有的表创建

	SyncAddrInfo();
	//theApp.m_SqliteDeal.UpdataAllTableData();
	
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
	RecivetxTimestart=(int) UiFun::SystemTimeToTimet(curTime);
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
		RecivetxTimeLast= (int)UiFun::SystemTimeToTimet(curTime);
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

bool  CDacrsUIDlg::IsP2pBetFinsh()
{

	/// 处于发赌约状态
	CString strCond;
	strCond.Format(_T("height > %d and (state = 0 or state = 4)"),(theApp.blocktipheight-500));
	uistruct::P2PBETRECORDLIST pPoolList;
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pPoolList);
	// 处于接赌状态
	uistruct::P2PBETRECORDLIST pPoolList1;
	strCond.Format(_T("height > %d and (state = 1 or state = 5)"),(theApp.blocktipheight-10));
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pPoolList1);
	if (pPoolList.size() != 0 && pPoolList1.size() != 0)
	{
		return false;
	}
	return true;
}
void  CDacrsUIDlg::ClosWallet()
{
	COut outdlg;
	if ( IDOK == outdlg.DoModal()){
		BeginWaitCursor();
		if ( NULL != m_pOutGifDlg ) {
			CRect rc;
			GetWindowRect(&rc);	
			m_pOutGifDlg->LoadGifing(TRUE);
			m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
			m_pOutGifDlg->ShowWindow(SW_SHOW) ;
		}
		::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
		SetTimer( 0x10 , 2000 , NULL ) ; 
	}else{
		;
	}
}
void CDacrsUIDlg::OnBnClickedButtonClose()
{
	if (!IsP2pBetFinsh())
	{
		CString strDisplay;
		strDisplay.Format(_T("猜你妹有些单还未接赌或者开奖,关闭系统接赌了未在指定时间内开奖,自动判输"));
		COut outdlg(NULL, strDisplay,100,_T("继续"),_T("退出"));
		if ( IDOK == outdlg.DoModal()){
			ClosWallet();
		}
	}else{
		ClosWallet();
	}
	//COut outdlg;
	//if ( IDOK == outdlg.DoModal()){
	//	BeginWaitCursor();
	//	if ( NULL != m_pOutGifDlg ) {
	//		CRect rc;
	//		GetWindowRect(&rc);	
	//		m_pOutGifDlg->LoadGifing(TRUE);
	//		m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
	//		m_pOutGifDlg->ShowWindow(SW_SHOW) ;
	//	}
	//	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
	//	SetTimer( 0x10 , 2000 , NULL ) ; 
	//}else{
	//	;
	//}
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

BOOL CDacrsUIDlg::PreTranslateMessage(MSG* pMsg)  
{  
  
	if(pMsg->message == WM_KEYDOWN) 
	{  
  
		switch(pMsg->wParam) 
		{  
  
		case VK_RETURN: //回车   
			return TRUE;  
  
		case VK_ESCAPE: //ESC  
			return TRUE;  
  
		}  
  
	}  
	return CDialog::PreTranslateMessage(pMsg);  
}  

void CDacrsUIDlg::OnOk()
{
}

LRESULT CDacrsUIDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch(message) 
	{
	case WM_COMMAND:
		{
			if ( ID_RPC_CMD == LOWORD(wParam) ) {
				CRect rcWindow;
				GetWindowRect(&rcWindow);
				if ( NULL != m_pRPCDlg ){
					m_pRPCDlg->MoveWindow(rcWindow.right,rcWindow.top,300,rcWindow.Height()-8);
					m_pRPCDlg->ShowWindow(SW_SHOW);
				}
			}
		}
		break;
	default:
		break ;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
bool CDacrsUIDlg::GetFileName(CString &fileName,CString strexe )
{
	int pos = fileName.Find(".",0);
	if (pos <0)
	{
		fileName.AppendFormat(strexe);
	}
	pos = fileName.Find(".",0);
	fileName = fileName.Left(pos);
	fileName.AppendFormat(strexe);
	if(PathFileExistsA(fileName)){
		CString strDisplay;
		strDisplay.Format(_T("此文件已经存在是否要覆盖"));
		COut outdlg(NULL, strDisplay,100);
		if ( IDOK != outdlg.DoModal()){
			return false;
		}
	}
	
	return TRUE;
}
void CDacrsUIDlg::BakWallet()
{
	// TODO: 在此添加命令处理程序代码
	
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("*.dat||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		if (!GetFileName(strPath,_T(".dat")))
		{
			return;
		}

		//strPath.AppendFormat(_T(".dat"));
		CString strCommand;
		strCommand.Format(_T("%s %s"),_T("backupwallet"),strPath);
		CStringA strSendData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);

		CString strShowData;
		strShowData.Format(_T("钱包备份成功:%s"),strPath);
		::MessageBox( this->GetSafeHwnd() ,strShowData , _T("提示") , MB_ICONINFORMATION ) ;
	}

}

void CDacrsUIDlg::ToTray() 
{ 
	NOTIFYICONDATA nid; 
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
	nid.uCallbackMessage=WM_SHOWTASK;//自定义的消息名称 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy_s(nid.szTip,"dacrs"); //信息提示条 
	Shell_NotifyIcon(NIM_ADD,&nid); //在托盘区添加图标 
	ShowWindow(SW_HIDE); //隐藏主窗口 
} 
//wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
LRESULT CDacrsUIDlg::OnShowTask(WPARAM wParam,LPARAM lParam) 
{ 
	if(wParam!=IDR_MAINFRAME) 
		return 1; 
	switch(lParam) 
	{ 
	case WM_RBUTTONUP://右键起来时弹出快捷菜单，这里只有一个“关闭” 
		{ 
			LPPOINT lpoint=new tagPOINT; 
			::GetCursorPos(lpoint);//得到鼠标位置 
			CMenu menu; 
			menu.CreatePopupMenu();//声明一个弹出式菜单 

			menu.AppendMenu(MF_STRING,WM_DESTROY,"关闭"); //增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已隐藏），将程序结束。 
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //确定弹出式菜单的位置 
			HMENU hmenu=menu.Detach(); 
			menu.DestroyMenu(); //资源回收 
			delete lpoint; 
		} break; 
	case WM_LBUTTONDBLCLK: //双击左键的处理 
		{ 
			this->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿 
			//DeleteTray(); 
		} break; 
	case  WM_LBUTTONDOWN:
		{
			if (IsWindowVisible())
			{
				this->ShowWindow(SW_HIDE);//简单的显示主窗口完事儿
			}else{
				this->ShowWindow(SW_SHOW);//简单的显示主窗口完事儿 
			}
			 
		}
	default: break; 
	} 
	return 0; 
} 
void CDacrsUIDlg::DeleteTray() 
{ 
	NOTIFYICONDATA nid; 
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
	nid.uCallbackMessage=WM_SHOWTASK; //自定义的消息名称 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy_s(nid.szTip,"dacrs"); //信息提示条为“计划任务提醒” 
	//Shell_NotifyIcon(NIM_DELETE,&nid); //在托盘区删除图标 
} 
void CDacrsUIDlg::SetAppFee()
{
	CSetAppFee setappdlg;
	if (setappdlg.DoModal() ==IDOK)
	{
		return;
	}
	return;
}
void CDacrsUIDlg::encryptwallet()
{
	if (theApp.HaveLocked)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("已经加过密") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	CEncryptWallet enwalletdlg;
	if (enwalletdlg.DoModal() ==IDOK)
	{
		return;
	}
	return;
}
void CDacrsUIDlg::walletpassphrasechange(){

	CChangPassWord changwalletdlg;
	if (changwalletdlg.DoModal() ==IDOK)
	{
		return;
	}
	return;
}
void CDacrsUIDlg:: LockWallet()
{
	if (!theApp.HaveLocked)
	{
		MessageBox(_T("钱包没有加锁"));
		return;
	}
	CString strCommand;
	strCommand.Format(_T("%s"),_T("walletlock"));
	CStringA strShowData =_T("");

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData ==_T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;

	if (strShowData.Find("walletlock") > 0)
	{
		bool isEntryp = root["walletlock"].asBool();
		if (!isEntryp)
		{
			MessageBox(_T("钱包锁定失败"));
			return;
		}
	}else
	{
		MessageBox(_T("钱包锁定失败"));
	}

}
void CDacrsUIDlg::WriteExportWalletAndBookAddr(CString fileName)
{
	CString strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(fileName);
	if (strFile == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 

	if (!reader.parse(strFile.GetString(), root)) 
		return;

	if (strFile.Find("walletaddr") >=0)
	{
		Json::Value addrValue = root["walletaddr"]; 
		for(int i =0;i<(int)addrValue.size();i++){
			Json::Value obj = addrValue[i];

			CString addr = obj["addr"].asCString();
			CString label =obj["label"].asCString();
			CString regid = obj["reg_id"].asCString();
			double money = obj["money"].asDouble();
			int cold = obj["cold_dig"].asInt();
			int sig = obj["sign"].asInt();

			CString conditon = _T("");
			conditon.Format(_T("address = '%s'"),addr);
			uistruct::LISTADDR_t pAddr;
			theApp.m_SqliteDeal.GetWalletAddressItem(conditon,&pAddr);
			if (strlen(pAddr.address) == 0)
			{
				CString strSourceData= _T("");
				strSourceData.Format(_T("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'") , addr ,regid ,money ,cold ,sig,label ) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strSourceData ) )
				{
					TRACE("Insert t_wallet_address error!\n");
				}

			}else{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("label = '%s'") ,label) ;
				strWhere.Format(_T("address = '%s'") , addr ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}
			}
		}
	}

	if (strFile.Find("wbookaddr") >=0)
	{
		Json::Value addrValue = root["bookaddr"]; 
		for(int i =0;i<(int)addrValue.size();i++){
			Json::Value obj = addrValue[i];
			CString addr = obj["addr"].asCString();
			CString label =obj["label"].asCString();
			CString conditon = _T("");
			conditon.Format(_T("address = '%s'"),addr);
			uistruct::ADDRBOOK_t  pAddr;
			theApp.m_SqliteDeal.GetAddressBookItem(conditon,&pAddr);
			if (strlen(pAddr.address) == 0)
			{
				CString strSourceData= _T("");
				strSourceData.Format(_T("'%s' , '%s' ")  ,label, addr) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_address_book") ,strSourceData ) )
				{
					TRACE("Insert t_wallet_address error!\n");
				}

			}else{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("Label = '%s'") ,label) ;
				strWhere.Format(_T("address = '%s'") , addr ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_address_book") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}
			}
		}
	}
}

void CDacrsUIDlg::AddImportWalletAndBookAddr(CString fileName)
{
	CString strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(fileName);
	if (strFile == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 

	if (!reader.parse(strFile.GetString(), root)) 
		return;
	/// 自己钱包地址保存  walletaddr
	Json::Value walletaddr; 
	map<CString,uistruct::LISTADDR_t> pListInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), (&pListInfo));
	if (pListInfo.size() != 0)
	{
		Json::Value Array;
		Json::Value itemValue;
		map<CString,uistruct::LISTADDR_t>::iterator item = pListInfo.begin();
		for (;item != pListInfo.end();item++)
		{
			if (strlen(item->second.Label) != 0)
			{
				itemValue["addr"]=item->second.address;
				itemValue["label"] = item->second.Label;
				itemValue["reg_id"] = item->second.RegID;
				itemValue["money"] = item->second.fMoney;
				itemValue["cold_dig"] = item->second.nColdDig;
				itemValue["sign"] = item->second.bSign;
				Array.append(itemValue);
			}
		}
	root["walletaddr"] = Array;
	}

	map<CString,uistruct::ADDRBOOK_t> pAddrBookMap;
	theApp.m_SqliteDeal.GetAddressBookList(_T(" 1=1 "),(&pAddrBookMap));

	if (pAddrBookMap.size() != 0)
	{
		Json::Value Array;
		Json::Value itemValue;
		map<CString,uistruct::ADDRBOOK_t>::iterator item = pAddrBookMap.begin();
		for (;item != pAddrBookMap.end();item++)
		{
			//if (item->second.label != _T(""))
			{
				itemValue["addr"]=item->second.address.GetString();
				itemValue["label"] = item->second.label.GetString();
				Array.append(itemValue);
			}
		}
		root["wbookaddr"] = Array;
	}

	/// 保存到文件
	CStdioFile  File;
	File.Open(fileName,CFile::modeWrite | CFile::modeCreate); 
	string strfile = root.toStyledString();
	File.WriteString(strfile.c_str());
	File.Close();

}
void CDacrsUIDlg:: ExportPriveKey()
{
	if (theApp.IsLockWallet())
	{
		return ;
	}
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,_T("*.smc||"));
	if (IDOK == dlg.DoModal())
	{
		CString strPath = dlg.GetPathName();
		if (!GetFileName(strPath,_T(".smc")))
		{
			return;
		}
		//strPath.AppendFormat(_T(".smc"));
		CString strCommand;
		strCommand.Format(_T("%s %s"),_T("dumpwallet"),strPath);
		CStringA strSendData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);
		CString strShowData;
		strShowData.Format(_T("导出私钥成功:%s"),strPath);
		AddImportWalletAndBookAddr(strPath);
		::MessageBox( this->GetSafeHwnd() ,strShowData , _T("提示") , MB_ICONINFORMATION ) ;
	}
}
void CDacrsUIDlg:: ImportPrvieKey()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加命令处理程序代码
	if (theApp.IsLockWallet())
	{
		return ;
	}
	OPENFILENAME ofn;
	char szFile[MAX_PATH];
	ZeroMemory(&ofn,sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = TEXT('\0'); 
	ofn.nMaxFile = sizeof(szFile); 
	ofn.lpstrFilter =  "文本文件(*.smc)\0*.smc\0所有文件(*.*)\0*.*\0\0";  
	ofn.nFilterIndex = 1; 
	ofn.lpstrFileTitle = NULL; 
	ofn.nMaxFileTitle = 0; 
	ofn.lpstrInitialDir = NULL;	ofn.hwndOwner = m_hWnd; 
	ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (::GetOpenFileName(&ofn))
	{
			CString strPath = ofn.lpstrFile;
			CString strCommand;
			strCommand.Format(_T("%s %s"),_T("importwallet"),strPath);
			CStringA strSendData;
	
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);	

			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(strSendData.GetString(), root)) 
				return  ;
			if (strSendData.Find(_T("imorpt key size")) >=0)
			{
				int size = root["imorpt key size"].asInt();
				if (size > 0)
				{
					WriteExportWalletAndBookAddr(strPath);
					MessageBox(_T("导入钱包成功请重新启动钱包"));
					//ClosWallet();
					//((CDacrsUIDlg*)(this->GetParent()))->Close();
					ClosWalletWind();
				}else{
					MessageBox(_T("导入钱包失败"));
				}
				
			}else
			{
				MessageBox(_T("导入钱包失败"));
			}
	}
}

void CDacrsUIDlg::OnUpdataState(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
//	pCmdUI->Enable(FALSE); 
}

void  CDacrsUIDlg::ClosWalletWind()
{
		BeginWaitCursor();
		if ( NULL != m_pOutGifDlg ) {
			CRect rc;
			GetWindowRect(&rc);	
			m_pOutGifDlg->LoadGifing(TRUE);
			m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
			m_pOutGifDlg->ShowWindow(SW_SHOW) ;
		}
		::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_OUT , 0 , 0 ) ;
		SetTimer( 0x10 , 2000 , NULL ) ; 
}