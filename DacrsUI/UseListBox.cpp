// UseListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "UseListBox.h"


// CUseListBox
#define IDC_BUTTON_ID 0x1235

IMPLEMENT_DYNAMIC(CUseListBox, CListBox)

CUseListBox::CUseListBox()
{
	m_uID = IDC_BUTTON_ID;
}

CUseListBox::~CUseListBox()
{
}


BEGIN_MESSAGE_MAP(CUseListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_CONTROL_REFLECT(LBN_SELCHANGE, &CUseListBox::OnLbnSelchange)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CUseListBox 消息处理程序

//void CUseListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
//
//	// TODO:  添加您的代码以绘制指定项
//	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
//	COLORREF crOldTextColor = pDC->GetTextColor();
//	COLORREF crOldBkColor = pDC->GetBkColor();
//
//	m_rectListBox = lpDrawItemStruct->rcItem;
//
//	//pData->pSta0->SetFont(90, _T("宋体"));				//设置显示字体和大小
//	//// pData->pSta0->SetTextColor(RGB(0,0,0));			    //字体颜色
//	//pData->pSta0->ShowWindow( SW_SHOW );
//	//pData->pSta0->SetWindowPos(NULL , 20 , pData->nItem*35 +10 , 275 , 35 , SWP_SHOWWINDOW );
//
//	//pData->pSta1->SetFont(90, _T("宋体"));				//设置显示字体和大小
//	////  pData->pSta1->SetTextColor(RGB(0,0,0));			    //字体颜色
//	//pData->pSta1->ShowWindow( SW_SHOW );
//	//pData->pSta1->SetWindowPos(NULL , 275+10+25 , pData->nItem*35 +10 , 40 , 25 , SWP_SHOWWINDOW );
//
//	//pData->pBut2->ShowWindow( SW_SHOW );
//	//pData->pBut2->SetWindowPos(NULL , 275+10+50 +40 , pData->nItem*35 , 43 , 33 , SWP_SHOWWINDOW );
//	// 列表Item中的图标位置
//	CRect rectIcon(m_rectListBox.left + 10, m_rectListBox.top + 5,m_rectListBox.left + 50, m_rectListBox.top + 45);
//	// 列表Item中的软件名称的文本位置
//	CRect rectName(m_rectListBox.left + 60, m_rectListBox.top + 5,m_rectListBox.left + 300, m_rectListBox.top + 45);
//	// 列表Items中的软件是否安装的文本位置
//	CRect rectIsInstall(m_rectListBox.left + 300, m_rectListBox.top + 5,m_rectListBox.left + 800, m_rectListBox.top + 45);
//
//	List_AppendData * pData =(List_AppendData *) GetItemDataPtr(lpDrawItemStruct->itemID);
//	//m_iCurIndex = pItem->nID;
//
//	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
//		(lpDrawItemStruct->itemState & ODS_SELECTED))
//	{
//		// 选中状态
//		pDC->SetTextColor(RGB(0,0,0));
//		pDC->FillSolidRect(&m_rectListBox,RGB(182,218,243));
//	}
//	else
//	{
//		// 未选中状态
//		pDC->SetTextColor(RGB(27,73,115));
//		pDC->FillSolidRect(&m_rectListBox, crOldBkColor);
//	}
////	pDC->set
//	//updateListBoxButtonPos();
//
//}

void CUseListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return ;
	if (GetCount() == 0)
	{
		return;
	}
	List_AppendData * pData =(List_AppendData *) GetItemDataPtr(lpDrawItemStruct->itemID);
	TRACE1("DrawItem:%d\r\n",lpDrawItemStruct->itemID);
	ASSERT(pData);
	InvalidateListBox();

	UpdateWindow();
	Invalidate();
}

void CUseListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight = 28;
}
void CUseListBox::InsertStr(int iIndex,HWND hMain)
{
	List_AppendData * pData = new List_AppendData;
	pData->nItem = iIndex ;
	CRect rcClient(0 ,0 , 0 , 0);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	pData->pSta0 = new CStaticTrans;
	pData->pSta0->Create(_T(""),dwStyle, rcClient, this, ++m_uID);

	pData->pBut2 = new CButtonEx(iIndex,1,rcClient,hMain);
	pData->pBut2->Create(_T(""),dwStyle, rcClient, this, ++m_uID);

	CStaticTrans *pSta = new CStaticTrans;
	pSta->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;
	pSta->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	pData->pSta1 = pSta ;

	pData->pSta2 = new CStaticTrans;
	pData->pSta2->Create(_T(""),dwStyle, rcClient, this, ++m_uID);
	pData->pSta2->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);
	m_mButton.insert( make_pair( iIndex, pData ) );

	int i = SetItemDataPtr(InsertString(iIndex,_T("")),pData);

	updateListBoxButtonPos();
}
void CUseListBox::DeleteAllIndex()
{	
	for (int i=0; i< GetCount(); i++) {
		List_AppendData * pData = (List_AppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pSta0;
			pData->pSta0 = NULL ;

			delete pData->pBut2;
			pData->pBut2 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

			delete pData->pSta2;
			pData->pSta2 = NULL ;

			delete pData;
			pData = NULL;
		}
	}
	ResetContent();
	m_mButton.clear();
}
void CUseListBox::DeleteIndex(int iIndex)
{
	List_AppendData * pData = (List_AppendData *)GetItemDataPtr(iIndex);

	if ( NULL != pData ) {
		delete pData->pSta0;
		pData->pSta0 = NULL ;
		delete pData->pBut2;
		pData->pBut2 = NULL ;

		delete pData->pSta1;
		pData->pSta1 = NULL ;

		delete pData->pSta2;
		pData->pSta2 = NULL ;

		delete pData;
		pData = NULL;
	}

	DeleteString(iIndex);
	button_map::iterator iter = m_mButton.find(iIndex);
	if (iter != m_mButton.end())
	{
		m_mButton.erase(iter);
	}

}
void CUseListBox::InvalidateListBox()
{
	for (int i=0; i<GetCount(); i++)
	{
		List_AppendData * pData = (List_AppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			pData->pSta0->ShowWindow( SW_HIDE );
			pData->pSta0->ShowWindow( SW_SHOW );

			pData->pSta1->ShowWindow( SW_HIDE );
			pData->pSta1->ShowWindow( SW_SHOW );

			pData->pSta2->ShowWindow( SW_HIDE );
			pData->pSta2->ShowWindow( SW_SHOW );

			pData->pBut2->ShowWindow( SW_HIDE );
			pData->pBut2->ShowWindow( SW_SHOW );
		}
	}
}
void CUseListBox::updateListBoxButtonPos()
{
	button_map::iterator iter;
	button_map::iterator iterUp;
	int iLine = 0;
	for (int nItem = 0  ; nItem < m_mButton.size(); nItem++ ){
		iter = m_mButton.find(nItem);
		if( iter!= m_mButton.end() ){
		   iterUp = m_mButton.find(iLine);
		   List_AppendData *pData = (List_AppendData*)iter->second;
		   if ( NULL != pData ) {
			  pData->pSta0->SetFont(110, _T("宋体"));				//设置显示字体和大小
			 // pData->pSta0->SetTextColor(RGB(0,0,0));			    //字体颜色
              pData->pSta0->ShowWindow( SW_SHOW );
			  pData->pSta0->SetWindowPos(NULL , 40 , pData->nItem*28 +10 , 60 , 35 , SWP_SHOWWINDOW );

			  pData->pSta1->SetFont(110, _T("宋体"));				//设置显示字体和大小
			  pData->pSta1->ShowWindow( SW_SHOW );
			  pData->pSta1->SetWindowPos(NULL , 100 , pData->nItem*28 +10 , 100 , 25 , SWP_SHOWWINDOW );

			  pData->pSta2->SetFont(110, _T("宋体"));				//设置显示字体和大小
			  pData->pSta2->ShowWindow( SW_SHOW );
			  pData->pSta2->SetWindowPos(NULL , 210 , pData->nItem*28 +10 , 150 , 25 , SWP_SHOWWINDOW );

			  pData->pBut2->ShowWindow( SW_SHOW );
			  pData->pBut2->SetWindowPos(NULL , 275+10+50 +40 , pData->nItem*28 + 10 , 28 , 21 , SWP_SHOWWINDOW );
		   }
		   iLine++;
		}
	}
}
void CUseListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	int iCount = GetCount();
	for (int i=0; i<iCount; i++)
	{
		List_AppendData * pData = (List_AppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pSta0;
			pData->pSta0 = NULL ;
			delete pData->pBut2;
			pData->pBut2 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

			delete pData->pSta2;
			pData->pSta2 = NULL ;

			delete pData;
			pData = NULL;
		}
	}
}

BOOL CUseListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CListBox::OnEraseBkgnd(pDC);
}

void CUseListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListBox::OnMouseMove(nFlags, point);
}
List_AppendData* CUseListBox::GetAppendDataInfo(int iIndex)
{
	std::map< int,List_AppendData*>::iterator it ;
	for ( it = m_mButton.begin() ; it != m_mButton.end() ;it++ ) {
		if( iIndex == it->first ) {
			List_AppendData *pData = (List_AppendData*)it->second;
			return pData ;
		}
	}
	return NULL ;
}
void CUseListBox::SetIndexInage(int iIndex , UINT nButImage  )
{
	List_AppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pBut2->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		pData->pBut2->SetBitmaps( nButImage , RGB(255, 255, 0) , nButImage , RGB(255, 255, 255) );
	}
}
void CUseListBox::SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col)
{
	List_AppendData *pData = GetAppendDataInfo(iIndex) ;
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
void CUseListBox::SetIndexString(int iIndex , CString strSta0 ,CString strSta1 ,CString strSta2 ,CString strBut2 ,CString strSta3 )
{
	List_AppendData *pData = GetAppendDataInfo(iIndex) ;
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

		pData->pBut2->SetWindowText(strSta2) ;
		pData->pBut2->SetFontEx(20 , _T("微软雅黑"));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));

		pData->pstr.Format(_T("%s") ,strSta3 ) ;

	}
}
BOOL CUseListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE ;
	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}
void CUseListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return  ;
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CUseListBox::OnLbnSelchange()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUseListBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CListBox::OnPaint()
}


BOOL CUseListBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CListBox::PreTranslateMessage(pMsg);
}
