// DlgDocument.cpp : implementation file
//

#include "stdafx.h"
#include "DlgDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDocument

IMPLEMENT_DYNCREATE(CDlgDocument, CDocument)

CDlgDocument::CDlgDocument()
{
}

BOOL CDlgDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CDlgDocument::~CDlgDocument()
{
}


BEGIN_MESSAGE_MAP(CDlgDocument, CDocument)
	//{{AFX_MSG_MAP(CDlgDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDocument diagnostics

#ifdef _DEBUG
void CDlgDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CDlgDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDlgDocument serialization

void CDlgDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDlgDocument commands
