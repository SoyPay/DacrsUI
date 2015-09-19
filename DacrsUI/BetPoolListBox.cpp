// UseListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "BetPoolListBox.h"


// CUseListBox
#define IDC_BUTTON_ID 0x1235

IMPLEMENT_DYNAMIC(CBetPoolListBox, CListBox)

CBetPoolListBox::CBetPoolListBox()
{
	m_uID = IDC_BUTTON_ID;
}

CBetPoolListBox::~CBetPoolListBox()
{
}


BEGIN_MESSAGE_MAP(CBetPoolListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, &CBetPoolListBox::OnLbnSelchange)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CBetPoolListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return ;
	if (GetCount() == 0)
	{
		return;
	}
	PoolBox_AppendData * pData =(PoolBox_AppendData *) GetItemDataPtr(lpDrawItemStruct->itemID);
	TRACE1("DrawItem:%d\r\n",lpDrawItemStruct->itemID);
	ASSERT(pData);
	InvalidateListBox();

	UpdateWindow();
	Invalidate();
}

void CBetPoolListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight = 28;
}
void CBetPoolListBox::InsertStr(int iIndex,HWND hMain)
{
	PoolBox_AppendData * pData = new PoolBox_AppendData;
	pData->nItem = iIndex ;
	CRect rcClient(0 ,0 , 0 , 0);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	pData->pSta0 = new CStaticTrans;
	pData->pSta0->Create(_T(""),dwStyle, rcClient, this, ++m_uID);
	pData->pSta0->ModifyStyle(SS_LEFT|SS_LEFT,SS_RIGHT,TRUE);

	pData->pBut2 = new CButtonEx(iIndex,1,rcClient,hMain);
	pData->pBut2->Create(_T(""),dwStyle, rcClient, this, ++m_uID);

	CStaticTrans *pSta = new CStaticTrans;
	pSta->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;
	pSta->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	pData->pSta1 = pSta ;

	pData->pSta2 = new CStaticTrans;
	pData->pSta2->Create(_T(""),dwStyle, rcClient, this, ++m_uID);
	pData->pSta2->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);

	pData->pSta3 = new CStaticTrans;
	pData->pSta3->Create(_T(""),dwStyle, rcClient, this, ++m_uID);
	pData->pSta3->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);

	m_mButton.insert( make_pair( iIndex, pData ) );

	int i = SetItemDataPtr(InsertString(iIndex,_T("")),pData);

	updateListBoxButtonPos();
}
void CBetPoolListBox::DeleteAllIndex()
{	
	for (int i=0; i< GetCount(); i++) {
		PoolBox_AppendData * pData = (PoolBox_AppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pSta0;
			pData->pSta0 = NULL ;

			delete pData->pBut2;
			pData->pBut2 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

			delete pData->pSta3;
			pData->pSta3 = NULL;

			delete pData->pSta2;
			pData->pSta2 = NULL ;

			delete pData;
			pData = NULL;
		}
	}
	ResetContent();
	m_mButton.clear();
}
void CBetPoolListBox::DeleteIndex(int iIndex)
{
	PoolBox_AppendData * pData = (PoolBox_AppendData *)GetItemDataPtr(iIndex);

	if ( NULL != pData ) {
		delete pData->pSta0;
		pData->pSta0 = NULL ;

		delete pData->pBut2;
		pData->pBut2 = NULL ;

		delete pData->pSta1;
		pData->pSta1 = NULL ;

		delete pData->pSta3;
		pData->pSta3 = NULL;

		delete pData->pSta2;
		pData->pSta2 = NULL ;

		delete pData;
		pData = NULL;
	}

	DeleteString(iIndex);
	button_betmap::iterator iter = m_mButton.find(iIndex);
	if (iter != m_mButton.end())
	{
		m_mButton.erase(iter);
	}

}
void CBetPoolListBox::InvalidateListBox()
{
	for (int i=0; i<GetCount(); i++)
	{
		PoolBox_AppendData * pData = (PoolBox_AppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			pData->pSta0->ShowWindow( SW_HIDE );
			pData->pSta0->ShowWindow( SW_SHOW );

			pData->pSta1->ShowWindow( SW_HIDE );
			pData->pSta1->ShowWindow( SW_SHOW );

			pData->pSta2->ShowWindow( SW_HIDE );
			pData->pSta2->ShowWindow( SW_SHOW );

			pData->pSta3->ShowWindow( SW_HIDE );
			pData->pSta3->ShowWindow( SW_SHOW );

			pData->pBut2->ShowWindow( SW_HIDE );
			pData->pBut2->ShowWindow( SW_SHOW );
		}
	}
}
void CBetPoolListBox::updateListBoxButtonPos()
{
	button_betmap::iterator iter;
	button_betmap::iterator iterUp;
	int iLine = 0;
	for (unsigned int nItem = 0  ; nItem < m_mButton.size(); nItem++ ){
		iter = m_mButton.find(nItem);
		if( iter!= m_mButton.end() ){
		   iterUp = m_mButton.find(iLine);
		   PoolBox_AppendData *pData = (PoolBox_AppendData*)iter->second;
		   if ( NULL != pData ) {
			  pData->pSta0->SetFont(110, _T("宋体"));				//设置显示字体和大小
			 // pData->pSta0->SetTextColor(RGB(0,0,0));			    //字体颜色
              pData->pSta0->ShowWindow( SW_SHOW );
			  pData->pSta0->SetWindowPos(NULL , 10 , pData->nItem*28 +10 , 60 , 35 , SWP_SHOWWINDOW );

			  pData->pSta1->SetFont(110, _T("宋体"));				//设置显示字体和大小
			  pData->pSta1->ShowWindow( SW_SHOW );
			  pData->pSta1->SetWindowPos(NULL , 70 , pData->nItem*28 +10 , 100 , 25 , SWP_SHOWWINDOW );

			  pData->pSta2->SetFont(110, _T("宋体"));				//设置显示字体和大小
			  pData->pSta2->ShowWindow( SW_SHOW );
			  pData->pSta2->SetWindowPos(NULL , 170 , pData->nItem*28 +10 , 115 , 25 , SWP_SHOWWINDOW );

			  pData->pSta3->SetFont(110, _T("宋体"));				//设置显示字体和大小
			  pData->pSta3->ShowWindow( SW_SHOW );
			  pData->pSta3->SetWindowPos(NULL , 285 , pData->nItem*28 +10 , 115 , 25 , SWP_SHOWWINDOW );

			  pData->pBut2->ShowWindow( SW_SHOW );
			  pData->pBut2->SetWindowPos(NULL , 400 , pData->nItem*28 + 10 , 28 , 21 , SWP_SHOWWINDOW );
		   }
		   iLine++;
		}
	}
}
void CBetPoolListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	int iCount = GetCount();
	for (int i=0; i<iCount; i++)
	{
		PoolBox_AppendData * pData = (PoolBox_AppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pSta0;
			pData->pSta0 = NULL ;

			delete pData->pBut2;
			pData->pBut2 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

			delete pData->pSta3;
			pData->pSta3 = NULL;

			delete pData->pSta2;
			pData->pSta2 = NULL ;

			delete pData;
			pData = NULL;
		}
	}
}

