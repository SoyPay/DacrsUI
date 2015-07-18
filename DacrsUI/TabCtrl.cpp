// 360TabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "TabCtrl.h"


// C360TabCtrl

IMPLEMENT_DYNAMIC(CCTabCtrl, CTabCtrl)

CCTabCtrl::CCTabCtrl()
{
   m_nItempadding=8;
}

CCTabCtrl::~CCTabCtrl()
{
}

void CCTabCtrl::SetItemPadding(int nPadding/*=8*/)
{
   m_nItempadding=nPadding;
}

BEGIN_MESSAGE_MAP(CCTabCtrl, CTabCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// C360TabCtrl message handlers



void CCTabCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bmap;
	bmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	MemDC.SelectObject(&bmap);
	CBrush br;
	br.CreateSolidBrush(RGB(227,230,235));
	MemDC.FillRect(&rect,&br);

	int   nTab;
	int   nItemCount =   GetItemCount();   
	int   nSel   =   GetCurSel();
	nTab = nItemCount;
	if   (!nTab)   // no   pages   added   
		return;   
	CRect rcItem;
	CRect rectTemp;
	CString strItem = _T("");
	TCITEM tcItem;
	tcItem.pszText = strItem.GetBuffer(1024);
	tcItem.cchTextMax = 1023;

	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(211,211,211));//211,211,211
	MemDC.SelectObject(&pen);

	CImage image;
	image.LoadFromResource(AfxGetInstanceHandle(),IDB_TABCTRL);
	while(nTab--) 
	{   

		//memset(tcItem.pszText,0,1024*2);
		GetItemRect(nTab,&rcItem);
		rectTemp=rcItem;
		rectTemp.OffsetRect(0,-2);
		MemDC.FillSolidRect(&rectTemp,RGB(227,230,235));
		rectTemp.right-=m_nItempadding; // 留一个空隙做每个item的
		MemDC.MoveTo(rectTemp.right,rectTemp.bottom-1);
		MemDC.LineTo(rcItem.right,rectTemp.bottom-1);
		if (nTab==(nItemCount-1))
		{
			MemDC.MoveTo(rectTemp.right,rectTemp.bottom-1);
			MemDC.LineTo(rect.right,rectTemp.bottom-1);
			MemDC.MoveTo(rect.left+2,rectTemp.bottom);
			MemDC.LineTo(rect.left+2,rect.bottom);
			MemDC.FillSolidRect(rect.left+3,rectTemp.bottom,rect.right,rect.bottom,RGB(255,255,255));
			MemDC.MoveTo(rect.right-1,rectTemp.bottom);
			MemDC.LineTo(rect.right-1,rect.bottom);
		}
		/*if (nTab==nSel)
		{
		rcItem.InflateRect(0,3,0,0);
		}*/

		//rectTemp.DeflateRect(1,1);
		if (nTab ==nSel)
		{
			image.Draw(MemDC,rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(),1,29,13,30);
			image.Draw(MemDC,rectTemp.left, rectTemp.top, 1, rectTemp.Height(),0,29,1,30);
			image.Draw(MemDC,rectTemp.right-1, rectTemp.top, 1, rectTemp.Height(),14,29,1,30);	

			//MemDC.FillSolidRect(&rectTemp,RGB(255,255,255));
		}
		else
		{
			/*MemDC.Rectangle(&rcItem);
			MemDC.FillSolidRect(&rectTemp,RGB(217,217,217));*/

			image.Draw(MemDC,rectTemp.left, rectTemp.top, rectTemp.Width(), rectTemp.Height(),2,0,11,30);
			image.Draw(MemDC,rectTemp.left, rectTemp.top, 1, rectTemp.Height(),0,0,1,30);
			image.Draw(MemDC,rectTemp.right-1, rectTemp.top, 1, rectTemp.Height(),14,0,1,30);	
		}

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		/*if (rcItem.PtInRect(point)||(nTab ==nSel)) // 鼠标在标签或者标签被选中，高亮显示
		{
		rectTemp = rcItem;
		rectTemp.bottom=rectTemp.top+3;
		MemDC.FillSolidRect(&rectTemp,RGB(255,200,60));
		}*/

		CFont font;
		font.CreateFont(12,   // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("宋体"));
		MemDC.SelectObject(&font);
		MemDC.SetBkMode(TRANSPARENT);
		tcItem.mask = TCIF_TEXT;
		GetItem(nTab,&tcItem);
		strItem.Format(_T("%s"),tcItem.pszText);
		MemDC.DrawText(strItem,strItem.GetLength(),&rectTemp,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		//CBrush      cbr;
		//cbr.CreateSolidBrush(RGB(255,76,0));
		//MemDC.FillRect(&rectTemp, &cbr);
	}

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);

}

BOOL CCTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bmp.GetSafeHandle() && m_nType)
	{
		CRect rect;
		GetClientRect(rect);

		CDC bgDC;
		bgDC.CreateCompatibleDC(NULL);
		bgDC.SelectObject(&m_bmp);

		BITMAP bmp;
		m_bmp.GetBitmap(&bmp);
		int nWidth = bmp.bmWidth;
		int nHeight = bmp.bmHeight;

		switch(m_nType)
		{
		case BITMAP_CENTER:
			CTabCtrl::OnEraseBkgnd(pDC);
			BITMAP bmp;
			m_bmp.GetBitmap (&bmp);
			pDC->BitBlt ((rect.Width() - bmp.bmWidth) / 2, (rect.Height() - bmp.bmHeight) / 2, bmp.bmWidth, bmp.bmHeight, &bgDC, 0, 0, SRCCOPY);
			return TRUE;
		case BITMAP_STRETCH:
			pDC->SetStretchBltMode(HALFTONE);
			pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &bgDC, 0, 0, nWidth, nHeight, SRCCOPY);
			return TRUE;
		default:
			CBrush bgBrush(&m_bmp);
			pDC->FillRect(rect,&bgBrush);
			return TRUE;
		}
	}else{
		CTabCtrl::OnEraseBkgnd(pDC);
	}

	return TRUE;
}

void CCTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int   nTab =   GetItemCount();
	CRect rcItem,rectTemp;
	while(nTab--) 
	{   
		GetItemRect(nTab,&rcItem);
		rectTemp=rcItem;
		rectTemp.left=rectTemp.right-m_nItempadding; // 留一个空隙做每个item的
		rectTemp.OffsetRect(0,-2);
		if (rectTemp.PtInRect(point))
		{
			return;
		}
	}

	CTabCtrl::OnLButtonDown(nFlags, point);
}
BOOL CCTabCtrl::SetBitmap(UINT uResource, int nType)
{
	m_nType = nType;
//	ASSERT(m_nType == BITMAP_TILE || m_nType == BITMAP_STRETCH || m_nType == BITMAP_CENTER);
	BOOL res = m_bmp.LoadBitmap(uResource);
	return res;
}

