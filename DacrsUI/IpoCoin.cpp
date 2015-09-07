// IpoCoin.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "IpoCoin.h"
#include "afxdialogex.h"


// CIpoCoin 对话框

IMPLEMENT_DYNAMIC(CIpoCoin, CDialogBar)

CIpoCoin::CIpoCoin()
{
	m_pBmp = NULL ;
}

CIpoCoin::~CIpoCoin()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CIpoCoin::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ADDR , m_strTx1);
	DDX_Control(pDX, IDC_LIST_SHOW , m_listCtrl);
	DDX_Control(pDX, IDC_STATIC_AMOUNT , m_strTx2);
	DDX_Control(pDX, IDC_BUTTON_DRAWAL , m_rBtnSend);
	DDX_Control(pDX, IDC_BUTTON_QUERY , m_rQueryAmout);
}


BEGIN_MESSAGE_MAP(CIpoCoin, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_DRAWAL, &CIpoCoin::OnBnClickedButtonDrawal)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_QUERY, &CIpoCoin::OnBnClickedButtonQuery)
END_MESSAGE_MAP()


// CIpoCoin 消息处理程序
void CIpoCoin::SetBkBmpNid( UINT nBitmapIn ) 
{
	if( NULL != m_pBmp ) {
		::DeleteObject( m_pBmp ) ;
		m_pBmp = NULL ;
	}
	m_pBmp	= NULL ;
	HINSTANCE	hInstResource = NULL;	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);
	if( NULL != hInstResource ) {
		m_pBmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
	}
}

int CIpoCoin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_IPO_BJ);
	return 0;
}


BOOL CIpoCoin::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect   rect; 
	GetClientRect(&rect); 

	if(m_pBmp   !=   NULL) { 
		BITMAP   bm; 
		CDC   dcMem; 
		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
		pDC-> StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


BOOL CIpoCoin::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(0);
		m_rBtnSend.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnSend.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnSend.SetWindowText(UiFun::UI_LoadString("IPO_MODULE" , "IPO_WITHDRAWALS" ,theApp.gsLanguage)) ;
		m_rBtnSend.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnSend.SizeToContent();

		m_rQueryAmout.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rQueryAmout.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rQueryAmout.SetFontEx(20 , _T("微软雅黑"));
		m_rQueryAmout.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rQueryAmout.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rQueryAmout.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rQueryAmout.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rQueryAmout.SizeToContent();

		m_strTx1.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_strTx1.SetTextColor(RGB(0,0,0));	

		m_strTx2.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_strTx2.SetTextColor(RGB(0,0,0));	
		m_strTx2.SetWindowText(_T(""));

		struct LISTCol {
			CString		name ;
			UINT		size ;
		} listcol[3]  = {
			{UiFun::UI_LoadString("IPO_MODULE" , "IPO_NUMBER" ,theApp.gsLanguage) ,      150},
			{UiFun::UI_LoadString("IPO_MODULE" , "IPO_MONEY" ,theApp.gsLanguage) ,      300},
			{UiFun::UI_LoadString("IPO_MODULE" , "IPO_THAW_HEIGHT" ,theApp.gsLanguage) ,      405}
		};
		m_listCtrl.SetBkColor(RGB(240,240,240));       
		m_listCtrl.SetRowHeigt(23);               
		m_listCtrl.SetHeaderHeight(1.5);         
		m_listCtrl.SetHeaderFontHW(15,0);
		m_listCtrl.SetHeaderBKColor(32,30,32,8); 
		m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); //设置头部字体颜色
		m_listCtrl.SetTextColor(RGB(0,0,0));  
		for( int i = 0 ; i < 3 ; i++  ) {
			m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
		}
		m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );

		CString strTxFee;
		INT64 minFee = 45266;
		double dnum = (minFee*1.0/COIN);
		strTxFee.Format(_T("%.8f"),dnum);
	}
	return bRes ;
}

