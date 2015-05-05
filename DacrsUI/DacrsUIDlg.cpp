
// DacrsUIDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"
#include "afxdialogex.h"

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
	ON_BN_CLICKED(IDC_BUTTON_MAIN_UI, &CDacrsUIDlg::OnBnClickedButtonMainUI)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CDacrsUIDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECE, &CDacrsUIDlg::OnBnClickedButtonRece)
	ON_BN_CLICKED(IDC_BUTTON_TRAD_INFO, &CDacrsUIDlg::OnBnClickedButtonTrad)
	ON_BN_CLICKED(IDC_BUTTON_P2P, &CDacrsUIDlg::OnBnClickedButtonP2P)
	ON_BN_CLICKED(IDC_BUTTON_MORTGAGE, &CDacrsUIDlg::OnBnClickedButtonMortgage)
	ON_BN_CLICKED(IDC_BUTTON_IPO, &CDacrsUIDlg::OnBnClickedButtonDeals)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDacrsUIDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CDacrsUIDlg::OnBnClickedButtonMin)
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
	}
	if( NULL == m_pMainDlg ){
		m_pMainDlg = new CMainDlg ;
		m_pMainDlg->Create(IDD_DIALOG_MAIN , this) ;
		m_pMainDlg->ShowWindow(SW_HIDE) ;
	}

	//加入到m_dlgMap
	m_dlgMap.insert( std::map<UINT , CDialog *>::value_type( IDD_DIALOG_MAIN , m_pMainDlg)) ; 

	//显示主界面
	ShowDialog(IDD_DIALOG_MAIN) ;

	return 0;
}
void CDacrsUIDlg::ShowDialog(UINT dlgid) 
{
	CRect rc , rt;
	GetClientRect(&rc);
	std::map< UINT , CDialog * >::iterator it ;
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
}
//主界面
void CDacrsUIDlg::OnBnClickedButtonMainUI()
{
	
    AfxMessageBox("主界面!") ;
}
//发送
void CDacrsUIDlg::OnBnClickedButtonSend()
{
	AfxMessageBox("发送!") ;
}
//接收
void CDacrsUIDlg::OnBnClickedButtonRece()
{
	AfxMessageBox("接收!") ;
}
//交易详情
void CDacrsUIDlg::OnBnClickedButtonTrad()
{
	AfxMessageBox("交易详情!") ;
}
//P2P掷色子
void CDacrsUIDlg::OnBnClickedButtonP2P()
{
	AfxMessageBox("P2P掷色子!") ;
}
//抵押交易
void CDacrsUIDlg::OnBnClickedButtonMortgage()
{
	AfxMessageBox("抵押交易!") ;
}
//IPO领币
void CDacrsUIDlg::OnBnClickedButtonDeals()
{
	AfxMessageBox("IPO领币!") ;
}
void CDacrsUIDlg::OnBnClickedButtonClose()
{
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