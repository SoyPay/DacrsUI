// RedPacketList.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "RedPacketList.h"
#include "afxdialogex.h"


// CRedPacketList 对话框

IMPLEMENT_DYNAMIC(CRedPacketList, CDialogEx)

CRedPacketList::CRedPacketList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRedPacketList::IDD, pParent)
{

}

CRedPacketList::~CRedPacketList()
{
}

void CRedPacketList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CRedPacketList, CDialogEx)
END_MESSAGE_MAP()


// CRedPacketList 消息处理程序

void CRedPacketList::ShowTxDetail(CString txhash)
{
	CString strCommand,strShowData;
	strCommand.Format(_T("%s %s"),_T("gettxdetail") ,txhash );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData == _T(""))
	{
		return ;
	}
	Json::Reader reader; 
	Json::Value root;
	if (!reader.parse(strShowData.GetString(), root)) 
		return ;
	int npos = strShowData.Find("confirmHeight");
	int confirHeight = 1440;
	if ( npos >= 0 ) { //
		confirHeight += root["confirmHeight"].asInt() ;    //交易被确认的高度
	}

	vector<unsigned char>vHash;
	CSoyPayHelp::getInstance()->revert((char*)&confirHeight);

	char key[4];
	memset(key,0,4);

	memcpy(key,&confirHeight,sizeof(confirHeight));
	vHash.assign(key,key+sizeof(key));
	string strKeyHex = CSoyPayHelp::getInstance()->HexStr(vHash);

	vHash =CSoyPayHelp::getInstance()->ParseHex(txhash.GetString());
	reverse(vHash.begin(),vHash.end());
	string SendHash = CSoyPayHelp::getInstance()->HexStr(vHash);

	CString keyValue;
	keyValue.Format(_T("%s%s"),strKeyHex.c_str(),SendHash.c_str());
	strCommand.Format(_T("%s %s %s"),_T("getscriptdata") ,theApp.m_redPacketScriptid,keyValue );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData == _T("") || strShowData.Find("value") < 0)
		return ;

	if (!reader.parse(strShowData.GetString(), root)) 
		return ;

	CString nValue = root["value"].asCString();
	uistruct::RED_DATA redPacket;
	memset(&redPacket , 0 , sizeof(uistruct::RED_DATA));
	std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue.GetString());
	if (vTemp.size() <=0)
	{
		return;
	}
	memcpy(&redPacket, &vTemp[0], sizeof(uistruct::RED_DATA));

	CString showdata = _T(""); //= _T(" 地址                            金额 \r\n\r\n");
	m_listCtrl.DeleteAllItems();
	 if (redPacket.dbdata.type == 3 && !redPacket.dbdata.fover)
	{
		//showdata.Format(_T("接龙红包还未抢完"));
		::MessageBox( this->GetSafeHwnd() ,_T("接龙红包还未抢完"), _T("提示") , MB_ICONINFORMATION );
		CDialog::OnOK();
	}else{
		for (int i =0;i <redPacket.dbdata.takennum;i++)
		{
			int nSubIdx = 0;
			uistruct::USER_INFO userinfo = redPacket.userinfo[i];
			std::vector<unsigned char> vSendid;
			vSendid.assign(userinfo.regid,userinfo.regid+sizeof(userinfo.regid));
			string regid  =CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);
			double money = (userinfo.amount*1.0)/COIN;
			//showdata.AppendFormat(_T(" %s                       %.8f\r\n\r\n"),regid.c_str(),money);
			showdata.Format(_T("%d"),(i+1));
			m_listCtrl.InsertItem(i, showdata);					//序号

			showdata.Format(_T("%s"),regid.c_str());
			m_listCtrl.SetItemText(i , ++nSubIdx , showdata );  //

			strShowData.Format(_T("%.8f"), money); 
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData );  //交易hash

		}
	 }

	//GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(showdata);
}
BOOL CRedPacketList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[3]  = {
		{"序号",          70},
		{"账户ID",          85},
		{"金额" ,      150}
	};

	for( int i = 0 ; i < 3 ; i++  ) {
		m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );
	// TODO:  在此添加额外的初始化
	ShowTxDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
