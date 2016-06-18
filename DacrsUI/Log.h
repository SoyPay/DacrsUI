#pragma once
#include <string>
#include "tinyformat.h"
using namespace std;
#define LogPrint(tag, ...) LogTrace(tag, __LINE__, __FILE__, __VA_ARGS__);

#define MAKE_ERROR_AND_TRACE_FUNC(n)                                        \
	/*   Print to debug.log if -debug=category switch is given OR category is NULL. */ \
	template<TINYFORMAT_ARGTYPES(n)>                                          \
	static inline int LogTrace(const char* category, int line, const char* file, const char* format, TINYFORMAT_VARARGS(n))  \
	{                                                                         \
		return LogPrintStr(category, GetLogHead(line,file,category) + tfm::format(format, TINYFORMAT_PASSARGS(n))); \
	}   

TINYFORMAT_FOREACH_ARGNUM(MAKE_ERROR_AND_TRACE_FUNC)

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

extern string GetLogHead(int line, const char* file, const char* category);

extern int LogPrintStr(const char* category, const string &str);

static inline int LogTrace(const char* category, int line, const char* file, const char* format) {
	return LogPrintStr(category,   GetLogHead(line, file, category)+format);
}

extern CString GetCurrentWorkDir();

extern void InitLogCfg();

int LogFilePreProcess(const char *path, size_t len, FILE** stream);