// ListAddr.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ListAddr.h"
#include "afxdialogex.h"


// CListAddr 对话框

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


// CListAddr 消息处理程序


BOOL CListAddr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[7]  = { {"标签" , 100 }     ,
	{"地址" , 140}  , 
	{"激活状态" , 250}  , 
	{"余额" , 140}  ,
	{"支持冷挖矿" ,250} 
	};
	for( int i = 0 ; i < 7 ; i++  ) {
		m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );

	ShowListInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CListAddr::ShowListInfo(){

	uistruct::LISTADDRLIST m_pListaddrInfo;
	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.GetListaddrData(&m_pListaddrInfo);
	theApp.cs_SqlData.Unlock();

	if ( 0 == m_pListaddrInfo.size() ) return  ;

	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDR_OUT))->ResetContent();
	//加载到ComBox控件
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
				strShowData.Format(_T("已激活")) ;
			}else{
				strShowData.Format(_T("未激活")) ;
			}

			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;
			strShowData.Format(_T("%.8f") , const_it->fMoney ) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			if (const_it->nColdDig== 1)
			{
				strShowData.Format(_T("支持")) ;
			}else{
				strShowData.Format(_T("不支持")) ;
			}
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

			i++;
	}
}