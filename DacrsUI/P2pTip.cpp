// P2pTip.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "P2pTip.h"
#include "afxdialogex.h"


// CP2pTip 对话框

IMPLEMENT_DYNAMIC(CP2pTip, CDialogBase)

CP2pTip::CP2pTip(CWnd* pParent /*=NULL*/,CString strtips,CString stmoeny)
	: CDialogBase(CP2pTip::IDD, pParent)
{
	m_strtips = strtips;
	m_money = stmoeny;
}

CP2pTip::~CP2pTip()
{
}

void CP2pTip::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
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


BEGIN_MESSAGE_MAP(CP2pTip, CDialogBase)
	ON_BN_CLICKED(IDC_CLOSE, &CP2pTip::OnBnClickedClose)
	ON_BN_CLICKED(IDOK, &CP2pTip::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CP2pTip::OnBnClickedCancel)
	ON_EN_UPDATE(IDC_PERCENT, &CP2pTip::OnEnUpdatePercent)
END_MESSAGE_MAP()


// CP2pTip 消息处理程序

static CString m_percentage = "100";
BOOL CP2pTip::OnInitDialog()
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

	m_tips.SetWindowText(m_strtips);
	m_set.SetWindowText(UiFun::UI_LoadString("P2P_MODULE" , "P2P_SET_RETURNPEE" ,theApp.gsLanguage)+":");

	GetDlgItem(IDC_PERCENT)->SetWindowText(m_percentage);
	double percent = (atoi(m_percentage)*1.0)/100;

	double money = strtod(m_money,NULL)*percent;
	string strTemp = strprintf("冻结:%.4f",money);
	GetDlgItem(IDC_GETAMOUNT)->SetWindowText(strTemp.c_str());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CP2pTip::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress = _T("");
	CDialogBase::OnCancel();
}

bool CP2pTip::IsAllDigtal(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!(pData[i] >= '0' && pData[i] <='9')&&pData[i] !='.')
			return false;
	}

	return true;
}
void CP2pTip::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxMoney;
	GetDlgItem(IDC_PERCENT)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{

		UiFun::MessageBoxEx(UiFun::UI_LoadString("SETAPPID" , "SETAPPFEE_PER" ,theApp.gsLanguage) , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return ;
	}

	if (atoi(strTxMoney)<=90 || atoi(strTxMoney)>=200||!IsAllDigtal(strTxMoney))
	{
		string error=UiFun::UI_LoadString("P2P_MODULE" , "P2P_SET_RETURNEVALUE" ,theApp.gsLanguage);
		UiFun::MessageBoxEx(error.c_str(), UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) ,MFB_OK|MFB_TIP );
		return;
	}

	m_percentage=strTxMoney;

	double percent = (atoi(m_percentage)*1.0)/100;

	double money = strtod(m_money,NULL)*percent;
	string strMoney = strprintf("%.4f",money);
	theApp.m_strAddress = strMoney.c_str();
	CDialogBase::OnOK();
}


void CP2pTip::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.m_strAddress = _T("");
	CDialogBase::OnCancel();
}


void CP2pTip::OnEnUpdatePercent()
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
