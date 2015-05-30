// ReceiveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ReceiveDlg.h"
#include "NewAddressDlg.h"
#include "SignAccountsDlg.h"
#include "afxdialogex.h"


// CReceiveDlg �Ի���

IMPLEMENT_DYNAMIC(CReceiveDlg, CDialogBar)

CReceiveDlg::CReceiveDlg()
{
	m_pBmp = NULL ;
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
END_MESSAGE_MAP()


// CReceiveDlg ��Ϣ�������

void CReceiveDlg::ShowListInfo()
{


	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), &m_MapAddrInfo);

	if ( 0 == m_MapAddrInfo.size() ) return  ;

	m_listCtrl.DeleteAllItems();

	//���ص�ComBox�ؼ�
	int nSubIdx = 0 , i = 0 ;
	CString strShowData = _T("");
	std::map<CString,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		CString strOrder("");
		strOrder.Format(_T("%d"), i+1);
		m_listCtrl.InsertItem(i,strOrder);

		uistruct::LISTADDR_t address = const_it->second;

		strShowData.Format(_T("%s") ,address.RegID) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		strShowData.Format(_T("%s") ,address.Label) ;
		m_listCtrl.SetItemText( i , ++nSubIdx, strShowData) ;
		//m_listCtrl.SetItemData(item , (DWORD_PTR)&(*const_it)) ;


		strShowData.Format(_T("%s") ,address.address) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		if (address.bSign == 1)
		{
			strShowData.Format(_T("�Ѽ���")) ;
		}else{
			strShowData.Format(_T("δ����")) ;
		}

		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		if (address.nColdDig== 1)
		{
			strShowData.Format(_T("֧��")) ;
		}else{
			strShowData.Format(_T("��֧��")) ;
		}
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		strShowData.Format(_T("%.8f") , address.fMoney ) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

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
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes =   CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if (bRes)
	{
		UpdateData(0);
		struct LISTCol {
			CString		name ;
			UINT		size ;
		} listcol[7]  = {
							{"���" ,      50},
							{"�˻�ID" ,    100},
							{"��ǩ" ,      100},
							{"��ַ" ,      230}, 
							{"����״̬" ,  80}, 
							{"���ڿ�" ,100},
							{"���" ,      168}
							
						};
		m_listCtrl.SetBkColor(RGB(240,240,240));       
		m_listCtrl.SetRowHeigt(23);               
		m_listCtrl.SetHeaderHeight(1.5);         
		m_listCtrl.SetHeaderFontHW(15,0);
		m_listCtrl.SetHeaderBKColor(32,30,32,8); 
		m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); //����ͷ��������ɫ
		m_listCtrl.SetTextColor(RGB(0,0,0));  
		for( int i = 0 ; i <7 ; i++  ) {
			m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
		}
		m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );
		
		m_rBtnAcitve.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnAcitve.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnAcitve.SetWindowText("�����ַ") ;
		m_rBtnAcitve.SetFontEx(20 , _T("΢���ź�"));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnAcitve.SizeToContent();

		m_rBtnNewaddr.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnNewaddr.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNewaddr.SetWindowText("�½���ַ") ;
		m_rBtnNewaddr.SetFontEx(20 , _T("΢���ź�"));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		m_rBtnNewaddr.SizeToContent();

		m_rBtnCopyaddr.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnCopyaddr.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnCopyaddr.SetWindowText("���Ƶ�ַ") ;
		m_rBtnCopyaddr.SetFontEx(20 , _T("΢���ź�"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
			int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
			//uistruct::LISTADDR_t * pDbbetData = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
			CString source =m_listCtrl.GetItemText(nRow, 3);
			//�ı����ݱ�����source������
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
				StrShow.Format(_T("��ַ�Ѹ��Ƶ�������\n"));
				::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
		     }
	}
	else{
		StrShow.Format(_T("��ѡ���ַ!\n"));
		::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*if ( NULL != m_pNewAddressDlg ) {
		m_pNewAddressDlg->ShowWindow(SW_SHOW) ;
	}*/
	CNewAddressDlg dlg;
	dlg.DoModal();
}
void CReceiveDlg::OnBnClickedButtonSignAccount()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ͬ��δ���,���ܷ��ͼ����") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		CString str = m_listCtrl.GetItemText(nRow, 3);
		if(!m_MapAddrInfo.count(str))
			{
					TRACE("ERROR");
				StrShow.Format(_T("��ַ������\n"));
				::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
				return;
		   }
		uistruct::LISTADDR_t te =  m_MapAddrInfo[str];
		//uistruct::LISTADDR_t * pAddrItem = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
		if (te.fMoney <=0)
		{
			StrShow.Format(_T("�˻����Ϊ��,���ܼ���!\n"));
			::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
			return;
		}
		if(te.bSign) 
		{
			StrShow.Format(_T("�˻��Ѽ���!\n"));
			::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
			return;
		}
		theApp.m_strAddress.Format(_T("%s") ,te.address ) ;
		CSignAccountsDlg dlg;
		dlg.DoModal();

		/*m_accountDlg->SetShowAddr(pDbbetData->address);
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		m_accountDlg->MoveWindow(rcWindow.right/2+50,rcWindow.top+200,400,rcWindow.Height()/2);
		m_accountDlg->ShowWindow(SW_SHOW);*/
	}else{
		StrShow.Format(_T("��ѡ���ַ!\n"));
		::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
	}
}

