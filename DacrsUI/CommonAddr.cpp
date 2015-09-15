// CommonAddr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "CommonAddr.h"
#include "afxdialogex.h"


// CCommonAddr �Ի���

IMPLEMENT_DYNAMIC(CCommonAddr, CDialogEx)

CCommonAddr::CCommonAddr(UiType uType , CWnd* pParent /*=NULL*/)
	: CDialogBase(CCommonAddr::IDD, pParent)
{
	m_uAddrType = uType ;
}

CCommonAddr::~CCommonAddr()
{
}

void CCommonAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_ButClose);
	DDX_Control(pDX, IDC_LIST_ALLADDR, m_AllAddrlistBox);
	DDX_Control(pDX, IDC_LIST_COMMONADDR, m_CommonAddrListBox);
	DDX_Control(pDX, IDC_HEAD, m_head);
}


BEGIN_MESSAGE_MAP(CCommonAddr, CDialogBase)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CCommonAddr::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CCommonAddr::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CCommonAddr::OnBnClickedButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_ALLADDR, &CCommonAddr::OnLbnSelchangeListAlladdr)
	ON_LBN_SELCHANGE(IDC_LIST_COMMONADDR, &CCommonAddr::OnLbnSelchangeListCommonaddr)
	ON_BN_CLICKED(IDC_BUTTON_ADDROK, &CCommonAddr::OnBnClickedButtonAddrok)
END_MESSAGE_MAP()


// CCommonAddr ��Ϣ�������


void CCommonAddr::OnSize(UINT nType, int cx, int cy)
{
	CDialogBase::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}


BOOL CCommonAddr::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_head.SetFont(100, _T("΢���ź�"));
	m_head.SetTextColor(RGB(255,255,255));	
	m_head.SetWindowText(UiFun::UI_LoadString("ADDRBOOK_MODULE" , "ADDRBOOK_COMMMANAGE" ,theApp.gsLanguage));
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);

	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);

	m_ButClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_ButClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_ButClose.SetWindowText("") ;
	m_ButClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_ButClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_ButClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_ButClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_ButClose.SizeToContent();

	RECT ret;
	GetWindowRect(&ret);
	CRect titleRect ;
	m_ButClose.GetClientRect(titleRect);
	m_ButClose.SetWindowPos(NULL ,(ret.right-ret.left)-titleRect.Width() , 2 , 0 , 0 , SWP_NOSIZE); 

	GetDlgItem(IDC_BUTTON_ADD)->SetWindowText(UiFun::UI_LoadString("ADDRBOOK_MODULE" , "ADDRBOOK_ADD_ADDRESS" ,theApp.gsLanguage));
	GetDlgItem(IDC_BUTTON_DELETE)->SetWindowText(UiFun::UI_LoadString("ADDRBOOK_MODULE" , "ADDRBOOK_DELETE_ADDRESS" ,theApp.gsLanguage));
	GetDlgItem(IDC_ALL)->SetWindowText(UiFun::UI_LoadString("ADDRBOOK_MODULE" , "ADDRBOOK_ALLADDR" ,theApp.gsLanguage));
	GetDlgItem(IDC_STATIC)->SetWindowText(UiFun::UI_LoadString("ADDRBOOK_MODULE" , "ADDRBOOK_COMMADDR" ,theApp.gsLanguage));

	AddListaddrDataBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CCommonAddr::AddListaddrDataBox()
{
	m_mapAddrInfo.clear(); //ȫ����ַ
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" sign=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	m_mapCommonAddrInfo.clear();//���õ�ַ
	string strconditom = "";
	switch(m_uAddrType)
	{
	case UI_SENDP2P_RECORD:
		{
			strconditom =strprintf("app_id = '%s'",theApp.m_betScritptid);
			theApp.m_SqliteDeal.GetCommonWalletAddressList(strconditom, &m_mapCommonAddrInfo);
		}
		break;
	case UI_READPACKET_RECORD:
		{
			strconditom =strprintf("app_id = '%s'",theApp.m_redPacketScriptid);
			theApp.m_SqliteDeal.GetCommonWalletAddressList(strconditom, &m_mapCommonAddrInfo);
		}
		break;
	}

	m_AllAddrlistBox.ResetContent(); //���ȫ����ַComBox�ؼ�
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it; //���ص�ȫ����ַComBox�ؼ�
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {
		int pos = m_AllAddrlistBox.AddString( const_it->second.RegID.c_str() ) ;
		m_AllAddrlistBox.SetItemData( pos , (DWORD_PTR)(&const_it->second) ) ;
	}

	//���³��õ�ַ
	UpdataCommonAddrList(&m_mapCommonAddrInfo);
	return TRUE;
}
void CCommonAddr::UpdataCommonAddrList(map<int,uistruct::COMMONLISTADDR_t> *pListInfo)
{
	if ( NULL == pListInfo ) return ;
	//���õ�ַ
	m_CommonAddrListBox.ResetContent(); //���ȫ����ַComBox�ؼ�
	std::map<int,uistruct::COMMONLISTADDR_t>::const_iterator const_it; //���ص�ȫ����ַComBox�ؼ�
	for ( const_it = pListInfo->begin() ; const_it != pListInfo->end() ; const_it++ ) {
		int pos = m_CommonAddrListBox.AddString( const_it->second.reg_id.c_str() ) ;
		m_CommonAddrListBox.SetItemData( pos , (DWORD_PTR)(&const_it->second) ) ;
	}
}
void CCommonAddr::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(IDOK);
	CDialogBase::OnClose();
}

