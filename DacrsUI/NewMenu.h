#pragma once
#include "afxwin.h"
class CNewMenu :
	public CMenu
{
public:
	CNewMenu(void);
	~CNewMenu(void);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void ChangeMenuItem(CMenu *pMenu);
	void SetWith(int with);
private:
	int with;
	vector<CString *> m_relea;
};

