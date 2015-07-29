// SignAccountsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SignAccountsDlg.h"
#include "afxdialogex.h"


// CSignAccountsDlg 对话框

IMPLEMENT_DYNAMIC(CSignAccountsDlg, CDialogEx)

CSignAccountsDlg::CSignAccountsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSignAccountsDlg::IDD, pParent)
{
	m_pBmp = NULL ;
}
CSignAccountsDlg::~CSignAccountsDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CSignAccountsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_rBtnSend);
}


BEGIN_MESSAGE_MAP(CSignAccountsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSignAccountsDlg::OnBnClickedButtonSend)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSignAccountsDlg::OnBnClickedButtonClose)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CSignAccountsDlg 消息处理程序
void CSignAccountsDlg::SetBkBmpNid( UINT nBitmapIn ) 
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

void CSignAccountsDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (theApp.IsLockWallet())
	{
		return ;
	}

	CString address;
	GetDlgItem(IDC_EDIT_ADDRESS)->GetWindowText(address);
	if ( _T("") != address ) {
		string strCommand , strShowData;CString strFee ;

	/*	strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,address);
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		if (strShowData == _T(""))
		{
			::MessageBox( this->GetSafeHwnd() ,_T("服务器没有反应") , _T("提示") , MB_ICONINFORMATION ) ;
		}*/
		Json::Reader reader;  
		Json::Value root; 
		//if (!reader.parse(strShowData.GetString(), root)) 
		//	return  ;


		GetDlgItem(IDC_EDIT_FEE)->GetWindowText(strFee);
		strCommand = strprintf("%s %s %lld","registaccounttx" ,address  , (INT64)REAL_MONEY(atof(strFee)) );

		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == _T(""))
		{
			::MessageBox( this->GetSafeHwnd() ,_T("服务器没有反应") , _T("提示") , MB_ICONINFORMATION ) ;
		}

		if (!reader.parse(strShowData, root)) 
			return  ;

		string strData;
		int pos = strShowData.find("hash");

		if ( pos >=0 ) {
			//插入到数据库
			string strHash;
			strHash =root["hash"].asString();
			string strCond;
			strCond = strprintf(" hash = '%s' ", (LPSTR)(LPCTSTR)strHash.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , strCond) ;

			if ( 0 == nItem ) {

				CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
				postmsg.SetData(strHash);
				theApp.m_MsgQueue.push(postmsg);

			}
		}

		if ( pos >=0 ) {
			strData = strprintf("激活交易发送成功，请等待1-2分钟确认激活交易\n%s" , root["hash"].asCString() ) ;
		}else{
			strData="激活账户失败!" ;
		}
		if ( IDOK == ::MessageBox( this->GetSafeHwnd() ,strData.c_str(), _T("提示") , MB_ICONINFORMATION ) ){
			EndDialog(IDOK);
		}
	}

}
void CSignAccountsDlg::SetShowAddr(CString addr)
{
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);
	GetDlgItem(IDC_EDIT_ADDRESS)->SetWindowText(addr);
}

int CSignAccountsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}


BOOL CSignAccountsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	m_rBtnClose.SetWindowPos(NULL ,476-26 , 0 , 0 , 0 , SWP_NOSIZE);

	m_rBtnSend.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	m_rBtnSend.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnSend.SetWindowText("激   活") ;
	m_rBtnSend.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnSend.SizeToContent();

	SetShowAddr( theApp.m_strAddress);

	SetBkBmpNid( IDB_BITMAP_DLG_BALCK ) ;
	m_fontGrid.CreatePointFont(100,_T("新宋体"));

	GetDlgItem(IDC_EDIT_FEE)->SetWindowTextA("0.0001");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CSignAccountsDlg::OnEraseBkgnd(CDC* pDC)
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
		pDC->StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


HBRUSH CSignAccountsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(&m_fontGrid);
		hbr = (HBRUSH)CreateSolidBrush(RGB(249,249,249));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSignAccountsDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDOK);
}


void CSignAccountsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);//发送一个移动的消息
	CDialogEx::OnLButtonDown(nFlags, point);
}


LRESULT CSignAccountsDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nResult = CDialog::OnNcHitTest(point);
	return nResult  == HTCLIENT ? HTCAPTION : nResult;//鼠标的坐标在客户区么？在的话就把它当成标题栏

	return CDialogEx::OnNcHitTest(point);
}
