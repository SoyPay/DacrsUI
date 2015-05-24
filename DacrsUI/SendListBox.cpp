// RecordListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SendListBox.h"


// CRecordListBox
#define IDC_RE_BUTTON_ID 0x123

IMPLEMENT_DYNAMIC(CSendListBox, CListBox)

CSendListBox::CSendListBox()
{
	m_uID = IDC_RE_BUTTON_ID;
}

CSendListBox::~CSendListBox()
{
}


BEGIN_MESSAGE_MAP(CSendListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()



// CRecordListBox 消息处理程序

void CSendListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return ;
	if (GetCount() == 0)
	{
		return;
	}
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	List_SendAppendData * pData =(List_SendAppendData *) GetItemDataPtr(lpDrawItemStruct->itemID);

	//获取列表数据
	CString str;
	GetText(lpDrawItemStruct->itemID,str);
	if (lpDrawItemStruct->itemAction | ODA_SELECT && lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		CBrush t_brush1;
		t_brush1.CreateSolidBrush(RGB(229,239,244));
		dc.FillRect(&lpDrawItemStruct->rcItem,&t_brush1);

	}
	else
	{
		CBrush t_brush1;
		t_brush1.CreateSolidBrush(RGB(255,255,255));
		dc.FillRect(&lpDrawItemStruct->rcItem,&t_brush1);
	}
	//底部边线
	CPen t_pen(PS_SOLID,1,RGB(211,218,223));
	dc.SelectObject(t_pen);
	dc.MoveTo(0,lpDrawItemStruct->rcItem.bottom-1);
	dc.LineTo(lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom-1);

	dc.Detach();
}
void CSendListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight = 35;
}
void CSendListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	int iCount = GetCount();
	for (int i=0; i<iCount; i++)
	{
		List_SendAppendData * pData = (List_SendAppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pSta0;
			pData->pSta0 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

			delete pData->pSta2;
			pData->pSta2 = NULL ;

			delete pData->pBut0;
			pData->pBut0 = NULL ;

			delete pData;
			pData = NULL;
		}
	}
}
BOOL CSendListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CListBox::OnEraseBkgnd(pDC);
}
void CSendListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListBox::OnMouseMove(nFlags, point);
}
void CSendListBox::InsertStr(int iIndex,HWND hMain)
{
	List_SendAppendData * pData = new List_SendAppendData;
	pData->nItem = iIndex ;
	CRect rcClient(0 ,0 , 0 , 0);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	pData->pSta0 = new CStaticTrans;
	pData->pSta0->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta1 = new CStaticTrans;
	pData->pSta1->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta2 = new CStaticTrans;
	pData->pSta2->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pBut0 = new CButtonEx(iIndex,1,rcClient,hMain);
	pData->pBut0->Create(_T(""),dwStyle, rcClient, this, ++m_uID);


	m_mButton.insert( make_pair( iIndex, pData ) );

	int i = SetItemDataPtr(InsertString(iIndex,_T("")),pData);

	updateListBoxButtonPos();
}
void CSendListBox::updateListBoxButtonPos()
{
	SendButton_map::iterator iter;
	SendButton_map::iterator iterUp;
	int iLine = 0;
	for (int nItem = 0  ; nItem < m_mButton.size(); nItem++ ){
		iter = m_mButton.find(nItem);
		if( iter!= m_mButton.end() ){
			iterUp = m_mButton.find(iLine);
			List_SendAppendData *pData = (List_SendAppendData*)iter->second;
			if ( NULL != pData ) {
				pData->pSta0->SetFont(110, _T("楷体"));				//设置显示字体和大小
				//pData->pSta0->SetTextColor(RGB(0,0,0));			    //字体颜色
				pData->pSta0->ShowWindow( SW_SHOW );
				//pData->pSta0->SetBackColor(RGB(0,0,0));	
				pData->pSta0->ModifyStyle(0,SS_CENTERIMAGE|SS_CENTER);
				pData->pSta0->SetWindowPos(NULL , 10 , pData->nItem*35+10 , 305 , 29 , SWP_SHOWWINDOW );

				pData->pSta1->SetFont(110, _T("楷体"));				//设置显示字体和大小
				//pData->pSta1->SetTextColor(RGB(0,0,0));			    //字体颜色
				pData->pSta1->ShowWindow( SW_SHOW );
				//pData->pSta0->SetBackColor(RGB(0,0,0));	
				pData->pSta1->ModifyStyle(0,SS_CENTERIMAGE|SS_CENTER);
				pData->pSta1->SetWindowPos(NULL , 305 + 10 + 30 , pData->nItem*35+10 , 200 , 29 , SWP_SHOWWINDOW );	

				pData->pSta2->SetFont(110, _T("宋体"));				//设置显示字体和大小
				//pData->pSta2->SetTextColor(RGB(0,0,0));			    //字体颜色
				pData->pSta2->ShowWindow( SW_SHOW );
				//pData->pSta0->SetBackColor(RGB(0,0,0));	
				pData->pSta2->ModifyStyle(0,SS_CENTERIMAGE|SS_CENTER);
				pData->pSta2->SetWindowPos(NULL , 305 + 10 + 60 +200  , pData->nItem*35 +10 , 80 , 29 , SWP_SHOWWINDOW );

				pData->pBut0->ShowWindow( SW_SHOW );
				pData->pBut0->SetWindowPos(NULL , 305 + 10 + 90 +280 , pData->nItem*35 +10 , 43 , 33 , SWP_SHOWWINDOW );

			}
			iLine++;
		}
	}
}
List_SendAppendData* CSendListBox::GetAppendDataInfo(int iIndex)
{
	std::map< int,List_SendAppendData*>::iterator it ;
	for ( it = m_mButton.begin() ; it != m_mButton.end() ;it++ ) {
		if( iIndex == it->first ) {
			List_SendAppendData *pData = (List_SendAppendData*)it->second;
			return pData ;
		}
	}
	return NULL ;
}
void CSendListBox::SetIndexInage(int iIndex ,  UINT nButImage )
{
	List_SendAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pBut0->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		pData->pBut0->SetBitmaps( nButImage , RGB(255, 255, 0) , nButImage , RGB(255, 255, 255) );
	}
}
void CSendListBox::SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col)
{
	List_SendAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		switch (nline)
		{
		case 0:
			pData->pSta0->SetTextColor(col);
			break;
		case 1:
			pData->pSta1->SetTextColor(col);
			break;
		case 2:
			pData->pSta2->SetTextColor(col);
			break;
		}
	}
}
void CSendListBox::SetIndexString(int iIndex , CString strSta0 ,CString strSta1 ,CString strSta2 ,CString strBut0,CString pstr )
{
	List_SendAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pSta1->ShowWindow(SW_HIDE);
		pData->pSta0->SetWindowText(strSta0) ;
		pData->pSta0->ShowWindow( SW_SHOW );

		pData->pSta1->ShowWindow(SW_HIDE);
		pData->pSta1->SetWindowText(strSta1);
		pData->pSta1->ShowWindow(SW_SHOW);

		pData->pSta2->ShowWindow(SW_HIDE);
		pData->pSta2->SetWindowText(strSta2);
		pData->pSta2->ShowWindow(SW_SHOW);

		
		pData->pBut0->SetWindowText(strBut0);
		pData->pBut0->SetFontEx(20 , _T("微软雅黑"));
		pData->pBut0->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		pData->pBut0->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		pData->pBut0->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		pData->pBut0->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		pData->pstr.Format(_T("%s") ,pstr ) ;
	}
}

BOOL CSendListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE ;
	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}
