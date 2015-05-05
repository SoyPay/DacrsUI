// Language.cpp: implementation of the CLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Language.h"
//#include "dchglob.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLanguage g_language;
extern CString GetAppPath();

CLanguage::CLanguage()
{

}

CLanguage::~CLanguage()
{
	DeleteAllItem();
}

void CLanguage::DeleteAllItem()
{
	POSITION psTranslateItemGroup = GetStartPosition();
	while(psTranslateItemGroup)
	{
		CString sKey;
		TranslateItemGroup* pTranslateItemGroup;
		GetNextAssoc(psTranslateItemGroup,sKey,pTranslateItemGroup);
		delete pTranslateItemGroup;
		pTranslateItemGroup= NULL;
	}
	RemoveAll();
}
//BOOL CLanguage::ReadLanguageFromID(UINT nID)
//{
//	g_language.DeleteAllItem();
//	nID -= ID_LANGUAGE_START;
//	if(nID<0)return FALSE;
//	int nTotalLanguage = g_language.m_Languages.GetSize();
//	if(nID>=nTotalLanguage)return FALSE;
//	CString sFileName = g_language.m_Languages.GetAt(nID);
//	sFileName=GetOsGlob().GetAppPath()+_T("language\\") + sFileName+_T(".txt");
//	return ReadLanguageFromFile(sFileName);
//}
CString CLanguage::TranLanguage(UINT nID,CString sText)
{
	return TranLanguage(TranIdToStr(nID),sText);
}

CString CLanguage::TranLanguage(CString sClassName,long nResID)
{
	CString s;
	s.LoadString(nResID);
	return TranLanguage(sClassName,s);
}

CString CLanguage::TranLanguage(CString sClassName,CString sText)
{
	TranslateItemGroup* pTranslateItemGroup = NULL;
	g_language.Lookup(sClassName,pTranslateItemGroup);
	if(pTranslateItemGroup == NULL)return sText;
	CString sValue = (*pTranslateItemGroup)[sText];
	if(!sValue.IsEmpty())
		sText = sValue;
	return sText;
}

void CLanguage::TranslateDialogLanguage(CWnd *pWnd,long nID)
{
	TranslateDialogLanguage(pWnd,TranIdToStr(nID));
}
void CLanguage::TranslateDialogLanguage(CWnd *pWnd,CString sClassName)
{
	if(sClassName.IsEmpty())return;
	if(pWnd == NULL)return;
	CString sText;
	pWnd->GetWindowText(sText);
	sText = CLanguage::TranLanguage(sClassName,sText);
	pWnd->SetWindowText(sText);
	pWnd = pWnd->GetWindow(GW_CHILD);
	while (pWnd) {
		pWnd->GetWindowText(sText);
		sText = TranLanguage(sClassName,sText);
		pWnd->SetWindowText(sText);
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}
BOOL CLanguage::ReadLanguageFromResource(UINT uiSourceId)
{
	HINSTANCE hInst = AfxGetResourceHandle(); 
	HRSRC hRsrc = ::FindResource(hInst,MAKEINTRESOURCE(uiSourceId),_T("LANGUAGE"));
	DWORD len = SizeofResource(hInst, hRsrc); 
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc); 
	CMemFile file(lpRsrc, len); 
	CArchive ar(&file,CArchive::load);
	ReadLanguageFromArchive(ar);
	ar.Close();
	file.Close();
	return TRUE;
}

BOOL CLanguage::ReadLanguageFromArchive(CArchive& ar)
{
	CString str;
	TranslateItemGroup* pTranslateItemGroup = NULL;
	while(ar.ReadString(str))
	{
		str.TrimRight();
		if(str.IsEmpty())continue;
		if(str.GetAt(0) == ';')continue;
		if(str.GetAt(0) == '//')continue;
		if(str.GetAt(0) == '\\\\')continue;
		if(str.GetAt(0) == '\'')continue;
		int nStrLen = str.GetLength();
		if(nStrLen<4)continue;
		if(str.GetAt(0) == '[' && str.GetAt(nStrLen-1) == ']')
		{
			str = str.Left(nStrLen-1);
			str = str.Right(nStrLen-2);
			pTranslateItemGroup = new TranslateItemGroup;
			g_language.SetAt(str,pTranslateItemGroup);
			continue;
		}
		if(pTranslateItemGroup == NULL)continue;
		str.Replace(_T("\\t"),_T("\t"));
		str.Replace(_T("\\n"),_T("\n"));
		int nPos = str.Find(_T("="));
		if(nPos<0)continue;
		if(nPos>=nStrLen)continue;
		nStrLen = str.GetLength();
		CString sKey = str.Left(nPos);
		CString sValue = str.Right(nStrLen - nPos-1);
		(*pTranslateItemGroup)[sKey]=sValue;
	}
	return TRUE;
}
BOOL CLanguage::ReadLanguageFromFile(CString sFileName)
{
	g_language.DeleteAllItem();
	CFile fs;
	if(!fs.Open(sFileName,CFile::modeRead,NULL))return FALSE;
	CArchive ar(&fs,CArchive::load);
	ReadLanguageFromArchive(ar);
	ar.Close();
	fs.Close();
	return TRUE;
}

void CLanguage::FingLanguageFile()
{
	g_language.m_Languages.RemoveAll();
	CFileFind finder;
	CString sPath=GetAppPath()+_T("language");
	sPath+=_T("\\*.txt");
	BOOL bResult = finder.FindFile(sPath);
	while (bResult)
	{
		bResult = finder.FindNextFile();
		CString sFileName = finder.GetFileName();
		if(sFileName.GetLength()<5)continue;
		sFileName=sFileName.Left(sFileName.GetLength()-4);
		g_language.m_Languages.Add(sFileName);
	}
	qsort(g_language.m_Languages.GetData(),g_language.m_Languages.GetSize(),sizeof(CString),CompareString);
}

int CLanguage::CompareString( const void *arg1, const void *arg2 )
{
	return _wcsicmp( * ( wchar_t** ) arg1, * ( wchar_t** ) arg2 );
}


CString CLanguage::TranIdToStr(UINT nID)
{
	//switch(nID) {
	//case IDD_DLG_OPTION: return L"IDD_DLG_OPTION";
	//}
	return _T("");
}
