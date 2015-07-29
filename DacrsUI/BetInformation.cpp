// BetInformation.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "BetInformation.h"
#include "afxdialogex.h"


// CBetInformation 对话框

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


// CBetInformation 消息处理程序
BOOL CBetInformation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ShowBetRecordDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CBetInformation::ShowBetRecordDetail(CString jsontx)
{
	if (jsontx == _T(""))
	{
		string strShowData = "此交易不存在";
		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(strShowData.c_str());
		return;
	}

	uistruct::P2P_QUIZ_RECORD_t betrecord;
	if (!betrecord.JsonToStruct(jsontx.GetString()))
	{
		return;
	}

	string txdetail,strShowData;
	txdetail= strprintf("发起竞猜交易ID: %s\r\n\r\n",betrecord.tx_hash.c_str());
	txdetail += strprintf("发起人ID: %s \r\n\r\n",betrecord.left_addr.c_str());
	txdetail += strprintf("金额: %.4f \r\n\r\n",betrecord.amount);
	txdetail += strprintf("超时时间: %d  分\r\n\r\n",betrecord.time_out);

	//// 只是发了赌约，还没有接单
	if (betrecord.state == 0)
	{
		if (betrecord.height == 0)
		{
			txdetail+=" 发起竞猜交易交易还未确认\r\n\r\n";
		}else{
			if (betrecord.send_time != 0)
			{
				char buffer[1024] = {0};
				SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
				sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
				txdetail+= strprintf("发起竞猜交易确认时间: %s\r\n\r\n",buffer);
				txdetail+= strprintf("发起竞猜交易确认高度: %d\r\n\r\n",betrecord.confirmed);
			}
			if (theApp.IsSyncBlock && theApp.blocktipheight != 0)
			{
				if ((betrecord.height + 500)<theApp.blocktipheight)
				{
					txdetail+= "发起竞猜交易超时未被接单(默认500分钟)\r\n\r\n";
				}
			}
		}

	}else if (betrecord.state == 1)  ///赌约被接单了
	{
		char buffer[1024] = {0};
		if (betrecord.send_time != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf(" 发起竞猜交易确认时间: %s\r\n\r\n",buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.recv_time != 0)
		{
			txdetail+= strprintf("接单交易: %s\r\n\r\n",betrecord.relate_hash.c_str());
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.recv_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("接单交易确认时间: %s\r\n\r\n",buffer);
			txdetail+= strprintf("接单交易确认高度: %d\r\n\r\n",betrecord.height);
		}

		if (theApp.IsSyncBlock && theApp.blocktipheight != 0)
		{
			if ((betrecord.height + betrecord.time_out)<theApp.blocktipheight)
			{
				txdetail+= "发起竞猜交易超时未开奖(默认超时时间60分钟)\r\n\r\n";
			}
		}
		
	}else if (betrecord.state == 2) /// 开奖
	{
		char buffer[1024] = {0};
		if (betrecord.send_time != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("发起竞猜交易确认时间: %s\r\n\r\n",buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.recv_time != 0)
		{
			txdetail+= strprintf("接单交易: %s\r\n\r\n",betrecord.relate_hash.c_str());
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.recv_time);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("接单交易确认时间: %s\r\n\r\n",buffer);
		}
		memset(buffer,0,1024);
		if (betrecord.confirmed != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.confirmed);
			sprintf_s(buffer,"%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("竞猜开奖时间: %s\r\n\r\n",buffer);
			txdetail+= strprintf("竞猜开奖高度: %d\r\n\r\n",betrecord.height);
		}
	}
	
	if (betrecord.actor == 1 )
	{
		if (betrecord.guess_num == 1)
		{
			txdetail+= " 竞猜内容:妹\r\n\r\n";
		}else
		{
			txdetail+= " 竞猜内容:哥\r\n\r\n";
		}
	}

	if (betrecord.actor == 0)
	{
		if (betrecord.content[32] == 1)
		{
			txdetail+= " 开奖底牌:妹\r\n\r\n";
		}else
		{
			txdetail+= " 开奖底牌:哥\r\n\r\n";
		}
	}

	if (betrecord.actor == 2 || betrecord.state == 2)
	{
		if (betrecord.guess_num == 1)
		{
			txdetail+= " 竞猜内容:妹\r\n\r\n";
		}else
		{
			txdetail+= " 竞猜内容:哥\r\n\r\n";
		}

		if (betrecord.content[32] == 1)
		{
			txdetail+= " 开奖底牌:妹\r\n\r\n";
		}else
		{
			txdetail+= " 开奖底牌:哥\r\n\r\n";
		}
	}

		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail.c_str());
}