// UseListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "RedPacketPoolListBox.h"


// CUseListBox
#define IDC_BUTTON_ID 0x123567

IMPLEMENT_DYNAMIC(CRedPacketPoolListBox, CListBox)

CRedPacketPoolListBox::CRedPacketPoolListBox()
{
	m_uID = IDC_BUTTON_ID;
}

CRedPacketPoolListBox::~CRedPacketPoolListBox()
{
}


BEGIN_MESSAGE_MAP(CRedPacketPoolListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CUseListBox 消息处理程序

void CRedPacketPoolListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return ;
	if (GetCount() == 0)
	{
		return;
	}
	List_RedAppendData * pData =(List_RedAppendData *) GetItemDataPtr(lpDrawItemStruct->itemID);
	TRACE1("DrawItem:%d\r\n",lpDrawItemStruct->itemID);
	ASSERT(pData);
	InvalidateListBox();

	UpdateWindow();
	Invalidate();
}

void CRedPacketPoolListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight = 28;
}
void CRedPacketPoolListBox::InsertStr(int iIndex,HWND hMain)
{
	List_RedAppendData * pData = new List_RedAppendData;
	pData->nItem = iIndex ;
	CRect rcClient(0 ,0 , 0 , 0);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	pData->pSta0 = new CStaticTrans;
	pData->pSta0->Create(_T(""),dwStyle, rcClient, this, ++m_uID);
	pData->pSta0->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);

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

	pData->pSta4 = new CStaticTrans;
	pData->pSta4->Create(_T(""),dwStyle, rcClient, this, ++m_uID);
	pData->pSta4->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);

	pData->pSta5 = new CStaticTrans;
	pData->pSta5->Create(_T(""),dwStyle, rcClient, this, ++m_uID);
	pData->pSta5->ModifyStyle(SS_LEFT|SS_RIGHT,SS_CENTER,TRUE);

	m_mButton.insert( make_pair( iIndex, pData ) );

	int i = SetItemDataPtr(InsertString(iIndex,_T("")),pData);

	updateListBoxButtonPos();
}
void CRedPacketPoolListBox::DeleteAllIndex()
{	
	for (int i=0; i< GetCount(); i++) {
		List_RedAppendData * pData = (List_RedAppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pSta0;
			pData->pSta0 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

			delete pData->pSta2;
			pData->pSta2 = NULL ;

			delete pData->pSta3;
			pData->pSta3 = NULL ;

			delete pData->pSta4;
			pData->pSta4 = NULL ;

			delete pData->pSta5;
			pData->pSta5 = NULL ;

			delete pData;
			pData = NULL;
		}
	}
	ResetContent();
	m_mButton.clear();
}
void CRedPacketPoolListBox::DeleteIndex(int iIndex)
{
	List_RedAppendData * pData = (List_RedAppendData *)GetItemDataPtr(iIndex);

	if ( NULL != pData ) {
		delete pData->pSta0;
		pData->pSta0 = NULL ;

		delete pData->pSta1;
		pData->pSta1 = NULL ;

		delete pData->pSta2;
		pData->pSta2 = NULL ;

		delete pData->pSta3;
		pData->pSta3 = NULL ;

		delete pData->pSta4;
		pData->pSta4 = NULL ;

		delete pData->pSta5;
		pData->pSta5 = NULL ;

		delete pData;
		pData = NULL;
	}

	DeleteString(iIndex);
	Redbutton_map::iterator iter = m_mButton.find(iIndex);
	if (iter != m_mButton.end())
	{
		m_mButton.erase(iter);
	}

}
void CRedPacketPoolListBox::InvalidateListBox()
{
	for (int i=0; i<GetCount(); i++)
	{
		List_RedAppendData * pData = (List_RedAppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			pData->pSta0->ShowWindow( SW_HIDE );
			pData->pSta0->ShowWindow( SW_SHOW );

			pData->pSta1->ShowWindow( SW_HIDE );
			pData->pSta1->ShowWindow( SW_SHOW );

			pData->pSta2->ShowWindow( SW_HIDE );
			pData->pSta2->ShowWindow( SW_SHOW );

			pData->pSta3->ShowWindow( SW_HIDE );
			pData->pSta3->ShowWindow( SW_SHOW );

			pData->pSta4->ShowWindow( SW_HIDE );
			pData->pSta4->ShowWindow( SW_SHOW );

			pData->pSta5->ShowWindow( SW_HIDE );
			pData->pSta5->ShowWindow( SW_SHOW );

		}
	}
}
void CRedPacketPoolListBox::updateListBoxButtonPos()
{
	Redbutton_map::iterator iter;
	Redbutton_map::iterator iterUp;
	int iLine = 0;
	for (int nItem = 0  ; nItem < m_mButton.size(); nItem++ ){
		iter = m_mButton.find(nItem);
		if( iter!= m_mButton.end() ){
			iterUp = m_mButton.find(iLine);
			List_RedAppendData *pData = (List_RedAppendData*)iter->second;
			if ( NULL != pData ) {
				pData->pSta0->SetFont(110, _T("宋体"));				//设置显示字体和大小
				pData->pSta0->ShowWindow( SW_SHOW );
				pData->pSta0->SetWindowPos(NULL , 10 , pData->nItem*28 +10 , 100 , 29 , SWP_SHOWWINDOW );

				pData->pSta1->SetFont(110, _T("宋体"));				//设置显示字体和大小
				pData->pSta1->ShowWindow( SW_SHOW );
				pData->pSta1->SetWindowPos(NULL , 110 , pData->nItem*28 +10 , 90 , 29 , SWP_SHOWWINDOW );

				pData->pSta2->SetFont(110, _T("宋体"));				//设置显示字体和大小
				pData->pSta2->ShowWindow( SW_SHOW );
				pData->pSta2->SetWindowPos(NULL , 210 , pData->nItem*28 +10 , 150 , 29 , SWP_SHOWWINDOW );

				pData->pSta3->SetFont(110, _T("宋体"));				//设置显示字体和大小
				pData->pSta3->ShowWindow( SW_SHOW );
				pData->pSta3->SetWindowPos(NULL , 360 , pData->nItem*28 +10 , 150 , 29 , SWP_SHOWWINDOW );

				pData->pSta4->SetFont(110, _T("宋体"));				//设置显示字体和大小
				pData->pSta4->ShowWindow( SW_SHOW );
				pData->pSta4->SetWindowPos(NULL , 550 , pData->nItem*28 +10 , 60 , 29 , SWP_SHOWWINDOW );

				pData->pSta5->SetFont(110, _T("宋体"));				//设置显示字体和大小
				pData->pSta5->ShowWindow( SW_SHOW );
				pData->pSta5->SetWindowPos(NULL , 660 , pData->nItem*28 +10 , 60 , 29 , SWP_SHOWWINDOW );
			}
			iLine++;
		}
	}
}
void CRedPacketPoolListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	int iCount = GetCount();
	for (int i=0; i<iCount; i++)
	{
		List_RedAppendData * pData = (List_RedAppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pSta0;
			pData->pSta0 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

			delete pData->pSta2;
			pData->pSta2 = NULL ;

			delete pData->pSta3;
			pData->pSta3 = NULL ;

			delete pData->pSta4;
			pData->pSta4 = NULL ;

			delete pData->pSta5;
			pData->pSta5 = NULL ;

			delete pData;
			pData = NULL;
		}
	}
}

