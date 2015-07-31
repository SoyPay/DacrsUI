// SetAppId.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SetAppId.h"
#include "afxdialogex.h"


// CSetAppId 对话框

IMPLEMENT_DYNAMIC(CSetAppId, CDialogEx)

CSetAppId::CSetAppId(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetAppId::IDD, pParent)
{

}

CSetAppId::~CSetAppId()
{
}

void CSetAppId::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetAppId, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetAppId::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetAppId 消息处理程序

void CSetAppId::SetDaluft()
{
	string strShow = "";

	GetDlgItem(IDC_BETID)->SetWindowText(theApp.m_betScritptid.c_str());
	GetDlgItem(IDC_GRBID)->SetWindowText(theApp.m_redPacketScriptid.c_str());
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
				::MessageBox( NULL , _T("猜你妹AppID格式不正确") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("抢红包AppID格式不正确") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("IPO AppID格式不正确") , "Error" , MB_ICONERROR) ;
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
	CDialogEx::OnOK();
}


BOOL CSetAppId::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDaluft();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
