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
		//t_brush1.CreateSolidBrush(RGB(229,239,244));
		//dc.FillRect(&lpDrawItemStruct->rcItem,&t_brush1);

	}
	else
	{
		CBrush t_brush1;
		//	t_brush1.CreateSolidBrush(RGB(255,255,255));
		//dc.FillRect(&lpDrawItemStruct->rcItem,&t_brush1);
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
	lpMeasureItemStruct->itemHeight = 40;
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
	CButtonEx *pButton = new CButtonEx(iIndex,0,rcClient,hMain);
	pButton->Create(_T(""),dwStyle, rcClient, this, ++m_uID);
	pData->pBut1 = pButton ;

	CStaticTrans *pSta = new CStaticTrans;
	pSta->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;
	pData->pSta1 = pSta ;
	pSta = new CStaticTrans;
	pSta->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;
	pData->pSta2 = pSta ;
	pSta = new CStaticTrans;
	pSta->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;
	pData->pSta3 = pSta ;
	pSta = new CStaticTrans;
	pSta->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;
	pData->pSta4 = pSta ;

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
				pData->pSta1->ShowWindow( SW_SHOW );
				pData->pSta1->SetWindowPos(NULL , 20 , pData->nItem*40 +25 , 80 , 25 , SWP_SHOWWINDOW );

				pData->pBut1->ShowWindow( SW_SHOW );
				pData->pBut1->SetWindowPos(NULL , 165 , pData->nItem*40 +15 , 78 , 31 , SWP_SHOWWINDOW );

				pData->pSta2->ShowWindow( SW_SHOW );
				pData->pSta2->SetWindowPos(NULL , 285 , pData->nItem*40 +25 , 80 , 25 , SWP_SHOWWINDOW );

				pData->pSta3->ShowWindow( SW_SHOW );
				pData->pSta3->SetWindowPos(NULL , 285 , pData->nItem*40 +25 , 80 , 25 , SWP_SHOWWINDOW );
			}
			iLine++;
		}
	}
}
