
// DacrsUIDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDacrsUIDlg �Ի���




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


// CDacrsUIDlg ��Ϣ�������

BOOL CDacrsUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetWindowText("���ܷ�");

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if ( ((CDacrsUIApp*)AfxGetApp())->pSplashThread != NULL)
		((CDacrsUIApp*)AfxGetApp())->pSplashThread->HideSplash();   

	CDC* dc = GetDC();
	int dpiX = GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSX);
	int dpiY = GetDeviceCaps(dc->GetSafeHdc(), LOGPIXELSY);
	SetDPI(dpiX, dpiY);
	ReleaseDC(dc);

	ToTray() ;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDacrsUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}else if (nID==SC_MINIMIZE) 
	{	
		ToTray(); //��С�������̵ĺ��� )
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDacrsUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

	// TODO:  �ڴ������ר�õĴ�������
	SetWindowPos(NULL, 0, 0, 900/*1024*/, 600/*768*/, 0);

	//SetWindowLong(m_hWnd,   GWL_STYLE,GetWindowLong(m_hWnd,   GWL_STYLE)   &   
	//	(~(WS_CAPTION   |   WS_BORDER)));  
	CRect rect ;
	GetClientRect( rect ) ;

	//���EXE�ļ�����·���Ƿ�������Ļ��߿ո�
	//CheckPathValid( theApp.str_InsPath );

	//��ʼ��RPC Cmd
	InitialRpcCmd();

	LoadListDataInfo();//�������ݿ���Ϣ

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
	//���뵽m_dlgMap
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMainDlg::IDD , m_pMainDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CSendDlg::IDD , m_pSendDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CReceiveDlg::IDD , m_pReceiveDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CTradDlg::IDD , m_pTradDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CP2PDlg::IDD , m_pP2PDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMortgageTardDlg::IDD , m_pMortgageTardDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CIpoCoin::IDD , m_pIpoCoinDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CAddApp::IDD , m_pAddApp)) ;

	//��ʾ������
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
	p_CurSelDlg = m_dlgMap[dlgid] ;  //��ǰѡ��Ի���ָ��
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
//������
void CDacrsUIDlg::OnBnClickedButtonMainUI()
{
	ShowDialog(CMainDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_MAIN_UI);
}
//����
void CDacrsUIDlg::OnBnClickedButtonSend()
{
	ShowDialog(CSendDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_SEND);
}
//����
void CDacrsUIDlg::OnBnClickedButtonRece()
{
	ShowDialog(CReceiveDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_RECE);
}
//��������
void CDacrsUIDlg::OnBnClickedButtonTrad()
{
	ShowDialog(CTradDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_TRAD_INFO);
}
//P2P��ɫ��
void CDacrsUIDlg::OnBnClickedButtonP2P()
{
	ShowDialog(CP2PDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_P2P);
}
//��Ѻ����
void CDacrsUIDlg::OnBnClickedButtonMortgage()
{
	ShowDialog(CMortgageTardDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_MORTGAGE);
}
//IPO���
void CDacrsUIDlg::OnBnClickedButtonDeals()
{
	ShowDialog(CIpoCoin::IDD) ;
	ShowStateTip(IDC_BUTTON_IPO);
}
//���Ӧ��
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

	/// ���ݿ���û�еĵ�ַҪ����
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
		//���
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//�Ƿ�֧�����ڿ�
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//�Ƿ�ע��GetLength();
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

	////// �޳����ݿ���Ǯ��û�еĵ�ַ
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
	//������������,û�еı���

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

	/// ���ڷ���Լ״̬
	CString strCond;
	strCond.Format(_T("height > %d and (state = 0 or state = 4)"),(theApp.blocktipheight-500));
	uistruct::P2PBETRECORDLIST pPoolList;
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pPoolList);
	// ���ڽӶ�״̬
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
		strDisplay.Format(_T("��������Щ����δ�ӶĻ��߿���,�ر�ϵͳ�Ӷ���δ��ָ��ʱ���ڿ���,�Զ�����"));
		COut outdlg(NULL, strDisplay,100,_T("����"),_T("�˳�"));
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
  
		case VK_RETURN: //�س�   
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
	// TODO: �ڴ����ר�ô����/����û���
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
		strDisplay.Format(_T("���ļ��Ѿ������Ƿ�Ҫ����"));
		COut outdlg(NULL, strDisplay,100);
		if ( IDOK != outdlg.DoModal()){
			return false;
		}
	}
	
	return TRUE;
}
void CDacrsUIDlg::BakWallet()
{
	// TODO: �ڴ���������������
	
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
		strShowData.Format(_T("Ǯ�����ݳɹ�:%s"),strPath);
		::MessageBox( this->GetSafeHwnd() ,strShowData , _T("��ʾ") , MB_ICONINFORMATION ) ;
	}

}

