#pragma once
#include <string>
using namespace std;
#define LogPrint(tag, ...) LogTrace(tag, __LINE__, __FILE__, __VA_ARGS__);


struct DebugLogFile
{
	DebugLogFile():m_newLine(true), m_fileout(NULL), m_clsMutex(NULL) {}
	~DebugLogFile()
	{
		if(m_fileout)
		{
			fclose(m_fileout);
			m_fileout = NULL;
		}
		if(m_clsMutex)
		{
			delete m_clsMutex;
			m_clsMutex = NULL;
		}
	}
	bool m_newLine;
	FILE* m_fileout;
	CMutex *m_clsMutex;
};

extern CString GetLogHead(int line, const char* file, const char* category);

extern int LogPrintStr(const char* category, const CString &str);

static inline int LogTrace(const char* category, int line, const char* file, const char* format) {
	CString strTemp = file;
	int nPos = strTemp.ReverseFind('\\');
	if(nPos > 0) {
		strTemp = strTemp.Right(strTemp.GetLength()-nPos-1);
	}
	CString strHeader = GetLogHead(line, (LPSTR)(LPCTSTR)strTemp, category);
	CString pTemp;
	pTemp.Format(_T("%s%s"),strHeader, format);
	return LogPrintStr(category,  pTemp);
}

extern CString GetCurrentWorkDir();

extern void InitLogCfg();
