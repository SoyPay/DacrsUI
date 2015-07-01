// SetAppFee.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SetAppFee.h"
#include "afxdialogex.h"


// CSetAppFee 对话框

IMPLEMENT_DYNAMIC(CSetAppFee, CDialogEx)

CSetAppFee::CSetAppFee(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetAppFee::IDD, pParent)
{

}

CSetAppFee::~CSetAppFee()
{
}

void CSetAppFee::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetAppFee, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetAppFee::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetAppFee::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetAppFee 消息处理程序

bool CSetAppFee::IsAllDigtal(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!isdigit(pData[i]))
			return false;
	}

	return true;
}
bool CSetAppFee::IsRegId(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!isdigit(pData[i]) && pData[i] !='_')
			return false;
	}

	return true;
}
void CSetAppFee::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (PathFileExistsA(theApp.str_InsPath))
	{
		CStringA configpath = "";
		configpath.AppendFormat("%s",theApp.str_InsPath);
		configpath.AppendFormat("\\%s","dacrsclient.conf");
		CString strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile.GetString(), root)) 
			return;

		CString strTemp = _T("");
		GetDlgItem(IDC_SENDBETFEE)->GetWindowText(strTemp);
		if (strTemp != _T("") )
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("发送赌约小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("SendBetFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTEFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("接赌约小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("AcceptBetnFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_OPENFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("开奖小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("OpenBetnFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_SENDCOMREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("发普通红包小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("sendredcommFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTCOMREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("接普通红包小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("acceptredcommFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_SENDSPECAILREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("发接龙红包小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("sendredspecalFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ACCEPTSPECAILREDFEE)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("接接龙红包小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("redpacket"),_T("acceptredspecalFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_ReChange)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("充值小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("GetRechangeFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_WITHDRAW)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsAllDigtal(strTemp))
			{
				::MessageBox( NULL , _T("提现小费必须是数字") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("p2pbet"),_T("GetAppAmountnFee"),strTemp);
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_BETID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				::MessageBox( NULL , _T("猜你妹AppID格式不正确") , "Error" , MB_ICONERROR) ;
			}else
			{
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("script"),_T("betscript"),strTemp);
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
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("script"),_T("redpacketscript"),strTemp);
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
				CJsonConfigHelp::getInstance()->ModifyAppFeeCfgData(root,_T("script"),_T("iposcript"),strTemp);
			}
		}

		CStdioFile  File;
		File.Open(theApp.str_InsPath+_T("\\dacrsclient.conf"),CFile::modeWrite | CFile::modeCreate); 
		string strfile = root.toStyledString();
		File.WriteString(strfile.c_str());
		File.Close();
	}

	theApp.ParseUIConfigFile(theApp.str_InsPath);
	CDialogEx::OnOK();
}


void CSetAppFee::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}