void CDacrsUIDlg::ToTray() 
{ 
	NOTIFYICONDATA nid; 
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
	nid.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ���� 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy_s(nid.szTip,"dacrs"); //��Ϣ��ʾ�� 
	Shell_NotifyIcon(NIM_ADD,&nid); //�����������ͼ�� 
	ShowWindow(SW_HIDE); //���������� 
} 
//wParam���յ���ͼ���ID����lParam���յ���������Ϊ 
LRESULT CDacrsUIDlg::OnShowTask(WPARAM wParam,LPARAM lParam) 
{ 
	if(wParam!=IDR_MAINFRAME) 
		return 1; 
	switch(lParam) 
	{ 
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա� 
		{ 
			LPPOINT lpoint=new tagPOINT; 
			::GetCursorPos(lpoint);//�õ����λ�� 
			CMenu menu; 
			menu.CreatePopupMenu();//����һ������ʽ�˵� 

			menu.AppendMenu(MF_STRING,WM_DESTROY,"�ر�"); //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ������أ�������������� 
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //ȷ������ʽ�˵���λ�� 
			HMENU hmenu=menu.Detach(); 
			menu.DestroyMenu(); //��Դ���� 
			delete lpoint; 
		} break; 
	case WM_LBUTTONDBLCLK: //˫������Ĵ��� 
		{ 
			this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
			//DeleteTray(); 
		} break; 
	case  WM_LBUTTONDOWN:
		{
			if (IsWindowVisible())
			{
				this->ShowWindow(SW_HIDE);//�򵥵���ʾ���������¶�
			}else{
				this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
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
	nid.uCallbackMessage=WM_SHOWTASK; //�Զ������Ϣ���� 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy_s(nid.szTip,"dacrs"); //��Ϣ��ʾ��Ϊ���ƻ��������ѡ� 
	//Shell_NotifyIcon(NIM_DELETE,&nid); //��������ɾ��ͼ�� 
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
		::MessageBox( this->GetSafeHwnd() ,_T("�Ѿ��ӹ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
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
		MessageBox(_T("Ǯ��û�м���"));
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
			MessageBox(_T("Ǯ������ʧ��"));
			return;
		}
	}else
	{
		MessageBox(_T("Ǯ������ʧ��"));
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
	/// �Լ�Ǯ����ַ����  walletaddr
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

	/// ���浽�ļ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		strShowData.Format(_T("����˽Կ�ɹ�:%s"),strPath);
		AddImportWalletAndBookAddr(strPath);
		::MessageBox( this->GetSafeHwnd() ,strShowData , _T("��ʾ") , MB_ICONINFORMATION ) ;
	}
}
void CDacrsUIDlg:: ImportPrvieKey()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���������������
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
	ofn.lpstrFilter =  "�ı��ļ�(*.smc)\0*.smc\0�����ļ�(*.*)\0*.*\0\0";  
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
					MessageBox(_T("����Ǯ���ɹ�����������Ǯ��"));
					//ClosWallet();
					//((CDacrsUIDlg*)(this->GetParent()))->Close();
					ClosWalletWind();
				}else{
					MessageBox(_T("����Ǯ��ʧ��"));
				}
				
			}else
			{
				MessageBox(_T("����Ǯ��ʧ��"));
			}
	}
}

void CDacrsUIDlg::OnUpdataState(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
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