
// DacrsUI.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <vector>
#include <map>
#include <list>
#include <queue>

using namespace std;
// CDacrsUIApp:
// �йش����ʵ�֣������ DacrsUI.cpp
//

class CDacrsUIApp : public CWinApp
{
public:
	CDacrsUIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDacrsUIApp theApp;