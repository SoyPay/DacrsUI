
// DacrsUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "afxdialogex.h"

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
	m_pIpoDlg  = NULL  ;
	dlgType = 0;
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if ( ((CDacrsUIApp*)AfxGetApp())->pSplashThread != NULL)
		((CDacrsUIApp*)AfxGetApp())->pSplashThread->HideSplash();   

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	CheckPathValid( theApp.str_InsPath );

	//��ʼ��RPC Cmd
	InitialRpcCmd();
	LoadListDataInfo();//�������ݿ���Ϣ

	//TOP
	if( NULL == m_pTitleBar ){
		m_pTitleBar = new CIndTitleBar ;
		m_pTitleBar->Create(this, IDD_DIALOGBAR_TOOL,CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,1) ;
		m_pTitleBar->SetWindowPos(NULL , 0 , 0  , rect.Width() ,72 , SWP_SHOWWINDOW) ;
		m_pTitleBar->ShowWindow(SW_SHOW) ;
	}
	//BOTTOM
	if( NULL == m_pStatusBar ){
		m_pStatusBar = new CProgStatusBar ;
		m_pStatusBar->Create(this, IDD_DIALOG_STATUS,CBRS_ALIGN_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,2) ;
		m_pStatusBar->SetWindowPos(NULL , 0 , rect.Height() - 32  , rect.Width() ,32 , SWP_SHOWWINDOW) ;
		m_pStatusBar->ShowWindow(SW_SHOW) ;
		m_pStatusBar->LoadGifing(TRUE);
	}
	if( NULL == m_pMainDlg ){
		m_pMainDlg = new CMainDlg ;
		m_pMainDlg->Create(this, CMainDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,3) ;
		m_pMainDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pSendDlg ){
		m_pSendDlg = new CSendDlg ;
		m_pSendDlg->Create(this, CSendDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,4) ;
		m_pSendDlg->ShowWindow(SW_HIDE) ;
	}
	//
	if( NULL == m_pReceiveDlg ){
		m_pReceiveDlg = new CReceiveDlg ;
		m_pReceiveDlg->Create(this, CReceiveDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,5) ;
		m_pReceiveDlg->ShowWindow(SW_HIDE) ;
	}
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

	//���뵽m_dlgMap
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMainDlg::IDD , m_pMainDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CSendDlg::IDD , m_pSendDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CReceiveDlg::IDD , m_pReceiveDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CTradDlg::IDD , m_pTradDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CP2PDlg::IDD , m_pP2PDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMortgageTardDlg::IDD , m_pMortgageTardDlg)) ;
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CIpoDlg::IDD , m_pIpoDlg)) ;

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
	theApp.UpdataUIData();
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
	ShowDialog(CIpoDlg::IDD) ;
	ShowStateTip(IDC_BUTTON_IPO);
}
//���Ӧ��
void CDacrsUIDlg::OnBnClickedButtonAddApp()
{
	ShowStateTip(IDC_BUTTON_ADDAPP);
}
void CDacrsUIDlg::OnBnClickedButtonClose()
{
	//CloseThread();
	//StopSever();

	DestroyDlg();
	PostMessage( WM_QUIT ) ; 
	PostMessage( WM_CLOSE ); 	
	TRACE("DestroyWindow()");
	DestroyWindow();
	Sleep(200) ;
}
void CDacrsUIDlg::OnBnClickedButtonMin()
{
	ShowWindow(SW_SHOWMINIMIZED);
}

void CDacrsUIDlg::CheckPathValid(const CStringA& strDir)
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
		MessageBox(_T("���򲻿��Է��ں��пո��Ŀ¼��\r\n"));
		exit(0);
	}
}

void CDacrsUIDlg::InitialRpcCmd()
{
	//ProductHttpHead(theApp.str_InsPath ,m_strServerCfgFileName,m_rpcport,m_sendPreHeadstr,m_sendendHeadstr);
	CSoyPayHelp::getInstance()->InitialRpcCmd(theApp.m_severip,theApp.m_sendPreHeadstr,theApp.m_sendendHeadstr,theApp.m_rpcport);
}

void  CDacrsUIDlg::LoadListDataInfo()
{
	//������������,û�еı���
	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.UpdataAllTable();
	theApp.m_SqliteDeal.UpdataAllTableData(theApp.m_SqliteDeal.isinBlock());
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
		Sleep(1000);
	}

	while( ::GetExitCodeThread( theApp.m_hblockThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(1000);
	}
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
	while(TRUE){
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);
		if (strSendData.Find(strret) >=0)
		{
			return;
		}
		GetLocalTime( &curTime ) ;
		int RecivetxTimeLast =0;
		RecivetxTimeLast= UiFun::SystemTimeToTimet(curTime);
		if ((RecivetxTimeLast - RecivetxTimestart) > 10)
		{
			return;
		}
	}

}