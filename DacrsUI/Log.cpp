#include "stdafx.h"
#include "Log.h"
#include "JsonConfigHelp.h"
#include <set>
#include <map>
using namespace std;

CLogParamCfg logParamCfg;
static map<CString,DebugLogFile> g_DebugLogs;


int LogPrintStr(const char* category, const string &str) {

	if (!logParamCfg.bLogFlag)
		return 0;

	int ret = 0; // Returns total number of characters written

	map<CString, DebugLogFile>::iterator it;

	it = g_DebugLogs.find((NULL == category) ? _T("debug") : _T(category));
	if (it == g_DebugLogs.end()) {
		return 0;
		
	}

	DebugLogFile& log = it->second;
	log.m_clsMutex->Lock();
	// Debug print useful for profiling
	if (logParamCfg.bPrinttimestamps && log.m_newLine) {
		SYSTEMTIME st;
		CString strDate,strTime, strTimestamps;
		GetLocalTime(&st);
		strDate.Format("%04d-%02d-%02d ",st.wYear,st.wMonth,st.wDay);
		strTime.Format("%02d:%02d:%02d",st.wHour,st.wMinute,st.wSecond);
		strTimestamps = strDate + strTime;
		ret += fprintf(log.m_fileout, "%s ", strTimestamps.GetBuffer());
	}
	if (!str.empty() && str[str.size()-1] == '\n') {
		log.m_newLine = true;
	} else {
		log.m_newLine = false;
	}
	ret = fwrite(str.data(), 1, str.size(), log.m_fileout);
	log.m_clsMutex->Unlock();
	return ret;
}

string GetLogHead(int line, const char* file, const char* category) {
	
	CString strTemp = file;
	int nPos = strTemp.ReverseFind('\\');
	if(nPos > 0) {
		strTemp = strTemp.Right(strTemp.GetLength()-nPos-1);
	}

	if(logParamCfg.bPrintFileLine){
		return tfm::format("[%s:%d]%s: ", (LPSTR)(LPCTSTR)strTemp, line, category != NULL ? category : "");
	}
	return string("");
}

static void DebugPrintInit() {
	set<CString> logfiles(logParamCfg.vTag.begin(), logParamCfg.vTag.end());
	set<CString>::iterator iterLogFile = logfiles.begin();
	for (; iterLogFile!=logfiles.end(); ++iterLogFile) {
		FILE* fileout = NULL;
		CString strWorkDir(_T(""));
		CString pathDebug = GetCurrentWorkDir();
		CString fileName = *iterLogFile + ".log";
		pathDebug.AppendFormat(_T("\\%s"),fileName);
		//pathDebug +=  "\\" + fileName;
		//fileout = fopen(pathDebug.GetBuffer(pathDebug.GetLength()), "a");
		if( (fopen_s( &fileout, pathDebug.GetBuffer(pathDebug.GetLength()), "a" )) !=0 )
			printf( "The file %s was not opened\n", pathDebug);
		else
			printf( "The file %s was opened\n", pathDebug);

		if (fileout) {
			DebugLogFile& log = g_DebugLogs[*iterLogFile];
			setbuf(fileout, NULL); // unbuffered
			log.m_fileout = fileout;
			log.m_clsMutex = new CMutex(FALSE, NULL);
		}
	}
}

CString GetCurrentWorkDir() 
{

	char strPath[MAX_PATH] ;
	GetModuleFileName( AfxGetApp()->m_hInstance , strPath , MAX_PATH );
	CString str=strPath;
	int i=str.ReverseFind('\\');
	CString strFullPath;
	strFullPath = str.Left(i);
	return strFullPath;

	/*const DWORD dwPathLength = 1024;
	char pFileName[dwPathLength];
	int nPos = GetCurrentDirectory( dwPathLength, pFileName); 

	CString csFullPath(pFileName);  
	if( nPos < 0 ) 
		return CString(""); 
	else 
		return csFullPath; */
}

void InitLogCfg()
{
	CJsonConfigHelp::getInstance()->GetLogParamCfg(logParamCfg);
	DebugPrintInit();
}