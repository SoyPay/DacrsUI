// SplashThread.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SplashThread.h"


// CSplashThread

IMPLEMENT_DYNCREATE(CSplashThread, CWinThread)

CSplashThread::CSplashThread()
{
		m_pSplashDlg = NULL;
}

CSplashThread::~CSplashThread()
{
	if (m_pSplashDlg != NULL)
	{
		delete m_pSplashDlg;
		m_pSplashDlg = NULL;
	}
}

BOOL CSplashThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	m_pSplashDlg=new CStartProgress;   
	m_pSplashDlg->Create(IDD_DIALOG_INIT);
	m_pSplashDlg->ShowWindow(SW_SHOW);
	return TRUE;
}

int CSplashThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	if (m_pSplashDlg != NULL)
	{
		m_pSplashDlg->DestroyWindow();
		delete m_pSplashDlg;
		m_pSplashDlg = NULL;
	}

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSplashThread, CWinThread)
END_MESSAGE_MAP()


// CSplashThread 消息处理程序
void CSplashThread::HideSplash()
{
	if (m_pSplashDlg != NULL)
	{
			m_pSplashDlg->SendMessage(WM_CLOSE);
	}

}
