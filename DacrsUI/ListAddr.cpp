// ListAddr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ListAddr.h"
#include "afxdialogex.h"


// CListAddr �Ի���

IMPLEMENT_DYNAMIC(CListAddr, CDialogEx)

CListAddr::CListAddr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListAddr::IDD, pParent)
{

}

CListAddr::~CListAddr()
{
}

void CListAddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADDR, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CListAddr, CDialogEx)
END_MESSAGE_MAP()


// CListAddr ��Ϣ�������


BOOL CListAddr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[7]  = { {"��ǩ" , 100 }     ,
	{"��ַ" , 140}  , 
	{"����״̬" , 250}  , 
	{"���" , 140}  ,
	{"֧�����ڿ�" ,250} 
	};
	for( int i = 0 ; i < 7 ; i++  ) {
		m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );

	ShowListInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CListAddr::ShowListInfo(){

	uistruct::LISTADDRLIST m_pListaddrInfo;
	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.GetListaddrData(&m_pListaddrInfo);
	theApp.cs_SqlData.Unlock();

	if ( 0 == m_pListaddrInfo.size() ) return  ;

	//���ComBox�ؼ�
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->ResetContent();
	//���ص�ComBox�ؼ�
	int nSubIdx = 0 , i = 0 ;
	CString strShowData = _T("");
	std::vector<uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_pListaddrInfo.begin() ; const_it != m_pListaddrInfo.end() ; const_it++ ) {
			nSubIdx = 0;
			int item = m_listCtrl.InsertItem( i , strShowData ) ;
			m_listCtrl.SetItemData(item , (DWORD_PTR)&(*const_it)) ;

			strShowData.Format(_T("%s") ,const_it->address) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			if (const_it->bSign == 1)
			{
				strShowData.Format(_T("�Ѽ���")) ;
			}else{
				strShowData.Format(_T("δ����")) ;
			}

			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;
			strShowData.Format(_T("%.8f") , const_it->fMoney ) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			if (const_it->nColdDig== 1)
			{
				strShowData.Format(_T("֧��")) ;
			}else{
				strShowData.Format(_T("��֧��")) ;
			}
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			i++;
	}
}