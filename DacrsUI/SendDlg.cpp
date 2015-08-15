// SendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SendDlg.h"
#include "afxdialogex.h"
#include "Out.h"
#include "AddrBook.h"
// CSendDlg 对话框

IMPLEMENT_DYNAMIC(CSendDlg, CDialogBar)

CSendDlg::CSendDlg()
{
	m_pBmp = NULL ;
}

CSendDlg::~CSendDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADDBOOK , m_rBtnAddbook);
	DDX_Control(pDX, IDC_SENDTRNSFER , m_rBtnSend);
	DDX_Control(pDX, IDC_STATIC_XM , m_strTx1);
	DDX_Control(pDX, IDC_COMBO_ADDR_OUT , m_addrbook);
}


BEGIN_MESSAGE_MAP(CSendDlg, CDialogBar)
	ON_BN_CLICKED(IDC_SENDTRNSFER, &CSendDlg::OnBnClickedSendtrnsfer)
	ON_CBN_SELCHANGE(IDC_COMBO_ADDR_OUT, &CSendDlg::OnCbnSelchangeCombo1)
	ON_MESSAGE(MSG_USER_SEND_UI , &CSendDlg::OnShowListaddrData  )
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_ADDBOOK, &CSendDlg::OnBnClickedButtonAddbook)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTransfer 消息处理程序

void CSendDlg::SetBkBmpNid( UINT nBitmapIn ) 
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


