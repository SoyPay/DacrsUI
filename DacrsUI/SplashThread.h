#pragma once
#include "StartProgress.h"

#include "MyMutex.h"
// CSplashThread

class CSplashThread : public CWinThread
{
	DECLARE_DYNCREATE(CSplashThread)

protected:
	CSplashThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CSplashThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void HideSplash();
	void SetDlgPos(int pos);
	CMyMutex		cs_dlg ;    //消息订阅临界
protected:
	DECLARE_MESSAGE_MAP()
protected:
	CStartProgress*  m_pSplashDlg;  //声明一个对话框指针
};


