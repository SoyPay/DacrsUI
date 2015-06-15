#pragma once
#include "ButtonEx.h"
#include "StaticTrans.h"
#include <map>
using namespace std;



typedef struct _RedListBox_Data 
{
	int         nItem; //ÐÐÊý
	CStaticTrans * pSta0; //°´Å¥1
	CStaticTrans   * pSta1; //¾²Ì¬¿ò1
	CStaticTrans   * pSta2; //¾²Ì¬¿ò1
	CStaticTrans   * pSta3; //¾²Ì¬¿ò1
	CStaticTrans   * pSta4; //¾²Ì¬¿ò1
	CStaticTrans   * pSta5; //¾²Ì¬¿ò1

	_RedListBox_Data()
	{
		pSta0 = NULL ;
		pSta1 = NULL ;
		pSta2 = NULL ;
		pSta3 = NULL ;
		pSta4 = NULL ;
		pSta5 = NULL ;
	}
} List_RedAppendData;

typedef map<int,List_RedAppendData*>Redbutton_map;

// CUseListBox

class CRedPacketPoolListBox : public CListBox
{
	DECLARE_DYNAMIC(CRedPacketPoolListBox)

public:
	CRedPacketPoolListBox();
	virtual ~CRedPacketPoolListBox();
public:

protected:
	DECLARE_MESSAGE_MAP()	
public:
	Redbutton_map m_mButton;
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
	List_RedAppendData *GetAppendDataInfo(int iIndex);
	void    SetIndexInage(int iIndex , UINT nButImage  );
	void    SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col);
	void   SetIndexString(int iIndex , CString strSta0 ,CString strSta1 ,CString strSta2 ,CString strSta3 ,CString strSta4,CString strSta5);
	void    DeleteAllIndex();
	void    DeleteIndex(int iIndex);
public:
	  CRect m_rectListBox;      // ÁÐ±íÏîµ½»æÖÆRect
	  afx_msg void OnPaint();
	  virtual BOOL PreTranslateMessage(MSG* pMsg);
};


