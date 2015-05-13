#include "stdafx.h"
#include "ChangeDPI.h"

#define DEFAULT_DPI 96.0


static int m_nDpix;
static int m_nDpiy;


void SetDPI(int x, int y)
{
	m_nDpix = x;
	m_nDpiy = y;
}
int GetDpix()
{
	return m_nDpix;
}
int GetDpiy()
{
	return m_nDpiy;
}
	
void ReSetWindow(CWnd &wnd)
{
	CRect rect;
	wnd.GetWindowRect(rect);
	wnd.GetParent()->ScreenToClient(rect);
	rect.left = (int)(rect.left*DEFAULT_DPI/m_nDpix);
	rect.right = (int)(rect.right*DEFAULT_DPI/m_nDpix);
	rect.top = (int)(rect.top*DEFAULT_DPI/m_nDpiy);
	rect.bottom = (int)(rect.bottom*DEFAULT_DPI/m_nDpiy);
	rect.OffsetRect(CPoint((int)((m_nDpix-DEFAULT_DPI)/2), (int)((m_nDpiy-DEFAULT_DPI)/2)));

	wnd.MoveWindow(rect);
}