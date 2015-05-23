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
		m_ListBox.InsertStr(0,this->GetSafeHwnd());
		m_ListBox.SetIndexBackCol(0 , 0 , RGB(181,185,212));
		m_ListBox.SetIndexBackCol(0 , 1 , RGB(181,185,212));
		m_ListBox.SetIndexBackCol(0 , 2 , RGB(181,185,212));
		m_ListBox.SetIndexBackCol(0 , 3 , RGB(181,185,212));
		m_ListBox.SetIndexBackCol(0 , 4 , RGB(181,185,212));
		m_ListBox.SetIndexString(0 , _T("durCyWC8MTdQdpCo9QXZ5wxSyL9jtyDFri"), _T("+100"), _T("vbvvvvv"), _T("ddddddd") , _T("2015-01-01 01:02:03"));

		m_ListBox.InsertStr(1,this->GetSafeHwnd());
		m_ListBox.SetIndexBackCol(1 , 0,RGB(255,76,0));
		m_ListBox.SetIndexBackCol(1 , 1 , RGB(255,76,0));
		m_ListBox.SetIndexBackCol(1 , 2 , RGB(255,76,0));
		m_ListBox.SetIndexBackCol(1 , 3 , RGB(255,76,0));
		m_ListBox.SetIndexBackCol(1 , 4 , RGB(255,76,0));
		m_ListBox.SetIndexString(1 , _T("durCyWC8MTdQdpCo9QXZ5wxSyL9jtyDFri"), _T("+100"), _T("vbvvvvv"), _T("ddddddd") , _T("2015-01-01 01:02:03"));
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
			pst->SetWindowPos( NULL ,0 , 33 , 875, 185  ,SWP_SHOWWINDOW ) ; 
		}
	}
}
void CBetRecord::Showlistbox(CString address)
{
	//// 查找数据库中是否存在此记录
	CString conditon;
	conditon.Format(_T("right_addr ='%s' and actor = 1 or actor = 2") , address);
	uistruct::P2PBETRECORDLIST  pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon ,&pPoolItem ) ;
	if (pPoolItem.size() == 0) ///此记录不存在,插入记录
	{
		int i = 0;
		std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
		for (const_it = pPoolItem.begin();const_it!=pPoolItem.end();const_it++)
		{
			CString dmoney,reward,result,guess;
			dmoney.Format(_T("%.4f"),const_it->amount);
			if (const_it->guess_num == 0)
			{
				guess.Format(_T("%s"),"妹");
			}else
			{
				guess.Format(_T("%s"),"哥");
			}

			///说明开奖了
			if (const_it->state == 2)
			{
				SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it->recv_time);
				CString strTime;
				strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
				if (const_it->guess_num == const_it->content[32])
				{
					reward.Format(_T("+%.4f"),const_it->amount);
				}else
				{
					reward.Format(_T("-%.4f"),const_it->amount);
				}
				if (const_it->content[32] == 0)
				{
					result.Format(_T("%s"),"妹");
				}else
				{
					result.Format(_T("%s"),"哥");
				}
				m_ListBox.SetIndexString(i , address,reward, result, guess , strTime);

			}else{
				m_ListBox.SetIndexString(i , address,dmoney, _T("--"), guess , _T("--"));
			}
			i++;
		}
	}
}