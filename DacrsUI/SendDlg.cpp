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


	if (m_mapAddrInfo.size() == 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("发送地址不存在") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	CString text;
	int sel = m_addrbook.GetCurSel();
	m_addrbook.GetLBText(sel,text);

	uistruct::LISTADDR_t data;
	CString strCommand , strMaddress , strMoney;
	if(text!=_T(""))
	{
		if(m_mapAddrInfo.count(text)<=0)
		{
			//::MessageBox( this->GetSafeHwnd() ,_T("发送地址不存在") , _T("提示") , MB_ICONINFORMATION ) ;
			return;
		}
		//uistruct::LISTADDR_t te = m_pListaddrInfo[text];
		data = m_mapAddrInfo[text];
	
	}

		if(!data.bSign) 
		{
			::MessageBox( this->GetSafeHwnd() ,_T("发送地址未激活") , _T("提示") , MB_ICONINFORMATION ) ;
			return;
		}

		GetDlgItem(IDC_EDIT_DESADDRESS)->GetWindowTextA(strMaddress);
		if (strMaddress == _T(""))
		{
			::MessageBox( this->GetSafeHwnd() ,_T("接受地址不能未空") , _T("提示") , MB_ICONINFORMATION ) ;
			return;
		}
		if(!strcmp(strMaddress.GetString(), data.address))
		{
			::MessageBox( this->GetSafeHwnd() ,_T("发送地址和目的地址不能相同") , _T("提示") , MB_ICONINFORMATION ) ;
			return;
		}
		
		GetDlgItem(IDC_EDIT_MONEY)->GetWindowTextA(strMoney);
		double dSendMoney = atof(strMoney);
		if(dSendMoney > data.fMoney || ( data.fMoney>-0.0000001 && data.fMoney< 0.000001 )) 
		{
			::MessageBox( this->GetSafeHwnd() ,_T("账户余额不足") , _T("提示") , MB_ICONINFORMATION ) ;
			return;
		}

		if(_T("") == strMoney.GetString() || (dSendMoney >-0.0000001 && dSendMoney< 0.000001))
		{
			::MessageBox( this->GetSafeHwnd() ,_T("发送金额不能为0") , _T("提示") , MB_ICONINFORMATION ) ;
			return;
		}

		strCommand.Format(_T("%s %s %s %lld"),_T("sendtoaddress") ,data.address ,strMaddress ,REAL_MONEY(dSendMoney));
		CStringA strShowData ;

		CString strDisplay;
		strDisplay.Format(_T("确定发送%.4lfsmc到%s"), dSendMoney, strMaddress);
		COut outdlg(NULL, strDisplay,100);
		if ( IDOK != outdlg.DoModal()){
			return;
		}
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strShowData.GetString(), root)) 
		{
			::MessageBox( this->GetSafeHwnd() ,strShowData , _T("提示") , MB_ICONINFORMATION ) ;
			return  ;
		}
		BOOL bRes = FALSE ;
		CString strGettxdetail;
		int pos = strShowData.Find("hash");
		if ( pos >=0 ) {
			//插入到数据库
			CString strHash,strHash1 ;
			strHash.Format(_T("'%s'") , root["hash"].asCString() );
			strHash1.Format(_T("%s") , root["hash"].asCString() );
//			theApp.cs_SqlData.Lock();
			int nItem =  theApp.m_SqliteDeal.FindDB(_T("revtransaction") , strHash1 ,_T("hash") ) ;
//			theApp.cs_SqlData.Unlock();

			if ( 0 == nItem ) {

				CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
				postmsg.SetData(strHash);
				theApp.m_MsgQueue.push(postmsg);
			}
		}

		CString strData;
		if ( pos >=0 ) {
			strData.Format( _T("转账成功\n%s") , root["hash"].asCString() ) ;
		}else{
			strData.Format( _T("转账失败!") ) ;
		}
		::MessageBox( this->GetSafeHwnd() ,strData , _T("提示") , MB_ICONINFORMATION ) ;

		//// 插入数据库,将收款人添加到地址簿
		CString lebel;
		GetDlgItem(IDC_EDIT2)->GetWindowTextA(lebel);
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRBOOK);
		uistruct::ADDRBOOK_t addr;
		addr.address = strMaddress;
		addr.lebel = lebel;
		string temp =addr.ToJson();
		postmsg.SetData(temp.c_str());
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
	m_addrbook.GetLBText(sel,text);
	
	//m_addrbook.GetWindowText(text) ;
	if(text!=_T(""))
	{
	   if(m_mapAddrInfo.count(text)<=0)
	   {
		   TRACE("map OnCbnSelchangeCombo1 error");
		   return;
	   }
	//uistruct::LISTADDR_t te = m_pListaddrInfo[text];
	CString strshow;
	strshow.Format(_T("%.8f"),m_mapAddrInfo[text].fMoney);
	((CStatic*)GetDlgItem(IDC_STATIC_XM))->SetWindowText(strshow);
	Invalidate();
	}

}
BOOL CSendDlg::AddListaddrDataBox(){

//	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.GetListaddrData(&m_mapAddrInfo);
//	theApp.cs_SqlData.Unlock();

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->ResetContent();
	//加载到ComBox控件
	int nItem = 0;
	std::map<CString,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {

		((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->InsertString(nItem , const_it->first );
		//((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->SetItemData(nItem, (DWORD_PTR)&(*const_it));
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->SetCurSel(0);

	CString address;
	int sel = m_addrbook.GetCurSel();
	m_addrbook.GetLBText(sel,address);
	std::map<CString,uistruct::LISTADDR_t>::const_iterator item = m_mapAddrInfo.find(address);

	if (m_mapAddrInfo.count(address)>0 ) {
		uistruct::LISTADDR_t addrstruc = m_mapAddrInfo[address];
		CString strshow;
		strshow.Format(_T("%.8f"),addrstruc.fMoney);
		m_strTx1.SetWindowText(strshow);
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
	CString strShowData;
	INT_PTR nResponse = addrbook.DoModal();;
	if (nResponse == IDOK)
	{
		uistruct::ADDRBOOK_t addr;
		addrbook.GetAddrbook(addr);
		GetDlgItem(IDC_EDIT_DESADDRESS)->SetWindowTextA(addr.address);
		GetDlgItem(IDC_EDIT2)->SetWindowTextA(addr.lebel);
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

	CString addressd;
	addressd.Format(_T("%s"),addr.address);

	if(m_mapAddrInfo.count(addressd)<=0)
	{
		TRACE("map ModifyComboxItem error");
		return;
	}
	m_mapAddrInfo[addressd]=addr;
	
		
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
	
	CString addressd;
	addressd.Format(_T("%s"),addr.address);

	if(m_mapAddrInfo.count(addressd)>0)
	{
		TRACE("map InsertComboxIitem error");
		return;
	}
	m_mapAddrInfo[addressd]=addr;

	int item = m_addrbook.GetCount();
	m_addrbook.InsertString(item,addressd);
}
