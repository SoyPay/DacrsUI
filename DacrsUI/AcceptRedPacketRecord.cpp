// AcceptRedPacketRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "AcceptRedPacketRecord.h"
#include "afxdialogex.h"
#include "RedPacketList.h"

// CAcceptRedPacketRecord 对话框

IMPLEMENT_DYNAMIC(CAcceptRedPacketRecord, CDialogEx)

CAcceptRedPacketRecord::CAcceptRedPacketRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAcceptRedPacketRecord::IDD, pParent)
{
	m_pBmp = NULL ;
	m_pagecount = 0;
	m_curpage = 0;
	m_AcceptRedPacketList.clear();
	m_pagesize = 4;
}

CAcceptRedPacketRecord::~CAcceptRedPacketRecord()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CAcceptRedPacketRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_ListBox);
	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CAcceptRedPacketRecord, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UP, &CAcceptRedPacketRecord::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CAcceptRedPacketRecord::OnBnClickedButtonNext)
	ON_LBN_DBLCLK(IDC_LIST_BOX, &CAcceptRedPacketRecord::OnLbnDblclkListBox)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAcceptRedPacketRecord 消息处理程序

void CAcceptRedPacketRecord::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CAcceptRedPacketRecord::OnEraseBkgnd(CDC* pDC)
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


int CAcceptRedPacketRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_GRBREDPACKET);
	return 0;
}


BOOL CAcceptRedPacketRecord::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ){
		UpdateData(0);

		m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_sCountpage.SetWindowText(_T("共:"));

		m_rBtnUp.SetBitmaps( IDB_BITMAP_RED_UP , RGB(255, 255, 0) , IDB_BITMAP_RED_UP , RGB(255, 255, 255) );
		m_rBtnUp.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnUp.SetWindowText("") ;
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnUp.SizeToContent();

		m_rBtnNext.SetBitmaps( IDB_BITMAP_RED_NEX , RGB(255, 255, 0) , IDB_BITMAP_RED_NEX , RGB(255, 255, 255) );
		m_rBtnNext.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNext.SetWindowText("") ;
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNext.SizeToContent();

		GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T("共:0"));
	}
	/// listbox 背景颜色
	//m_hbrush = CreateSolidBrush(RGB(255,252,247));RGB(255,236,229)
	m_hbrush = CreateSolidBrush(RGB(255,236,229));
	return bRes;
}


void CAcceptRedPacketRecord::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		CWnd *pst = GetDlgItem( IDC_LIST_BOX ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,0 , 25 , 875, 120  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_UP ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-140 , 120+25 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875 - 115, 120+25 , 40, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_NEXT ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-70 , 120+25 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-50 ,120+28 , 50, 20  ,SWP_SHOWWINDOW ) ; 
		}
	}
}
void CAcceptRedPacketRecord::Showlistbox(CString address)
{
	//// 查找数据库中是否存在此记录
	//m_ListBox.DeleteAllIndex();
	m_AcceptRedPacketList.clear();
	m_curpage = 0;
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T(""));
	string conditon;
	char buffer[1000] = {0};
	sprintf_s(buffer,"grab_acct_id ='%s' order by grab_time desc",address);
	conditon = buffer;
	//conditon.Format(_T("grab_acct_id ='%s' order by grab_time desc") , address);

	int nItem =  theApp.m_SqliteDeal.GetRedPacketGrabRecordList(conditon.c_str() ,&m_AcceptRedPacketList ) ;

	m_pagecount = (m_AcceptRedPacketList.size()%m_pagesize)==0?(m_AcceptRedPacketList.size()/m_pagesize):(m_AcceptRedPacketList.size()/m_pagesize)+1;

	memset(buffer,0,1000);
	sprintf_s(buffer,"共:%d",m_pagecount);
	string temp = buffer;

	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_ListBox.DeleteAllIndex();
	OnShowPagePool(1);
}


