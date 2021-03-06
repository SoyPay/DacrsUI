// RedPacketList.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "RedPacketList.h"
#include "afxdialogex.h"
#include <map>

// CRedPacketList 对话框

IMPLEMENT_DYNAMIC(CRedPacketList, CDialogBase)

CRedPacketList::CRedPacketList(CWnd* pParent /*=NULL*/)
	: CDialogBase(CRedPacketList::IDD, pParent)
{
}

CRedPacketList::~CRedPacketList()
{
}

void CRedPacketList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listCtrl);

	DDX_Control(pDX, IDC_REDID, m_strTx1);
	DDX_Control(pDX, IDC_TXHASH, m_strTx2);	
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CRedPacketList, CDialogBase)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CRedPacketList::OnLvnColumnclickList2)
	ON_BN_CLICKED(IDOK, &CRedPacketList::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CRedPacketList::OnBnClickedClose)
END_MESSAGE_MAP()


// CRedPacketList 消息处理程序

void CRedPacketList::ShowTxDetail(CString txhash)
{
	string strCommand,strShowData;
	strCommand = strprintf("%s %s","gettxdetail" ,txhash );
	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateAddressData rpccmd listaddr error");
		return;
	}
	strShowData = root.toStyledString();
	
	int npos = strShowData.find("confirmHeight");
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

	string keyValue;
	keyValue = strprintf("%s%s",strKeyHex.c_str(),SendHash.c_str());
	strCommand =strprintf("%s %s %s","getscriptdata" ,theApp.m_redPacketScriptid,keyValue );
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateAddressData rpccmd listaddr error");
		return;
	}

	string nValue = root["value"].asString();
	uistruct::RED_DATA redPacket;
	memset(&redPacket , 0 , sizeof(uistruct::RED_DATA));
	std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
	if (vTemp.size() <=0)
	{
		return;
	}
	memcpy(&redPacket, &vTemp[0], sizeof(uistruct::RED_DATA));

	string showdata = _T(""); //= _T(" 地址                            金额 \r\n\r\n");
	m_listCtrl.DeleteAllItems();
	 if (redPacket.dbdata.type == 3 && !redPacket.dbdata.fover)
	{
		//showdata.Format(_T("接龙红包还未抢完"));
		
		UiFun::MessageBoxEx(UiFun::UI_LoadString("COMM_MODULE" , "COMM_LISTTIP" ,theApp.gsLanguage), UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage)  ,MFB_OK|MFB_TIP );
		CDialog::OnOK();
	}else{
		 //map<INT64,CString > mapindex;
		  map<CString,INT64 > mapindex;
		 for (int i =0;i <redPacket.dbdata.takennum;i++)
		 {
			 uistruct::USER_INFO userinfo = redPacket.userinfo[i];
			 std::vector<unsigned char> vSendid;
			 vSendid.assign(userinfo.regid,userinfo.regid+sizeof(userinfo.regid));
			 string regid  =CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);

			 CString regId = _T("");
			 regId.Format(_T("%s"),regid.c_str());
			// mapindex[userinfo.amount] = regId;
			 mapindex[regId] = userinfo.amount;

		 }
		 map<CString,INT64 >::reverse_iterator item ;
		 int i = 0;
		 for (item = mapindex.rbegin();item != mapindex.rend();item++,i++)
		 {
			 int nSubIdx = 0;

			 double money = (item->second*1.0)/COIN;
			 //showdata.AppendFormat(_T(" %s                       %.8f\r\n\r\n"),regid.c_str(),money);
			 showdata =strprintf("%d",(i+1));
			 m_listCtrl.InsertItem(i, showdata.c_str());					//序号

			 showdata = strprintf("%s",item->first);
			 m_listCtrl.SetItemText(i , ++nSubIdx , showdata.c_str() );  //

			 showdata = strprintf("%.8f", money); 
			 m_listCtrl.SetItemText(i , ++nSubIdx , showdata.c_str() );  //交易hash
		 }
		//for (int i =0;i <redPacket.dbdata.takennum;i++)
		//{
		//	int nSubIdx = 0;
		//	uistruct::USER_INFO userinfo = redPacket.userinfo[i];
		//	std::vector<unsigned char> vSendid;
		//	vSendid.assign(userinfo.regid,userinfo.regid+sizeof(userinfo.regid));
		//	string regid  =CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);
		//	double money = (userinfo.amount*1.0)/COIN;
		//	//showdata.AppendFormat(_T(" %s                       %.8f\r\n\r\n"),regid.c_str(),money);
		//	showdata.Format(_T("%d"),(i+1));
		//	m_listCtrl.InsertItem(i, showdata);					//序号

		//	showdata.Format(_T("%s"),regid.c_str());
		//	m_listCtrl.SetItemText(i , ++nSubIdx , showdata );  //

		//	strShowData.Format(_T("%.8f"), money); 
		//	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData );  //交易hash

		//}
	 }

	//GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(showdata);
}
BOOL CRedPacketList::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(90, _T("微软雅黑"));
	m_headText.SetTextColor(RGB(255,255,255));	
	m_headText.SetWindowText(UiFun::UI_LoadString("COMM_MODULE" , "COMM_LIST" ,theApp.gsLanguage));

	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	//m_rBtnClose.SetWindowPos(NULL ,320 , 0 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText(UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_CONFIRM" ,theApp.gsLanguage)) ;
	//m_rBtnOk.SetFontEx(20 , _T("微软雅黑"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();


	struct LISTCol {
		CString		name ;
		UINT		size ;
	} listcol[3]  = {
		{UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_SERIAL_NUMBER" ,theApp.gsLanguage),          70},
		{UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ACCOUNT_ID" ,theApp.gsLanguage) ,          85},
		{UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_MONEY" ,theApp.gsLanguage) ,      180}
	};

	for( int i = 0 ; i < 3 ; i++  ) {
		m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
	}
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );
	// TODO:  在此添加额外的初始化
	
	GetDlgItem(IDC_REDID)->SetWindowText(UiFun::UI_LoadString("TITLEBAR_MODULE" , "TITLEBAR_MORTGAGE" ,theApp.gsLanguage)+" id:");

	CString strShow = theApp.m_strAddress.Left(30);
	GetDlgItem(IDC_TXHASH)->SetWindowText(strShow);
	ShowTxDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
static int sort_column; // 记录点击的列
static BOOL method = TRUE; // 记录比较方法
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{

 // 从参数中提取所需比较lc的两行数据

	int row1 = (int) lParam1;
	int row2 = (int) lParam2;

	CListCtrl* lc = (CListCtrl*)lParamSort;

	CString lp1 = lc->GetItemText(row1,sort_column);
	CString lp2 = lc->GetItemText(row2,sort_column);


	// 比较，对不同的列，不同比较，注意记录前一次排序方向，下一次要相反排序
 
	if (sort_column==2)
	{
		INT64 p1 = (INT64)(strtod(lp1,NULL)*COIN);
		INT64 p2 = (INT64)(strtod(lp2,NULL)*COIN);
	// int型比较
		if (!method)
		return (int)(p1-p2);
		else
		return (int)(p2-p1);
	}
	else if (sort_column==0)
	{
		int p1 = atoi(lp1);
		int p2 = atoi(lp2);
		if (method)
		return p1-p2;
		else
		return p2-p1;
	}
	else
	{
		 //文字型比较
		if (method)
		return lp1.CompareNoCase(lp2);
		else
		return lp2.CompareNoCase(lp1);
	}

	return 0;
}


void CRedPacketList::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	sort_column = pNMLV->iSubItem;//点击的列

	int count = m_listCtrl.GetItemCount();

	for (int i=0;i<count;i++)
		m_listCtrl.SetItemData(i,i); // 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他 比较函数的第一二个参数

	m_listCtrl.SortItems(MyCompareProc,(DWORD_PTR)&m_listCtrl);//排序 第二个参数是比较函数的第三个参数
	method =!method;
	*pResult =0;
}


void CRedPacketList::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogBase::OnOK();
}


void CRedPacketList::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogBase::OnCancel();
}
