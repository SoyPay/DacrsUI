#if !defined(AFX_BMPBTN_H__1966AD87_09E9_4F38_8117_279674AF8BD2__INCLUDED_)
#define AFX_BMPBTN_H__1966AD87_09E9_4F38_8117_279674AF8BD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BmpBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBmpBtn window

class CBmpBtn : public CBitmapButton
{
// Construction
	CDC m_dc1,m_dc2,m_dc3;

public:
	CBmpBtn();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmpBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBitmaps(UINT nID1,UINT nID2,UINT nID3);
	virtual ~CBmpBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBmpBtn)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMPBTN_H__1966AD87_09E9_4F38_8117_279674AF8BD2__INCLUDED_)
