// Language.h: interface for the CLanguage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LANGUAGE_H__04D56CA9_3EF2_43A3_A1D3_8CFD85C66263__INCLUDED_)
#define AFX_LANGUAGE_H__04D56CA9_3EF2_43A3_A1D3_8CFD85C66263__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "C:\database\base.cpp\ArchiveEx.h"
//#include "C:\database\base.cpp\stdiofileex.h"
typedef CMap<CString,LPCTSTR,CString,LPCTSTR> TranslateItemGroup;

class CLanguage  : public CMap<CString,LPCTSTR,TranslateItemGroup*,TranslateItemGroup*>
{
public:
	static int CompareString( const void *arg1, const void *arg2 );
	static CString TranLanguage(UINT nID,CString sText);
	static CString TranLanguage(CString sClassName,long nResID);
	static CString TranLanguage(CString sClassName,CString sText);
	static void TranslateDialogLanguage(CWnd *pWnd,CString sClassName);
	static void TranslateDialogLanguage(CWnd *pWnd,long nID);
	static void FingLanguageFile();
	static BOOL ReadLanguageFromFile(CString sFileName);
	static BOOL ReadLanguageFromArchive(CArchive& ar);
	static BOOL ReadLanguageFromResource(UINT uiSourceId);
	static CString TranIdToStr(UINT nID);		
	void DeleteAllItem();
	CLanguage();
	virtual ~CLanguage();
	CStringArray m_Languages;
};

#endif // !defined(AFX_LANGUAGE_H__04D56CA9_3EF2_43A3_A1D3_8CFD85C66263__INCLUDED_)
