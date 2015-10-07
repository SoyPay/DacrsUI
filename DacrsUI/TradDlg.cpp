// TradDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TradDlg.h"
#include "DacrsUIDlg.h"
#include "afxdialogex.h"
#include "TxDetailDlg.h"
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorksheet.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
//#include "Cnterior.h"

// CTradDlg 对话框

IMPLEMENT_DYNAMIC(CTradDlg, CDialogBar)

CTradDlg::CTradDlg()
{
	m_pBmp = NULL ;
	m_seteditcolor = TRUE;
	m_nConut = 0 ;
	m_pagesize = 13;
	m_offset = 0;
}

CTradDlg::~CTradDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	/*v_linkCtrl1.InternalRelease();
	v_linkCtrl1.ExternalRelease();
	v_linkCtrl1.OnFinalRelease();
	v_linkCtrl1.DestroyWindow();

	v_linkCtrl2.InternalRelease();
	v_linkCtrl2.ExternalRelease();
	v_linkCtrl2.OnFinalRelease();
	v_linkCtrl2.DestroyWindow();*/
}

void CTradDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LISTTX, m_listCtrl);
	DDX_Control(pDX, IDC_BUTTON_TXDETAIL, m_rBtnTxdetail);
	DDX_Control(pDX, IDC_EXPORT_EXEL, m_rBtnExportTx);
	DDX_Control(pDX, IDC_COMBO1, m_condition);
	DDX_Control(pDX, IDC_COMBO_TIME, m_time);
	DDX_Control(pDX, IDC_EDIT_ADDR, m_edit);
	DDX_Control(pDX, IDC_UP_PAGE, m_rBtnUp);
	DDX_Control(pDX, IDC_NEXT_PAGE, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
	DDX_Control(pDX ,IDC_COMBO_PAGE ,m_pageitem ) ;
}


BEGIN_MESSAGE_MAP(CTradDlg, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_TXDETAIL, &CTradDlg::OnBnClickedButtonTxdetail)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LISTTX, &CTradDlg::OnNMDblclkListListtx)
	ON_MESSAGE(MSG_USER_TRANSRECORD_UI , &CTradDlg::OnShowListCtrl )

	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_EXPORT_EXEL, &CTradDlg::OnBnClickedExportExel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CTradDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME, &CTradDlg::OnCbnSelchangeComboTime)
	//ON_EN_CHANGE(IDC_EDIT_ADDR, &CTradDlg::OnEnChangeEditAddr)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_UP_PAGE, &CTradDlg::OnBnClickedUpPage)
	ON_BN_CLICKED(IDC_NEXT_PAGE, &CTradDlg::OnBnClickedNextPage)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE, &CTradDlg::OnCbnSelchangeComboPage)
END_MESSAGE_MAP()


// CTradDlg 消息处理程序

void CTradDlg::SetBkBmpNid( UINT nBitmapIn ) 
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
int CTradDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetBkBmpNid(IDB_BITMAP_RECEIVE);
	// TODO:  在此添加您专用的创建代码

	return 0;
}


BOOL CTradDlg::OnEraseBkgnd(CDC* pDC)
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