BOOL CBetPoolListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CListBox::OnEraseBkgnd(pDC);
}

void CBetPoolListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListBox::OnMouseMove(nFlags, point);
}
PoolBox_AppendData* CBetPoolListBox::GetAppendDataInfo(int iIndex)
{
	std::map< int,PoolBox_AppendData*>::iterator it ;
	for ( it = m_mButton.begin() ; it != m_mButton.end() ;it++ ) {
		if( iIndex == it->first ) {
			PoolBox_AppendData *pData = (PoolBox_AppendData*)it->second;
			return pData ;
		}
	}
	return NULL ;
}
void CBetPoolListBox::SetIndexInage(int iIndex , UINT nButImage  )
{
	PoolBox_AppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		//pData->pBut2->SetBitmaps( nButImage , RGB(255, 255, 0) , nButImage , RGB(255, 255, 255) );
		pData->pBut2->SetBitmaps( nButImage , RGB(255, 255, 0), nButImage , RGB(255, 255, 255) );
		pData->pBut2->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		pData->pBut2->SetFontEx(20 , _T("微软雅黑"));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
	}
}
void    CBetPoolListBox::SetotherIndexInage(int iIndex , UINT nButImage  )
{
	{
		PoolBox_AppendData *pData = GetAppendDataInfo(iIndex) ;
		if ( NULL != pData ) {
			//pData->pBut2->SetBitmaps( nButImage , RGB(255, 255, 0) , nButImage , RGB(255, 255, 255) );
			pData->pBut2->SetBitmaps( nButImage , RGB(255, 255, 0), nButImage , RGB(255, 255, 255) );
			pData->pBut2->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
			pData->pBut2->SetFontEx(20 , _T("微软雅黑"));
			pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(200, 75, 60));
			pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255, 255, 255));
			pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(200, 75, 60));
			pData->pBut2->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(200, 75, 60));
		}
	}
}
void CBetPoolListBox::SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col)
{
	PoolBox_AppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		switch (nline)
		{
		case 0:
			pData->pSta0->SetTextColor(col);
			break;
		case 1:
			break;
		case 2:
			pData->pSta1->SetTextColor(col);
			break;
		}
	}
}
void CBetPoolListBox::SetIndexString(int iIndex , CString strSta0 ,CString strSta1 ,CString strSta2 ,CString strBut2 ,CString strSta3,CString strSta4 )
{
	PoolBox_AppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pSta0->ShowWindow(SW_HIDE);
		pData->pSta0->SetWindowText(strSta0);
		pData->pSta0->ShowWindow(SW_SHOW);

		pData->pSta1->ShowWindow(SW_HIDE);
		pData->pSta1->SetWindowText(strSta1);
		pData->pSta1->ShowWindow(SW_SHOW);

		pData->pSta2->ShowWindow(SW_HIDE);
		pData->pSta2->SetWindowText(strBut2);
		pData->pSta2->ShowWindow(SW_SHOW);

		pData->pSta3->ShowWindow(SW_HIDE);
		pData->pSta3->SetWindowText(strSta3);
		pData->pSta3->ShowWindow(SW_SHOW);

		pData->pBut2->SetWindowText(strSta2) ;
	/*	pData->pBut2->SetFontEx(20 , _T("微软雅黑"));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));*/

		pData->pstr.Format(_T("%s") ,strSta3 ) ;
		pData->pstr1.Format(_T("%s") ,strSta4 ) ;

	}
}
BOOL CBetPoolListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE ;
	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}
void CBetPoolListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return  ;
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CBetPoolListBox::OnLbnSelchange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CBetPoolListBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CListBox::OnPaint()
}


BOOL CBetPoolListBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CListBox::PreTranslateMessage(pMsg);
}
