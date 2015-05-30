#pragma once
#include "ButtonEx.h"
#include "StaticTrans.h"
#include <map>
using namespace std;



typedef struct _SendListBox_Data 
{
	int         nItem; //ÐÐÊý
	CStaticTrans   * pSta0; //¾²Ì¬¿ò1
	CStaticTrans   * pSta1; //¾²Ì¬¿ò1
	CStaticTrans   * pSta2; //¾²Ì¬¿ò1
	CStaticTrans   * pSta3; //¾²Ì¬¿ò1
	CStaticTrans   * pSta4; //¾²Ì¬¿ò1
	CStaticTrans   * pSta5; //¾²Ì¬¿ò1
	CStaticTrans   * pSta6; //¾²Ì¬¿ò1
	CStaticTrans   * pSta7; //¾²Ì¬¿ò1
	CStaticTrans   * pSta8; //¾²Ì¬¿ò1
	//CButtonEx   * pBut0; //¾²Ì¬¿ò1
	CString      pstr;
	_SendListBox_Data()
	{
		pSta0 = NULL ;
		pSta1 = NULL ;
		pSta2 = NULL ;
		pSta8 = NULL ;
		pSta3= NULL ;
		pSta4= NULL ;
		pSta5= NULL ;
		pSta6= NULL ;
		pSta7= NULL ;
		pstr = _T("");
	}
} List_SendAppendData;

typedef map<int,List_SendAppendData*>SendButton_map;
// CRecordListBox

class CSendListBox : public CListBox
{
	DECLARE_DYNAMIC(CSendListBox)

public:
	CSendListBox();
	virtual ~CSendListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	SendButton_map m_mButton;
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
	List_SendAppendData *GetAppendDataInfo(int iIndex);
	void    SetIndexInage(int iIndex ,  UINT nButImage );
	void    SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col);
	void    SetIndexString(int iIndex , CString strSta0 ,CString strSta1 ,CString strSta2 ,CString strBut3 ,CString strBut4 ,CString strBut5 ,CString strBut6 ,CString strBut7,CString strbutton,CString pstr =_T(""));
	void    DeleteAllIndex();
	void    DeleteIndex(int iIndex);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void RefushHorizontalScrollBar( void );
};