BOOL CTradDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(0);
		/*v_linkCtrl1.SetWindowText(_T(""));
		v_linkCtrl2.SetWindowText(_T(""));

		v_linkCtrl1.SetWindowText(_T("关注开发进度"));
		v_linkCtrl2.SetWindowText(_T("QQ群"));

		v_linkCtrl1.SetURL(_T("http://www.dacrs.com/forum.php?mod=viewthread&tid=2598"));
		v_linkCtrl2.SetURL(_T("http://jq.qq.com/?_wv=1027&k=T5mlmd"));*/

		struct LISTCol {
			CString		name ;
			UINT		size ;
		} listcol[6]  = {
			{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SERIAL_NUMBER" ,theApp.gsLanguage),          50},
			{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage) ,      60},
			{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SOURCE_ADDRESS" ,theApp.gsLanguage) ,       220}, 
			{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_MONEY" ,theApp.gsLanguage),          100},
			{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_DESTINATION_ADDRESS" ,theApp.gsLanguage) ,     220},
			{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ID" ,theApp.gsLanguage) ,     400}
			
		};
		m_listCtrl.SetBkColor(RGB(240,240,240));       
		m_listCtrl.SetRowHeigt(23);               
		m_listCtrl.SetHeaderHeight(1.5);         
		m_listCtrl.SetHeaderFontHW(15,0);
		m_listCtrl.SetHeaderBKColor(32,30,32,8); 
		m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); //设置头部字体颜色
		m_listCtrl.SetTextColor(RGB(0,0,0)); 
		for( int i = 0 ; i < 6 ; i++  ) {
			m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
		}
		m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );

		m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_sCountpage.SetWindowText(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TOTAL" ,theApp.gsLanguage));

		m_rBtnTxdetail.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnTxdetail.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnTxdetail.SetWindowText(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_VIEW_DETAILS" ,theApp.gsLanguage)) ;
		m_rBtnTxdetail.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnTxdetail.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnTxdetail.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnTxdetail.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnTxdetail.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnTxdetail.SizeToContent();

		m_rBtnExportTx.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnExportTx.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnExportTx.SetWindowText(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OUT_EXCEL" ,theApp.gsLanguage)) ;
		m_rBtnExportTx.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnExportTx.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnExportTx.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnExportTx.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnExportTx.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnExportTx.SizeToContent();


		m_rBtnUp.SetBitmaps( IDB_TRADE_UP , RGB(255, 255, 0) , IDB_TRADE_UP , RGB(255, 255, 255) );
		m_rBtnUp.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnUp.SetWindowText("") ;
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnUp.SizeToContent();

		m_rBtnNext.SetBitmaps( IDB_TRADE_NEXT , RGB(255, 255, 0) , IDB_TRADE_NEXT , RGB(255, 255, 255) );
		m_rBtnNext.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNext.SetWindowText("") ;
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNext.SizeToContent();

		m_condition.InsertString(0,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ALL" ,theApp.gsLanguage));
		m_condition.InsertString(1,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_RECEIVE" ,theApp.gsLanguage));
		m_condition.InsertString(2,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND" ,theApp.gsLanguage));
		m_condition.InsertString(3,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_MINING" ,theApp.gsLanguage));
		m_condition.InsertString(4,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT" ,theApp.gsLanguage));
		m_condition.InsertString(5,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ACTIVATION" ,theApp.gsLanguage));
		//m_condition.AddString(_T("全部"));
		//m_condition.AddString(_T("接收"));
		//m_condition.AddString(_T("发送"));
		//m_condition.AddString(_T("挖矿所得"));

		m_time.InsertString(0,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ALL" ,theApp.gsLanguage));
		m_time.InsertString(1,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TODAY" ,theApp.gsLanguage));
		m_time.InsertString(2,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WEEK" ,theApp.gsLanguage));
		m_time.InsertString(3,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_THIS_MONTH" ,theApp.gsLanguage));
		m_time.InsertString(4,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LAST_MONTH" ,theApp.gsLanguage));
		m_time.InsertString(5,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_THIS_YEAR" ,theApp.gsLanguage));
		m_time.InsertString(6,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_YESTERDAY" ,theApp.gsLanguage));

		m_pageitem.InsertString(0,_T("13"));
		m_pageitem.InsertString(1,_T("20"));
		m_pageitem.InsertString(2,_T("30"));
		m_pageitem.InsertString(3,_T("50"));
		m_pageitem.InsertString(4,_T("100"));


		m_edit.SetWindowText(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ADDRESS_SEARCH" ,theApp.gsLanguage));

		GetDlgItem(IDC_STATIC_PAGE)->SetWindowText(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_EVERYPAGE" ,theApp.gsLanguage));
		GetDlgItem(IDC_STATIC_ITEM)->SetWindowText(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ITEM" ,theApp.gsLanguage));

		m_condition.SetCurSel(0);
		m_time.SetCurSel(0);
		m_pageitem.SetCurSel(0);

		m_colorEditText = RGB(159,161,164);

		OninitializeList();

		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_TRANSRECORD_UI ) ;
	}
	return bRes ;
}


void CTradDlg::OnBnClickedButtonTxdetail()
{
	// TODO: 在此添加控件通知处理程序代码
		CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		CString strtxhash = m_listCtrl.GetItemText(nRow, 5);
		uistruct::REVTRANSACTION_t  txdetail;
	    string strCond;
		strCond = strprintf(" hash='%s' ", strtxhash);
		int nItem =  theApp.m_SqliteDeal.GetTransactionItem( strCond ,&txdetail ) ;
		if (txdetail.txhash != _T(""))
		{
			theApp.m_strAddress.Format(_T("%s") ,txdetail.ToJson().c_str() ) ;
		}else
		{		
			theApp.m_strAddress =_T("");
		}
		CTxDetailDlg dlg;
		dlg.DoModal();

	}else{
		StrShow.Format(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SELECT_ADDRESS" ,theApp.gsLanguage));
		UiFun::MessageBoxEx(StrShow , _T("Error") ,MFB_OK|MFB_ERROR );
	}

}
void  CTradDlg::ShowListCtrl(uistruct::TRANSRECORDLIST* pListInfo)
{
	if ( NULL == pListInfo ) return ;

	m_listCtrl.DeleteAllItems();
	int count = (m_nConut)*m_pagesize+1;

	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
	for ( const_it = pListInfo->begin() ; const_it != pListInfo->end() ; const_it++ ) {
		nSubIdx = 0;
		strShowData =strprintf("%d", count++);
		int item = m_listCtrl.InsertItem(i, strShowData.c_str());					//序号
		m_listCtrl.SetItemData( item , (DWORD_PTR)(&(*const_it)) ) ;

		string txtype = const_it->txtype;
		if (!strcmp(txtype.c_str(),"REWARD_TX"))
		{
			strShowData = strprintf("%s" ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_MINING" ,theApp.gsLanguage)) ;
		}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
		{
			strShowData= UiFun::UI_LoadString("COMM_MODULE" , "COMM_ACTIVATEDS" ,theApp.gsLanguage) ;
		}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
		{
			if (const_it->state == 3)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_PING" ,theApp.gsLanguage) ;
			}else if (const_it->state == 2)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_COLLECT" ,theApp.gsLanguage) ;
			}else if (const_it->state == 1)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_HAIR" ,theApp.gsLanguage);
			}else
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN" ,theApp.gsLanguage) ;
			}
		}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT" ,theApp.gsLanguage) ;
		}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_REGISTER" ,theApp.gsLanguage) ;
		}
		m_listCtrl.SetItemText( i , ++nSubIdx, strShowData.c_str());   //交易类型

		
		m_listCtrl.SetItemText(i , ++nSubIdx , const_it->addr.c_str() );   //源地址

		strShowData =strprintf("%.8f" , const_it->money ) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() );   //交易金额

		strShowData =strprintf("%s",const_it->desaddr.c_str());
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() );   //目的地址

		

		strShowData = strprintf("%s", const_it->txhash.c_str()); 
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() );  //交易hash

		i++;
	}
}
void  CTradDlg::OninitializeList()
{
	ShowComboxCotent();
}

void CTradDlg::OnNMDblclkListListtx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(-1 != pNMItemActivate->iItem) 
	{  
		int nRow = pNMItemActivate->iItem;
		CString strtxhash =m_listCtrl.GetItemText(nRow, 5);
		uistruct::REVTRANSACTION_t  txdetail;
		string strCond;
		strCond = strprintf(" hash='%s' ", strtxhash);
		int nItem =  theApp.m_SqliteDeal.GetTransactionItem( strCond ,&txdetail ) ;
		if (txdetail.txhash != _T(""))
		{
			theApp.m_strAddress.Format(_T("%s") ,txdetail.ToJson().c_str() ) ;
		}else
		{		
			theApp.m_strAddress =_T("");
		}
		CTxDetailDlg dlg;
		dlg.DoModal();
	}  
	*pResult = 0;
}
void CTradDlg::InsertItemData()
{
	CPostMsg postmsg;
	if (!theApp.m_UiTxDetailQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::REVTRANSACTION_t txdetail; 
	string strTemp = postmsg.GetData();
	txdetail.JsonToStruct(strTemp.c_str());

	if (!IsInsertTx(txdetail))
	{
		return;
	}
	int count = m_listCtrl.GetItemCount();

	string strShowData;
	int nSubIdx = 0;
	strShowData =strprintf("%d", count+1);
	m_listCtrl.InsertItem(count,strShowData.c_str());       //序号
	
	string txtype = txdetail.txtype;
	if (!strcmp(txtype.c_str(),"REWARD_TX"))
	{
		strShowData =strprintf("%s" ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_MINING" ,theApp.gsLanguage)) ;
	}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
	{
		strShowData =strprintf("%s" ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_ACTIVATEDS" ,theApp.gsLanguage)) ;
	}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
	{
		if (txdetail.state == 3)
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_PING" ,theApp.gsLanguage) ;
		}else if (txdetail.state == 2)
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_COLLECT" ,theApp.gsLanguage) ;
		}else if (txdetail.state == 1)
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_HAIR" ,theApp.gsLanguage) ;
		}else{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN" ,theApp.gsLanguage) ;
		}
	}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
	{
		strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT" ,theApp.gsLanguage) ;
	}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
	{
		strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_REGISTER" ,theApp.gsLanguage);
	}
	m_listCtrl.SetItemText( count , ++nSubIdx, strShowData.c_str()); //交易类型

	
	m_listCtrl.SetItemText(count , ++nSubIdx , txdetail.addr.c_str() );//源地址

	strShowData = strprintf("%.8f" , (txdetail.money*100000000) ) ;
	m_listCtrl.SetItemText(count , ++nSubIdx , strShowData.c_str() );//金额

	
	m_listCtrl.SetItemText(count , ++nSubIdx , txdetail.desaddr.c_str() );//目的地址

	strShowData = strprintf("%s", txdetail.txhash.c_str());
	m_listCtrl.SetItemText( count , ++nSubIdx, strShowData.c_str()); //交易hash

    m_listCtrl.EnsureVisible(count, FALSE);
}

 LRESULT CTradDlg::OnShowListCtrl(  WPARAM wParam, LPARAM lParam )
 {
	 int type = (int)wParam;
	 switch(type)
	 {
	 case WM_INSERT:
		 {
			 //InsertItemData();
			 if ( 0 == m_nConut ) {   //只有当前是首页 , 才去显示
				 m_listCtrl.DeleteAllItems();
				 OninitializeList();
			 }
		 }
		 break;
	 case WM_REMOVETX:
	{
		if ( 0 == m_nConut ) {   //只有当前是首页 , 才去显示
			m_listCtrl.DeleteAllItems();
			OninitializeList();
		}
	}
	break;
	default:
		 break;
	 }
	 return 0;
 }

void CTradDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;
		CRect rc  ;
		GetClientRect( rc ) ;
		CWnd *pst = GetDlgItem( IDC_COMBO1 ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,140 , 25 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_COMBO_TIME ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,40 , 25 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_EDIT_ADDR ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,250 , 23 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		
		CButton *pList = (CButton*)GetDlgItem(IDC_LIST_LISTTX);
		if( NULL != pList ) {	
			//pList->SetWindowPos(NULL ,32, 50 , 837 , 380 , SWP_SHOWWINDOW);
			pList->SetWindowPos(NULL ,32, 50 , 837 , 350 , SWP_SHOWWINDOW);
		}
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_TXDETAIL);
		if( NULL != pButton ) {	
			CRect m_BtnRc ;
			pButton->GetClientRect(&m_BtnRc);
			pButton->SetWindowPos(NULL ,900 - 1*(103 + 5)- 23 , 600 - 72 - 32 - 46 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		}

		pButton = (CButton*)GetDlgItem(IDC_EXPORT_EXEL);
		if( NULL != pButton ) {	
			CRect m_BtnRc ;
			pButton->GetClientRect(&m_BtnRc);
			pButton->SetWindowPos(NULL ,900 - 1*(103 + 5)- 23-m_BtnRc.Width()-18 , 600 - 72 - 32 - 46 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		}

		//pButton = (CButton*)GetDlgItem( IDC_BUTTON_REFRESH ) ;
		//if ( NULL != pst ) {
		//	CRect m_BtnRc ;
		//	pButton->GetClientRect( m_BtnRc ) ;
		//	pButton->SetWindowPos(NULL ,900 - 1*(103 + 5)- 23-m_BtnRc.Width()-120 , 600 - 72 - 32 - 46 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		//}

		pButton = (CButton*)GetDlgItem( IDC_UP_PAGE ) ;
		if ( NULL != pst ) {
			CRect m_BtnRc ;
			pButton->GetClientRect( m_BtnRc ) ;
			pButton->SetWindowPos(NULL ,(rc.Width()/div)*85 , (rc.Height()/div)*102 , 20 , 20 , SWP_SHOWWINDOW);
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,(rc.Width()/div)*88, (rc.Height()/div)*102 , 63, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pButton = (CButton*)GetDlgItem( IDC_NEXT_PAGE ) ;
		if ( NULL != pst ) {
			CRect m_BtnRc ;
			pButton->GetClientRect( m_BtnRc ) ;
			pButton->SetWindowPos(NULL ,(rc.Width()/div)*96+2, (rc.Height()/div)*102 , 20 , 20 , SWP_SHOWWINDOW);
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/div)*99, (rc.Height()/div)*102, 70, 30  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_PAGE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/div)*71, (rc.Height()/div)*102+3, 43, 18 ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_COMBO_PAGE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/div)*77-5, (rc.Height()/div)*102,45,18  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_STATIC_ITEM ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/div)*82, (rc.Height()/div)*102+3, rect.Width(), 18  ,SWP_SHOWWINDOW ) ; 
		}
	}
}


BOOL CTradDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CDialogBar::OnMouseWheel(nFlags, zDelta, pt);
}

void CTradDlg:: GetExportCol(int index,map<int,string> &item,uistruct::REVTRANSACTION_t const_it)
{
	     string strShowData = "";
		string txtype = const_it.txtype;

		int i = 0;
		//strShowData= strprintf("%d" , index) ;
		//item[i++] =strShowData;
		if (!strcmp(txtype.c_str(),"REWARD_TX"))
		{
			strShowData = UiFun::UI_LoadString("COMM_MODULE" , "COMM_MINING" ,theApp.gsLanguage) ;
		}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
		{
			strShowData= UiFun::UI_LoadString("COMM_MODULE" , "COMM_ACTIVATEDS" ,theApp.gsLanguage) ;
		}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
		{
			if (const_it.state == 3)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_PING" ,theApp.gsLanguage);
			}else if (const_it.state == 2)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_COLLECT" ,theApp.gsLanguage);
			}else if (const_it.state == 1)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_HAIR" ,theApp.gsLanguage) ;
			}else
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN" ,theApp.gsLanguage) ;
			}
		}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT" ,theApp.gsLanguage);
		}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_REGISTER" ,theApp.gsLanguage) ;
		}
		item[i++] =strShowData;
		item[i++] =const_it.addr;
		strShowData= strprintf("%.8f" , const_it.money ) ;
		item[i++] =strShowData;
		item[i++] =const_it.desaddr;
		item[i++] =const_it.txhash;
}

