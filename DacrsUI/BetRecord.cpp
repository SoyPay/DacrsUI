// BetRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "BetRecord.h"
#include "afxdialogex.h"


// CBetRecord 对话框

IMPLEMENT_DYNAMIC(CBetRecord, CDialogEx)

CBetRecord::CBetRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBetRecord::IDD, pParent)
{
	m_pBmp = NULL ;
}

CBetRecord::~CBetRecord()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CBetRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_ListBox);
	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CBetRecord, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CBetRecord 消息处理程序
void CBetRecord::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CBetRecord::OnEraseBkgnd(CDC* pDC)
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


int CBetRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_BET_RECORD);
	return 0;
}


BOOL CBetRecord::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ){
		UpdateData(0);
	
		m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_sCountpage.SetWindowText(_T("共:"));

		m_rBtnUp.SetBitmaps( IDB_BITMAP_UP , RGB(255, 255, 0) , IDB_BITMAP_UP , RGB(255, 255, 255) );
		m_rBtnUp.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnUp.SetWindowText("") ;
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnUp.SizeToContent();

		m_rBtnNext.SetBitmaps( IDB_BITMAP_NEXT , RGB(255, 255, 0) , IDB_BITMAP_NEXT , RGB(255, 255, 255) );
		m_rBtnNext.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNext.SetWindowText("") ;
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNext.SizeToContent();
	}
	return bRes;
}


void CBetRecord::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		CWnd *pst = GetDlgItem( IDC_LIST_BOX ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,0 , 33 , 875, 130  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_UP ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-130 , 130+33 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875 - 100, 130+33 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_NEXT ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-70 , 130+33 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-40 ,130+36 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
	}
}
void CBetRecord::Showlistbox(CString address)
{
	//// 查找数据库中是否存在此记录
	m_ListBox.DeleteAllIndex();
	CString conditon;
	conditon.Format(_T("right_addr ='%s' and (actor = 1 or actor = 2)") , address);
	uistruct::P2PBETRECORDLIST  pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon ,&pPoolItem ) ;
	if (pPoolItem.size() != 0) ///此记录不存在,插入记录
	{
		int i = 0;
		std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
		for (const_it = pPoolItem.begin();const_it!=pPoolItem.end();const_it++)
		{
			CString dmoney,reward,result,guess,Sendaddr;
			Sendaddr.Format(_T("%s"),const_it->left_addr);
			dmoney.Format(_T("%.4f"),const_it->amount);
			if (const_it->guess_num == 1)
			{
				guess.Format(_T("%s"),"妹");
			}else
			{
				guess.Format(_T("%s"),"哥");
			}
			m_ListBox.InsertStr(i,this->GetSafeHwnd());
			m_ListBox.SetIndexBackCol(0 , 0 , RGB(242,32,32));
			m_ListBox.SetIndexBackCol(0 , 1 , RGB(181,185,212));
			m_ListBox.SetIndexBackCol(0 , 2 , RGB(181,185,212));
			m_ListBox.SetIndexBackCol(0 , 3 , RGB(181,185,212));
			m_ListBox.SetIndexBackCol(0 , 4 , RGB(181,185,212));

			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it->send_time);
			CString sendTime,reciveTime;
			sendTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(const_it->recv_time);
			reciveTime.Format("%04d-%02d-%02d %02d:%02d:%02d",rTime.wYear, rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);

			reward.Format(_T("%.4f"),const_it->amount);
			///说明开奖了
			if (const_it->state == 2)
			{
				int rewardnum = (int)const_it->content[32];
				if (const_it->guess_num == const_it->content[32])
				{
					reward.Format(_T("+%.4f"),const_it->amount);
				}else
				{
					reward.Format(_T("-%.4f"),const_it->amount);
				}
				if (const_it->content[32] == 1)
				{
					result.Format(_T("%s"),"妹");
				}else
				{
					result.Format(_T("%s"),"哥");
				}
				m_ListBox.SetIndexString(i , Sendaddr,address,sendTime,reciveTime, result,guess, reward);

			}else{
				if ((const_it->time_out + const_it->height)> theApp.blocktipheight&& theApp.IsSyncBlock)
				{
					m_ListBox.SetIndexString(i , Sendaddr,address,sendTime,reciveTime, _T("未开奖"),guess,reward);
				}else if(theApp.IsSyncBlock){
					reward.Format(_T("+%.4f"),const_it->amount);
					m_ListBox.SetIndexString(i , Sendaddr,address,sendTime,reciveTime, _T("超时"),guess,reward);
				}else{
					m_ListBox.SetIndexString(i , Sendaddr,address,sendTime,reciveTime, _T("未开奖"),guess,reward);
				}
			}
			i++;
		}
	}
}