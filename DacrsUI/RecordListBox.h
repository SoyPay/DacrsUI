#pragma once
#include "StaticTrans.h"
#include <map>
using namespace std;



typedef struct _ReListBox_Data 
{
	int         nItem; //ÐÐÊý
	CStaticTrans   * pSta0; //¾²Ì¬¿ò1
	CStaticTrans   * pSta1; //¾²Ì¬¿ò1
	CStaticTrans   * pSta2; //¾²Ì¬¿ò1
	CStaticTrans   * pSta3; //¾²Ì¬¿ò1
	CStaticTrans   * pSta4; //¾²Ì¬¿ò1
	CStaticTrans   * pSta5; //¾²Ì¬¿ò1
	CStaticTrans   * pSta6; //¾²Ì¬¿ò1
	_ReListBox_Data()
	{
		pSta0 = NULL ;
		pSta1 = NULL ;
		pSta2 = NULL ;
		pSta3 = NULL ;
		pSta4 = NULL ;
		pSta5 = NULL ;
		pSta6= NULL ;
	}
} List_ReAppendData;

typedef map<int,List_ReAppendData*>ReButton_map;
// CRecordListBox

class CRecordListBox : public CListBox
{
	DECLARE_DYNAMIC(CRecordListBox)

public:
	CRecordListBox();
	virtual ~CRecordListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	ReButton_map m_mButton;
	UINT m_uID;
	void updateListBoxButtonPos();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	void InsertStr(int iIndex,HWND hMain);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	List_ReAppendData *GetAppendDataInfo(int iIndex);
	void    SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col);
	void    SetIndexString(int iIndex , CString strBut1 ,CString strSta1 ,CString strSta2 ,CString strSta3 ,CString strSta4,CString strSta5 ,CString strSta6);
	void    DeleteAllIndex();
	void    DeleteIndex(int iIndex);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


