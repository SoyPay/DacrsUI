#pragma once


// C360TabCtrl

class CCTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CCTabCtrl)

public:
	CCTabCtrl();
	virtual ~CCTabCtrl();
    void	SetItemPadding(int nPadding=8);
public:
	enum {
		BITMAP_NONE = 0,// do not show bitmap
		BITMAP_TILE,// tile the bitmap to fill the dialog
		BITMAP_STRETCH,// stretch the bitmap so it fits to the dialog
		BITMAP_CENTER// center the bitmap inside the dialog
	};
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	BOOL SetBitmap(UINT uResource, int nType);
private:
	   int	   m_nItempadding;
protected:
	   int     m_nType;// see enum above
	   CBitmap m_bmp;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