void CIpoCoin::OnBnClickedButtonDrawal()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( IDNO == UiFun::MessageBoxEx(UiFun::UI_LoadString("IPO_MODULE" , "IPO_WITHDRAWALS_OK" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_YESNO|MFB_TIP ) )
		return;

	if (!CheckRegIDValid( theApp.m_ipoScritptid )) return ;

	string  strShowData = _T("");

	CString addr;
	GetDlgItem(IDC_EDIT_ADDR)->GetWindowText(addr);
	if (addr == _T(""))
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("IPO_MODULE" , "IPO_ADDRESS_NOT_NULL" ,theApp.gsLanguage) ,  UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	string strCond;
	strCond = strprintf(" address='%s' ", addr);
	uistruct::LISTADDR_t pAddr;
	int nItem =  theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &pAddr) ;
	if (pAddr.address == "")
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("IPO_MODULE" , "IPO_WALLET_ADDRESS_NOT" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	if (pAddr.bSign == 0 )
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("IPO_MODULE" , "IPO_REGISTERED_ADDRESS_NOT" ,theApp.gsLanguage) ,  UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	double dmoney = GetFreeMoney(addr);
	if (dmoney <=0.0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("IPO_MODULE" , "IPO_MONEY_ADDRESS_NOT" ,theApp.gsLanguage) ,  UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	string strContractData = m_P2PBetHelp.GetAppAccountMoneyContract(addr.GetString(),1,2,REAL_MONEY(dmoney));

	CString strTxFee;
	INT64 minFee = theApp.m_P2PBetCfg.GetAppAmountnFee; //45266;theApp.m_P2PBetCfg.GetAppAmountnFee
	double dnum = (minFee*1.0/COIN);
	strTxFee.Format(_T("%.8f"),dnum);

	if (  (INT64)REAL_MONEY(strtod(strTxFee,NULL)) < 10000  ) {
		UiFun::MessageBoxEx(UiFun::UI_LoadString("IPO_MODULE" , "IPO_TIP_INSU" ,theApp.gsLanguage) ,  UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_ipoScritptid,addr.GetString(),strContractData,0,(INT64)REAL_MONEY((strtod(strTxFee,NULL))),0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData =="")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash = root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		strTip = strprintf("%s\n%s" , UiFun::UI_LoadString("IPO_MODULE" , "IPO_WITHDRAWALS_SUCCESS" ,theApp.gsLanguage) ,root["hash"].asCString() ) ;
	}else{
		strTip = UiFun::UI_LoadString("IPO_MODULE" , "IPO_WITHDRAWALS_FAIL" ,theApp.gsLanguage) ;
	}
	UiFun::MessageBoxEx(strTip.c_str() , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
}


void CIpoCoin::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if( NULL != GetSafeHwnd() ) {
		CRect rc ;
		GetClientRect( rc ) ;


		CWnd *pst = GetDlgItem( IDC_STATIC_ADDR ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*3+10, (rc.Height()/100)*5+8 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_ADDR ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*8+20 ,(rc.Height()/100)*5+6  , (rc.Width()/100)*40, (rc.Height()/100)*6+2 ,SWP_SHOWWINDOW ); 
			pst->SetFont(&theApp.m_fontBlackbody);
		}

		pst = GetDlgItem( IDC_STATIC_AMOUNT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*50+15 ,(rc.Height()/100)*5+8  , (rc.Width()/100)*30, (rc.Height()/100)*6  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_QUERY ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*80+10 ,(rc.Height()/100)*5, rect.Width(), rect.Height(), SWP_SHOWWINDOW ) ; 
		}

		//
		//	
		pst = GetDlgItem( IDC_BUTTON_DRAWAL ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect(rect) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*95 ,(rc.Height()/100)*5, rect.Width(), rect.Height(), SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_LIST_SHOW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*3 ,(rc.Height()/100)*16, rc.Width()-(rc.Width()/100)*5+2, (rc.Height()/100)*97+2  ,SWP_SHOWWINDOW ) ; 
		}
	}
}
double CIpoCoin::GetFreeMoney(CString addr)
{
	string strCommand,strShowData ="";
	strCommand =strprintf("%s %s %s","getappaccinfo" , theApp.m_ipoScritptid ,addr);
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("GetFreeMoney rpccmd getappaccinfo error");
		return 0.0;
	}

	INT64 nMoney = 0;
	
	nMoney = root["FreeValues"].asInt64() ;

	double money = (nMoney*1.0/COIN);
	return money;
}
void CIpoCoin::OnShowListCtrol(CString addr)
{
	string strCommand,strShowData ="";
	strCommand =strprintf("%s %s %s","getappaccinfo" , theApp.m_ipoScritptid ,addr);
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("OnShowListCtrol rpccmd getappaccinfo error");
		return;
	}
	strShowData = root.toStyledString();
	
	m_listCtrl.DeleteAllItems();

	int pos = strShowData.find("FreeValues");
	INT64 nMoney = 0;
	if (pos >0)
	{
		nMoney = root["FreeValues"].asInt64() ;
	}
	
	double money = (nMoney*1.0/COIN);
	strShowData = strprintf("%s:%.8f",UiFun::UI_LoadString("IPO_MODULE" , "IPO_USABLE_MONEY" ,theApp.gsLanguage) ,money);
	((CStatic*)GetDlgItem(IDC_STATIC_AMOUNT))->SetWindowText(strShowData.c_str());
	
	Invalidate();
	Json::Value valuearray = root["vFreezedFund"]; 

	int coulum = 0;
	for(unsigned int i =0;i<valuearray.size();i++)
	{
		int nSubIdx = 0;
		string strOrder ="";
		strOrder= strprintf("%d", i+1);
		m_listCtrl.InsertItem(coulum,strOrder.c_str());

		nMoney = valuearray[i]["value"].asInt64() ;
		money = (nMoney*1.0/COIN);
		strShowData = strprintf("%.8f",money);
		m_listCtrl.SetItemText( coulum , ++nSubIdx, strShowData.c_str()) ;

		strShowData =strprintf("%d" , valuearray[i]["nHeight"].asInt()) ;
		m_listCtrl.SetItemText(coulum , ++nSubIdx , strShowData.c_str() ) ;
		coulum++;
	}
}


void CIpoCoin::OnBnClickedButtonQuery()
{
	// TODO: 在此添加控件通知处理程序代码
	CString addr;
	GetDlgItem(IDC_EDIT_ADDR)->GetWindowText(addr);
	if (addr == _T(""))
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("IPO_MODULE" , "IPO_ADDRESS_NOT_NULL" ,theApp.gsLanguage) ,  UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	OnShowListCtrol(addr);
}
