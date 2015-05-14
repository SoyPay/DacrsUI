// AddrBook.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "AddrBook.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CAddrBook ��Ϣ�������


void CAddrBook::OnBnClickedButtonAddaddrbook()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CAddrBook::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[3]  = {
		{"���" ,      50},
		{"��ǩ" ,      130},
		{"��ַ" ,      275}
	};
	m_listCtrl.SetBkColor(RGB(240,240,240));       
	m_listCtrl.SetRowHeigt(23);               
	m_listCtrl.SetHeaderHeight(1.5);         
	m_listCtrl.SetHeaderFontHW(16,0);
	m_listCtrl.SetHeaderBKColor(32,30,32,8); 
	m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); //����ͷ��������ɫ
	m_listCtrl.SetTextColor(RGB(0,0,0));  
	for( int i = 0 ; i < 3 ; i++  ) {
		m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );

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
		CString strOrder("");
		strOrder.Format(_T("%d"), i+1);
		m_listCtrl.InsertItem(i,strOrder);

		uistruct::ADDRBOOK_t address = const_it->second;
		strShowData.Format(_T("%s") ,address.lebel) ;
		m_listCtrl.SetItemText( i , ++nSubIdx, strShowData) ;
		//m_listCtrl.SetItemData(item , (DWORD_PTR)&(*const_it)) ;


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
