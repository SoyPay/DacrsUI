// SplashThread.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	m_pSplashDlg=new CStartProgress;   
	m_pSplashDlg->Create(IDD_DIALOG_INIT);
	m_pSplashDlg->ShowWindow(SW_SHOW);
	return TRUE;
}

int CSplashThread::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
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


// CSplashThread ��Ϣ�������
void CSplashThread::HideSplash()
{
	if (m_pSplashDlg != NULL)
	{
			m_pSplashDlg->SendMessage(WM_CLOSE);
	}

}
