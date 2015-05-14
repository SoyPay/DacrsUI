// AddrBook.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "AddrBook.h"
#include "afxdialogex.h"

#include "NewSendAddr.h"
// CAddrBook �Ի���

IMPLEMENT_DYNAMIC(CAddrBook, CDialogEx)

CAddrBook::CAddrBook(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddrBook::IDD, pParent)
{

}

CAddrBook::~CAddrBook()
{
}

void CAddrBook::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CAddrBook, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADDADDRBOOK, &CAddrBook::OnBnClickedButtonAddaddrbook)
	ON_BN_CLICKED(IDOK, &CAddrBook::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DELEITEM, &CAddrBook::OnBnClickedButtonDeleitem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CAddrBook::OnNMDblclkList1)
END_MESSAGE_MAP()


// CAddrBook ��Ϣ�������


void CAddrBook::OnBnClickedButtonAddaddrbook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CNewSendAddr addsendaddr;
	CString strShowData;
	INT_PTR nResponse = addsendaddr.DoModal();;
	if (nResponse == IDOK)
	{
		uistruct::ADDRBOOK_t addr;
		addsendaddr.GetAddrbook(addr);
		int count = m_listCtrl.GetItemCount();
		int nSubIdx = 0;

		strShowData.Format(_T("%s") ,addr.lebel) ;
		m_listCtrl.InsertItem( count ,strShowData) ;

		strShowData.Format(_T("%s") ,addr.address) ;
		m_listCtrl.SetItemText(count , ++nSubIdx , strShowData ) ;

		//// �������ݿ�
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRBOOK);
		string temp =addr.ToJson();
		postmsg.SetData(temp.c_str());
		theApp.m_MsgQueue.push(postmsg);
	}

}

BOOL CAddrBook::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[3]  = {
		{"��ǩ" ,      300},
		{"��ַ" ,      370}
	};
	m_listCtrl.SetBkColor(RGB(240,240,240));       
	m_listCtrl.SetRowHeigt(23);               
	m_listCtrl.SetHeaderHeight(1.5);         
	m_listCtrl.SetHeaderFontHW(16,0);
	m_listCtrl.SetHeaderBKColor(32,30,32,8); 
	m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); //����ͷ��������ɫ
	m_listCtrl.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i < 2 ; i++  ) {
		m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );
	LoadAddrBook();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
bool CAddrBook::LoadAddrBook()
{
	theApp.m_SqliteDeal.GetaddrBookData(&m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	m_listCtrl.DeleteAllItems();

	int nSubIdx = 0 , i = 0 ;
	CString strShowData = _T("");
	std::map<CString,uistruct::ADDRBOOK_t>::const_iterator const_it;
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		uistruct::ADDRBOOK_t address = const_it->second;
		strShowData.Format(_T("%s") ,address.lebel) ;
		m_listCtrl.InsertItem(i,strShowData);

		strShowData.Format(_T("%s") ,address.address) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;
		i++;
	}

	return TRUE ;
}

void CAddrBook::GetAddrbook(uistruct::ADDRBOOK_t &addr)
{
	addr = m_selectAddr;
}

void CAddrBook::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		CString Lebel =m_listCtrl.GetItemText(nRow, 0);
		CString addr =m_listCtrl.GetItemText(nRow, 1);
		m_selectAddr.lebel = Lebel;
		m_selectAddr.address = addr;
		}else{
			StrShow.Format(_T("��ѡ���ַ!\n"));
			::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
			return;
		}
	CDialogEx::OnOK();
}


void CAddrBook::OnBnClickedButtonDeleitem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if ( IDYES == ::MessageBox( this->GetSafeHwnd() ,_T("�Ƿ�Ҫɾ�����տ��ַ") , _T("��ʾ") , MB_YESNO|MB_ICONINFORMATION ) ){
		//EndDialog(IDOK);
		CString StrShow;
		POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
		if ( pos ) {
			int nRow = m_listCtrl.GetNextSelectedItem(pos) ;

			//// ɾ���������ݿ�
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_UP_DELETERBOOK);
			uistruct::ADDRBOOK_t addr;
			CString Leble = m_listCtrl.GetItemText(nRow, 0); 
			CString address = m_listCtrl.GetItemText(nRow, 1); 
			addr.address = address;
			addr.lebel = Leble;
			string temp =addr.ToJson();
			postmsg.SetData(temp.c_str());
			theApp.m_MsgQueue.push(postmsg);

			//// �ӿؼ���ɾ��
			m_listCtrl.DeleteItem(nRow);
		}else{
			StrShow.Format(_T("��ѡ���ַ!\n"));
			::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
			return;
		}
	}
}


void CAddrBook::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(-1 != pNMItemActivate->iItem) 
	{  
		int nRow = pNMItemActivate->iItem;
		CString Lebel =m_listCtrl.GetItemText(nRow, 0);
		CString addr =m_listCtrl.GetItemText(nRow, 1);
		m_selectAddr.lebel = Lebel;
		m_selectAddr.address = addr;
		CDialogEx::OnOK();
	}  
	*pResult = 0;
}
