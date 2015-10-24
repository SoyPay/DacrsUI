// ListCtrlCl.cpp : 实现文件
//

#include "stdafx.h"
#include "ListCtrlCl.h"

struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};
// CListCtrlCl

IMPLEMENT_DYNAMIC(CListCtrlCl, CListCtrl)

CListCtrlCl::CListCtrlCl()
: m_nRowHeight(0)
, m_fontHeight(12)
, m_fontWith(0)
{
	m_color = RGB(0,0,0);
	m_uID = 0;
	font.CreatePointFont(100,_T("宋体"));
}

CListCtrlCl::~CListCtrlCl()
{
	release();
}


BEGIN_MESSAGE_MAP(CListCtrlCl, CListCtrl)
	ON_WM_MEASUREITEM()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY(HDN_BEGINTRACKA, 0, &CListCtrlCl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, &CListCtrlCl::OnHdnBegintrack)
END_MESSAGE_MAP()



// CListCtrlCl 消息处理程序



void CListCtrlCl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0,LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}

void CListCtrlCl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	// TODO:  添加您的代码以绘制指定项
	TCHAR lpBuffer[256];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rtClient;
	GetClientRect(&rtClient);
	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}

		CRect rcItem;   
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;   

		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CRect rcTemp;
		rcTemp = rcItem;

		if (nCol==0)
		{
			rcTemp.left -=2;
		}

		DWORD extstyle;
		extstyle = GetExtendedStyle();
		if ( (lpDrawItemStruct->itemState & ODS_SELECTED)&&( (GetStyle() & LVS_TYPEMASK) == LVS_REPORT&& (extstyle & (LVS_EX_GRIDLINES)) == LVS_EX_GRIDLINES) )
		{
			pDC->FillSolidRect(&rcTemp, GetSysColor(COLOR_HIGHLIGHT)) ;
			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
		}
		else
		{
			COLORREF color;
			color = GetBkColor();
			pDC->FillSolidRect(rcTemp,color);

			if (FindColColor(nCol,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			if (FindItemColor(nCol,lpDrawItemStruct->itemID,color))
			{
				pDC->FillSolidRect(rcTemp,color);
			}
			
			//pDC->SetTextColor(m_color);
		}

		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

		UINT   uFormat    = DT_CENTER ;
		if (m_Header.m_Format[nCol]=='0')
		{
			uFormat = DT_LEFT;
		}
		else if (m_Header.m_Format[nCol]=='1')
		{
			uFormat = DT_CENTER;
		}
		else if (m_Header.m_Format[nCol]=='2')
		{
			uFormat = DT_RIGHT;
		}
		TEXTMETRIC metric;
		pDC->GetTextMetrics(&metric);
		int ofst;
		ofst = rcItem.Height() - metric.tmHeight;
		rcItem.OffsetRect(0,ofst/2);
		pDC->SetTextColor(m_color);
		COLORREF color;
		if (FindColTextColor(nCol,color))
		{
			pDC->SetTextColor(color);
		}
		if (FindItemTextColor(nCol,lpDrawItemStruct->itemID,color))
		{
			pDC->SetTextColor(color);
		}
		CFont nFont ,* nOldFont; 
		nFont.CreateFont(m_fontHeight,m_fontWith,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("宋体"));//创建字体 
		nOldFont = pDC->SelectObject(&nFont);
		DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), 
			&rcItem, uFormat) ;

		pDC->SelectStockObject(SYSTEM_FONT) ;

		CPen pen(PS_DASH ,2,RGB(168,168,168));//可以设置画笔粗细
		pDC->SelectObject(&pen);
		for (int i = rtClient.left;i <=rtClient.right;)
		{
			pDC->MoveTo(i,rtClient.bottom);
			pDC->LineTo(i+1,rtClient.bottom);
			i +=3;
		}
	}

}