void CSendDlg::OnBnClickedSendtrnsfer()
{
	// TODO: 在此添加控件通知处理程序代码
	/*CString strDisplay1;
	strDisplay1.Format(_T(" 转账交易发送成功\n\n源地址:DV9u9jgtyJoDsZ4m2MmrtQwLfNDZs952qx\r\n\r\n目的地址:DhxrQ9hsvo3fVVSy6By8bePt8cmPtts88R4eac46f7718ae7175d67b58a4886f2f91746cbdb30b4f2ee0dd96c70168f4fae \r\n\r\n金额：1.0000\r\n\r\nhash:4eac46f7718ae7175d67b58a4886f2f91746cbdb30b4f2ee0dd96c70168f4fae"));
	if (IDYES == UiFun::MessageBoxEx(strDisplay1 , _T("提示") , MFB_YESNO|MFB_TIP ) )
	{
		int b =5;
	}*/
	if (theApp.IsLockWallet())
	{
		return ;
	}

	if (m_mapAddrInfo.size() == 0)
	{
		UiFun::MessageBoxEx(_T("发送地址不存在") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}
	CString text =_T("");
	//int sel = m_addrbook.GetCurSel();
	//if (sel < 0)
	//{
	//	return ;
	//}
	//m_addrbook.GetLBText(sel,text);
	m_addrbook.GetWindowText(text);
	if (text == _T(""))
	{
		return;
	}
	uistruct::LISTADDR_t data;
	string strCommand  ;CString strMoney,strMaddress;
	if(text!=_T(""))
	{
		string strAddr =strprintf("%s",text);
		strAddr =UiFun::trimleft(strAddr);
		strAddr =UiFun::trimright(strAddr);
		if(m_mapAddrInfo.count(strAddr)<=0)
		{
			UiFun::MessageBoxEx(_T("发送地址不是钱包地址") , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}
		//uistruct::LISTADDR_t te = m_pListaddrInfo[text];
		data = m_mapAddrInfo[strAddr];
	
	}

		if(!data.bSign) 
		{
			UiFun::MessageBoxEx(_T("发送地址未激活") , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}

		GetDlgItem(IDC_EDIT_DESADDRESS)->GetWindowTextA(strMaddress);
		if (strMaddress == _T(""))
		{
			UiFun::MessageBoxEx(_T("接受地址不能未空") , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}
		if(!strcmp(strMaddress.GetString(), data.address.c_str()))
		{
			UiFun::MessageBoxEx(_T("发送地址和目的地址不能相同") , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}
		
		GetDlgItem(IDC_EDIT_MONEY)->GetWindowTextA(strMoney);
		double dSendMoney = strtod(strMoney,NULL);
		if(dSendMoney > data.fMoney || ( data.fMoney>-0.0000001 && data.fMoney< 0.000001 )) 
		{
			UiFun::MessageBoxEx(_T("账户余额不足") , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}

		if(_T("") == strMoney.GetString() || (dSendMoney >-0.0000001 && dSendMoney< 0.000001))
		{
			UiFun::MessageBoxEx(_T("发送金额不能为0") , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}

		if (!theApp.IsSyncBlock )
		{
			UiFun::MessageBoxEx(_T("同步未完成,不能发送交易") , _T("提示") ,MFB_OK|MFB_TIP );
			return;
		}
		strCommand = strprintf("%s %s %s %lld","sendtoaddress" ,data.address.c_str() ,strMaddress ,REAL_MONEY(dSendMoney));
		string strShowData ;

		CString strDisplay;
		strDisplay.Format(_T("转账%.4lfsmc至%s"), dSendMoney, strMaddress);

		if (IDCANCEL == UiFun::MessageBoxEx(strDisplay , _T("提示") , MFB_OKCANCEL|MFB_TIP ) )
		{
			return;
		}

		strShowData = _T("");
		Json::Value root;
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("OnBnClickedSendtrnsfer rpccmd sendtoaddress error");
			return;
		}
		BOOL bRes = FALSE ;
		strShowData = root.toStyledString();
		int pos = strShowData.find("hash");
		if ( pos >=0 ) {
			//插入到数据库
			string strHash = root["hash"].asString() ;
			string strCond;
			strCond = strprintf(" hash='%s' ", strHash.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , strCond) ;

			if ( 0 == nItem ) {

				CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
				postmsg.SetData(strHash);
				theApp.m_MsgQueue.push(postmsg);
			}
		}

		string strData;
		if ( pos >=0 ) {
			strData = strprintf(" 转账交易发送成功\n\n 源地址:%s\n\n 目的地址:%s\n\n 金额：%.4lf\n  hash:%s", data.address.c_str(),strMaddress, dSendMoney,root["hash"].asCString()) ;
		}else{
			strData = "转账失败!";
		}
		UiFun::MessageBoxEx(strData.c_str() , _T("提示") ,MFB_OK|MFB_TIP );
		

		//// 插入数据库,将收款人添加到地址簿
		CString label;
		GetDlgItem(IDC_EDIT2)->GetWindowTextA(label);
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRBOOK);
		uistruct::ADDRBOOK_t addr;
		addr.address = strMaddress;
		addr.label = label;
		string temp =addr.ToJson();
		postmsg.SetData(temp);
		theApp.m_MsgQueue.push(postmsg);

}


void CSendDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_mapAddrInfo.size() == 0)
	{
		return;
	}
	CString text;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return ;
	}
	m_addrbook.GetLBText(sel,text);
	
	//m_addrbook.GetWindowText(text) ;
	string strAddr= strprintf("%s",text);
	if(text!=_T(""))
	{
		
	   if(m_mapAddrInfo.count(strAddr)<=0)
	   {
		   TRACE("map OnCbnSelchangeCombo1 error");
		   return;
	   }
	//uistruct::LISTADDR_t te = m_pListaddrInfo[text];
	string strshow;
	strshow = strprintf("%.8f",m_mapAddrInfo[strAddr].fMoney);
	((CStatic*)GetDlgItem(IDC_STATIC_XM))->SetWindowText(strshow.c_str());
	Invalidate();
	}

}
BOOL CSendDlg::AddListaddrDataBox(){

	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->ResetContent();
	//加载到ComBox控件
	int nItem = 0;
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {

		((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->InsertString(nItem , const_it->first.c_str() );
		//((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->SetItemData(nItem, (DWORD_PTR)&(*const_it));
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->SetCurSel(0);

	CString address;
	int sel = m_addrbook.GetCurSel();
	if (sel < 0)
	{
		return FALSE;
	}
	m_addrbook.GetLBText(sel,address);
	string strAddr =strprintf("%s",address);
	std::map<string,uistruct::LISTADDR_t>::const_iterator item = m_mapAddrInfo.find(strAddr);

	if (m_mapAddrInfo.count(strAddr)>0 ) {
		uistruct::LISTADDR_t addrstruc = m_mapAddrInfo[strAddr];
		string strshow;
		strshow = strprintf("%.8f",addrstruc.fMoney);
		m_strTx1.SetWindowText(strshow.c_str());
		Invalidate();
	}
	return TRUE ;
}

LRESULT CSendDlg::OnShowListaddrData( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_ADDRESS:
		{
			ModifyComboxItem();
			OnCbnSelchangeCombo1();
			break;
		}
		break;
	case WM_UP_NEWADDRESS:
		{
			InsertComboxIitem();
			break;
		}
		break;
	default:
		break;

	}
	return 0 ;
}
// CSendDlg 消息处理程序


BOOL CSendDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes =   CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if (bRes) 
	{
		//m_rBtnSend.LoadBitmaps(IDB_BITMAP_BOTTON_SEND1,IDB_BITMAP_BOTTON_SEND2,IDB_BITMAP_BOTTON_SEND3,IDB_BITMAP_BOTTON_SEND3);
		m_rBtnAddbook.LoadBitmaps(IDB_BITMAP_ADDBOOK,IDB_BITMAP_ADDBOOK,IDB_BITMAP_ADDBOOK,IDB_BITMAP_ADDBOOK);
		UpdateData(0);
		m_strTx1.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_strTx1.SetWindowText(_T(""));
		AddListaddrDataBox();

		m_rBtnSend.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnSend.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnSend.SetWindowText("发  送") ;
		m_rBtnSend.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnSend.SizeToContent();



		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_SEND_UI ) ;
		((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);
		
	}
	return bRes;
}


int CSendDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_SENDUI_BJ);

	return 0;
}


BOOL CSendDlg::OnEraseBkgnd(CDC* pDC)
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

	return 1;
}

