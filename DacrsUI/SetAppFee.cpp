// SetAppFee.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SetAppFee.h"
#include "afxdialogex.h"


// CSetAppFee 对话框

IMPLEMENT_DYNAMIC(CSetAppFee, CDialogEx)

CSetAppFee::CSetAppFee(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetAppFee::IDD, pParent)
{

}

CSetAppFee::~CSetAppFee()
{
}

void CSetAppFee::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LEAD , m_strTrading);
	DDX_Control(pDX, IDC_STATIC_SB , m_strTrading2);
	DDX_Control(pDX, IDC_STATIC_AB , m_strTrading3);
	DDX_Control(pDX, IDC_STATIC_OB , m_strTrading4);
	DDX_Control(pDX, IDC_STATIC_RLEAD , m_strTrading5);
	DDX_Control(pDX, IDC_STATIC_SCR , m_strTx1);
	DDX_Control(pDX, IDC_STATIC_GCR , m_strTx2);
	DDX_Control(pDX, IDC_STATIC_SSR , m_strTx3);
	DDX_Control(pDX, IDC_STATIC_ASR , m_strTx4);
	DDX_Control(pDX, IDC_STATIC_AR , m_strTx5);
	DDX_Control(pDX, IDC_STATIC_D , m_strDesTx1);
	DDX_Control(pDX, IDC_STATIC_BETID , m_strDesTx2);
	DDX_Control(pDX, IDC_STATIC_GRBID , m_strDesTx3);
	DDX_Control(pDX, IDC_STATIC_IPOID , m_strDesTx4);
	//DDX_Control(pDX, IDOK , m_rBtnOk);
	//DDX_Control(pDX, IDCANCEL , m_rBtnCancel);


}


BEGIN_MESSAGE_MAP(CSetAppFee, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetAppFee::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetAppFee::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSetAppFee::OnBnClickedButtonClose)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CSetAppFee 消息处理程序

bool CSetAppFee::IsAllDigtal(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!isdigit(pData[i])&&pData[i] !='.')
			return false;
	}

	return true;
}
bool CSetAppFee::IsRegId(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!isdigit(pData[i]) && pData[i] !='_')
			return false;
	}

	return true;
}
void CSetAppFee::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (PathFileExistsA(theApp.str_InsPath))
	{
		CStringA configpath = "";
		configpath.AppendFormat("%s",theApp.str_InsPath);
		configpath.AppendFormat("\\%s","dacrsclient.conf");
		CString strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile.GetString(), root)) 
			return;

		CString strTemp = _T("");
		GetDlgItem(IDC_SENDBETFEE)->GetWindowText(strTemp);
		if (strTemp != _T("") )
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("发送赌约小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("SendBetFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTEFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("接赌约小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("AcceptBetnFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_OPENFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("开奖小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("OpenBetnFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_SENDCOMREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("发普通红包小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("sendredcommFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTCOMREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("接普通红包小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("acceptredcommFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_SENDSPECAILREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("发接龙红包小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("sendredspecalFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTSPECAILREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("接接龙红包小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("acceptredspecalFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ReChange)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("充值小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("GetRechangeFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_WITHDRAW)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("提现小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("GetAppAmountnFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_BETID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				::MessageBox( NULL , _T("猜你妹AppID格式不正确") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("script"),_T("betscript"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_GRBID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				::MessageBox( NULL , _T("抢红包AppID格式不正确") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("script"),_T("redpacketscript"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_IPOID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				::MessageBox( NULL , _T("IPO AppID格式不正确") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("script"),_T("iposcript"),strTemp);
			}
		}

		CStdioFile  File;
		File.Open(theApp.str_InsPath+_T("\\dacrsclient.conf"),CFile::modeWrite | CFile::modeCreate); 
		string strfile = root.toStyledString();
		File.WriteString(strfile.c_str());
		File.Close();
	}

	theApp.ParseUIConfigFile(theApp.str_InsPath);
	CDialogEx::OnOK();
}


void CSetAppFee::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}


BOOL CSetAppFee::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_rBtnOk.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	//m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	//m_rBtnOk.SetFontEx(20 , _T("微软雅黑"));
	//m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	//m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	//m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	//m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	//m_rBtnOk.SizeToContent();

	//m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUTTON , RGB(255, 255, 0) , IDB_BITMAP_BUTTON , RGB(255, 255, 255) );
	//m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	//m_rBtnCancel.SetFontEx(20 , _T("微软雅黑"));
	//m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	//m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	//m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	//m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	//m_rBtnCancel.SizeToContent();
	// TODO:  在此添加额外的初始化
	//SetBkBmpNid( IDB_BITMAP_DLG_BALCK ) ;
	m_fontGrid.CreatePointFont(100,_T("新宋体"));
	/// listbox 背景颜色
	m_hbrush = CreateSolidBrush(RGB(255,236,229));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CSetAppFee::OnEraseBkgnd(CDC* pDC)
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
void CSetAppFee::SetBkBmpNid( UINT nBitmapIn ) 
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

HBRUSH CSetAppFee::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//if (nCtlColor == CTLCOLOR_STATIC)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SelectObject(&m_fontGrid);
	//	hbr = (HBRUSH)CreateSolidBrush(RGB(240,240,240));
	//	return m_hbrush;
	//}
	//return hbr;

	if (nCtlColor== CTLCOLOR_LISTBOX || nCtlColor==CTLCOLOR_BTN)
	{
		return m_hbrush; 
		
	} 
	else
	{
		return  CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


void CSetAppFee::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;

		CRect rc ;
		GetClientRect( rc ) ;

		CWnd *pst = GetDlgItem( IDC_BUTTON_CLOSE ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,rc.Width() - rect.Width() ,5  , rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}
	}
	// TODO: 在此处添加消息处理程序代码
}


void CSetAppFee::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
		EndDialog(IDOK);
}


void CSetAppFee::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);//发送一个移动的消息
	CDialogEx::OnLButtonDown(nFlags, point);
}


LRESULT CSetAppFee::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT nResult = CDialog::OnNcHitTest(point);
	return nResult  == HTCLIENT ? HTCAPTION : nResult;//鼠标的坐标在客户区么？在的话就把它当成标题栏

	return CDialogEx::OnNcHitTest(point);
}