BOOL CReceiveDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//�ж��Ƿ��¼���Enter��
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

	// TODO: �ڴ˴������Ϣ����������
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

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid(IDB_BITMAP_RECEIVE);

	return 0;
}


BOOL CReceiveDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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

	CString addressd;
	addressd.Format(_T("%s"),addr.address);

	if(m_MapAddrInfo.count(addressd) <= 0)
	{
		TRACE("map ModifyListCtrlItem ERROR");
		return;
	}
	
	m_MapAddrInfo[addressd]=addr;
	
	int count = m_listCtrl.GetItemCount();
	if ( 17 <= count )  {
		m_listCtrl.SetColumnWidth(5 , 86 ); 
	}else{
		m_listCtrl.SetColumnWidth(5 , 104 ); 
	}
	for(int i = 0; i < count; i++)
	{
		CString str = m_listCtrl.GetItemText(i, 3); // ����������������ˣ�����ȡ��ÿ��item��0�е�ֵ
		if(str == addressd)
		////uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(i);
		//if (!memcmp(pListAddr->address,addr.address,sizeof(pListAddr->address)) &&\
		//	(pListAddr->fMoney != addr.fMoney || pListAddr->bSign != addr.bSign))
		{
			/*pListAddr->fMoney = addr.fMoney;
			pListAddr->bSign = addr.bSign;*/
		
			int nSubIdx = 0;
			CString  strShowData;
			strShowData.Format(_T("%s") ,addr.RegID ) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			strShowData.Format(_T("%s") ,addr.Label) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;
			//m_listCtrl.SetItemData(i , (DWORD_PTR)&(*m_pListaddrInfo.rbegin())) ;

		//	uistruct::LISTADDR_t *pListAddr1 = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(i);

			strShowData.Format(_T("%s") ,addr.address) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			if (addr.bSign == 1)
			{
				strShowData.Format(_T("�Ѽ���")) ;
			}else{
				strShowData.Format(_T("δ����")) ;
			}

			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			if (addr.nColdDig== 1)
			{
				strShowData.Format(_T("֧��")) ;
			}else{
				strShowData.Format(_T("��֧��")) ;
			}
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			strShowData.Format(_T("%.8f") ,addr.fMoney ) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;
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

	CString addressd;
	addressd.Format(_T("%s"),addr.address);

	if(m_MapAddrInfo.count(addressd) > 0){
		TRACE("map InsertListCtrlItem ERROR");
		return ;
	}

	int count = m_listCtrl.GetItemCount();

	int nSubIdx = 0,i =count;

	CString strOrder(_T(""));
	strOrder.Format(_T("%d"), ++count);
	
	m_listCtrl.InsertItem( i , strOrder);  //���
	CString  strShowData;
	strShowData.Format(_T("%s") ,addr.RegID ) ;
	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

	strShowData.Format(_T("%s") ,addr.Label) ;
	m_listCtrl.SetItemText( i , ++nSubIdx, strShowData ) ; //��ǩ
	
	m_MapAddrInfo[addressd]=addr;

	strShowData.Format(_T("%s"),addr.address);    //��ַ
	m_listCtrl.SetItemText( i , ++nSubIdx, strShowData ); 

	if (addr.bSign == 1)
	{
		strShowData.Format(_T("�Ѽ���")) ;
	}else{
		strShowData.Format(_T("δ����")) ;
	}

	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

	if (addr.nColdDig== 1)
	{
		strShowData.Format(_T("֧��")) ;
	}else{
		strShowData.Format(_T("��֧��")) ;
	}
	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

	strShowData.Format(_T("%.8f") , addr.fMoney ) ;
	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

}