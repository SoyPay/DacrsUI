#pragma once
#include "ButtonEx.h"
#include "StaticTrans.h"
#include <map>
using namespace std;



typedef struct _ListBox_Data 
{
	int         nItem; //����
	CStaticTrans * pSta0; //��ť1
	CButtonEx * pBut2; //��ť2
	CStaticTrans   * pSta1; //��̬��1
	CString       pstr;
	_ListBox_Data()
	{
		pSta0 = NULL ;
		pBut2 = NULL ;
		pSta1 = NULL ;
		pstr = _T("");
	}
} List_AppendData;

typedef map<int,List_AppendData*>button_map;

// CUseListBox

class CUseListBox : public CListBox
{
	DECLARE_DYNAMIC(CUseListBox)

public:
	CUseListBox();
	virtual ~CUseListBox();
public:

protected:
	DECLARE_MESSAGE_MAP()	
private:
	CStaticTrans   * m_pTitleSta1; //��̬��1
	CStaticTrans   * m_pTitleSta2; //��̬��2
public:
	button_map m_mButton;
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
	List_AppendData *GetAppendDataInfo(int iIndex);
	void    SetIndexInage(int iIndex ,  UINT nButImage1 , UINT nButImage2  );
	void    SetIndexBackCol(int iIndex ,  int nline ,COLORREF   col);
	void    SetIndexString(int iIndex , CString strSta0 ,CString strBut2 ,CString strSta1 ,CString strSta2 );
	void    CreateTitle();
};


