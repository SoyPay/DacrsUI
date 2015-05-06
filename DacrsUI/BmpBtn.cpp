// BmpBtn.cpp : implementation file
//

#include "stdafx.h"

#include "BmpBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpBtn

CBmpBtn::CBmpBtn()
{
}

CBmpBtn::~CBmpBtn() 
{
}


BEGIN_MESSAGE_MAP(CBmpBtn, CButton)
	//{{AFX_MSG_MAP(CBmpBtn)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBmpBtn message handlers
void CBmpBtn::DrawItem(LPDRAWITEMSTRUCT  lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS ->hDC);
	if (lpDIS->itemState & ODS_SELECTED)
		pDC ->BitBlt(0,0,lpDIS ->rcItem.right,lpDIS ->rcItem.bottom,
			&m_dc3,0,0,SRCCOPY);
	else
	{

		if(	GetCapture() == this)
			pDC ->BitBlt(0,0,lpDIS ->rcItem.right,lpDIS ->rcItem.bottom,
				&m_dc2,0,0,SRCCOPY);
		else
			pDC ->BitBlt(0,0,lpDIS ->rcItem.right,lpDIS ->rcItem.bottom,
			&m_dc1,0,0,SRCCOPY);
	}

}

void CBmpBtn::SetBitmaps(UINT nID1, UINT nID2, UINT nID3)
{
	CClientDC dc(this);
	CBitmap b1,b2,b3;
	
	b1.LoadBitmap(nID1);
	m_dc1.CreateCompatibleDC(&dc);
	m_dc1.SelectObject(&b1);
	
	m_dc2.CreateCompatibleDC(&dc);
	b2.LoadBitmap(nID2);
	m_dc2.SelectObject(&b2);

	m_dc3.CreateCompatibleDC(&dc);
	b3.LoadBitmap(nID3);
	m_dc3.SelectObject(&b3);
}
#ifndef IDC_HAND
#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif /* WINVER >= 0x0500 */

void CBmpBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	::SetCursor(::LoadCursor(NULL,IDC_HAND));
	CRect rect;
	GetClientRect(rect);
	if(GetCapture() != this)
	{
		SetCapture();
		Invalidate(FALSE);
	}
	else
	{
		if(!rect.PtInRect(point))
		{
			ReleaseCapture();
			Invalidate(FALSE);

		}
	}
	
	CButton::OnMouseMove(nFlags, point);
}
