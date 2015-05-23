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
	m_pTitleSta1 = NULL ;
	m_pTitleSta2 = NULL ;
}

CUseListBox::~CUseListBox()
{
}


BEGIN_MESSAGE_MAP(CUseListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CUseListBox 消息处理程序



void CUseListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return;
	if (GetCount() == 0)
	{
		return;
	}
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	List_AppendData * pData =(List_AppendData *) GetItemDataPtr(lpDrawItemStruct->itemID);

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
	CPen t_pen(PS_SOLID,1,RGB(0,0,0));
	dc.SelectObject(t_pen);
	dc.MoveTo(0,lpDrawItemStruct->rcItem.bottom-1);
	dc.LineTo(lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom-1);



	dc.Detach();
}

void CUseListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight = 50;
}
void CUseListBox::InsertStr(int iIndex,HWND hMain)
{
	List_AppendData * pData = new List_AppendData;
	pData->nItem = iIndex ;
	CRect rcClient(0 ,0 , 0 , 0);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	pData->pBut1 = new CButtonEx(iIndex,0,rcClient,hMain);
	pData->pBut1->Create(_T(""),dwStyle, rcClient, this, ++m_uID);

	pData->pBut2 = new CButtonEx(iIndex,1,rcClient,hMain);
	pData->pBut2->Create(_T(""),dwStyle, rcClient, this, ++m_uID);

	CStaticTrans *pSta = new CStaticTrans;
	pSta->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;
	pData->pSta1 = pSta ;

	m_mButton.insert( make_pair( iIndex, pData ) );

	int i = SetItemDataPtr(InsertString(iIndex,_T("")),pData);

	updateListBoxButtonPos();
}
void CUseListBox::CreateTitle()
{
	if ( NULL == m_pTitleSta1 ) {
		m_pTitleSta1 = new CStaticTrans;
		m_pTitleSta1->Create(_T("发起人") , WS_CHILD | WS_VISIBLE , CRect(0,0,0,0), this, ++m_uID) ;
		m_pTitleSta1->SetWindowPos(NULL , 118 , 0 , 254 , 35 , SWP_SHOWWINDOW );
		m_pTitleSta1->ShowWindow( SW_SHOW );
	}
	if ( NULL == m_pTitleSta2 ) {
		m_pTitleSta2 = new CStaticTrans;
		m_pTitleSta2->Create(_T("金额") , WS_CHILD | WS_VISIBLE , CRect(0,0,0,0), this, ++m_uID) ;
		m_pTitleSta2->SetWindowPos(NULL , 280 , 0 , 80 , 35 , SWP_SHOWWINDOW );
		m_pTitleSta2->ShowWindow( SW_SHOW );
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
              pData->pBut1->ShowWindow( SW_SHOW );
			  pData->pBut1->SetWindowPos(NULL , 10 , pData->nItem*50 +20 , 254 , 35 , SWP_SHOWWINDOW );
			  pData->pBut2->ShowWindow( SW_SHOW );
			  pData->pBut2->SetWindowPos(NULL , 370 , pData->nItem*50 +20 , 43 , 33 , SWP_SHOWWINDOW );
			  pData->pSta1->ShowWindow( SW_SHOW );
			  pData->pSta1->SetWindowPos(NULL , 280 , pData->nItem*50 +30 , 80 , 25 , SWP_SHOWWINDOW );
		   }
		   iLine++;
		}
	}
}
void CUseListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if ( NULL != m_pTitleSta1 ) {
		delete m_pTitleSta1 ;
		m_pTitleSta1 = NULL ;
	}
	if ( NULL != m_pTitleSta2 ) {
		delete m_pTitleSta2 ;
		m_pTitleSta2 = NULL ;
	}
	int iCount = GetCount();
	for (int i=0; i<iCount; i++)
	{
		List_AppendData * pData = (List_AppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pBut1;
			pData->pBut1 = NULL ;
			delete pData->pBut2;
			pData->pBut2 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

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
void CUseListBox::SetIndexInage(int iIndex ,  UINT nButImage1 , UINT nButImage2  )
{
	List_AppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pBut1->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		pData->pBut2->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		pData->pBut1->SetBitmaps( nButImage1 , RGB(255, 255, 0) , nButImage1 , RGB(255, 255, 255) );
		pData->pBut2->SetBitmaps( nButImage2 , RGB(255, 255, 0) , nButImage2 , RGB(255, 255, 255) );
	}
}
void CUseListBox::SetIndexString(int iIndex , CString strBut1 ,CString strBut2 ,CString strSta1 ,CString strSta2 )
{
	List_AppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pBut1->SetWindowText(strBut1) ;
		pData->pBut2->SetWindowText(strBut2) ;

		pData->pBut1->SetFontEx(20 , _T("微软雅黑"));
		pData->pBut1->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		pData->pBut1->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		pData->pBut1->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		pData->pBut1->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));

		pData->pBut2->SetFontEx(20 , _T("微软雅黑"));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		pData->pBut2->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));

		pData->pSta1->SetFont(90, _T("宋体"));				//设置显示字体和大小
		pData->pSta1->SetTextColor(RGB(0,0,0));			    //字体颜色
		pData->pSta1->ShowWindow(SW_HIDE);
		pData->pSta1->SetWindowText(strSta1);
		pData->pSta1->ShowWindow(SW_SHOW);

	}
}

