// SplashThread.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SplashThread.h"


// CSplashThread

IMPLEMENT_DYNCREATE(CSplashThread, CWinThread)

CSplashThread::CSplashThread()
{
}

CSplashThread::~CSplashThread()
{
}

BOOL CSplashThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	cs_dlg.Lock();
	m_pSplashDlg=new CStartProgress;   
	//m_pSplashDlg->SetEnable(true);
	m_pSplashDlg->Create(IDD_DIALOG_INIT);
	m_pSplashDlg->ShowWindow(SW_SHOW);
	cs_dlg.Unlock();
	return TRUE;
}

int CSplashThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	cs_dlg.Lock();
	m_pSplashDlg->DestroyWindow();
	delete m_pSplashDlg;
	cs_dlg.Unlock();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSplashThread, CWinThread)
END_MESSAGE_MAP()


// CSplashThread 消息处理程序
void CSplashThread::HideSplash()
{
	cs_dlg.Lock();
	m_pSplashDlg->SendMessage(WM_CLOSE);
	cs_dlg.Unlock();
}
void CSplashThread::SetDlgPos(int pos)
{
	cs_dlg.Lock();
	m_pSplashDlg->SetProgessRange(pos);
	cs_dlg.Unlock();
}