// TradDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TradDlg.h"
#include "afxdialogex.h"
#include "TxDetailDlg.h"

// CTradDlg �Ի���

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


// CTradDlg ��Ϣ�������

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
	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}


BOOL CTradDlg::OnEraseBkgnd(CDC* pDC)
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


BOOL CTradDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(0);
		/*v_linkCtrl1.SetWindowText(_T(""));
		v_linkCtrl2.SetWindowText(_T(""));

		v_linkCtrl1.SetWindowText(_T("��ע��������"));
		v_linkCtrl2.SetWindowText(_T("QQȺ"));

		v_linkCtrl1.SetURL(_T("http://www.dacrs.com/forum.php?mod=viewthread&tid=2598"));
		v_linkCtrl2.SetURL(_T("http://jq.qq.com/?_wv=1027&k=T5mlmd"));*/

		struct LISTCol {
			CString		name ;
			UINT		size ;
		} listcol[5]  = {
			{"	����hash" ,      255},
			{"��������" ,      130},
			{"Դ��ַ" ,      275}, 
			{"Ŀ�ĵ�ַ" ,  100}, 
			{"���" ,      100}
		};
		m_listCtrl.SetBkColor(RGB(240,240,240));       
		m_listCtrl.SetRowHeigt(23);               
		m_listCtrl.SetHeaderHeight(1.5);         
		m_listCtrl.SetHeaderFontHW(15,0);
		m_listCtrl.SetHeaderBKColor(32,30,32,8); 
		m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); //����ͷ��������ɫ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		StrShow.Format(_T("��ѡ���ַ!\n"));
		::MessageBox( this->GetSafeHwnd() ,StrShow , _T("��ʾ") , MB_ICONINFORMATION ) ;
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
			strShowData.Format(_T("%s") ,_T("�ڿ�������")) ;
		}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
		{
			strShowData.Format(_T("%s") ,_T("ע���˻�����")) ;
		}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
		{
			strShowData.Format(_T("%s") ,_T("ת�˽���")) ;
		}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
		{
			strShowData.Format(_T("%s") ,_T("��Լ����")) ;
		}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
		{
			strShowData.Format(_T("%s") ,_T("ע��Ӧ�ý���")) ;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
