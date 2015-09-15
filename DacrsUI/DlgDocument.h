#if !defined(AFX_DLGDOCUMENT_H__5D4DCBF9_6183_464E_9C61_AD0A062358EE__INCLUDED_)
#define AFX_DLGDOCUMENT_H__5D4DCBF9_6183_464E_9C61_AD0A062358EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDocument.h : header file
//
//#include "DlgView.h"

class CAddApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgDocument document

class CDlgDocument : public CDocument
{
	friend class CAddApp;
protected:
	CDlgDocument();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDlgDocument)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDlgDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDlgDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDOCUMENT_H__5D4DCBF9_6183_464E_9C61_AD0A062358EE__INCLUDED_)
