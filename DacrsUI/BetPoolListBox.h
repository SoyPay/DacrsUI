#pragma once
#include "ButtonEx.h"
#include "StaticTrans.h"
#include <map>
using namespace std;



typedef struct _PoolBox_Data 
{
	int            nItem; //行数
	CStaticTrans   *pSta0; //按钮1
	CStaticTrans   * pSta1; //静态框1
	CStaticTrans   * pSta2; //静态框1
	CStaticTrans   * pSta3; //静态框1
	CButtonEx      * pBut2; //按钮2
	CString       pstr;
	CString       pstr1;
	_PoolBox_Data()
	{
		pSta0 = NULL ;
		pBut2 = NULL ;
		pSta1 = NULL ;
		pSta2 =  NULL ;
		pstr = _T("");
		pstr1 = _T("");
	}
} PoolBox_AppendData;

typedef map<int,PoolBox_AppendData*>button_betmap;

// CUseListBox

class CBetPoolListBox : public CListBox
{
	DECLARE_DYNAMIC(CBetPoolListBox)

public:
	CBetPoolListBox();
	virtual ~CBetPoolListBox();
public:

protected:
	DECLARE_MESSAGE_MAP()	
public:
	button_betmap m_mButton;
	UINT m_uID;
	void updateListBoxButtonPos();
	void InvalidateListBox();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	void InsertStr(int iIndex,HWND hMain);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	PoolBox_AppendData *GetAppendDataInfo(int iIndex);
	void    SetIndexInage(int iIndex , UINT nButImage  );
	void    SetotherIndexInage(int iIndex , UINT nButImage  );
	void    SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col);
	void    SetIndexString(int iIndex , CString strSta0 ,CString strSta1 ,CString strSta2 ,CString strBut2 ,CString strSta3,CString strSta4 );
	void    DeleteAllIndex();
	void    DeleteIndex(int iIndex);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLbnSelchange();
public:
	  CRect m_rectListBox;      // 列表项到绘制Rect
	  afx_msg void OnPaint();
	  virtual BOOL PreTranslateMessage(MSG* pMsg);
};