void CTradDlg::OnBnClickedExportExel()
{
	int operate = 0;
	string condtion = GetConditonStr(operate);
	uistruct::TRANSRECORDLIST pListInfo;
	string strCond =  condtion;
	theApp.m_SqliteDeal.GetTransactionList(strCond, &pListInfo); 
	if (pListInfo.size() == 0)
	{
		UiFun::MessageBoxEx(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_NOT_RECORD_OUT" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	CFileDialog dlg(FALSE,NULL,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_RECORD" ,theApp.gsLanguage),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_FILE" ,theApp.gsLanguage)+" (*.xls)|*.xls||");
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	CString strFile = dlg.GetPathName();
	if (!((CDacrsUIDlg*)(theApp.m_pMainWnd))->GetFileName(strFile,_T(".xls")))
	{
		return;
	}

	struct LISTCol {
		string		name ;
		UINT		size ;
	} listheadr[5]  = {
		//{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SERIAL_NUMBER" ,theApp.gsLanguage) ,    10},
		{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage) ,    30},
		{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SOURCE_ADDRESS" ,theApp.gsLanguage) ,    40},
		{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_MONEY" ,theApp.gsLanguage) ,    10},
		{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_DESTINATION_ADDRESS" ,theApp.gsLanguage) ,  40}, 
		{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ID" ,theApp.gsLanguage) ,100}
	};

	COleVariant

		covTrue((short)TRUE),

		covFalse((short)FALSE),

		covOptional((long)DISP_E_PARAMNOTFOUND,   VT_ERROR);

	CApplication   app;

	CWorkbooks   books;

	CWorkbook   book;

	CWorksheets   sheets;

	CWorksheet   sheet;

	CRange   range;

	CFont0   font;



	if (!app.CreateDispatch(_T("Excel.Application")))

	{

		UiFun::MessageBoxEx(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_NOT_OFFICE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;

	}



	//Get   a   new   workbook.

	books = app.get_Workbooks();

	book = books.Add(covOptional);



	sheets = book.get_Worksheets();

	sheet = sheets.get_Item(COleVariant((short)1));



	////////////////////////////////////CListCtrl控件report风格//////////////////////////////////////////////////////////

	//CHeaderCtrl   *pmyHeaderCtrl;

	//pmyHeaderCtrl = m_listCtrl.GetHeaderCtrl();//此句取得CListCtrl控件的列表頭



	int   iRow,iCol;

	int   m_cols   =   5;

	int   m_rows = pListInfo.size();

	HDITEM   hdi;

	TCHAR     lpBuffer[256];

	bool       fFound   =   false;



	hdi.mask   =   HDI_TEXT;

	hdi.pszText   =   lpBuffer;

	hdi.cchTextMax   =   256;

	CString   colname;

	CString strTemp;

	for(iCol=0;   iCol <m_cols;   iCol++)//将列表的标题头写入EXCEL

	{

		UiFun::GetCellName(1 ,iCol + 1, colname);

		range   =   sheet.get_Range(COleVariant(colname),COleVariant(colname));

		//pmyHeaderCtrl-> GetItem(iCol,   &hdi);

		range.put_Value2(COleVariant(listheadr[iCol].name.c_str()));

		int   nWidth   = listheadr[iCol].size;  //m_listCtrl.GetColumnWidth(iCol)/6;

		//得到第iCol+1列  

		range.AttachDispatch(range.get_Item(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);  

		//设置列宽 

		range.put_ColumnWidth(_variant_t((long)nWidth));

	}

	range   =   sheet.get_Range(COleVariant( _T("A1 ")),   COleVariant(colname));

	range.put_RowHeight(_variant_t((long)50));//设置行的高度

	font = range.get_Font();

	font.put_Bold(covTrue);

	range.put_VerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108



	COleSafeArray   saRet;

	DWORD   numElements[]={m_rows,m_cols};       //5x2   element   array

	saRet.Create(VT_BSTR,   2,   numElements);

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range = range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));

	long   index[2];

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range   =   range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));


	int iLine = 0;
	iRow   =   1;
	iCol   =   1;
	int ncount =1;
	std::vector<uistruct::REVTRANSACTION_t>::const_iterator pitem = pListInfo.begin();
	for(;pitem != pListInfo.end();pitem++,iRow++)
	{
		map<int,string> item;
		GetExportCol(ncount,item,*pitem);
		ncount++;
		for   (   iCol   =   1;   iCol   <=   m_cols;   iCol++)  

		{

			index[0]=iRow-1;

			index[1]=iCol-1;
			string strTemp =  item[iCol-1];
			CString   szTemp = strTemp.c_str();

			BSTR   bstr   =   szTemp.AllocSysString();

			saRet.PutElement(index,bstr);

			SysFreeString(bstr);

		}
	}


	range.put_Value2(COleVariant(saRet));


	saRet.Detach();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	book.SaveCopyAs(COleVariant(strFile));

	//       cellinterior.ReleaseDispatch();

	book.put_Saved(true);

	book.ReleaseDispatch();  

	books.ReleaseDispatch();  

	app.Quit();

	app.ReleaseDispatch();
}

void CTradDlg::OnShowListCtrl(uistruct::TRANSRECORDLIST pListInfo){

	m_listCtrl.DeleteAllItems();
	if (pListInfo.size() == 0)
	{
		return;
	}
	int count = (m_nConut)*m_pagesize+1;
	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
	for ( const_it = pListInfo.begin() ; const_it != pListInfo.end() ; const_it++ ) {

		nSubIdx = 0;
		strShowData = strprintf("%d", count++);
		m_listCtrl.InsertItem(i, strShowData.c_str());					//序号

		string txtype = const_it->txtype;
		if (!strcmp(txtype.c_str(),"REWARD_TX"))
		{
			strShowData = UiFun::UI_LoadString("COMM_MODULE" , "COMM_MINING" ,theApp.gsLanguage);
		}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
		{
			strShowData= UiFun::UI_LoadString("COMM_MODULE" , "COMM_ACTIVATEDS" ,theApp.gsLanguage) ;
		}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
		{
			if (const_it->state == 3)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_PING" ,theApp.gsLanguage);
			}else if (const_it->state == 2)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_COLLECT" ,theApp.gsLanguage);
			}else if (const_it->state == 1)
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_HAIR" ,theApp.gsLanguage) ;
			}else
			{
				strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN" ,theApp.gsLanguage) ;
			}
		}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT" ,theApp.gsLanguage);
		}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
		{
			strShowData= UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_REGISTER" ,theApp.gsLanguage) ;
		}
		m_listCtrl.SetItemText( i , ++nSubIdx, strShowData.c_str());   //交易类型

		
		m_listCtrl.SetItemText(i , ++nSubIdx , const_it->addr.c_str() );   //源地址

		strShowData= strprintf("%.8f" , const_it->money ) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() );   //交易金额

		m_listCtrl.SetItemText(i , ++nSubIdx , const_it->desaddr.c_str() );   //目的地址



		strShowData = strprintf("%s", const_it->txhash.c_str()); 
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() );  //交易hash

		i++;
	}
}
bool  CTradDlg::isMine(CString addr)
{
	string strCond;
	strCond = strprintf(" address = '%s' ", addr);
	uistruct::LISTADDR_t addrsql;
	int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;
	if (addrsql.address == "" )
	{
		return false;
	}
	return true;
}
void CTradDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nConut = 0;
	ShowComboxCotent();
}

