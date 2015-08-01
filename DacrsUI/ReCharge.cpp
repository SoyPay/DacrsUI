// ReCharge.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ReCharge.h"
#include "afxdialogex.h"


// CReCharge 对话框

IMPLEMENT_DYNAMIC(CReCharge, CDialogEx)

CReCharge::CReCharge(CWnd* pParent /*=NULL*/,CString text,CString strsms,CString strhead)
	: CDialogEx(CReCharge::IDD, pParent)
{
	m_pBmp = NULL ;
	m_strText = text;
	m_strSms = strsms;
	m_strHead = strhead;
}

CReCharge::~CReCharge()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CReCharge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SHOW, m_Text);
	DDX_Control(pDX, IDC_BUTTON_GB1, m_rBtnClose);	
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CReCharge, CDialogEx)
	ON_BN_CLICKED(IDOK, &CReCharge::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CReCharge::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_GB1, &CReCharge::OnBnClickedButtonGb1)
END_MESSAGE_MAP()


// CReCharge 消息处理程序


void CReCharge::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strText == _T(""))
	{
		CString strTxMoney;
		GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
		if (strTxMoney == _T(""))
		{
			CMessageBoxEx message(_T("\n金额不能为空!") , 0 );
	        message.DoModal();
			//::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
			return ;
		}
		theApp.m_strAddress = strTxMoney;
	}else{
		CString strTxmessage;
		GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxmessage) ;
		if (strTxmessage.GetLength() >200)
		{
			CMessageBoxEx message(_T("\n广告语不能超过两百个字节!") , 0 );
	        message.DoModal();
			//::MessageBox( this->GetSafeHwnd() ,_T("广告语不能超过两百个字节") , _T("提示") , MB_ICONINFORMATION ) ;
			return ;
		}
		theApp.m_strAddress = strTxmessage;
	}
	CDialogEx::OnOK();
}


void CReCharge::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress = _T("");
	CDialogEx::OnCancel();
}


BOOL CReCharge::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Text.SetFont(110, _T("微软雅黑"));				//设置显示字体和大小
	m_Text.SetTextColor(RGB(255,255,255));	    //字体颜色
	if (m_strText == _T(""))
	{
		m_Text.SetWindowText(_T("充值")) ;
	}else{
		m_Text.SetWindowText(m_strText) ;
	}

	
	m_Text.SetWindowPos( NULL , 3 , 3 , 200, 20  ,SWP_SHOWWINDOW ) ; 
	if (m_strHead != _T(""))
	{
		((CStatic*)GetDlgItem(IDC_STATIC_DW))->SetWindowText(m_strHead);
	}

	if (m_strSms != _T(""))
	{
		((CStatic*)GetDlgItem(IDC_STATIC))->SetWindowText(m_strSms);
	}

	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	m_rBtnClose.SetWindowPos(NULL ,320 , 0 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText("确 定") ;
	m_rBtnOk.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText("取 消") ;
	m_rBtnCancel.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();
	SetBkBmpNid( IDB_BITMAP_ADD_BOOK_BJ ) ;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CReCharge::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;

		CRect rc ;
		GetClientRect( rc ) ;


		CWnd *pst = GetDlgItem( IDC_STATIC ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (rc.Width()/100)*3 , (rc.Height()/100)*5 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_MONEY ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*10 ,(rc.Height()/100)*5  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}


		//
		//	
		pst = GetDlgItem( IDC_STATIC_DW ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 ,(rc.Height()/100)*5  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDOK ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*15 ,(rc.Height()/100)*10  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}


		//
		//	
		pst = GetDlgItem( IDCANCEL ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*25 ,(rc.Height()/100)*10 , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
	}
	// TODO: 在此处添加消息处理程序代码
}
void CReCharge::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CReCharge::OnEraseBkgnd(CDC* pDC)
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


void CReCharge::OnBnClickedButtonGb1()
{
	// TODO: 在此添加控件通知处理程序代码
		EndDialog(IDCANCEL);
}
