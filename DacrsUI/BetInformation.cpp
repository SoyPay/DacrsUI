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
		string strShowData = "�˽��ײ�����";
		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(strShowData.c_str());
		return;
	}

	uistruct::P2P_QUIZ_RECORD_t betrecord;
	if (!betrecord.JsonToStruct(jsontx.GetString()))
	{
		return;
	}

	string txdetail,strShowData;
	txdetail= strprintf("���𾺲½���ID: %s\r\n\r\n",betrecord.tx_hash.c_str());
	txdetail += strprintf("������ID: %s \r\n\r\n",betrecord.left_addr.c_str());
	txdetail += strprintf("���: %.4f \r\n\r\n",betrecord.amount);
	txdetail += strprintf("��ʱʱ��: %d  ��\r\n\r\n",betrecord.time_out);

	//// ֻ�Ƿ��˶�Լ����û�нӵ�
	if (betrecord.state == 0)
	{
		if (betrecord.height == 0)
		{
			txdetail+=" ���𾺲½��׽��׻�δȷ��\r\n\r\n";
		}else{
			if (betrecord.send_time != 0)
			{
				char buffer[1024] = {0};
				SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
				sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
				txdetail+= strprintf("���𾺲½���ȷ��ʱ��: %s\r\n\r\n",buffer);
				txdetail+= strprintf("���𾺲½���ȷ�ϸ߶�: %d\r\n\r\n",betrecord.confirmed);
			}
			if (theApp.IsSyncBlock && theApp.blocktipheight != 0)
			{
				if ((betrecord.height + 500)<theApp.blocktipheight)
				{
					txdetail+= "���𾺲½��׳�ʱδ���ӵ�(Ĭ��500����)\r\n\r\n";
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
			txdetail+= strprintf(" ���𾺲½���ȷ��ʱ��: %s\r\n\r\n",buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.recv_time != 0)
		{
			txdetail+= strprintf("�ӵ�����: %s\r\n\r\n",betrecord.relate_hash.c_str());
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.recv_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("�ӵ�����ȷ��ʱ��: %s\r\n\r\n",buffer);
			txdetail+= strprintf("�ӵ�����ȷ�ϸ߶�: %d\r\n\r\n",betrecord.height);
		}

		if (theApp.IsSyncBlock && theApp.blocktipheight != 0)
		{
			if ((betrecord.height + betrecord.time_out)<theApp.blocktipheight)
			{
				txdetail+= "���𾺲½��׳�ʱδ����(Ĭ�ϳ�ʱʱ��60����)\r\n\r\n";
			}
		}
		
	}else if (betrecord.state == 2) /// ����
	{
		char buffer[1024] = {0};
		if (betrecord.send_time != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("���𾺲½���ȷ��ʱ��: %s\r\n\r\n",buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.recv_time != 0)
		{
			txdetail+= strprintf("�ӵ�����: %s\r\n\r\n",betrecord.relate_hash.c_str());
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.recv_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("�ӵ�����ȷ��ʱ��: %s\r\n\r\n",buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.confirmed != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.confirmed);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("���¿���ʱ��: %s\r\n\r\n",buffer);
			txdetail+= strprintf("���¿����߶�: %d\r\n\r\n",betrecord.height);
		}
	}
	
	if (betrecord.actor == 1 )
	{
		if (betrecord.guess_num == 1)
		{
			txdetail+= " ��������:��\r\n\r\n";
		}else
		{
			txdetail+= " ��������:��\r\n\r\n";
		}
	}

	if (betrecord.actor == 0)
	{
		if (betrecord.content[32] == 1)
		{
			txdetail+= " ��������:��\r\n\r\n";
		}else
		{
			txdetail+= " ��������:��\r\n\r\n";
		}
	}

	if (betrecord.actor == 2 || betrecord.state == 2)
	{
		if (betrecord.guess_num == 1)
		{
			txdetail+= " ��������:��\r\n\r\n";
		}else
		{
			txdetail+= " ��������:��\r\n\r\n";
		}

		if (betrecord.content[32] == 1)
		{
			txdetail+= " ��������:��\r\n\r\n";
		}else
		{
			txdetail+= " ��������:��\r\n\r\n";
		}
	}

		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail.c_str());
}