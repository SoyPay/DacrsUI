// SplashThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	m_pSplashDlg=new CStartProgress;   
	//m_pSplashDlg->SetEnable(true);
	m_pSplashDlg->Create(IDD_DIALOG_INIT);
	m_pSplashDlg->ShowWindow(SW_SHOW);
	return TRUE;
}

int CSplashThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	m_pSplashDlg->DestroyWindow();
	delete m_pSplashDlg;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSplashThread, CWinThread)
END_MESSAGE_MAP()


// CSplashThread ��Ϣ�������
void CSplashThread::HideSplash()
{
	m_pSplashDlg->SendMessage(WM_CLOSE);
}
