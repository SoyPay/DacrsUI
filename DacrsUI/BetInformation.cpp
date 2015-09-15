// BetInformation.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "BetInformation.h"
#include "afxdialogex.h"


// CBetInformation 对话框

IMPLEMENT_DYNAMIC(CBetInformation, CDialogBase)

CBetInformation::CBetInformation(CWnd* pParent /*=NULL*/)
	: CDialogBase(CBetInformation::IDD, pParent)
{

}

CBetInformation::~CBetInformation()
{
}

void CBetInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CBetInformation, CDialogBase)
	ON_BN_CLICKED(IDC_CLOSE, &CBetInformation::OnBnClickedClose)
END_MESSAGE_MAP()


// CBetInformation 消息处理程序
BOOL CBetInformation::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(90, _T("微软雅黑"));
	m_headText.SetTextColor(RGB(255,255,255));	
	m_headText.SetWindowText(UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_HEAD" ,theApp.gsLanguage) );
	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();

	CRect rect ;
	m_rBtnClose.GetClientRect(rect);

	RECT ret;
	GetWindowRect(&ret);
	m_rBtnClose.SetWindowPos(NULL ,(ret.right-ret.left)-rect.Width() , 2 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_CONFIRM" ,theApp.gsLanguage)) ;
	//m_rBtnOk.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();
	// TODO:  在此添加额外的初始化
	ShowBetRecordDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CBetInformation::ShowBetRecordDetail(CString jsontx)
{
	if (jsontx == _T(""))
	{
		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRAN_NOT" ,theApp.gsLanguage));
		return;
	}

	uistruct::P2P_QUIZ_RECORD_t betrecord;
	if (!betrecord.JsonToStruct(jsontx.GetString()))
	{
		return;
	}

	string txdetail,strShowData;
	txdetail= strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_TRADID" ,theApp.gsLanguage) ,betrecord.tx_hash.c_str());
	txdetail += strprintf("%S: %s \r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_SEND" ,theApp.gsLanguage),betrecord.left_addr.c_str());
	txdetail += strprintf("%s: %.4f \r\n\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),betrecord.amount);
	txdetail += strprintf("%s: %d  %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_TIMEOUT" ,theApp.gsLanguage),betrecord.time_out,UiFun::UI_LoadString("PASS_WORD" , "PASS_WORD_MINUTE" ,theApp.gsLanguage));

	//// 只是发了赌约，还没有接单
	if (betrecord.state == 0)
	{
		if (betrecord.height == 0)
		{
			//txdetail+=" 发起竞猜交易交易还未确认\r\n\r\n";
			txdetail+=strprintf(" %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_STATE" ,theApp.gsLanguage));
		}else{
			if (betrecord.send_time != 0)
			{
				string buffer;
				SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
				buffer = strprintf("%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
				txdetail+= strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_CONFIRED" ,theApp.gsLanguage),buffer);
				txdetail+= strprintf("%s: %d\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_SHEIGHT" ,theApp.gsLanguage),betrecord.height);
			}
			if (theApp.IsSyncBlock && theApp.blocktipheight != 0)
			{
				if ((betrecord.height + 500)<theApp.blocktipheight)
				{
					//txdetail+= "发起竞猜交易超时未被接单(默认500分钟)\r\n\r\n";
					txdetail+= strprintf("%s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_STIMEOUT" ,theApp.gsLanguage));
				}
			}
		}

	}else if (betrecord.state == 1)  ///赌约被接单了
	{
		string buffer;
		if (betrecord.send_time != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
			buffer = strprintf("%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf(" %s: %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_CONFIRED" ,theApp.gsLanguage),buffer);
		}

		if (betrecord.recv_time != 0)
		{
			txdetail+= strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_TYPE" ,theApp.gsLanguage),betrecord.relate_hash.c_str());
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.recv_time);
			buffer= strprintf("%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_ACCEPT" ,theApp.gsLanguage),buffer);
			txdetail+= strprintf("%s: %d\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_HEIGHT" ,theApp.gsLanguage),betrecord.height);
		}

		if (theApp.IsSyncBlock && theApp.blocktipheight != 0)
		{
			if ((betrecord.height + betrecord.time_out)<theApp.blocktipheight)
			{
				//txdetail+= "发起竞猜交易超时未开奖(默认超时时间60分钟)\r\n\r\n";
				txdetail+= strprintf("%s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_OPEN" ,theApp.gsLanguage));
			}
		}
		
	}else if (betrecord.state == 2) /// 开奖
	{
		string buffer;
		if (betrecord.send_time != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.send_time);
			buffer= strprintf("%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_CONFIRED" ,theApp.gsLanguage),buffer);
		}

		if (betrecord.recv_time != 0)
		{
			txdetail+= strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_TYPE" ,theApp.gsLanguage),betrecord.relate_hash.c_str());
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.recv_time);
			buffer= strprintf("%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_ACCEPT" ,theApp.gsLanguage),buffer);
		}
	
		if (betrecord.confirmed != 0)
		{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(betrecord.confirmed);
			buffer= strprintf("%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
			txdetail+= strprintf("%s: %s\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_OPENTIME" ,theApp.gsLanguage),buffer);
			txdetail+= strprintf("%s: %d\r\n\r\n",UiFun::UI_LoadString("GUESS_SISTERLIST" , "GUESS_SISTERLIST_OPENHEIGHT" ,theApp.gsLanguage),betrecord.height);
		}
	}
	
	if (betrecord.actor == 1 )
	{
		if (betrecord.guess_num == 1)
		{
			//txdetail+= " 竞猜内容:妹\r\n\r\n";
			txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_QUIZ_CONTENT" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_SISTER" ,theApp.gsLanguage));
		}else
		{
			//txdetail+= " 竞猜内容:哥\r\n\r\n";
			txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_QUIZ_CONTENT" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_BROTHER" ,theApp.gsLanguage));
		}
	}

	if (betrecord.actor == 0)
	{
		if (betrecord.content[32] == 1)
		{
			//txdetail+= " 开奖底牌:妹\r\n\r\n";
			txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LOTTERY_CARDS" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_SISTER" ,theApp.gsLanguage));
		}else
		{
			//txdetail+= " 开奖底牌:哥\r\n\r\n";
			txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LOTTERY_CARDS" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_BROTHER" ,theApp.gsLanguage));
		}
	}

	if (betrecord.actor == 2 || betrecord.state == 2)
	{
		if (betrecord.actor == 2)
		{
			if (betrecord.guess_num == 1)
			{
				//txdetail+= " 竞猜内容:妹\r\n\r\n";
				txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_QUIZ_CONTENT" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_SISTER" ,theApp.gsLanguage));
			}else
			{
				//txdetail+= " 竞猜内容:哥\r\n\r\n";
				txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_QUIZ_CONTENT" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_BROTHER" ,theApp.gsLanguage));
			}
		}
		
		if (betrecord.actor == 2)
		{
			if (betrecord.content[32] == 1)
			{
				//txdetail+= " 开奖底牌:妹\r\n\r\n";
				txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LOTTERY_CARDS" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_SISTER" ,theApp.gsLanguage));
			}else
			{
				//txdetail+= " 开奖底牌:哥\r\n\r\n";
				txdetail+=strprintf("%s:%s\r\n\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_LOTTERY_CARDS" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_BROTHER" ,theApp.gsLanguage));
			}
		}

	}

		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail.c_str());
}

void CBetInformation::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogBase::OnCancel();
}