void CListCtrlCl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
void CListCtrlCl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}
int CListCtrlCl::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat /* = LVCFMT_LEFT */, int nWidth /* = -1 */, int nSubItem /* = -1 */)
{
	m_Header.m_HChar.Add(lpszColumnHeading);
	if (nFormat==LVCFMT_LEFT)
	{
		m_Header.m_Format = m_Header.m_Format + "0";
	}
	else if (nFormat==LVCFMT_CENTER)
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	else if (nFormat==LVCFMT_RIGHT)
	{
		m_Header.m_Format = m_Header.m_Format + "2";
	}
	else
	{
		m_Header.m_Format = m_Header.m_Format + "1";
	}
	return CListCtrl::InsertColumn(nCol,lpszColumnHeading,nFormat,nWidth,nSubItem);
}
// Gradient - 渐变系数，立体背景用,不用渐变设为0
void CListCtrlCl::SetHeaderBKColor(int R, int G, int B, int Gradient) //设置表头背景色
{
	m_Header.m_R = R;
	m_Header.m_G = G;
	m_Header.m_B = B;
	m_Header.m_Gradient = Gradient;
}

// 设置表头高度
void CListCtrlCl::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.m_Height = Height;
}
bool CListCtrlCl::FindColColor(int col,COLORREF &color) //查找列颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListCol.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListCol.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemColor(int col,int row,COLORREF &color) //查找颜色
{
	int flag = 0;
	for (POSITION pos = m_ptrListItem.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ptrListItem.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
void CListCtrlCl::SetColColor(int col,COLORREF color) //设置列颜色
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_ptrListCol.AddTail(pColor);
}
void CListCtrlCl::SetItemColor(int col,int row,COLORREF color) //设置格子颜色
{
	stColor *pColor  = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ptrListItem.AddTail(pColor);
}
void CListCtrlCl::SetRowHeigt(int nHeight) //高置行高
{
	m_nRowHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}
void CListCtrlCl::SetHeaderFontHW(int nHeight,int nWith) //设置头部字体宽和高
{
	m_Header.m_fontHeight = nHeight;
	m_Header.m_fontWith = nWith;
}
void CListCtrlCl::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.m_color = color;
}
BOOL CListCtrlCl::SetTextColor(COLORREF cr)  //设置字体颜色
{
	m_color = cr;
	return TRUE;
}
void CListCtrlCl::SetFontHW(int nHeight,int nWith) //设置字体高和宽
{
	m_fontHeight = nHeight;
	m_fontWith = nWith;
}
void CListCtrlCl::SetColTextColor(int col,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->rgb = color;
	m_colTextColor.AddTail(pColor);
}
bool CListCtrlCl::FindColTextColor(int col,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_colTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_colTextColor.GetNext(pos);
		if (pColor->nCol==col)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
bool CListCtrlCl::FindItemTextColor(int col,int row,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_ItemTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nCol==col&&pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
void CListCtrlCl::SetItemTextColor(int col,int row,COLORREF color)
{
	stColor *pColor = new stColor;
	pColor->nCol = col;
	pColor->nRow = row;
	pColor->rgb = color;
	m_ItemTextColor.AddTail(pColor);
}

void CListCtrlCl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CListCtrl::OnPaint()
	const MSG *msg = GetCurrentMessage();
	DefWindowProc( msg->message, msg->wParam, msg->lParam ); 
	DWORD extstyle;
	extstyle = GetExtendedStyle();
	int style= (extstyle & (LVS_EX_GRIDLINES));
	if( (GetStyle() & LVS_TYPEMASK) == LVS_REPORT  && (style == LVS_EX_GRIDLINES)){
		CClientDC dc(this );
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		RECT rect;
		pHeader->GetClientRect( &rect );
		int top = rect.bottom;

		GetClientRect( &rect );

		int borderx = 0 - GetScrollPos( SB_HORZ );
		CPen listSepPen(PS_SOLID, 1, RGB(0, 0, 0)); 
		CPen *pOldPen = dc.SelectObject(&listSepPen);

		for( int i = 0; i < nColumnCount; i++ ){
			  borderx += GetColumnWidth( i );
			  if( borderx >= rect.right ) break;

			  dc.MoveTo( borderx, top);
			  dc.LineTo( borderx, rect.bottom );
		}
		if( !GetItemRect( 0, &rect, LVIR_BOUNDS ))
			return;

		int height = rect.bottom - rect.top;

		GetClientRect( &rect );
		int width = rect.right;

		for(int i = 1; i <= GetCountPerPage(); i++ ) {
			dc.MoveTo( 0, top + height*i);
			dc.LineTo( width, top + height*i );
		}
		dc.SelectObject(pOldPen);
	}
}


BOOL CListCtrlCl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TRACE("X=%d, Y=%d\n", pt.x, pt.y);
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CListCtrlCl::createItemButton( int nItem, int nSubItem, HWND hMain,LPCTSTR lpszCaption ,void * pData,HBITMAP nBitmapIn, COLORREF crTransColorIn, HBITMAP nBitmapOut, COLORREF crTransColorOut)
{
	CRect rect;

	BOOL ret = GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect);
	BITMAP   bm; 
	CDC   dcMem; 
	::GetObject(nBitmapIn,sizeof(BITMAP),   (LPVOID)&bm); 
	int with = bm.bmWidth;
	int height = bm.bmHeight;

	if (nSubItem == 0)
	{
		rect.left = 0;
		rect.right = with;
		rect.bottom = rect.top+ height;
	}else{
		rect.right =rect.left+ with;
		rect.bottom = rect.top+ height;
	}
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | BS_MULTILINE;
	CButtonCtrl *pButton = new CButtonCtrl(nItem,nSubItem,rect,hMain,pData);
	m_uID++;

	pButton->Create(lpszCaption,dwStyle, rect, this, m_uID);
	pButton->SetFont(&font);

	pButton->SetBitmaps( nBitmapIn , crTransColorIn, nBitmapOut , crTransColorOut );
	pButton->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	pButton->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
	pButton->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
	pButton->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));
	pButton->DrawTransparent(TRUE);

	//delete pButton;
	m_mButton.insert( make_pair( nSubItem, pButton ) ); //单行横向添加用

	return;
}
// CListCtrlEx 消息处理程序
void CListCtrlCl::createItemButton( int nItem, int nSubItem, HWND hMain,LPCTSTR lpszCaption ,void * pData,int nBitmapIn, COLORREF crTransColorIn, int nBitmapOut, COLORREF crTransColorOut)
{
	CRect rect;

	BOOL ret = GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect);
	rect.bottom = rect.top +m_nRowHeight;
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | BS_MULTILINE;
	CButtonCtrl *pButton = new CButtonCtrl(nItem,nSubItem,rect,hMain,pData);
	m_uID++;

	pButton->Create(lpszCaption,dwStyle, rect, this, m_uID);
	pButton->SetFont(&font);

	pButton->SetBitmaps( nBitmapIn , crTransColorIn, nBitmapOut , crTransColorOut );
	pButton->SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	pButton->SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(41, 57, 85));
	pButton->SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(41, 57, 85));
	pButton->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(41, 57, 85));
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(41, 57, 85));

	m_mButton.insert( make_pair( nSubItem, pButton ) ); //单行横向添加用


	return;
}