BOOL CRedPacketPoolListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CListBox::OnEraseBkgnd(pDC);
}

void CRedPacketPoolListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListBox::OnMouseMove(nFlags, point);
}
List_RedAppendData* CRedPacketPoolListBox::GetAppendDataInfo(int iIndex)
{
	std::map< int,List_RedAppendData*>::iterator it ;
	for ( it = m_mButton.begin() ; it != m_mButton.end() ;it++ ) {
		if( iIndex == it->first ) {
			List_RedAppendData *pData = (List_RedAppendData*)it->second;
			return pData ;
		}
	}
	return NULL ;
}
void CRedPacketPoolListBox::SetIndexInage(int iIndex , UINT nButImage  )
{
	List_RedAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		//pData->pBut2->SetBitmaps( nButImage , RGB(255, 255, 0) , nButImage , RGB(255, 255, 255) );
	/*	pData->pBut2->SetBitmaps( nButImage , RGB(255, 255, 0), nButImage , RGB(255, 255, 255) );
		pData->pBut2->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		pData->pBut2->SetFontEx(20 , _T("微软雅黑"));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));*/
	}
}
void CRedPacketPoolListBox::SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col)
{
	List_RedAppendData *pData = GetAppendDataInfo(iIndex) ;
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
void CRedPacketPoolListBox::SetIndexString(int iIndex , CString strSta0 ,CString strSta1 ,CString strSta2 ,CString strSta3 ,CString strSta4,CString strSta5)
{
	List_RedAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pSta0->ShowWindow(SW_HIDE);
		pData->pSta0->SetWindowText(strSta0);
		pData->pSta0->ShowWindow(SW_SHOW);

		pData->pSta1->ShowWindow(SW_HIDE);
		pData->pSta1->SetWindowText(strSta1);
		pData->pSta1->ShowWindow(SW_SHOW);

		pData->pSta2->ShowWindow(SW_HIDE);
		pData->pSta2->SetWindowText(strSta2);
		pData->pSta2->ShowWindow(SW_SHOW);

		pData->pSta3->ShowWindow(SW_HIDE);
		pData->pSta3->SetWindowText(strSta3);
		pData->pSta3->ShowWindow(SW_SHOW);

		pData->pSta4->ShowWindow(SW_HIDE);
		pData->pSta4->SetWindowText(strSta4);
		pData->pSta4->ShowWindow(SW_SHOW);

		pData->pSta5->ShowWindow(SW_HIDE);
		pData->pSta5->SetWindowText(strSta5);
		pData->pSta5->ShowWindow(SW_SHOW);


	}
}

BOOL CRedPacketPoolListBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CListBox::PreTranslateMessage(pMsg);
}
