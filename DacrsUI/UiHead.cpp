#include "StdAfx.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <assert.h>
#include "UiHead.h"
using namespace std;

UINT UiFun::MallocP( void * * p , UINT size )
{
	if( 0 == size ) {
		return 0 ;
	} else {
		*p = (BYTE*)malloc( size*sizeof(BYTE) ) ;
		ASSERT( NULL != *p) ;
		if( NULL != p ) {
			memset( *p , 0 , size*sizeof(BYTE) ) ;
		} else {
			return 0 ;
		}
		return _msize( *p ) ;
	}
}
void UiFun::ReleaseP( void * p ) 
{
	if( NULL == p ) {
		return  ;	
	} else {
		free( p ) ;
		p = NULL ;
	}
}
CString UiFun::Rnd32()
{
	CString r = _T("");
	int i;
	srand((int)time(NULL));
	for(i=0;i<32;i++)
	{
		r = r + char(rand()%10+'0');
	}
	return r;
}
SYSTEMTIME UiFun::Time_tToSystemTime(time_t t)
{
	if (t == -1)
	{
		t = 0;
	}
	tm temptm = *localtime(&t);
	SYSTEMTIME st = {1900 + temptm.tm_year, 
		1 + temptm.tm_mon, 
		temptm.tm_wday, 
		temptm.tm_mday, 
		temptm.tm_hour, 
		temptm.tm_min, 
		temptm.tm_sec, 
		0};
	return st;
}
time_t UiFun::SystemTimeToTimet(const SYSTEMTIME& st) 
{
	tm temptm = {st.wSecond, 
		st.wMinute, 
		st.wHour, 
		st.wDay, 
		st.wMonth - 1, 
		st.wYear - 1900, 
		st.wDayOfWeek, 
		0, 
		0};
	return mktime(&temptm);
}
int UiFun::MessageBoxEx(CString strText , CString strCaption , UINT uType)
{
	CMessageBoxEx message(strText ,strCaption ,uType );
	return message.DoModal();
}
CString UiFun::UI_LoadString( CString secID , CString szID , UINT language ) 
{
	CString szValue;
	DWORD dwSize = 1000;
	CString szLanguagePath ;

	char strPath[MAX_PATH] ;
	GetModuleFileName( AfxGetApp()->m_hInstance , strPath , MAX_PATH );

	CString str=strPath;
	int i=str.ReverseFind('\\');
	CString path ;
	path = str.Left(i);

	switch( language ) 
	{
	case LANGUAGE_EN:
		{
			szLanguagePath.Format( "%s%s" , path , LANGUAGE_FILE_EN ) ;
		}
		break;
	case LANGUAGE_CN:
		{
			szLanguagePath.Format( "%s%s" , path , LANGUAGE_FILE_CN ) ;
		}
		break;
	}

	GetPrivateProfileString(secID,szID,"Not found",\
		szValue.GetBuffer(dwSize),dwSize , szLanguagePath );

	szValue.ReleaseBuffer();

	szValue.Replace("\\n","\n");

	return szValue;
}
int UiFun::RandNumber()
{
	srand((unsigned int)time(NULL));
    return rand()%6+1;
}
HBITMAP UiFun::GlobalBkgndBMP(UINT nIDBitmap)
{
	HINSTANCE	hInstResource = NULL;	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIDBitmap), RT_BITMAP);
	if( NULL != hInstResource )
		return (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nIDBitmap), IMAGE_BITMAP, 0, 0, 0);
	else
		TRACE("GlobalBkgndBMP : 资源无效...\n");
	return 0;
}
CString UiFun::MbcsToUtf8(const char *file)
{
	CString str;
	WCHAR   *pwchar=0;
	CHAR    *pchar=0;
	int len=0;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
	len=MultiByteToWideChar(codepage, 0, file, -1, NULL,0);
	pwchar=new WCHAR[len];
	if(pwchar!=0)
	{
		len = MultiByteToWideChar(codepage, 0, file, -1, pwchar, len);
		if( len!=0 ) 
		{
			len = WideCharToMultiByte(CP_UTF8, 0, pwchar, -1, 0, 0, 0, 0);
			pchar=new CHAR[len];
			if(pchar!=0)
			{
				len = WideCharToMultiByte(CP_UTF8, 0, pwchar, -1, pchar, len,0, 0);
				if(len!=0)                 
				{
					str=pchar;                    
				}
				delete pchar;
			}
			delete pwchar; 
		}
	}
	return str;
}

std::string& UiFun::trimleft(std::string &s) 
{
	if (s.empty()) 
	{
		return s;
	}

	s.erase(0,s.find_first_not_of(" "));
	return s;
}
std::string& UiFun::trimright(std::string &s) 
{
	if (s.empty()) 
	{
		return s;
	}

	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}
bool UiFun::IsCurrentAppId(string Curappid,string useAppid,string strShow)
{
	if (strcmp(Curappid.c_str(),useAppid.c_str()))
	{
		UiFun::MessageBoxEx(strShow.c_str() , _T("提示") ,MFB_OK|MFB_TIP );
		return false;
	}
	return true;
}
void UiFun::GetCellName(int nRow, int nCol, CString &strName)
{

	int nSeed = nCol;

	CString strRow;

	char cCell = 'A' + nCol - 1;



	strName.Format(_T("%c"), cCell);



	strRow.Format(_T( "%d "), nRow);

	strName += strRow;

}
int   UiFun::GetBmpId(int bmpCn,int bmpEn,int language)
{
	if (language == 1)
	{
		return bmpCn;
	}else if (language == 2)
	{
		return bmpEn;
	}
	return bmpCn;
}