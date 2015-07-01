// SetAppFee.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SetAppFee.h"
#include "afxdialogex.h"


// CSetAppFee �Ի���

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


// CSetAppFee ��Ϣ�������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
				::MessageBox( NULL , _T("���Ͷ�ԼС�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("�Ӷ�ԼС�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("����С�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("����ͨ���С�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("����ͨ���С�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("���������С�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("�ӽ������С�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("��ֵС�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("����С�ѱ���������") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("������AppID��ʽ����ȷ") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("�����AppID��ʽ����ȷ") , "Error" , MB_ICONERROR) ;
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
				::MessageBox( NULL , _T("IPO AppID��ʽ����ȷ") , "Error" , MB_ICONERROR) ;
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDialogEx::OnCancel();
}
