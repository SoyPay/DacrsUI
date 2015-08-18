// ReceiveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ReceiveDlg.h"
#include "NewAddressDlg.h"
#include "SignAccountsDlg.h"
#include "afxdialogex.h"


// CReceiveDlg 对话框

IMPLEMENT_DYNAMIC(CReceiveDlg, CDialogBar)

CReceiveDlg::CReceiveDlg()
{
	m_pBmp = NULL ;
	m_prehittex = _T("");
	hitRow = -1;
	hitCol = -1;
}

CReceiveDlg::~CReceiveDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CReceiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SHOW, m_listCtrl);
	DDX_Control(pDX, IDC_BUTTON_JHDZ, m_rBtnAcitve);
	DDX_Control(pDX, IDC_BUTTON_NEWADDRESS, m_rBtnNewaddr);
	DDX_Control(pDX, IDC_COPYADDRESS, m_rBtnCopyaddr);
}


BEGIN_MESSAGE_MAP(CReceiveDlg, CDialogBar)
	ON_BN_CLICKED(IDC_COPYADDRESS, &CReceiveDlg::OnBnClickedCopyaddress)
	ON_MESSAGE(MSG_USER_RECIVE_UI , &CReceiveDlg::OnShowListCtrl )
	ON_BN_CLICKED(IDC_BUTTON_JHDZ, &CReceiveDlg::OnBnClickedButtonSignAccount)
	ON_BN_CLICKED(IDC_BUTTON_NEWADDRESS, &CReceiveDlg::OnBnClickedButtonNewaddress)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SHOW, &CReceiveDlg::OnNMDblclkListShow)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SHOW, &CReceiveDlg::OnNMClickListShow)
END_MESSAGE_MAP()


// CReceiveDlg 消息处理程序

void CReceiveDlg::ShowListInfo()
{


	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 order by reg_id"), &m_MapAddrInfo);

	if ( 0 == m_MapAddrInfo.size() ) return  ;

	m_listCtrl.DeleteAllItems();

	//加载到ComBox控件
	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		string strOrder="";
		strOrder = strprintf("%d", i+1);
		m_listCtrl.InsertItem(i,strOrder.c_str());

		uistruct::LISTADDR_t address = const_it->second;

		m_listCtrl.SetItemText(i , ++nSubIdx , address.RegID.c_str() ) ;

		
		m_listCtrl.SetItemText( i , ++nSubIdx, address.Label.c_str()) ;
		//m_listCtrl.SetItemData(item , (DWORD_PTR)&(*const_it)) ;

		m_listCtrl.SetItemText(i , ++nSubIdx , address.address.c_str() ) ;

		if (address.bSign == 1)
		{
			strShowData="已激活" ;
		}else{
			strShowData="未激活" ;
		}

		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		if (address.nColdDig== 1)
		{
			strShowData="支持" ;
		}else{
			strShowData="不支持" ;
		}
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		strShowData = strprintf("%.8f" , address.fMoney ) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		i++;
	}
	if ( 17 <= m_MapAddrInfo.size() )  {
		m_listCtrl.SetColumnWidth(5 , 86 ); 
	}else{
		m_listCtrl.SetColumnWidth(5 , 104 ); 
	}
}

BOOL CReceiveDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bRes =   CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if (bRes)
	{
		UpdateData(0);
		struct LISTCol {
			CString		name ;
			UINT		size ;
		} listcol[7]  = {
							{"序号" ,      50},
							{"账户ID" ,    100},
							{"标签" ,      100},
							{"地址" ,      230}, 
							{"激活状态" ,  80}, 
							{"冷挖矿" ,100},
							{"余额" ,      168}
							
						};
		m_listCtrl.SetBkColor(RGB(240,240,240));       
		m_listCtrl.SetRowHeigt(23);               
		m_listCtrl.SetHeaderHeight(1.5);         
		m_listCtrl.SetHeaderFontHW(15,0);
		m_listCtrl.SetHeaderBKColor(32,30,32,8); 
		m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); //设置头部字体颜色
		m_listCtrl.SetTextColor(RGB(0,0,0));  
		for( int i = 0 ; i <7 ; i++  ) {
			m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
		}
		m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );
		
		m_rBtnAcitve.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnAcitve.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnAcitve.SetWindowText("激活地址") ;
		m_rBtnAcitve.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnAcitve.SizeToContent();

		m_rBtnNewaddr.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnNewaddr.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNewaddr.SetWindowText("新建地址") ;
		m_rBtnNewaddr.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnNewaddr.SizeToContent();

		m_rBtnCopyaddr.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnCopyaddr.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnCopyaddr.SetWindowText("复制地址") ;
		m_rBtnCopyaddr.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnCopyaddr.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnCopyaddr.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnCopyaddr.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnCopyaddr.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnCopyaddr.SizeToContent();

		ShowListInfo();

		/*if( NULL == m_accountDlg ){
		m_accountDlg = new CSignAccountsDlg ;
		m_accountDlg->Create(CSignAccountsDlg::IDD, this) ;
		m_accountDlg->ShowWindow(SW_HIDE) ;
		}*/

		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_RECIVE_UI ) ;
	}
	return bRes;
}


