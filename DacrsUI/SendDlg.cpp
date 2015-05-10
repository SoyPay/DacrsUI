// SendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SendDlg.h"
#include "afxdialogex.h"


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
	if (m_pListaddrInfo.size() == 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不存在") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetItemData(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetCurSel())) ;
	if ( NULL != pListAddr ) {
		CString strCommand , strMaddress , strMoney;
		GetDlgItem(IDC_EDIT_DESADDRESS)->GetWindowTextA(strMoney);

		GetDlgItem(IDC_EDIT_MONEY)->GetWindowTextA(strMoney);

		strCommand.Format(_T("%s %s %s %lld"),_T("sendtoaddress") ,pListAddr->address ,strMaddress ,REAL_MONEY(atof(strMoney)) );
		CStringA strShowData ;

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
			theApp.cs_SqlData.Lock();
			int nItem =  theApp.m_SqliteDeal.FindDB(_T("revtransaction") , strHash1 ,_T("hash") ) ;
			theApp.cs_SqlData.Unlock();

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
	}
}


void CSendDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pListaddrInfo.size() == 0)
	{
		return;
	}
	uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetItemData(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetCurSel())) ;
	if ( NULL != pListAddr ) {
		double money = CSoyPayHelp::getInstance()->GetAccountBalance(pListAddr->address);
		CString strshow;
		strshow.Format(_T("%.8f"),money);
		((CStatic*)GetDlgItem(IDC_STATIC_XM))->SetWindowText(strshow);
	}
}
BOOL CSendDlg::AddListaddrDataBox(){

	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.GetListaddrData(&m_pListaddrInfo);
	theApp.cs_SqlData.Unlock();

	if ( 0 == m_pListaddrInfo.size() ) return FALSE ;

	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->ResetContent();
	//加载到ComBox控件
	int nItem = 0;
	std::vector<uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_pListaddrInfo.begin() ; const_it != m_pListaddrInfo.end() ; const_it++ ) {
		((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->InsertString(nItem , const_it->address );
		((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->SetItemData(nItem, (DWORD_PTR)&(*const_it));
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->SetCurSel(0);

	uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetItemData(((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->GetCurSel())) ;
	if ( NULL != pListAddr ) {
		double money = CSoyPayHelp::getInstance()->GetAccountBalance(pListAddr->address);
		CString strshow;
		strshow.Format(_T("%.8f"),money);
		((CStatic*)GetDlgItem(IDC_STATIC_XM))->SetWindowText(strshow);
	}
	return TRUE ;
}

LRESULT CSendDlg::OnShowListaddrData( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	switch (wParam)
	{
	case WM_UP_ADDRESS:
		{
			AddListaddrDataBox();
		}
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
		m_strTx1.SetFont(90, _T("Arial"));				//设置显示字体和大小
		AddListaddrDataBox();

		m_rBtnSend.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnSend.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnSend.SetWindowText("发送") ;
		m_rBtnSend.SetFontEx(24 , _T("微软雅黑"));
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
}