void CListCtrlCl::release()
{
	Button_map::iterator iter = m_mButton.begin();
	while ( iter != m_mButton.end() )
	{
		CButtonCtrl* item = iter->second;
		if (item != NULL)
		{
			delete item;
			item = NULL;
		}
		iter++;
	}
	m_mButton.clear();
}
void  CListCtrlCl::updateListCtrlButtonPos()
{
	Button_map::iterator iter = m_mButton.begin();
	Button_map::iterator itrEnd = m_mButton.end();
	
		CRect rect;
	GetClientRect(rect);
	LONG width = rect.right;
	//调整横向的
	int posx = GetScrollPos(SB_HORZ);//取得水平滚动条的位置
	for (;iter != itrEnd;++iter)
		{
			iter->second->ShowWindow( SW_HIDE );

			rect = iter->second->m_rect;
			rect.left -= posx;
			rect.right -= posx;
			if (rect.right > 0)
			{
				if (rect.left > width)
					{
							//其他的都超出了显示范围
								break;
					}
				iter->second->MoveWindow( &rect );
				iter->second->ShowWindow( SW_SHOW );
			}
				              
			/*if( iLine < iTopIndex )
				{
					iterUp->second->ShowWindow( SW_HIDE );
				}*/
			}
	return;
}

CButtonCtrl*  CListCtrlCl::GetBtutton(int index)
{
	Button_map::iterator it = m_mButton.find(index) ;
	if (it == m_mButton.end())
	{
		return NULL;
	}

	return it->second;

}
void CListCtrlCl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
	return;
}