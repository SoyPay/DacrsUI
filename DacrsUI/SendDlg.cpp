// SendDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SendDlg.h"
#include "afxdialogex.h"
#include "Out.h"
#include "AddrBook.h"
// CSendDlg �Ի���

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


// CTransfer ��Ϣ�������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*CString strDisplay1;
	strDisplay1.Format(_T(" ת�˽��׷��ͳɹ�\n\nԴ��ַ:DV9u9jgtyJoDsZ4m2MmrtQwLfNDZs952qx\r\n\r\nĿ�ĵ�ַ:DhxrQ9hsvo3fVVSy6By8bePt8cmPtts88R4eac46f7718ae7175d67b58a4886f2f91746cbdb30b4f2ee0dd96c70168f4fae \r\n\r\n��1.0000\r\n\r\nhash:4eac46f7718ae7175d67b58a4886f2f91746cbdb30b4f2ee0dd96c70168f4fae"));
	if (IDYES == UiFun::MessageBoxEx(strDisplay1 , _T("��ʾ") , MFB_YESNO|MFB_TIP ) )
	{
		int b =5;
	}*/
	if (theApp.IsLockWallet())
	{
		return ;
	}

	if (m_mapAddrInfo.size() == 0)
	{
		UiFun::MessageBoxEx(_T("���͵�ַ������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
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
			UiFun::MessageBoxEx(_T("���͵�ַ����Ǯ����ַ") , _T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}
		//uistruct::LISTADDR_t te = m_pListaddrInfo[text];
		data = m_mapAddrInfo[strAddr];
	
	}

		if(!data.bSign) 
		{
			UiFun::MessageBoxEx(_T("���͵�ַδ����") , _T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}

		GetDlgItem(IDC_EDIT_DESADDRESS)->GetWindowTextA(strMaddress);
		if (strMaddress == _T(""))
		{
			UiFun::MessageBoxEx(_T("���ܵ�ַ����δ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}
		if(!strcmp(strMaddress.GetString(), data.address.c_str()))
		{
			UiFun::MessageBoxEx(_T("���͵�ַ��Ŀ�ĵ�ַ������ͬ") , _T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}
		
		GetDlgItem(IDC_EDIT_MONEY)->GetWindowTextA(strMoney);
		double dSendMoney = strtod(strMoney,NULL);
		if(dSendMoney > data.fMoney || ( data.fMoney>-0.0000001 && data.fMoney< 0.000001 )) 
		{
			UiFun::MessageBoxEx(_T("�˻�����") , _T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}

		if(_T("") == strMoney.GetString() || (dSendMoney >-0.0000001 && dSendMoney< 0.000001))
		{
			UiFun::MessageBoxEx(_T("���ͽ���Ϊ0") , _T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}

		if (!theApp.IsSyncBlock )
		{
			UiFun::MessageBoxEx(_T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}
		strCommand = strprintf("%s %s %s %lld","sendtoaddress" ,data.address.c_str() ,strMaddress ,REAL_MONEY(dSendMoney));
		string strShowData ;

		CString strDisplay;
		strDisplay.Format(_T("ת��%.4lfsmc��%s"), dSendMoney, strMaddress);

		if (IDCANCEL == UiFun::MessageBoxEx(strDisplay , _T("��ʾ") , MFB_OKCANCEL|MFB_TIP ) )
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
			//���뵽���ݿ�
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
			strData = strprintf(" ת�˽��׷��ͳɹ�\n\n Դ��ַ:%s\n\n Ŀ�ĵ�ַ:%s\n\n ��%.4lf\n  hash:%s", data.address.c_str(),strMaddress, dSendMoney,root["hash"].asCString()) ;
		}else{
			strData = "ת��ʧ��!";
		}
		UiFun::MessageBoxEx(strData.c_str() , _T("��ʾ") ,MFB_OK|MFB_TIP );
		

		//// �������ݿ�,���տ�����ӵ���ַ��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	//���ComBox�ؼ�
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->ResetContent();
	//���ص�ComBox�ؼ�
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
	//��������
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
// CSendDlg ��Ϣ�������


BOOL CSendDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes =   CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if (bRes) 
	{
		//m_rBtnSend.LoadBitmaps(IDB_BITMAP_BOTTON_SEND1,IDB_BITMAP_BOTTON_SEND2,IDB_BITMAP_BOTTON_SEND3,IDB_BITMAP_BOTTON_SEND3);
		m_rBtnAddbook.LoadBitmaps(IDB_BITMAP_ADDBOOK,IDB_BITMAP_ADDBOOK,IDB_BITMAP_ADDBOOK,IDB_BITMAP_ADDBOOK);
		UpdateData(0);
		m_strTx1.SetFont(120, _T("����"));				//������ʾ����ʹ�С
		m_strTx1.SetWindowText(_T(""));
		AddListaddrDataBox();

		m_rBtnSend.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
		m_rBtnSend.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnSend.SetWindowText("��  ��") ;
		m_rBtnSend.SetFontEx(20 , _T("΢���ź�"));
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

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid(IDB_BITMAP_SENDUI_BJ);

	return 0;
}


BOOL CSendDlg::OnEraseBkgnd(CDC* pDC)
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

	return 1;
}

void CSendDlg::OnBnClickedButtonAddbook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ����ר�ô����/����û���
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
					HWND hWnd = GetSafeHwnd(); // ��ȡ��ȫ���ھ��
					::OpenClipboard(hWnd); // �򿪼�����
					HANDLE hClipMemory = ::GetClipboardData(CF_TEXT);// ��ȡ���������ݾ��
					DWORD dwLength = GlobalSize(hClipMemory); // ����ָ���ڴ�����ĵ�ǰ��С
					LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory); // �����ڴ�
					CString message = CString(lpClipMemory); // ����õ����ı�����
					GlobalUnlock(hClipMemory); // �ڴ����
					::CloseClipboard(); // �رռ�����
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
					HWND hWnd = GetSafeHwnd(); // ��ȡ��ȫ���ھ��
					::OpenClipboard(hWnd); // �򿪼�����
					HANDLE hClipMemory = ::GetClipboardData(CF_TEXT);// ��ȡ���������ݾ��
					DWORD dwLength = GlobalSize(hClipMemory); // ����ָ���ڴ�����ĵ�ǰ��С
					LPBYTE lpClipMemory = (LPBYTE)GlobalLock(hClipMemory); // �����ڴ�
					CString message = CString(lpClipMemory); // ����õ����ı�����
					GlobalUnlock(hClipMemory); // �ڴ����
					::CloseClipboard(); // �رռ�����
					if (bCtrl)
					{
						string addr = strprintf("%s",message);
						if(m_mapAddrInfo.count(addr)<=0)
						{
							//TRACE("map OnCbnSelchangeCombo1 error");
							
							UiFun::MessageBoxEx(_T("���Ƶĵ�ַ����") , _T("��ʾ") ,MFB_OK|MFB_TIP );
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
			
			pCtl->SendMessage(CB_SETITEMHEIGHT,(WPARAM)-1,(LPARAM)20);   //�ı�ؼ�����ĸ߶�
			pCtl->SetItemHeight(0,15);									 //�ı������б�ÿ��Item�ĸ߶�
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

			pCtl->SendMessage(CB_SETITEMHEIGHT,(WPARAM)-1,(LPARAM)20);   //�ı�ؼ�����ĸ߶�
			pCtl->SetItemHeight(0,15);									 //�ı������б�ÿ��Item�ĸ߶�
		}

		pst = GetDlgItem( IDC_SENDTRNSFER) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*90 ,(rc.Height()/100)*65, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

	}
	// TODO: �ڴ˴������Ϣ����������
}
