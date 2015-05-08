#pragma once
#include "StartProgress.h"

#include "MyMutex.h"
// CSplashThread

class CSplashThread : public CWinThread
{
	DECLARE_DYNCREATE(CSplashThread)

protected:
	CSplashThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSplashThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void HideSplash();
	void SetDlgPos(int pos);
	CMyMutex		cs_dlg ;    //��Ϣ�����ٽ�
protected:
	DECLARE_MESSAGE_MAP()
protected:
	CStartProgress*  m_pSplashDlg;  //����һ���Ի���ָ��
};


