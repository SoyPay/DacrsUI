// BetInformation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "BetInformation.h"
#include "afxdialogex.h"


// CBetInformation �Ի���

IMPLEMENT_DYNAMIC(CBetInformation, CDialogEx)

CBetInformation::CBetInformation(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBetInformation::IDD, pParent)
{

}

CBetInformation::~CBetInformation()
{
}

void CBetInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBetInformation, CDialogEx)
END_MESSAGE_MAP()


// CBetInformation ��Ϣ�������
BOOL CBetInformation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ShowBetRecordDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CBetInformation::ShowBetRecordDetail(CString jsontx)
{
	if (jsontx == _T(""))
	{
		CString strShowData;
		strShowData.AppendFormat(_T("%s") ,_T("�˽��ײ�����")) ;
		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(strShowData);
		return;
	}

	uistruct::P2P_QUIZ_RECORD_t betrecord;
	if (!betrecord.JsonToStruct(jsontx.GetString()))
	{
		return;
	}

	CString txdetail,strShowData;
	txdetail.Format(_T("���𾺲½���ID: %s\r\n\r\n"),betrecord.tx_hash);
	txdetail.AppendFormat(_T("������ID: %s \r\n\r\n"),betrecord.left_addr);
	txdetail.AppendFormat(_T("���: %.4f \r\n\r\n"),betrecord.amount);
	txdetail.AppendFormat(_T("��ʱʱ��: %d  ��\r\n\r\n"),betrecord.time_out);

	//// ֻ�Ƿ��˶�Լ����û�нӵ�
	if (betrecord.state == 0)
	{
		if (betrecord.height == 0)
		{
			txdetail.AppendFormat(_T(" ���𾺲½��׽��׻�δȷ��\r\n\r\n"));
		}else{
			if (betrecord.send_time != 0)
			{
				char buffer[1024] = {0};
				SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
				sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
				txdetail.AppendFormat(_T("���𾺲½���ȷ��ʱ��: %s\r\n\r\n"),buffer);
				txdetail.AppendFormat(_T("���𾺲½���ȷ�ϸ߶�: %d\r\n\r\n"),betrecord.confirmed);
			}
			if (theApp.IsSyncBlock && theApp.blocktipheight != 0)
			{
				if ((betrecord.height + 500)<theApp.blocktipheight)
				{
					txdetail.AppendFormat(_T("���𾺲½��׳�ʱδ���ӵ�(Ĭ��500����)\r\n\r\n"));
				}
			}
		}

	}else if (betrecord.state == 1)  ///��Լ���ӵ���
	{
		char buffer[1024] = {0};
		if (betrecord.send_time != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail.AppendFormat(_T(" ���𾺲½���ȷ��ʱ��: %s\r\n\r\n"),buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.recv_time != 0)
		{
			txdetail.AppendFormat(_T("�ӵ�����: %s\r\n\r\n"),betrecord.relate_hash);
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.recv_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail.AppendFormat(_T("�ӵ�����ȷ��ʱ��: %s\r\n\r\n"),buffer);
			txdetail.AppendFormat(_T("�ӵ�����ȷ�ϸ߶�: %d\r\n\r\n"),betrecord.height);
		}

		if (theApp.IsSyncBlock && theApp.blocktipheight != 0)
		{
			if ((betrecord.height + betrecord.time_out)<theApp.blocktipheight)
			{
				txdetail.AppendFormat(_T("���𾺲½��׳�ʱδ����(Ĭ�ϳ�ʱʱ��60����)\r\n\r\n"));
			}
		}
		
	}else if (betrecord.state == 2) /// ����
	{
		char buffer[1024] = {0};
		if (betrecord.send_time != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail.AppendFormat(_T("���𾺲½���ȷ��ʱ��: %s\r\n\r\n"),buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.recv_time != 0)
		{
			txdetail.AppendFormat(_T("�ӵ�����: %s\r\n\r\n"),betrecord.relate_hash);
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.recv_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail.AppendFormat(_T("�ӵ�����ȷ��ʱ��: %s\r\n\r\n"),buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.confirmed != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.confirmed);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail.AppendFormat(_T("���¿���ʱ��: %s\r\n\r\n"),buffer);
			txdetail.AppendFormat(_T("���¿����߶�: %d\r\n\r\n"),betrecord.height);
		}
	}
	
	if (betrecord.actor == 1 )
	{
		if (betrecord.guess_num == 1)
		{
			txdetail.AppendFormat(_T(" ��������:��\r\n\r\n"));
		}else
		{
			txdetail.AppendFormat(_T(" ��������:��\r\n\r\n"));
		}
	}

	if (betrecord.actor == 0)
	{
		if (betrecord.content[32] == 1)
		{
			txdetail.AppendFormat(_T(" ��������:��\r\n\r\n"));
		}else
		{
			txdetail.AppendFormat(_T(" ��������:��\r\n\r\n"));
		}
	}

	if (betrecord.actor == 2 || betrecord.state == 2)
	{
		if (betrecord.guess_num == 1)
		{
			txdetail.AppendFormat(_T(" ��������:��\r\n\r\n"));
		}else
		{
			txdetail.AppendFormat(_T(" ��������:��\r\n\r\n"));
		}

		if (betrecord.content[32] == 1)
		{
			txdetail.AppendFormat(_T(" ��������:��\r\n\r\n"));
		}else
		{
			txdetail.AppendFormat(_T(" ��������:��\r\n\r\n"));
		}
	}

		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail);
}