void CSendDlg::OnBnClickedButtonAddbook()
{
	// TODO: 在此添加控件通知处理程序代码
	CAddrBook addrbook;
	INT_PTR nResponse = addrbook.DoModal();;
	if (nResponse == IDOK)
	{
		uistruct::ADDRBOOK_t addr;
		addrbook.GetAddrbook(addr);
		GetDlgItem(IDC_EDIT_DESADDRESS)->SetWindowTextA(addr.address.c_str());
		GetDlgItem(IDC_EDIT2)->SetWindowTextA(addr.label.c_str());
	}
}
void CSendDlg::ModifyComboxItem(){
	CPostMsg postmsg;
	if (!theApp.m_UiSendDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());


	if(m_mapAddrInfo.count(addr.address)<=0)
	{
		TRACE("map ModifyComboxItem error");
		return;
	}
	m_mapAddrInfo[addr.address]=addr;
	
		
}
void CSendDlg::InsertComboxIitem()
{
	CPostMsg postmsg;
	if (!theApp.m_UiSendDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());
	

	if(m_mapAddrInfo.count(addr.address)>0)
	{
		TRACE("map InsertComboxIitem error");
		return;
	}
	m_mapAddrInfo[addr.address]=addr;

	int item = m_addrbook.GetCount();
	m_addrbook.InsertString(item,addr.address.c_str());
}


BOOL CSendDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_PASTE)
	{
		int b =5;
	}
	if (pMsg->message==WM_KEYDOWN)
	{
		BOOL bCtrl=::GetKeyState(VK_CONTROL)&0x8000;
		BOOL bShift=::GetKeyState(VK_SHIFT)&0x8000;

		// only gets here if CTRL key is pressed
		BOOL bAlt=::GetKeyState(VK_MENU)&0x8000;
		switch( pMsg->wParam )
		{
			if (GetDlgItem(IDC_COMBO_ADDR_OUT)->SetFocus() == this->GetFocus())
			{
				int b = 6;
			}
		case 'V':
			{
				if (GetDlgItem(IDC_EDIT_DESADDRESS) == this->GetFocus())
				{
					HWND hWnd = GetSafeHwnd(); // 获取安全窗口句柄
					::OpenClipboard(hWnd); // 打开剪贴板
					HANDLE hClipMemory = ::GetClipboardData(CF_TEXT);// 获取剪贴板数据句柄
					DWORD dwLength = GlobalSize(hClipMemory); // 返回指定内存区域的当前大小
					LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory); // 锁定内存
					CString message = CString(lpClipMemory); // 保存得到的文本数据
					GlobalUnlock(hClipMemory); // 内存解锁
					::CloseClipboard(); // 关闭剪贴板
					if (bCtrl)
					{
						uistruct::ADDRBOOK_t addrBook;
						string strCond;
						strCond = strprintf(" address='%s' ", message);
						int nItem =  theApp.m_SqliteDeal.GetAddressBookItem( strCond, &addrBook) ;
						if (addrBook.label != _T(""))
						{
							GetDlgItem(IDC_EDIT2)->SetWindowText(addrBook.label.c_str());
						}
						
					}
					
				}else if (GetDlgItem(IDC_COMBO_ADDR_OUT)->SetFocus() == this->GetFocus())
				{
					HWND hWnd = GetSafeHwnd(); // 获取安全窗口句柄
					::OpenClipboard(hWnd); // 打开剪贴板
					HANDLE hClipMemory = ::GetClipboardData(CF_TEXT);// 获取剪贴板数据句柄
					DWORD dwLength = GlobalSize(hClipMemory); // 返回指定内存区域的当前大小
					LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory); // 锁定内存
					CString message = CString(lpClipMemory); // 保存得到的文本数据
					GlobalUnlock(hClipMemory); // 内存解锁
					::CloseClipboard(); // 关闭剪贴板
					if (bCtrl)
					{
						string addr = strprintf("%s",message);
						if(m_mapAddrInfo.count(addr)<=0)
						{
							//TRACE("map OnCbnSelchangeCombo1 error");
							
							UiFun::MessageBoxEx(_T("复制的地址有误") , _T("提示") ,MFB_OK|MFB_TIP );
						}else{
							//uistruct::LISTADDR_t te = m_pListaddrInfo[text];
							CString strshow;
							strshow.Format(_T("%.8f"),m_mapAddrInfo[addr].fMoney);
							((CStatic*)GetDlgItem(IDC_STATIC_XM))->SetWindowText(strshow);
							Invalidate();
						}

					}
				}

			}
			break;
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}


void CSendDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;

		CRect rc ;
		GetClientRect( rc ) ;

		CWnd *pst = GetDlgItem( IDC_COMBO_ADDR_OUT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*24 , (rc.Height()/100)*12-3 , (rc.Width()/100)*45, rc.Height()  ,SWP_SHOWWINDOW ) ; 
			CComboBox*   pCtl  = (CComboBox*)pst; 
			CWnd *p_edit = pCtl->GetDlgItem (0x3E9);
			p_edit->GetClientRect( rect );
			p_edit->SetWindowPos( NULL, (rc.Width()/100)*25,(rc.Height()/100)*25-3, rect.Width(), rect.Height(), SWP_SHOWWINDOW );
		    pCtl->SetFont (&theApp.m_fontBlackbody);
			p_edit->SetFont (&theApp.m_fontBlackbody);
			
			pCtl->SendMessage(CB_SETITEMHEIGHT,(WPARAM)-1,(LPARAM)20);   //改变控件本身的高度
			pCtl->SetItemHeight(0,15);									 //改变下拉列表每个Item的高度
		}

		pst = GetDlgItem( IDC_STATIC_XM ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*80 ,(rc.Height()/100)*12 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		
		pst = GetDlgItem( IDC_EDIT_DESADDRESS ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*24 ,(rc.Height()/100)*23, (rc.Width()/100)*45, (rc.Height()/100)*7 ,SWP_SHOWWINDOW );
			pst->SetFont(&theApp.m_fontBlackbody);
		}
		
		pst = GetDlgItem( IDC_BUTTON_ADDBOOK ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*70 ,(rc.Height()/100)*23-2 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW);
		}

		pst = GetDlgItem( IDC_EDIT2) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*24 ,(rc.Height()/100)*35, rect.Width(), (rc.Height()/100)*7  ,SWP_SHOWWINDOW ); 
			pst->SetFont(&theApp.m_fontBlackbody);
		}
		int with = 0;
		pst = GetDlgItem( IDC_EDIT_MONEY) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*24 ,(rc.Height()/100)*47, rect.Width(), (rc.Height()/100)*7  ,SWP_SHOWWINDOW ) ; 
			with = rect.Width();
			pst->SetFont(&theApp.m_fontBlackbody);
		}
		
		pst = GetDlgItem( IDC_COMBO2) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*25 +with,(rc.Height()/100)*47, rect.Width(), rect.Height(), SWP_SHOWWINDOW );
		
			CComboBox*   pCtl  = (CComboBox*)pst; 
			CWnd *p_edit = pCtl->GetDlgItem (0x3E9);
			p_edit->GetClientRect( rect );
			p_edit->SetWindowPos( NULL, (rc.Width()/100)*25 +with,(rc.Height()/100)*25-3, rect.Width(), rect.Height(), SWP_SHOWWINDOW );
		    pCtl->SetFont (&theApp.m_fontBlackbody);
			p_edit->SetFont (&theApp.m_fontBlackbody);

			pCtl->SendMessage(CB_SETITEMHEIGHT,(WPARAM)-1,(LPARAM)20);   //改变控件本身的高度
			pCtl->SetItemHeight(0,15);									 //改变下拉列表每个Item的高度
		}

		pst = GetDlgItem( IDC_SENDTRNSFER) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*90 ,(rc.Height()/100)*65, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

	}
	// TODO: 在此处添加消息处理程序代码
}
