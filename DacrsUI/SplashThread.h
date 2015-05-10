#pragma once
#include "StartProgress.h"
// CSplashThread

class CSplashThread : public CWinThread
{
	DECLARE_DYNCREATE(CSplashThread)

public:
	CSplashThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSplashThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void HideSplash();
protected:
	DECLARE_MESSAGE_MAP()
protected:
	CStartProgress*  m_pSplashDlg;  //����һ���Ի���ָ��
};