void CReceiveDlg::OnBnClickedCopyaddress()
{
	// TODO: 在此添加控件通知处理程序代码
	string StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
			int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
			//uistruct::LISTADDR_t * pDbbetData = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
			CString source =m_listCtrl.GetItemText(nRow, 3);
			//文本内容保存在source变量中
			if(OpenClipboard())
			{
				HGLOBAL clipbuffer;
				char * buffer;
				EmptyClipboard();
				clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
				buffer = (char*)GlobalLock(clipbuffer);
				strcpy(buffer, LPCSTR(source));
				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();
				StrShow ="地址已复制到剪贴板\n";
				
				UiFun::MessageBoxEx(StrShow.c_str() , _T("提示") ,MFB_OK|MFB_TIP );
		     }
	}
	else{
		StrShow = "请选择地址!\n";
		
		UiFun::MessageBoxEx(StrShow.c_str() , _T("提示") ,MFB_OK|MFB_TIP );
	}
	
}

LRESULT  CReceiveDlg::OnShowListCtrl(  WPARAM wParam, LPARAM lParam )
{
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_ADDRESS:
		{
			ModifyListCtrlItem();
			break;
		}
		break;
	case WM_UP_NEWADDRESS:
		{
			InsertListCtrlItem();
			break;
		}
		break;
	default:
		break;

	}
	//ShowListInfo();
	return 1;
}