void CCommonAddr::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int curSel = m_AllAddrlistBox.GetCurSel() ;
	uistruct::LISTADDR_t * pAddrList = (uistruct::LISTADDR_t*)m_AllAddrlistBox.GetItemData( curSel ) ;
	if ( NULL == pAddrList ) return ;

	string strSourceData , strCond;
	m_mapCommonAddrInfo.clear();//���õ�ַ

	if (m_uAddrType == UI_SENDP2P_RECORD)
	{
		strCond =strprintf(" reg_id = '%s'and app_id='%s' ", pAddrList->RegID.c_str(),theApp.m_betScritptid);
	}else if (m_uAddrType == UI_READPACKET_RECORD)
	{
		strCond =strprintf(" reg_id = '%s'and app_id='%s' ", pAddrList->RegID.c_str(),theApp.m_redPacketScriptid);
	}
	
	uistruct::COMMONLISTADDR_t addrsql;
	int item = theApp.m_SqliteDeal.GetCommonWalletAddressItem(strCond, &addrsql) ;

	if (addrsql.reg_id.length() == 0 ){  //����
	    switch(m_uAddrType)
		{
		case UI_SENDP2P_RECORD:
			{
				strSourceData=strprintf("'%s' , '%s',null" , pAddrList->RegID.c_str() ,theApp.m_betScritptid) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_common_address") ,strSourceData ) ) {
					  TRACE("Insert t_common_address error!\n");
				}
				strCond =strprintf(" app_id = '%s' ", theApp.m_betScritptid);
				theApp.m_SqliteDeal.GetCommonWalletAddressList(strCond, &m_mapCommonAddrInfo);
			}
			break;
		case UI_READPACKET_RECORD:
			{
				strSourceData=strprintf("'%s' , '%s',null" , pAddrList->RegID.c_str() ,theApp.m_redPacketScriptid) ;
				if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_common_address") ,strSourceData ) ) {
					TRACE("Insert t_common_address error!\n");
				}
				strCond =strprintf(" app_id = '%s' ", theApp.m_redPacketScriptid);
				theApp.m_SqliteDeal.GetCommonWalletAddressList(strCond, &m_mapCommonAddrInfo);
			}
			break;
		}
		//���µ����õ�ַ
	    UpdataCommonAddrList(&m_mapCommonAddrInfo);
		switch(m_uAddrType)
		{
		case UI_SENDP2P_RECORD:
			{
				CPostMsg postp2pmsg(MSG_USER_P2PADDRES,0);
				theApp.m_MsgQueue.push(postp2pmsg);
			}
			break;
		case UI_READPACKET_RECORD:
			{
				CPostMsg postp2pmsg(MSG_USER_REDPACKET_UI,WM_UP_NEWADDRESS);
				theApp.m_MsgQueue.push(postp2pmsg);
			}
			break;
		}

	}else{  //����
	    UiFun::MessageBoxEx(UiFun::UI_LoadString("ADDRBOOK_MODULE" , "ADDRBOOK_ADDRESS_TO_EXIST" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
	}
}

void CCommonAddr::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int curSel = m_CommonAddrListBox.GetCurSel() ;
	uistruct::COMMONLISTADDR_t * pAddrList = (uistruct::COMMONLISTADDR_t*)m_CommonAddrListBox.GetItemData( curSel ) ;
	if ( NULL == pAddrList ) return ;

	string strSourceData ;

	if (m_uAddrType == UI_SENDP2P_RECORD)
	{
		strSourceData =strprintf(" reg_id = '%s'and app_id='%s' ", pAddrList->reg_id.c_str(),theApp.m_betScritptid);
	}else if (m_uAddrType == UI_READPACKET_RECORD)
	{
		strSourceData =strprintf(" reg_id = '%s'and app_id='%s' ", pAddrList->reg_id.c_str(),theApp.m_redPacketScriptid);
	}
	int item = theApp.m_SqliteDeal.DeleteTableItem(_T("t_common_address"), strSourceData);

	m_mapCommonAddrInfo.clear();//���õ�ַ
	string conditon = "";
	switch(m_uAddrType)
	{
	case UI_SENDP2P_RECORD:
		{
			conditon = strprintf("app_id='%s'",theApp.m_betScritptid);
			theApp.m_SqliteDeal.GetCommonWalletAddressList(conditon, &m_mapCommonAddrInfo);
		}
		break;
	case UI_READPACKET_RECORD:
		{
			conditon = strprintf("app_id='%s'",theApp.m_redPacketScriptid);
			theApp.m_SqliteDeal.GetCommonWalletAddressList(conditon, &m_mapCommonAddrInfo);
		}
		break;
	}
	//���µ����õ�ַ
	UpdataCommonAddrList(&m_mapCommonAddrInfo);
	switch(m_uAddrType)
	{
	case UI_SENDP2P_RECORD:
		{
			CPostMsg postp2pmsg(MSG_USER_P2PADDRES,0);
			theApp.m_MsgQueue.push(postp2pmsg);
		}
		break;
	case UI_READPACKET_RECORD:
		{
			CPostMsg postp2pmsg(MSG_USER_REDPACKET_UI,WM_UP_NEWADDRESS);
			theApp.m_MsgQueue.push(postp2pmsg);
		}
		break;
	}
}


void CCommonAddr::OnLbnSelchangeListAlladdr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
}


void CCommonAddr::OnLbnSelchangeListCommonaddr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
}


void CCommonAddr::OnBnClickedButtonAddrok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	EndDialog(IDOK);
}
