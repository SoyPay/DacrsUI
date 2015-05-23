// RecordListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "RecordListBox.h"


// CRecordListBox
#define IDC_RE_BUTTON_ID 0x12356

IMPLEMENT_DYNAMIC(CRecordListBox, CListBox)

CRecordListBox::CRecordListBox()
{
	m_uID = IDC_RE_BUTTON_ID;
}

CRecordListBox::~CRecordListBox()
{
}


BEGIN_MESSAGE_MAP(CRecordListBox, CListBox)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CRecordListBox 消息处理程序

void CRecordListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return ;
	if (GetCount() == 0)
	{
		return;
	}
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	List_ReAppendData * pData =(List_ReAppendData *) GetItemDataPtr(lpDrawItemStruct->itemID);

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
void CRecordListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  添加您的代码以确定指定项的大小
	lpMeasureItemStruct->itemHeight = 50;
}
void CRecordListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	int iCount = GetCount();
	for (int i=0; i<iCount; i++)
	{
		List_ReAppendData * pData = (List_ReAppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pBut1;
			pData->pBut1 = NULL ;

			delete pData->pSta1;
			pData->pSta1 = NULL ;

			delete pData->pSta2;
			pData->pSta2 = NULL ;

			delete pData->pSta3;
			pData->pSta1 = NULL ;

			delete pData->pSta4;
			pData->pSta4 = NULL ;

			delete pData;
			pData = NULL;
		}
	}
}
BOOL CRecordListBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CListBox::OnEraseBkgnd(pDC);
}
void CRecordListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListBox::OnMouseMove(nFlags, point);
}
void CRecordListBox::InsertStr(int iIndex,HWND hMain)
{
	List_ReAppendData * pData = new List_ReAppendData;
	pData->nItem = iIndex ;
	CRect rcClient(0 ,0 , 0 , 0);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	pData->pBut1 = new CButtonEx(iIndex,0,rcClient,hMain);
	pData->pBut1->Create(_T(""),dwStyle, rcClient, this, ++m_uID);

	pData->pSta1 = new CStaticTrans;
	pData->pSta1->Create(_T("1") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta2 = new CStaticTrans;
	pData->pSta2->Create(_T("2") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta3 = new CStaticTrans;
	pData->pSta3->Create(_T("3") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta4 = new CStaticTrans;
	pData->pSta4->Create(_T("4") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	m_mButton.insert( make_pair( iIndex, pData ) );

	int i = SetItemDataPtr(InsertString(iIndex,_T("")),pData);

	updateListBoxButtonPos();
}
void CRecordListBox::updateListBoxButtonPos()
{
	ReButton_map::iterator iter;
	ReButton_map::iterator iterUp;
	int iLine = 0;
	for (int nItem = 0  ; nItem < m_mButton.size(); nItem++ ){
		iter = m_mButton.find(nItem);
		if( iter!= m_mButton.end() ){
			iterUp = m_mButton.find(iLine);
			List_ReAppendData *pData = (List_ReAppendData*)iter->second;
			if ( NULL != pData ) {
				pData->pBut1->SetFontEx(20 , _T("微软雅黑"));
				pData->pBut1->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
				pData->pBut1->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
				pData->pBut1->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
				pData->pBut1->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
				pData->pBut1->ShowWindow( SW_HIDE );
				pData->pBut1->SetWindowPos(NULL , 0 , pData->nItem*50 +15 , 240 , 29 , SWP_SHOWWINDOW );

				pData->pSta1->SetFont(90, _T("宋体"));				//设置显示字体和大小
				pData->pSta1->SetTextColor(RGB(0,0,0));			    //字体颜色
				pData->pSta1->ShowWindow( SW_SHOW );
				pData->pSta1->SetWindowPos(NULL , 10 , pData->nItem*50 +25 , 60 , 29 , SWP_SHOWWINDOW );	

				pData->pSta2->SetFont(90, _T("宋体"));				//设置显示字体和大小
				pData->pSta2->SetTextColor(RGB(0,0,0));			    //字体颜色
				pData->pSta2->ShowWindow( SW_SHOW );
				pData->pSta2->SetWindowPos(NULL , 370 , pData->nItem*50 +25 , 60 , 29 , SWP_SHOWWINDOW );

				pData->pSta3->SetFont(90, _T("宋体"));				//设置显示字体和大小
				pData->pSta3->SetTextColor(RGB(0,0,0));			    //字体颜色
				pData->pSta3->ShowWindow( SW_SHOW );
				pData->pSta3->SetWindowPos(NULL , 440 , pData->nItem*50 +25 , 60 , 29 , SWP_SHOWWINDOW );

				pData->pSta4->SetFont(90, _T("宋体"));				//设置显示字体和大小
				pData->pSta4->SetTextColor(RGB(0,0,0));			    //字体颜色
				pData->pSta4->ShowWindow( SW_SHOW );
				pData->pSta4->SetWindowPos(NULL , 510 , pData->nItem*50 +25 , 100 , 29 , SWP_SHOWWINDOW );
			}
			iLine++;
		}
	}
}
List_ReAppendData* CRecordListBox::GetAppendDataInfo(int iIndex)
{
	std::map< int,List_ReAppendData*>::iterator it ;
	for ( it = m_mButton.begin() ; it != m_mButton.end() ;it++ ) {
		if( iIndex == it->first ) {
			List_ReAppendData *pData = (List_ReAppendData*)it->second;
			return pData ;
		}
	}
	return NULL ;
}
void CRecordListBox::SetIndexInage(int iIndex ,  UINT nButImage1 )
{
	List_ReAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pBut1->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		pData->pBut1->SetBitmaps( nButImage1 , RGB(255, 255, 0) , nButImage1 , RGB(255, 255, 255) );
	}
}
void CRecordListBox::SetIndexString(int iIndex , CString strBut1 ,CString strSta1 ,CString strSta2 ,CString strSta3 ,CString strSta4  )
{
	List_ReAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pBut1->SetWindowText(strBut1) ;
		pData->pBut1->ShowWindow( SW_HIDE );

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

	}
}