void CTradDlg::OnCbnSelchangeComboTime()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nConut = 0;
	ShowComboxCotent();

}
string CTradDlg::GetConditonTime(){
	SYSTEMTIME curTime ;
	memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &curTime ) ;
	string strSendTime;
	strSendTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	INT64 maxcurtime = UiFun::SystemTimeToTimet(curTime);

	CString text;
	int sel = m_time.GetCurSel();
	if (sel < 0)
	{
		return _T("");
	}
	m_time.GetLBText(sel,text);
	uistruct::TRANSRECORDLIST pListInfo;

	string conditon;

	if (strcmp(text,  UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ALL" ,theApp.gsLanguage) ) == 0)
	{
		return _T("");
	}else if (strcmp(text, UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TODAY" ,theApp.gsLanguage) ) == 0)
	{
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);
		
		conditon = strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon += strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text, UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WEEK" ,theApp.gsLanguage) ) == 0)
	{
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = 0;
		if (curTime.wDayOfWeek == 0)
		{
			 mincurtime = UiFun::SystemTimeToTimet(curTime);
		}else{
			INT64 differ = 86400*curTime.wDayOfWeek;
			 mincurtime = UiFun::SystemTimeToTimet(curTime);
			 mincurtime = maxcurtime -mincurtime;
			 mincurtime = maxcurtime - (differ+mincurtime);
		}

		conditon =strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_THIS_MONTH" ,theApp.gsLanguage)) == 0)
	{
		curTime.wDay =1;
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);
		
		conditon = strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LAST_MONTH" ,theApp.gsLanguage)) == 0)
	{
		INT64 maxcurtime = 0;
		INT64 mincurtime = 0;
		curTime.wDay =1;
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		maxcurtime = UiFun::SystemTimeToTimet(curTime);
		if (curTime.wMonth == 1)
		{
			curTime.wMonth = 12;
			curTime.wYear -=1;

		}else{
			curTime.wMonth -= 1;
		}
		mincurtime = UiFun::SystemTimeToTimet(curTime);
		
		conditon= strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_THIS_YEAR" ,theApp.gsLanguage)) == 0)
	{
		curTime.wMonth =1;
		curTime.wDay=1;
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);
		
		conditon =strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon+=strprintf("%d",maxcurtime);
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_YESTERDAY" ,theApp.gsLanguage)) == 0)
	{
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 maxcurtime = UiFun::SystemTimeToTimet(curTime);
		INT64 mincurtime = maxcurtime - 24*60*60;
	
		conditon=strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		return conditon;
	}
	return _T("");
}
string CTradDlg::GetConditonTxType(int &operate){
	CString text;
	int sel = m_condition.GetCurSel();
	if (sel < 0)
	{
		return _T("");
	}
	m_condition.GetLBText(sel,text);
	uistruct::TRANSRECORDLIST pListInfo;

	string conditon;

	if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ALL" ,theApp.gsLanguage)) == 0)
	{
		return _T("");
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_RECEIVE" ,theApp.gsLanguage)) == 0)
	{
		operate = 1;
		
		conditon=" tx_type='COMMON_TX' and (state =2 or state =3)";
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND" ,theApp.gsLanguage)) == 0)
	{
		operate = 2;
		conditon=" tx_type='COMMON_TX' and (state =1 or state =3)";
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_MINING" ,theApp.gsLanguage)) == 0)
	{
		conditon=" tx_type='REWARD_TX'";
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_CONTRACT" ,theApp.gsLanguage)) == 0)
	{
		conditon=" tx_type='CONTRACT_TX'";
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ACTIVATION" ,theApp.gsLanguage)) == 0)
	{
		conditon=" tx_type='REG_ACCT_TX'";
		return conditon;
	}
	return "";
}

string CTradDlg::Getaddr(){

	CString ret = _T("");
	GetDlgItem(IDC_EDIT_ADDR)->GetWindowText(ret);
	if (strcmp(ret,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ADDRESS_SEARCH" ,theApp.gsLanguage)) == 0)
	{
		 ret = "";
	}
	string stret =strprintf("%s",ret);
	return stret;
}
void CTradDlg::ShowAddrConditon()
{
	m_nConut = 0;
	int operate;
	string condtion = GetConditonStr(operate);

	uistruct::TRANSRECORDLIST pListInfo;
	theApp.m_SqliteDeal.GetTransactionList(condtion, &pListInfo); 
	OnShowListCtrl(pListInfo);
}

BOOL CTradDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)  
	{   
		if (GetDlgItem(IDC_EDIT_ADDR) == this->GetFocus())
		{
			ShowAddrConditon();
			return TRUE;
		}else  if (GetDlgItem(IDC_EDIT_PAGE) == this->GetFocus()) {
		  CString nConut;
		  GetDlgItem(IDC_EDIT_PAGE)->GetWindowText(nConut);
		  if (IsAllDigtal(nConut)) {
			  ShowPageDataInfo(atoi(nConut));
			  return TRUE;
		  }else {
			  GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
			  UiFun::MessageBoxEx(UiFun::UI_LoadString("COMM_MODULE" , "COMM_INPUT_ERROR" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		  }
		}  
	}  
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ADDR);
	ASSERT(pEdit && pEdit->GetSafeHwnd());
	if(WM_LBUTTONDOWN == pMsg->message && pEdit->GetSafeHwnd() == pMsg->hwnd)
	{
		CString strTemp = _T("");
		m_edit.GetWindowText(strTemp);
		if (strcmp(strTemp, UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ADDRESS_SEARCH" ,theApp.gsLanguage)) == 0)
		{
			pEdit->SetFocus();
			pEdit->SetSel(1,1,FALSE);
			return TRUE;
		}

	}
	if (pMsg->message == WM_KEYDOWN &&pEdit->GetSafeHwnd() == pMsg->hwnd &&(pMsg->wParam != VK_RETURN))
	{
		CString strTemp = _T("");
		m_edit.GetWindowText(strTemp);
		if (((pMsg->wParam >=48 && pMsg->wParam <=57) 
			|| (pMsg->wParam >=65 && pMsg->wParam <=90)
			|| (pMsg->wParam >=0x60 && pMsg->wParam <=0x69)) && strcmp(strTemp,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ADDRESS_SEARCH" ,theApp.gsLanguage)) == 0)
		{
			m_edit.SetWindowText(_T(""));
			m_seteditcolor = FALSE;
		}
	}
	
	if ( pMsg->message == WM_KEYDOWN &&pEdit->GetSafeHwnd() == pMsg->hwnd &&(pMsg->wParam == VK_BACK) )
	{
		CString strTemp = _T("");
		m_edit.GetWindowText(strTemp);
		if (strTemp.GetLength() ==  1)
		{
			m_edit.SetWindowText(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ADDRESS_SEARCH" ,theApp.gsLanguage));
			m_seteditcolor = TRUE;
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}
string CTradDlg::GetConditonStr(int &operate)
{
	string condtion ="";
	string temp ="";

	temp = GetConditonTxType(operate);
	if (temp !="")
	{
		condtion = GetConditonTxType(operate);
	}

	if (condtion !="")
	{
		temp = GetConditonTime();
		if (temp !=_T(""))
		{
			condtion+=strprintf( " and %s",temp.c_str());
		}
		

	}else{
		temp = GetConditonTime();
		if (temp != _T(""))
		{
			condtion += strprintf( " %s",temp.c_str());
		}
	}

	if (condtion !=_T(""))
	{
		temp = Getaddr();
		if (temp != _T(""))
		{
			if (operate == 1)   ///  接收钱的地址
			{
				condtion+=strprintf( " and des_addr = '%s'",temp.c_str());
			}else if (operate == 2)
			{
				condtion+=strprintf( " and src_addr = '%s'",temp.c_str());
			}else{
				condtion+=strprintf( " and (src_addr = '%s' or des_addr = '%s')",temp.c_str(),temp.c_str());
			}
			
		}

	}else{
		temp = Getaddr();
		if (temp != "")
		{
			if (operate == 1)
			{
				condtion+=strprintf( "des_addr = '%s'",temp.c_str());
			}else if (operate == 2)
			{
				condtion+=strprintf( "src_addr = '%s'",temp.c_str());
			}else{
				condtion+=strprintf( " src_addr = '%s' or des_addr = '%s'",temp.c_str(),temp.c_str());
			}
		}

	}
	if (condtion =="")
	{
		condtion ="1=1";
	}
	condtion+=" order by confirmed_time DESC";
	return condtion;
}

//void CTradDlg::OnEnChangeEditAddr()
//{
//	 //TODO:  如果该控件是 RICHEDIT 控件，它将不
//	 //发送此通知，除非重写 CDialogBar::OnInitDialog()
//	 //函数并调用 CRichEditCtrl().SetEventMask()，
//	 //同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	 //TODO:  在此添加控件通知处理程序代码
//	//GetDlgItem(IDC_EDIT_ADDR)->SetWindowText(_T(""));
//}



HBRUSH CTradDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBar::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (CTLCOLOR_EDIT == nCtlColor && m_seteditcolor && pWnd->GetDlgCtrlID() == IDC_EDIT_ADDR)
	{
		pDC->SetTextColor(m_colorEditText);
	}
	return hbr;
}

string CTradDlg::GetConditonTime(INT64 &maxtime,INT64 &mintime)
{
	SYSTEMTIME curTime ;
	memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &curTime ) ;
	string strSendTime;
	strSendTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	INT64 maxcurtime = UiFun::SystemTimeToTimet(curTime);

	CString text;
	int sel = m_time.GetCurSel();
	if (sel < 0)
	{
		return _T("");
	}
	m_time.GetLBText(sel,text);
	uistruct::TRANSRECORDLIST pListInfo;

	string conditon;
	if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ALL" ,theApp.gsLanguage)) == 0)
	{
		return _T("");
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TODAY" ,theApp.gsLanguage)) == 0)
	{
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);

		conditon =strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		maxtime = maxcurtime;
		mintime=mincurtime;
		return conditon;
		//theApp.m_SqliteDeal.GetTransactionList(conditon, &pListInfo); 
		//OnShowListCtrl(pListInfo);
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_WEEK" ,theApp.gsLanguage)) == 0)
	{
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = 0;
		if (curTime.wDayOfWeek == 0)
		{
			mincurtime = UiFun::SystemTimeToTimet(curTime);
		}else{
			INT64 differ = 86400*curTime.wDayOfWeek;
			mincurtime = UiFun::SystemTimeToTimet(curTime);
			mincurtime = maxcurtime -mincurtime;
			mincurtime = maxcurtime - (differ+mincurtime);
		}
		//	SYSTEMTIME tttt = UiFun::Time_tToSystemTime(mincurtime);

		conditon = strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon +=strprintf("%d",maxcurtime);
		maxtime = maxcurtime;
		mintime=mincurtime;
		return conditon;
		//theApp.m_SqliteDeal.GetTransactionList(conditon, &pListInfo); 
		//OnShowListCtrl(pListInfo);
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_THIS_MONTH" ,theApp.gsLanguage)) == 0)
	{
		curTime.wDay =1;
		curTime.wHour = 0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);
	
		conditon = strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon+=strprintf("%d",maxcurtime);
		maxtime = maxcurtime;
		mintime=mincurtime;
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LAST_MONTH" ,theApp.gsLanguage)) == 0)
	{
		INT64 maxcurtime = 0;
		INT64 mincurtime = 0;
		curTime.wDay =1;
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		maxcurtime = UiFun::SystemTimeToTimet(curTime);
		if (curTime.wMonth == 1)
		{
			curTime.wMonth = 12;
			curTime.wYear -=1;

		}else{
			curTime.wMonth -= 1;
		}
		mincurtime = UiFun::SystemTimeToTimet(curTime);
	
		conditon=strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon+strprintf("%d",maxcurtime);
		maxtime = maxcurtime;
		mintime=mincurtime;
		return conditon;
	}else if (strcmp(text,_T("今年")) == 0)
	{
		curTime.wMonth =1;
		curTime.wDay=1;
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 mincurtime = UiFun::SystemTimeToTimet(curTime);
	
		conditon =strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon+=strprintf("%d",maxcurtime);
		maxtime = maxcurtime;
		mintime=mincurtime;
		return conditon;
	}else if (strcmp(text,UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_THIS_YEAR" ,theApp.gsLanguage)) == 0)
	{
		curTime.wHour=0;
		curTime.wMinute = 0;
		curTime.wSecond = 0;
		curTime.wMilliseconds = 0;
		INT64 maxcurtime = UiFun::SystemTimeToTimet(curTime);
		INT64 mincurtime = maxcurtime - 24*60*60;
	
		conditon=strprintf(" confirmed_time>=%d and confirmed_time<=",mincurtime);
		conditon+=strprintf("%d",maxcurtime);
		maxtime = maxcurtime;
		mintime=mincurtime;
		return conditon;
	}
	return _T("");
}
BOOL CTradDlg::IsInsertTx(uistruct::REVTRANSACTION_t txdetail)
{
	int operate = 0;
	INT64 maxtime = 0,mintime = 0;
	string txtype = GetConditonTxType(operate);
	string time = GetConditonTime(maxtime,mintime);
	string addr = Getaddr();
	if (txtype =="" && time == "" && addr == "")
	{
		return TRUE;
	}
	if (time != "")
	{
		if (!(txdetail.confirmedtime >= mintime && txdetail.confirmedtime <=maxtime))
		{
			return FALSE;
		}
	}

	if (txtype != "")
	{
		if (txtype.find(txdetail.txtype.c_str())<0)
		{
			return FALSE;
		}
		if (operate == 1)
		{
			if ( !isMine(txdetail.desaddr.c_str()))
			{
				return FALSE;
			}
		}
		if (operate == 2)
		{
			if ( !isMine(txdetail.addr.c_str()))
			{
				return FALSE;
			}
		}
	}

	if (addr != "")
	{
		if (operate == 1)
		{
			if ( strcmp(txdetail.desaddr.c_str(),addr.c_str()) != 0)
			{
				return FALSE;
			}
		}else if (operate == 2)
		{
			if ( strcmp(txdetail.addr.c_str(),addr.c_str()) != 0)
			{
				return FALSE;
			}
		}else{
			if ( strcmp(txdetail.addr.c_str(),addr.c_str()) != 0 &&  strcmp(txdetail.desaddr.c_str(),addr.c_str()) != 0)
			{
				return FALSE;
			}
		}
	}

	return TRUE;

}


void CTradDlg::OnBnClickedUpPage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nConut -= 1 ;
	ShowPageCotent(m_nConut);
}


void CTradDlg::OnBnClickedNextPage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nConut += 1 ;
	ShowPageCotent(m_nConut);
}
void CTradDlg::ShowPageDataInfo(int nConut)
{
	nConut -=  1 ;
	m_nConut = nConut ;
	ShowPageCotent(nConut);
	return ;
}
void  CTradDlg::ShowPageCotent(int pageIndex)
{
	if ( 0 > pageIndex ){
		UiFun::MessageBoxEx(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_BEGIN_PAGE" ,theApp.gsLanguage) ,UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	} 
	if (pageIndex == 0)
	{
		ShowComboxCotent();
		return;
	}
	string strCond ;
	uistruct::TRANSRECORDLIST pListInfo ;
	int num = (pageIndex * m_pagesize - m_offset)<0?0:(pageIndex * m_pagesize - m_offset);
	std::stringstream strTemp;  
	string str;
	strTemp<< num;
	strTemp>>str;
	int operate = 0;
	string condtion = GetConditonStr(operate);
	strCond =  condtion + strprintf(" LIMIT %d OFFSET ",m_pagesize) + str ;
	theApp.m_SqliteDeal.GetTransactionList(strCond, &pListInfo); 
	if ( 0 == pListInfo.size() ) {
		m_nConut -= 1 ;
		UiFun::MessageBoxEx(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_END_PAGE" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		string strpage;
		strpage = strprintf("%d",m_nConut + 1);
		GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());
		Invalidate();
		return ;
	}
	int nPage = 0 ;
	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , condtion );
	if ( nItem == 0 ) return ;
	if ( nItem%m_pagesize != 0  ) {
		nPage = nItem/m_pagesize + 1 ;
	}else {
		nPage =  nItem/m_pagesize ;
	}
	string temp;
	temp =strprintf("%s%d",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TOTAL" ,theApp.gsLanguage) ,nPage);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));

	string strpage;
	strpage = strprintf("%d",m_nConut + 1);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());

	Invalidate();

	ShowListCtrl(&pListInfo);
}
void    CTradDlg::ShowComboxCotent()
{
	int operate = 0;
	string condtion = GetConditonStr(operate);
	uistruct::TRANSRECORDLIST pListInfo;

	string strCond0 =  condtion;

	string tempConditon = "confirm_height=0 and ";
	tempConditon +=strCond0;
	strCond0 = tempConditon;

	theApp.m_SqliteDeal.GetTransactionList(strCond0, &pListInfo); 

	uistruct::TRANSRECORDLIST pListInfo1;
	string strCond =  condtion;// + _T(" LIMIT 17 OFFSET 0") ;
	strCond += strprintf(" LIMIT %d OFFSET 0",(m_pagesize-pListInfo.size()));
	theApp.m_SqliteDeal.GetTransactionList(strCond, &pListInfo1); 

	m_offset = pListInfo.size();
	pListInfo.insert(pListInfo.end(),pListInfo1.begin(),pListInfo1.end());

	int nPage = 0 ;
	int nItem =  theApp.m_SqliteDeal.GetTableCountItem("t_transaction",strCond);
	//	if ( nItem == 0 ) return ;
	if ( nItem%m_pagesize != 0  ) {
		nPage = nItem/m_pagesize + 1 ;
	}else {
		nPage =  nItem/m_pagesize ;
	}
	string temp;
	temp =strprintf("%s%d",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TOTAL" ,theApp.gsLanguage) ,nPage );
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	if (nPage > 0)
	{
		GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T("1"));
	}else{
		GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T("0"));
	}
	Invalidate();

	OnShowListCtrl(pListInfo);
}

void CTradDlg::OnCbnSelchangeComboPage()
{
	// TODO: 在此添加控件通知处理程序代码
	int cursel = m_pageitem.GetCurSel();
	switch(cursel)
	{
	case 0:
		{
			m_pagesize = 13;
		}
		break;
	case 1:
		{
			m_pagesize = 20;
		}
		break;
	case 2:
		{
			m_pagesize = 30;
		}
		break;
	case 3:
		{
			m_pagesize = 50;
		}
		break;
	case 4:
		{
			m_pagesize = 100;
		}
		break;
	default:
		break;
	}
	m_nConut = 0 ;
	ShowComboxCotent();
}
