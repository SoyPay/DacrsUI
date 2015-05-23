// RecordListBox.cpp : ʵ���ļ�
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



// CRecordListBox ��Ϣ�������

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

	//��ȡ�б�����
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
	//�ײ�����
	CPen t_pen(PS_SOLID,1,RGB(211,218,223));
	dc.SelectObject(t_pen);
	dc.MoveTo(0,lpDrawItemStruct->rcItem.bottom-1);
	dc.LineTo(lpDrawItemStruct->rcItem.right,lpDrawItemStruct->rcItem.bottom-1);

	dc.Detach();
}
void CRecordListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO:  ������Ĵ�����ȷ��ָ����Ĵ�С
	lpMeasureItemStruct->itemHeight = 35;
}
void CRecordListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	int iCount = GetCount();
	for (int i=0; i<iCount; i++)
	{
		List_ReAppendData * pData = (List_ReAppendData *)GetItemDataPtr(i);
		if ( NULL != pData ) {
			delete pData->pSta0;
			pData->pSta0 = NULL ;

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CListBox::OnEraseBkgnd(pDC);
}
void CRecordListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CListBox::OnMouseMove(nFlags, point);
}
void CRecordListBox::InsertStr(int iIndex,HWND hMain)
{
	List_ReAppendData * pData = new List_ReAppendData;
	pData->nItem = iIndex ;
	CRect rcClient(0 ,0 , 0 , 0);
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	pData->pSta0 = new CStaticTrans;
	pData->pSta0->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta1 = new CStaticTrans;
	pData->pSta1->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta2 = new CStaticTrans;
	pData->pSta2->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta3 = new CStaticTrans;
	pData->pSta3->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

	pData->pSta4 = new CStaticTrans;
	pData->pSta4->Create(_T("") , WS_CHILD | WS_VISIBLE , rcClient, this, ++m_uID) ;

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
				pData->pSta0->SetFont(110, _T("����"));				//������ʾ����ʹ�С
				pData->pSta0->SetTextColor(RGB(0,0,0));			    //������ɫ
				pData->pSta0->ShowWindow( SW_SHOW );
				//pData->pSta0->SetBackColor(RGB(0,0,0));	
				pData->pSta0->ModifyStyle(0,SS_CENTERIMAGE|SS_CENTER);
				pData->pSta0->SetWindowPos(NULL , 10 , pData->nItem*35+10 , 305 , 29 , SWP_SHOWWINDOW );

				pData->pSta1->SetFont(110, _T("����"));				//������ʾ����ʹ�С
				pData->pSta1->SetTextColor(RGB(0,0,0));			    //������ɫ
				pData->pSta1->ShowWindow( SW_SHOW );
				//pData->pSta0->SetBackColor(RGB(0,0,0));	
				pData->pSta1->ModifyStyle(0,SS_CENTERIMAGE|SS_CENTER);
				pData->pSta1->SetWindowPos(NULL , 305 + 10 + 25 , pData->nItem*35+10 , 80 , 29 , SWP_SHOWWINDOW );	

				pData->pSta2->SetFont(110, _T("����"));				//������ʾ����ʹ�С
				pData->pSta2->SetTextColor(RGB(0,0,0));			    //������ɫ
				pData->pSta2->ShowWindow( SW_SHOW );
				//pData->pSta0->SetBackColor(RGB(0,0,0));	
				pData->pSta2->ModifyStyle(0,SS_CENTERIMAGE|SS_CENTER);
				pData->pSta2->SetWindowPos(NULL , 305 + 10 + 50 +80  , pData->nItem*35 +10 , 80 , 29 , SWP_SHOWWINDOW );

				pData->pSta3->SetFont(110, _T("����"));				//������ʾ����ʹ�С
				pData->pSta3->SetTextColor(RGB(0,0,0));			    //������ɫ
				pData->pSta3->ShowWindow( SW_SHOW );
				//pData->pSta0->SetBackColor(RGB(0,0,0));	
				pData->pSta3->ModifyStyle(0,SS_CENTERIMAGE|SS_CENTER);
				pData->pSta3->SetWindowPos(NULL , 305 + 10 + 75 +160 , pData->nItem*35 +10 , 80 , 29 , SWP_SHOWWINDOW );

				pData->pSta4->SetFont(110, _T("����"));				//������ʾ����ʹ�С
				pData->pSta4->SetTextColor(RGB(0,0,0));			    //������ɫ
				pData->pSta4->ShowWindow( SW_SHOW );
				//pData->pSta0->SetBackColor(RGB(0,0,0));	
				pData->pSta4->ModifyStyle(0,SS_CENTERIMAGE|SS_CENTER);
				pData->pSta4->SetWindowPos(NULL , 305 + 10 + 100 +240 , pData->nItem*35 +10 , 200 , 29 , SWP_SHOWWINDOW );
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
void CRecordListBox::SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col)
{
	List_ReAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		switch (nline)
		{
		case 0:
			pData->pSta0->SetBackColor(col);
			break;
		case 1:
			pData->pSta1->SetBackColor(col);
			break;
		case 2:
			pData->pSta2->SetBackColor(col);
			break;
		case 3:
			pData->pSta3->SetBackColor(col);
			break;
		case 4:
			pData->pSta4->SetBackColor(col);
			break;
		}
	}
}
void CRecordListBox::SetIndexString(int iIndex , CString strBut1 ,CString strSta1 ,CString strSta2 ,CString strSta3 ,CString strSta4  )
{
	List_ReAppendData *pData = GetAppendDataInfo(iIndex) ;
	if ( NULL != pData ) {
		pData->pSta1->ShowWindow(SW_HIDE);
		pData->pSta0->SetWindowText(strBut1) ;
		pData->pSta0->ShowWindow( SW_SHOW );

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