void  CAcceptRedPacketRecord::OnShowPagePool(int page)
{
	if (page >m_pagecount || page == m_curpage || page <= 0)
	{
		return;
	}


	m_ListBox.DeleteAllIndex();

	string strpage = strprintf("%d",page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());

	m_curpage = page;
	int index = (page-1)*m_pagesize;
	unsigned int count = (m_AcceptRedPacketList.size() -index)>=m_pagesize?m_pagesize:(m_AcceptRedPacketList.size() -index);

	char sendTime[1024] = {0};
	string amount,Sendaddr,address,luckeMoney,strnum,type,luckeValue,operate;;
	char buffer[100] = {0};
	int i = 0;
	std::vector<uistruct::REDPACKETGRAB_t>::const_iterator const_it;
	for (unsigned int k = index;k< (index+count) && k < m_AcceptRedPacketList.size();k++)
	{
		uistruct::REDPACKETGRAB_t const_it = m_AcceptRedPacketList.at(k);
		sprintf_s(buffer,"%s",const_it.send_acc_id.c_str());
		Sendaddr =buffer;
		memset(buffer,0,100);
		sprintf_s(buffer,"%.4f",const_it.total_amount);
		amount =buffer;
		memset(buffer,0,100);
		//Sendaddr.Format(_T("%s"),const_it.send_acc_id);
		//amount.Format(_T("%.4f"),const_it.total_amount);

		if (const_it.packet_type == 1)
		{
			type="普通";
		}else if (const_it.packet_type == 2)
		{
			type="接龙";
			//type.Format(_T("接龙"));
		}

		if (const_it.grab_time == 0)
		{
			//luckeMoney = _T("--");
			luckeMoney="--";
			sprintf_s(sendTime,"%s","--");

		}else{
			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.grab_time);
			//sendTime.Format("%02d-%02d %02d:%02d:%02d", curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			//luckeMoney.Format(_T("%.4f"),const_it.lucky_amount);
			sprintf_s(buffer,"%.4f",const_it.lucky_amount);
			luckeMoney=buffer;
			memset(buffer,0,100);
			sprintf_s(sendTime,"%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			memset(buffer,0,100);
		}

		if (const_it.lucky_fortune == 1)
		{
			luckeValue="普通";
		}else if (const_it.lucky_fortune == 2)
		{
			luckeValue="运气王";
		}else
		{
			//luckeValue.Format(_T("--"));
			luckeValue="--";
		}
		memset(buffer,0,100);
		sprintf_s(buffer,"%d",const_it.total_num);
		strnum=buffer;
		memset(buffer,0,100);
		m_ListBox.InsertStr(i,this->GetSafeHwnd());
		m_ListBox.SetIndexString(i , Sendaddr.c_str(),type.c_str(),sendTime,amount.c_str(), strnum.c_str(),luckeValue.c_str(),luckeMoney.c_str());
	
		i++;
	}
}
BOOL CAcceptRedPacketRecord::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)  
	{   
		if(pMsg->wParam == VK_RETURN)  
		{  
			if (GetDlgItem(IDC_EDIT_PAGE) == this->GetFocus())
			{
				CString num;
				GetDlgItem(IDC_EDIT_PAGE)->GetWindowText(num);
				if (IsAllDigtal(num))
				{
					OnShowPagePool(atoi(num));
				}else
				{
					GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
					CMessageBoxEx message(_T("\n输入有误,请输入数字") , 0 );
	                message.DoModal();
					//::MessageBox( this->GetSafeHwnd() ,_T("输入有误,请输入数字") , _T("提示") , MB_ICONINFORMATION ) ;
				}
				return TRUE;
			}
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CAcceptRedPacketRecord::OnBnClickedButtonUp()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowPagePool((m_curpage-1));
}


void CAcceptRedPacketRecord::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowPagePool((m_curpage+1));
}


void CAcceptRedPacketRecord::OnLbnDblclkListBox()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel=m_ListBox.GetCurSel(); 
	int count = ((m_curpage -1)*m_pagesize) + nSel;
	uistruct::REDPACKETGRAB_t const_it = m_AcceptRedPacketList.at(count);

	if (const_it.send_hash != "")
	{
		theApp.m_strAddress.Format(_T("%s") ,const_it.send_hash.c_str() ) ;
	}else
	{		
		theApp.m_strAddress =_T("");
	}
	CRedPacketList dlg;
	dlg.DoModal();	
}


HBRUSH CAcceptRedPacketRecord::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (nCtlColor== CTLCOLOR_LISTBOX)
	{
		return m_hbrush; 
	} 
	else
	{
		return  CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}
