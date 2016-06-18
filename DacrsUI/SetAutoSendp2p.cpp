// SetAutoSendp2p.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SetAutoSendp2p.h"
#include "afxdialogex.h"


// CSetAutoSendp2p 对话框

IMPLEMENT_DYNAMIC(CSetAutoSendp2p, CDialogBase)

CSetAutoSendp2p::CSetAutoSendp2p(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSetAutoSendp2p::IDD, pParent)
{

}

CSetAutoSendp2p::~CSetAutoSendp2p()
{
}

void CSetAutoSendp2p::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
	DDX_Control(pDX, IDC_TIPS, m_tips);
	DDX_Control(pDX, IDC_SETAMOUNT, m_set);
	DDX_Control(pDX, IDC_PER, m_per);
	DDX_Control(pDX, IDC_GETAMOUNT, m_amount);

	DDX_Control(pDX, IDC_PERCENT, m_Edit);
}


BEGIN_MESSAGE_MAP(CSetAutoSendp2p, CDialogBase)
	ON_BN_CLICKED(IDC_CLOSE, &CSetAutoSendp2p::OnBnClickedClose)
	ON_BN_CLICKED(IDOK, &CSetAutoSendp2p::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetAutoSendp2p::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_PERCENT, &CSetAutoSendp2p::OnEnUpdatePercent)
	ON_EN_KILLFOCUS(IDC_SENDMONEY, &CSetAutoSendp2p::OnEnKillfocusSendmoney)
END_MESSAGE_MAP()


// CSetAutoSendp2p 消息处理程序
BOOL CSetAutoSendp2p::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_headText.SetFont(100, _T("微软雅黑"));
	m_headText.SetTextColor(RGB(255,255,255));	
	m_headText.SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage));

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
	m_rBtnOk.SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_CONFIRM" ,theApp.gsLanguage)) ;
	//m_rBtnOk.SetFontEx(12 , _T("宋体"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_CANCEL" ,theApp.gsLanguage)) ;
	//m_rBtnCancel.SetFontEx(12 , _T("宋体"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();
	m_Edit.SetLimitText(3);

	m_tips.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_BET_SEND_MONEY" ,theApp.gsLanguage)+":");
	m_set.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_SET_RETURNPEE" ,theApp.gsLanguage)+":");
	GetDlgItem(IDC_GETAMOUNT)->SetWindowText("");
	//GetDlgItem(IDC_PERCENT)->SetWindowText(m_percentage);
	//double percent = (atoi(m_percentage)*1.0)/100;

	//double money = strtod(m_money,NULL)*percent;
	//string strTemp = strprintf("%.4f",money);
	//GetDlgItem(IDC_GETAMOUNT)->SetWindowText(strTemp.c_str());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSetAutoSendp2p::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress = _T("");
	CDialogBase::OnCancel();
}

bool CSetAutoSendp2p::IsAllDigtal(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!(pData[i] >= '0' && pData[i] <='9')&&pData[i] !='.')
			return false;
	}

	return true;
}
void CSetAutoSendp2p::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString percentage;
	GetDlgItem(IDC_PERCENT)->GetWindowText(percentage) ;
	if (percentage == _T(""))
	{

		UiFun::MessageBoxEx(UiFun::UI_LoadString("SETAPPID" , "SETAPPFEE_PER" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	if (atoi(percentage)<=90 || atoi(percentage)>=200||!IsAllDigtal(percentage))
	{
		string error=UiFun::UI_LoadString("P2P_MODULE" , "P2P_SET_RETURNEVALUE" ,theApp.gsLanguage);
		UiFun::MessageBoxEx(error.c_str(), UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}
	m_percent=percentage;
	double percent = (atoi(percentage)*1.0)/100;

	double money = strtod(m_money,NULL)*percent;
	string strMoney = strprintf("%.4f",money);
	theApp.m_strAddress = strMoney.c_str();
	CDialogBase::OnOK();
}


void CSetAutoSendp2p::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress = _T("");
	CDialogBase::OnCancel();
}


void CSetAutoSendp2p::OnEnUpdatePercent()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogBase::OnInitDialog()
	// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
	// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString num;
	GetDlgItem(IDC_PERCENT)->GetWindowText(num);
	if (IsAllDigtal(num))
	{
		double percent = (atoi(num)*1.0)/100;

		double money = strtod(m_money,NULL)*percent;
		string strMoney = strprintf("冻结:%.4f",money);
		GetDlgItem(IDC_GETAMOUNT)->SetWindowText(strMoney.c_str());
		Invalidate();
	}
}


void CSetAutoSendp2p::OnEnKillfocusSendmoney()
{
	// TODO: 在此添加控件通知处理程序代码
	
	GetDlgItem(IDC_SENDMONEY)->GetWindowText(m_money);
}
void CSetAutoSendp2p::GetSendBetParam( string  &sendbetmoney,  string &accept_pecent)
{
	 accept_pecent =m_percent.GetString();
	 sendbetmoney =m_money.GetString();
}