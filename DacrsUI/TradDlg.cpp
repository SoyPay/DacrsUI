// TradDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TradDlg.h"
#include "afxdialogex.h"
#include "TxDetailDlg.h"

// CTradDlg 对话框

IMPLEMENT_DYNAMIC(CTradDlg, CDialogBar)

CTradDlg::CTradDlg()
{
	m_pBmp = NULL ;
}

CTradDlg::~CTradDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	/*v_linkCtrl1.InternalRelease();
	v_linkCtrl1.ExternalRelease();
	v_linkCtrl1.OnFinalRelease();
	v_linkCtrl1.DestroyWindow();

	v_linkCtrl2.InternalRelease();
	v_linkCtrl2.ExternalRelease();
	v_linkCtrl2.OnFinalRelease();
	v_linkCtrl2.DestroyWindow();*/
}

void CTradDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LISTTX, m_listCtrl);
	//DDX_Control(pDX, IDC_PROGRESS, v_linkCtrl1);
	//DDX_Control(pDX, IDC_MFCLINK2, v_linkCtrl2);
}


BEGIN_MESSAGE_MAP(CTradDlg, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_TXDETAIL, &CTradDlg::OnBnClickedButtonTxdetail)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LISTTX, &CTradDlg::OnNMDblclkListListtx)
END_MESSAGE_MAP()


// CTradDlg 消息处理程序

void CTradDlg::SetBkBmpNid( UINT nBitmapIn ) 
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
int CTradDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetBkBmpNid(IDB_BITMAP_TRAD_BJ);
	// TODO:  在此添加您专用的创建代码

	return 0;
}


BOOL CTradDlg::OnEraseBkgnd(CDC* pDC)
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

	return TRUE;
}


BOOL CTradDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(0);
		/*v_linkCtrl1.SetWindowText(_T(""));
		v_linkCtrl2.SetWindowText(_T(""));

		v_linkCtrl1.SetWindowText(_T("关注开发进度"));
		v_linkCtrl2.SetWindowText(_T("QQ群"));

		v_linkCtrl1.SetURL(_T("http://www.dacrs.com/forum.php?mod=viewthread&tid=2598"));
		v_linkCtrl2.SetURL(_T("http://jq.qq.com/?_wv=1027&k=T5mlmd"));*/

		struct LISTCol {
			CString		name ;
			UINT		size ;
		} listcol[5]  = {
			{"	交易hash" ,      255},
			{"交易类型" ,      130},
			{"源地址" ,      275}, 
			{"目的地址" ,  100}, 
			{"金额" ,      100}
		};
		m_listCtrl.SetBkColor(RGB(240,240,240));       
		m_listCtrl.SetRowHeigt(23);               
		m_listCtrl.SetHeaderHeight(1.5);         
		m_listCtrl.SetHeaderFontHW(15,0);
		m_listCtrl.SetHeaderBKColor(32,30,32,8); 
		m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); //设置头部字体颜色
		m_listCtrl.SetTextColor(RGB(0,0,0));  
		for( int i = 0 ; i < 5 ; i++  ) {
			m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
		}
		m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );
		OninitializeList();
	}
	return bRes ;
}


void CTradDlg::OnBnClickedButtonTxdetail()
{
	// TODO: 在此添加控件通知处理程序代码
		CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		CString strtxhash = m_listCtrl.GetItemText(nRow,0);
		//uistruct::REVTRANSACTION_t txdetail;
		CString findstr;
		findstr.Format(_T("'%s'"),strtxhash);
		uistruct::TRANSRECORDLIST  txdetail;
	
		int nItem =  theApp.m_SqliteDeal.FindDB(_T("revtransaction") , findstr ,_T("hash"),&txdetail ) ;
		if (nItem != 0)
		{
			uistruct::REVTRANSACTION_t fisrt = *txdetail.begin();
			theApp.m_strAddress.Format(_T("%s") ,fisrt.ToJson().c_str() ) ;
		}else
		{		
			theApp.m_strAddress =_T("");
		}

		CTxDetailDlg dlg;
		dlg.DoModal();

	}else{
		StrShow.Format(_T("请选择地址!\n"));
		::MessageBox( this->GetSafeHwnd() ,StrShow , _T("提示") , MB_ICONINFORMATION ) ;
	}

}
void  CTradDlg::OninitializeList()
{
	uistruct::TRANSRECORDLIST pListInfo;
	theApp.m_SqliteDeal.GetRevtransactionDatta(&pListInfo); 

	if (pListInfo.size() == 0)
	{
		return;
	}

	int nSubIdx = 0 , i = 0 ;
	CString strShowData = _T("");
	std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
	for ( const_it = pListInfo.begin() ; const_it != pListInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		strShowData.Format(_T("%s"), const_it->txhash);
		m_listCtrl.InsertItem(i,strShowData);

		string txtype = const_it->txtype;
		if (!strcmp(txtype.c_str(),"REWARD_TX"))
		{
			strShowData.Format(_T("%s") ,_T("挖矿奖励交易")) ;
		}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
		{
			strShowData.Format(_T("%s") ,_T("注册账户交易")) ;
		}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
		{
			strShowData.Format(_T("%s") ,_T("转账交易")) ;
		}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
		{
			strShowData.Format(_T("%s") ,_T("合约交易")) ;
		}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
		{
			strShowData.Format(_T("%s") ,_T("注册应用交易")) ;
		}
		m_listCtrl.SetItemText( i , ++nSubIdx, strShowData) ;


		strShowData.Format(_T("%s") ,const_it->addr.c_str()) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		strShowData.Format(_T("%s") ,const_it->desaddr.c_str()) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		strShowData.Format(_T("%.8f") , const_it->money ) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		i++;
	}
}

void CTradDlg::OnNMDblclkListListtx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if(-1 != pNMItemActivate->iItem) 
	{  
		int nRow = pNMItemActivate->iItem;
		CString strtxhash =m_listCtrl.GetItemText(nRow, 0);
		CString findstr;
		findstr.Format(_T("'%s'"),strtxhash);
		uistruct::TRANSRECORDLIST  txdetail;

		int nItem =  theApp.m_SqliteDeal.FindDB(_T("revtransaction") , findstr ,_T("hash"),&txdetail ) ;

		if (nItem != 0)
		{
			uistruct::REVTRANSACTION_t fisrt = *txdetail.begin();
			theApp.m_strAddress.Format(_T("%s") ,fisrt.ToJson().c_str() ) ;
		}else
		{		
			theApp.m_strAddress =_T("");
		}
		CTxDetailDlg dlg;
		dlg.DoModal();
	}  
	*pResult = 0;
}