void CReceiveDlg::OnBnClickedButtonNewaddress()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if ( NULL != m_pNewAddressDlg ) {
		m_pNewAddressDlg->ShowWindow(SW_SHOW) ;
	}*/
	CNewAddressDlg dlg;
	dlg.DoModal();
}
void CReceiveDlg::OnBnClickedButtonSignAccount()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (!theApp.IsSyncBlock )
	{
		
		UiFun::MessageBoxEx(_T("同步未完成,不能发送激活交易") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}

	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		CString str = m_listCtrl.GetItemText(nRow, 3);
		string addr = strprintf("%s",str);
		if(!m_MapAddrInfo.count(addr))
			{
					TRACE("ERROR");
				StrShow.Format(_T("地址不存在\n"));
				
				UiFun::MessageBoxEx(StrShow , _T("提示") ,MFB_OK|MFB_TIP );
				return;
		   }
		uistruct::LISTADDR_t te =  m_MapAddrInfo[addr];
		//uistruct::LISTADDR_t * pAddrItem = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
		if (te.fMoney <=0)
		{
			StrShow.Format(_T("账户余额为零,不能激活!\n"));
			
			UiFun::MessageBoxEx(StrShow , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}
		if(te.bSign) 
		{
			StrShow.Format(_T("账户已激活!\n"));
			
			UiFun::MessageBoxEx(StrShow, _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}
		theApp.m_strAddress.Format(_T("%s") ,te.address.c_str() ) ;
		CSignAccountsDlg dlg;
		dlg.DoModal();

		/*m_accountDlg->SetShowAddr(pDbbetData->address);
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		m_accountDlg->MoveWindow(rcWindow.right/2+50,rcWindow.top+200,400,rcWindow.Height()/2);
		m_accountDlg->ShowWindow(SW_SHOW);*/
	}else{
		StrShow.Format(_T("请选择地址!\n"));
		
		UiFun::MessageBoxEx(StrShow , _T("提示") ,MFB_OK|MFB_TIP );
	}
}

BOOL CReceiveDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//判断是否按下键盘Enter键
		if (pMsg->wParam == VK_CONTROL || pMsg->wParam == VK_ESCAPE ||pMsg->wParam == VK_SHIFT )
		{
			return true;
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}

void CReceiveDlg::SetBkBmpNid( UINT nBitmapIn ) 
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

void CReceiveDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;
		CRect rc  ;
		GetClientRect( rc ) ;
		CButton *pList = (CButton*)GetDlgItem(IDC_LIST_SHOW);
		if( NULL != pList ) {	
			pList->SetWindowPos(NULL ,32, 21 , 837 , 408 , SWP_SHOWWINDOW);
		}
		CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_JHDZ);
		if( NULL != pButton ) {	
			CRect m_BtnRc ;
			pButton->GetClientRect(&m_BtnRc);
			pButton->SetWindowPos(NULL ,900 - 3*(103 + 5) - 23 , 600 - 72 - 32 - 46 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		}
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_NEWADDRESS);
		if( NULL != pButton ) {	
			CRect m_BtnRc ;
			pButton->GetClientRect(&m_BtnRc);
			pButton->SetWindowPos(NULL ,900 - 2*(103 + 5) - 23 , 600 - 72 - 32 - 46 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		}
		pButton = (CButton*)GetDlgItem(IDC_COPYADDRESS);
		if( NULL != pButton ) {	
			CRect m_BtnRc ;
			pButton->GetClientRect(&m_BtnRc);
			pButton->SetWindowPos(NULL ,900 - 1*(103 + 5)- 23 , 600 - 72 - 32 - 46 , m_BtnRc.Width() , m_BtnRc.Height() , SWP_SHOWWINDOW);
		}
	}
}


int CReceiveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_RECEIVE);

	return 0;
}


BOOL CReceiveDlg::OnEraseBkgnd(CDC* pDC)
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

void   CReceiveDlg::ModifyListCtrlItem()
{
	CPostMsg postmsg;
	if (!theApp.m_UiReciveDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());



	if(m_MapAddrInfo.count(addr.address) <= 0)
	{
		TRACE("map ModifyListCtrlItem ERROR");
		return;
	}
	
	m_MapAddrInfo[addr.address]=addr;
	
	int count = m_listCtrl.GetItemCount();
	if ( 17 <= count )  {
		m_listCtrl.SetColumnWidth(5 , 86 ); 
	}else{
		m_listCtrl.SetColumnWidth(5 , 104 ); 
	}
	for(int i = 0; i < count; i++)
	{
		CString str = m_listCtrl.GetItemText(i, 3); // 这个函数名具体忘了，就是取得每个item第0列的值
		if(strcmp(str,addr.address.c_str()) == 0)
		////uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(i);
		//if (!memcmp(pListAddr->address,addr.address,sizeof(pListAddr->address)) &&\
		//	(pListAddr->fMoney != addr.fMoney || pListAddr->bSign != addr.bSign))
		{
			/*pListAddr->fMoney = addr.fMoney;
			pListAddr->bSign = addr.bSign;*/
		
			int nSubIdx = 0;
			string  strShowData;
		
			m_listCtrl.SetItemText(i , ++nSubIdx , addr.RegID.c_str() ) ;
	
			m_listCtrl.SetItemText(i , ++nSubIdx , addr.Label.c_str() ) ;
			//m_listCtrl.SetItemData(i , (DWORD_PTR)&(*m_pListaddrInfo.rbegin())) ;


			m_listCtrl.SetItemText(i , ++nSubIdx , addr.address.c_str() ) ;

			if (addr.bSign == 1)
			{
				strShowData="已激活" ;
			}else{
				strShowData = "未激活" ;
			}

			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

			if (addr.nColdDig== 1)
			{
				strShowData = "支持" ;
			}else{
				strShowData = "不支持" ;
			}
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str()) ;

			strShowData = strprintf("%.8f" ,addr.fMoney ) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;
			break;
		}
	}
}
void   CReceiveDlg::InsertListCtrlItem()
{
	CPostMsg postmsg;
	if (!theApp.m_UiReciveDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());



	if(m_MapAddrInfo.count(addr.address) > 0){
		TRACE("map InsertListCtrlItem ERROR");
		return ;
	}

	int count = m_listCtrl.GetItemCount();

	int nSubIdx = 0,i =count;

	string strOrder(_T(""));
	strOrder =strprintf("%d", ++count);
	
	m_listCtrl.InsertItem( i , strOrder.c_str());  //序号
	string  strShowData;

	m_listCtrl.SetItemText(i , ++nSubIdx , addr.RegID.c_str() ) ;


	m_listCtrl.SetItemText( i , ++nSubIdx, addr.Label.c_str() ) ; //标签
	
	m_MapAddrInfo[addr.address]=addr;

	    //地址
	m_listCtrl.SetItemText( i , ++nSubIdx, addr.address.c_str() ); 

	if (addr.bSign == 1)
	{
		strShowData="已激活" ;
	}else{
		strShowData="未激活" ;
	}

	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

	if (addr.nColdDig== 1)
	{
		strShowData="支持";
	}else{
		strShowData="不支持" ;
	}
	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

	strShowData =strprintf("%.8f" , addr.fMoney ) ;
	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

}

