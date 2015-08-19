// SetAppId.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SetAppId.h"
#include "afxdialogex.h"


// CSetAppId 对话框

IMPLEMENT_DYNAMIC(CSetAppId, CDialogEx)

CSetAppId::CSetAppId(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSetAppId::IDD, pParent)
{

}

CSetAppId::~CSetAppId()
{
}

void CSetAppId::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BETID, m_P2Pid);
	DDX_Control(pDX, IDC_STATIC_GRBID, m_redpackeid);	
	DDX_Control(pDX, IDC_STATIC_IPOID, m_ipoid);	
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CSetAppId, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSetAppId::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CSetAppId::OnBnClickedClose)
END_MESSAGE_MAP()


// CSetAppId 消息处理程序

void CSetAppId::SetDaluft()
{
	string strShow = "";

	GetDlgItem(IDC_BETID)->SetWindowText(theApp.m_betScritptid.c_str());
	GetDlgItem(IDC_GRBID)->SetWindowText(theApp.m_redPacketScriptid.c_str());
	GetDlgItem(IDC_IPOID)->SetWindowText(theApp.m_ipoScritptid.c_str());
}
bool CSetAppId::IsRegId(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!(pData[i] >= '0' && pData[i] <='9') && pData[i] !='-')
			return false;
	}

	return true;
}
void CSetAppId::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (PathFileExistsA(theApp.str_InsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.str_InsPath);
		configpath+= strprintf("\\%s","dacrsclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;

		CString strTemp = _T("");

		GetDlgItem(IDC_BETID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				UiFun::MessageBoxEx(_T("猜你妹AppID格式不正确") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				Json::Value setscriptid = root["script"];
				ASSERT(!setscriptid.isNull());
				setscriptid["betscript"]= strTemp.GetString();
				root["script"]=setscriptid;
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_GRBID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				UiFun::MessageBoxEx(_T("抢红包AppID格式不正确") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				Json::Value setscriptid = root["script"];
				ASSERT(!setscriptid.isNull());
				setscriptid["redpacketscript"]= strTemp.GetString();;
				root["script"]=setscriptid;
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_IPOID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				
				UiFun::MessageBoxEx(_T("IPO AppID格式不正确") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				Json::Value setscriptid = root["script"];
				ASSERT(!setscriptid.isNull());
				setscriptid["iposcript"]= strTemp.GetString();
				root["script"]=setscriptid;
			}
		}

		CStdioFile  File;
		string strpath = theApp.str_InsPath;
		strpath+="\\dacrsclient.conf";
		File.Open((LPCTSTR)(LPSTR)strpath.c_str(),CFile::modeWrite | CFile::modeCreate); 
		string strfile = root.toStyledString();
		File.WriteString(strfile.c_str());
		File.Close();
	}

	theApp.ParseUIConfigFile(theApp.str_InsPath);
	CDialogBase::OnOK();
}


BOOL CSetAppId::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(100, _T("微软雅黑"));
	m_headText.SetTextColor(RGB(255,255,255));	

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
	m_rBtnOk.SetWindowText("确 定") ;
	//m_rBtnOk.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText("取 消") ;
	//m_rBtnCancel.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();
	// TODO:  在此添加额外的初始化
	SetDaluft();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSetAppId::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogBase::OnCancel();
}
