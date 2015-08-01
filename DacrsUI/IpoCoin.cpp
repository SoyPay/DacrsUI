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

	LogPrint("INFO","CIpoCoin Create enter\n");
	LogPrint("INFO","CIpoCoin Create enter:%0x\n",&pParentWnd);
	LogPrint("INFO","CIpoCoin Create nIDTemplate:%d\n",nIDTemplate);
	LogPrint("INFO","CIpoCoin Create nStyle:%d\n",nStyle);
	LogPrint("INFO","CIpoCoin Create nID:%d\n",nID);
	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	LogPrint("INFO", "CIpoCoin create ret:%d\n", bRes);
	if ( bRes ) {
		LogPrint("INFO","ipo Create start\n");
		UpdateData(0);
		m_rBtnSend.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnSend.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
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
			{_T("序号") ,      150},
			{_T("金额") ,      300},
			{_T("解冻高度") ,      405}
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
		LogPrint("INFO","ipo AddListaddrDataBox before\n");

		CString strTxFee;
		INT64 minFee = 45266;
		double dnum = (minFee*1.0/COIN);
		strTxFee.Format(_T("%.8f"),dnum);

		LogPrint("INFO","ipo Create end\n");
	}
	LogPrint("INFO","ipo Create toenter\n");
	return bRes ;
}

void CIpoCoin::OnBnClickedButtonDrawal()
{
	// TODO: 在此添加控件通知处理程序代码
	CMessageBoxEx message(_T("\n是否确定要体现!") , 1 );
	if ( IDNO == message.DoModal() )
		return;

	if (!CheckRegIDValid( theApp.m_ipoScritptid )) return ;

	string  strShowData = _T("");

	//if (m_mapAddrInfo.size() == 0)
	//{
	//	::MessageBox( this->GetSafeHwnd() ,_T("发送地址不存在") , _T("提示") , MB_ICONINFORMATION ) ;
	//	return;
	//}
	CString addr;
	GetDlgItem(IDC_EDIT_ADDR)->GetWindowText(addr);
	if (addr == _T(""))
	{
		CMessageBoxEx message(_T("\n地址不能为空!") , 0 );
	        message.DoModal();
	//	::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CString Money = _T("");
	GetDlgItem(IDC_STATIC_AMOUNT)->GetWindowText(Money);
	
	double dmoney = strtod(Money,NULL);
	string strContractData = m_P2PBetHelp.GetAppAccountMoneyContract(addr.GetString(),1,2,REAL_MONEY(dmoney));

	CString strTxFee;
	INT64 minFee = theApp.m_P2PBetCfg.GetAppAmountnFee; //45266;theApp.m_P2PBetCfg.GetAppAmountnFee
	double dnum = (minFee*1.0/COIN);
	strTxFee.Format(_T("%.8f"),dnum);
	//GetDlgItem(IDC_EDIT_GETFEE)->GetWindowText(strTxFee) ;
	if (  (INT64)REAL_MONEY(atof(strTxFee)) < 10000  ) {
		CMessageBoxEx message(_T("\n小费不足!") , 0 );
	        message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_ipoScritptid,addr.GetString(),strContractData,0,(INT64)REAL_MONEY((atof(strTxFee))),0);
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
		strTip = strprintf("恭喜提现成功!\n%s" , root["hash"].asCString() ) ;
	}else{
		strTip = "提现失败!" ;
	}
	CMessageBoxEx message1(strTip.c_str() , 0 );
	        message1.DoModal();
	//::MessageBox( this->GetSafeHwnd() ,strTip.c_str() , _T("提示") , MB_ICONINFORMATION ) ;
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
			pst->SetWindowPos( NULL , (rc.Width()/100)*3, (rc.Height()/100)*5+4 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_ADDR ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*8 ,(rc.Height()/100)*5  , (rc.Width()/100)*40, (rc.Height()/100)*7 ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_AMOUNT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*48 ,(rc.Height()/100)*6  , (rc.Width()/100)*20, (rc.Height()/100)*5  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_BUTTON_QUERY ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,(rc.Width()/100)*70 ,(rc.Height()/100)*5  , (rc.Width()/100)*13-2, (rc.Height()/100)*9  ,SWP_SHOWWINDOW ) ; 
		}

		//
		//	
		pst = GetDlgItem( IDC_BUTTON_DRAWAL ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*85 ,(rc.Height()/100)*5  ,(rc.Width()/100)*13-2, (rc.Height()/100)*9   ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_LIST_SHOW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*3 ,(rc.Height()/100)*16  , rc.Width()-(rc.Width()/100)*5+2, (rc.Height()/100)*97+2  ,SWP_SHOWWINDOW ) ; 
		}
	}
}

void CIpoCoin::OnShowListCtrol(CString addr)
{
	string strCommand,strShowData ="";
	strCommand =strprintf("%s %s %s","getappaccinfo" , theApp.m_ipoScritptid ,addr);
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;
	m_listCtrl.DeleteAllItems();

	int pos = strShowData.find("FreeValues");
	INT64 nMoney = 0;
	if (pos >0)
	{
		nMoney = root["FreeValues"].asInt64() ;
	}
	double money = (nMoney*1.0/COIN);
	strShowData = strprintf("可提现金额:%.8f",money);
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
		CMessageBoxEx message(_T("\n地址不能为空!") , 0 );
	        message.DoModal();
		//::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	OnShowListCtrol(addr);
}