void CReceiveDlg::OnNMDblclkListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	LVHITTESTINFO info;
     info.pt  =  pNMItemActivate -> ptAction;
 
       if (m_listCtrl.SubItemHitTest( & info)  !=   - 1  )
      {
          hitRow  =  info.iItem;
          hitCol  =  info.iSubItem;
		  if ( hitCol!= 2)
		  {
			  *pResult = 0;
			  return;
		  }
           if (editItem.m_hWnd  ==  NULL ) // editItem为一输入框控件， 
          {
              RECT rect;
              rect.left  =   0 ;
              rect.top  =   0 ;
              rect.bottom  =   15 ;
              rect.right  =   200 ;
              editItem .Create(WS_CHILD  |  ES_LEFT  |  WS_BORDER  |  ES_AUTOHSCROLL  |  ES_WANTRETURN  |  ES_MULTILINE, rect,  this ,  101 );
              editItem.SetFont( this -> GetFont(), FALSE);
          }
		   /// 保存修改之前的字符串
		 editItem.GetWindowText(m_prehittex);
          CRect rect;
          m_listCtrl.GetSubItemRect(info.iItem, info.iSubItem, LVIR_BOUNDS, rect);
          rect.top  +=   22 ;
          rect.left  +=   35 ;
          rect.right  +=   35 ;
         rect.bottom  +=   22 ;
  
          editItem.SetWindowText(m_listCtrl.GetItemText( info .iItem,  info .iSubItem));
          editItem.MoveWindow( & rect, TRUE);
          editItem.ShowWindow( 1 );
		  editItem.SetSel(-1);
          editItem.SetFocus();
      }
	*pResult = 0;
}


void CReceiveDlg::OnNMClickListShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
		   if (editItem.m_hWnd  !=  NULL)
		 {
			 
			 if( hitCol!= 2)
			 {
				 hitCol  =  hitRow  =   - 1 ;
				*pResult = 0;
			 }
			 editItem.ShowWindow( 0 );
			  if (hitRow  !=   - 1 )
			{
				CString text;
				editItem.GetWindowText(text);
				m_listCtrl.SetItemText(hitRow, hitCol, text);
				if (text != m_prehittex)
				{
					CString addr = _T("");
					addr = m_listCtrl.GetItemText(hitRow,3);
					string strSourceData  , strW ;
					strSourceData =strprintf("label = '%s'", text  ) ;
					strW =strprintf("address = '%s'" , addr ) ;

					uistruct::DATABASEINFO_t DatabaseInfo;
					DatabaseInfo.strSource = strSourceData;
					DatabaseInfo.strWhere = strW ;
					DatabaseInfo.strTabName = _T("t_wallet_address");
					CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
					string strtemp = DatabaseInfo.ToJson();

					postmsg.SetData(strtemp);
					theApp.m_MsgQueue.push(postmsg);
				}
			}
	}
	hitCol  =  hitRow  =   - 1 ;
	*pResult = 0;
}
