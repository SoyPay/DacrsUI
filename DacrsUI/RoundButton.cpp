// RoundButton.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "RoundButton.h"


// CRoundButton

//IMPLEMENT_DYNAMIC(CRoundButton, CBitmapButton)

CRoundButton::CRoundButton()
{
	m_bMoving=FALSE;
}

CRoundButton::~CRoundButton()
{
	m_rgn.DeleteObject();
}


BEGIN_MESSAGE_MAP(CRoundButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()	
END_MESSAGE_MAP()



// CRoundButton 消息处理程序
void CRoundButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();

	BITMAP bm;
	m_bitmap.GetObject(sizeof(bm), &bm);
	SetWindowPos(NULL,0,0,bm.bmWidth,bm.bmHeight,SWP_NOMOVE);
	m_rgn.DeleteObject();
	SetWindowRgn(NULL,FALSE);
	//m_rgn.Attach(CreateRgn(&m_bitmap,RGB(0,0,200),RGB(0,0,55)));
	m_rgn.Attach(CreateRgn(&m_bitmap,RGB(0,0,255),RGB(0,0,0)));
	
	SetWindowRgn( m_rgn ,TRUE );

	ModifyStyle(0, BS_OWNERDRAW);

}


void CRoundButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);

	CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect = lpDrawItemStruct->rcItem;
	UINT state = lpDrawItemStruct->itemState;
	UINT nStyle = GetStyle();
	UINT action=lpDrawItemStruct->itemAction;
	//ACE("action=%d,state=%d\n",action,state);

	//int nSavedDC = pDC->SaveDC();
	//pDC->SelectStockObject(NULL_BRUSH);
	//pDC->FillSolidRect(rect,::GetSysColor(COLOR_BTNFACE));

	CBitmap* pBitmap;
	if((m_bMoving==TRUE) && !(state & ODS_DISABLED) && m_bitmapFocus.m_hObject != NULL)//m_bitmapDisabled.m_hObject != NULL)
	{
		state=	ODS_FOCUS;
		pBitmap = &m_bitmapFocus;   // third image for focused
	}
	else
	{
		// use the main bitmap for up, the selected bitmap for down
		pBitmap = &m_bitmap;
		if ((state & ODS_SELECTED) && m_bitmapSel.m_hObject != NULL)
			pBitmap = &m_bitmapSel;
		//else if ((state & ODS_FOCUS) && m_bitmapFocus.m_hObject != NULL)
		//	pBitmap = &m_bitmapFocus;   // third image for focused
		else if ((state & ODS_DISABLED) && m_bitmapDisabled.m_hObject != NULL)
			pBitmap = &m_bitmapDisabled;   // last image for disabled

		//if ((state & ODS_HOTLIGHT) && m_bitmapSel.m_hObject != NULL)
		//{
		//	MessageBox("ODS_HOTLIGHT");
		//	pBitmap = &m_bitmapDisabled;   // last image for disabled
		//}
	}
	BITMAP bm;
	pBitmap->GetObject(sizeof(bm), &bm);

	CDC memdc;
	if(!memdc.CreateCompatibleDC(pDC))return;
	CBitmap *pp= memdc.SelectObject(pBitmap);
	int i;
	//i=::SelectClipRgn(pDC->GetSafeHdc(),rgn);
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	i=GetWindowRgn(hRgn);
	if(i==ERROR||i==NULLREGION)
	{
		DWORD dw=::GetLastError();
		TRACE("CRoundButton::DrawItem() GetLastError=%ld\n",dw);
		return;
	}

	::SelectClipRgn(pDC -> GetSafeHdc(), hRgn);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memdc, 0, 0, SRCCOPY);
	::SelectClipRgn(pDC -> GetSafeHdc(), NULL);
	memdc.SelectObject(pp);
	DeleteObject(hRgn);
	//::SelectClipRgn(pDC->GetSafeHdc(),NULL);
	//pDC->RestoreDC(nSavedDC);
}

void CRoundButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(!(nFlags&MK_LBUTTON))
	{	
		CRect rc;
		GetClientRect(&rc);
		if(!rc.PtInRect(point))	// The mouse cursor is no longer above this button
		{
			if(m_bMoving==TRUE)
			{
				m_bMoving=FALSE;
				ReleaseCapture();
				Invalidate();
				UpdateWindow();
			}
			
		}
		else
		{
			if(m_bMoving==FALSE)
			{
				m_bMoving=TRUE;	
				SetCapture();
				Invalidate();
				UpdateWindow();
			}
			
		}
	}
	CButton::OnMouseMove(nFlags, point);
}

HRGN CreateRgn(CBitmap* pBitmap, COLORREF cTransparentColor, COLORREF cTolerance)
{
	return BitmapToRegion((HBITMAP)(*pBitmap), cTransparentColor, cTolerance);
}

HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor, COLORREF cTolerance)
{
	HRGN hRgn = NULL;

	if (!hBmp) return NULL;

	// Create a memory DC inside which we will scan the bitmap content
	HDC hMemDC = CreateCompatibleDC(NULL);
	if (hMemDC)
	{
		// Get bitmap size
		BITMAP bm;
		GetObject(hBmp, sizeof(bm), &bm);

		// Create a 32 bits depth bitmap and select it into the memory DC 
		BITMAPINFOHEADER RGB32BITSBITMAPINFO = {	
				sizeof(BITMAPINFOHEADER),	// biSize 
				bm.bmWidth,					// biWidth; 
				bm.bmHeight,				// biHeight; 
				1,							// biPlanes; 
				32,							// biBitCount 
				BI_RGB,						// biCompression; 
				0,							// biSizeImage; 
				0,							// biXPelsPerMeter; 
				0,							// biYPelsPerMeter; 
				0,							// biClrUsed; 
				0							// biClrImportant; 
		};
		VOID * pbits32; 
		HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
		if (hbm32)
		{
			HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

			HDC hDC = CreateCompatibleDC(hMemDC);
			if (hDC)
			{
				BITMAP bm32;
				GetObject(hbm32, sizeof(bm32), &bm32);
				while (bm32.bmWidthBytes % 4)
					bm32.bmWidthBytes++;

				HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
				BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);

				#define ALLOC_UNIT	100
				DWORD maxRects = ALLOC_UNIT;
				HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
				RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
				pData->rdh.dwSize = sizeof(RGNDATAHEADER);
				pData->rdh.iType = RDH_RECTANGLES;
				pData->rdh.nCount = pData->rdh.nRgnSize = 0;
				SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

				BYTE lr = GetRValue(cTransparentColor);
				BYTE lg = GetGValue(cTransparentColor);
				BYTE lb = GetBValue(cTransparentColor);
				BYTE hr = min(0xff, lr + GetRValue(cTolerance));
				BYTE hg = min(0xff, lg + GetGValue(cTolerance));
				BYTE hb = min(0xff, lb + GetBValue(cTolerance));

				BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
				for (int y = 0; y < bm.bmHeight; y++)
				{
					for (int x = 0; x < bm.bmWidth; x++)
					{
						int x0 = x;
						LONG *p = (LONG *)p32 + x;
						while (x < bm.bmWidth)
						{
							BYTE b = GetRValue(*p);
							if (b >= lr && b <= hr)
							{
								b = GetGValue(*p);
								if (b >= lg && b <= hg)
								{
									b = GetBValue(*p);
									if (b >= lb && b <= hb)
										break;
								}
							}
							p++;
							x++;
						}

						if (x > x0)
						{
							if (pData->rdh.nCount >= maxRects)
							{
								GlobalUnlock(hData);
								maxRects += ALLOC_UNIT;
								hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
								pData = (RGNDATA *)GlobalLock(hData);
							}
							RECT *pr = (RECT *)&pData->Buffer;
							SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
							if (x0 < pData->rdh.rcBound.left)
								pData->rdh.rcBound.left = x0;
							if (y < pData->rdh.rcBound.top)
								pData->rdh.rcBound.top = y;
							if (x > pData->rdh.rcBound.right)
								pData->rdh.rcBound.right = x;
							if (y+1 > pData->rdh.rcBound.bottom)
								pData->rdh.rcBound.bottom = y+1;
							pData->rdh.nCount++;

							if (pData->rdh.nCount == 2000)
							{
								HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
								if (hRgn)
								{
									CombineRgn(hRgn, hRgn, h, RGN_OR);
									DeleteObject(h);
								}
								else
									hRgn = h;
								pData->rdh.nCount = 0;
								SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
							}
						}
					}
					p32 -= bm32.bmWidthBytes;
				}
				HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
				if (hRgn)
				{
					CombineRgn(hRgn, hRgn, h, RGN_OR);
					DeleteObject(h);
				}
				else
					hRgn = h;

				SelectObject(hDC, holdBmp);
				DeleteDC(hDC);
				GlobalFree(hData);
			}
			DeleteObject(SelectObject(hMemDC, holdBmp));
		}
		DeleteDC(hMemDC);
	}
	return hRgn;
}

void CRoundButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//SetCapture();
	m_bMoving=FALSE;
	CButton::OnLButtonDown(nFlags, point);
}

void CRoundButton::ReloadBmp( UINT stat , int BitmapResId ) 
{	
	if( stat > 3 || stat < 0 ) {
		return ;
	}

	CBitmap * pbitmap[4] = { &m_bitmap , &m_bitmapSel , &m_bitmapFocus, &m_bitmapDisabled } ;
	if( NULL != pbitmap[stat] ) {
		HBITMAP m_pBmp = NULL ;
		HINSTANCE	hInstResource = NULL;	
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(BitmapResId), RT_BITMAP);
		if( NULL != hInstResource ) {
			m_pBmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(BitmapResId), IMAGE_BITMAP, 0, 0, 0);
		}
		pbitmap[stat]->DeleteObject() ;
		pbitmap[stat]->Attach(m_pBmp) ;
		RedrawWindow() ;
	}
}