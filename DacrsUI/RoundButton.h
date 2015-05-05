#pragma once


// CRoundButton

class CRoundButton : public CBitmapButton
{
	//DECLARE_DYNAMIC(CRoundButton)

public:
	CRoundButton();
	virtual ~CRoundButton();

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();

public:
	void ReloadBmp( UINT stat , int resid ) ;

public:
	CRgn   m_rgn;
	BOOL   m_bMoving;

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	

	DECLARE_MESSAGE_MAP()
};

HRGN CreateRgn(CBitmap* pBitmap, COLORREF cTransparentColor=0, COLORREF cTolerance=0x101010);

HRGN BitmapToRegion (HBITMAP hBmp, COLORREF cTransparentColor=0, COLORREF cTolerance=0x101010);/////////////////////////////////////////////////////////////////////////////



