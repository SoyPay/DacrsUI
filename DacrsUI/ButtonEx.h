#pragma once
#include "ShadeButtonST.h"

// CButtonEx

class CButtonEx : public CShadeButtonST
{
	//DECLARE_DYNAMIC(CButtonEx)

public:
	CButtonEx();
	CButtonEx( int nItem, int nSubItem, CRect rect, HWND hParent );
	virtual ~CButtonEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
	int m_inItem;
	int m_inSubItem;
	CRect m_rect;
	HWND m_hParent;
	BOOL bEnable;
};


