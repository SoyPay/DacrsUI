// P2PDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "P2PDlg.h"
#include "afxdialogex.h"
#include "Out.h"
#include "ReCharge.h"

#define OUT_HEIGHT  10

// CP2PDlg 对话框

IMPLEMENT_DYNAMIC(CP2PDlg, CDialogBar)

CP2PDlg::CP2PDlg()
{
	m_pBmp = NULL ;
}

CP2PDlg::~CP2PDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CP2PDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BALANCE , m_Balance);
	DDX_Control(pDX, IDC_STATIC_NOT_DRAW , m_NotDraw);
	DDX_Control(pDX, IDC_STATIC_DW , m_Dw);
	DDX_Control(pDX, IDC_STATIC_VS , m_VS);

	DDX_Control(pDX, IDC_BUTTON_WITHD , m_rBtnWithd);
	DDX_Control(pDX, IDC_BUTTON_RECH , m_rBtnRech);
	DDX_Control(pDX, IDC_BUTTON_MALE , m_rBtnMale);
	DDX_Control(pDX, IDC_BUTTON_WOMAN , m_rBtnWoman);

	DDX_Control(pDX, IDC_LIST_BONUS, m_BonusListBox);
	
	DDX_Control(pDX, IDC_COMBO_ADDRES , m_addrbook);
	//DDX_Control(pDX, IDC_LIST_RECORD, m_RecordListBox);
}


BEGIN_MESSAGE_MAP(CP2PDlg, CDialogBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE( WM_BN_CLICK, &CP2PDlg::onBnCLick)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_ADDRES, &CP2PDlg::OnCbnSelchangeComboAddres)
	ON_MESSAGE(MSG_USER_P2P_UI , &CP2PDlg::OnShowListCtrol )
	ON_BN_CLICKED(IDC_BUTTON_WITHD, &CP2PDlg::OnBnClickedButtonWithd)
	ON_BN_CLICKED(IDC_BUTTON_RECH, &CP2PDlg::OnBnClickedButtonRech)
	ON_BN_CLICKED(IDC_BUTTON_MALE, &CP2PDlg::OnBnClickedButtonMale)
	ON_BN_CLICKED(IDC_BUTTON_WOMAN, &CP2PDlg::OnBnClickedButtonWoman)
END_MESSAGE_MAP()


// CP2PDlg 消息处理程序
void CP2PDlg::SetBkBmpNid( UINT nBitmapIn ) 
{
	if( NULL != m_pBmp ) {
		::DeleteObject( m_pBmp ) ;
		m_pBmp = NULL ;
	}
	m_pBmp	= NULL ;
	HINSTANCE	hInstResource = NULL;	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);
	if( NULL != hInstResource ) {
		m_pBmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
	}
}

int CP2PDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_P2P_BJ);
	return 0;
}


BOOL CP2PDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect   rect; 
	GetClientRect(&rect); 

	if(m_pBmp   !=   NULL) { 
		BITMAP   bm; 
		CDC   dcMem; 
		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
		pDC-> StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


BOOL CP2PDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnMale.LoadBitmaps(IDB_BITMAP_SISTER_1,IDB_BITMAP_SISTER_3,IDB_BITMAP_SISTER_2,IDB_BITMAP_SISTER_1);
		m_rBtnWoman.LoadBitmaps(IDB_BITMAP_BOTHER_1,IDB_BITMAP_BOTHER_3,IDB_BITMAP_BOTHER_2,IDB_BITMAP_BOTHER_1);
		UpdateData(0);
		m_Balance.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_Balance.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_Balance.SetWindowText(_T("100.00SMC"));

		m_NotDraw.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_NotDraw.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_NotDraw.SetWindowText(_T("50.01SMC"));

		m_Dw.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_Dw.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_Dw.SetWindowText(_T("SMC"));

		m_VS.SetFont(120, _T("黑体"));				//设置显示字体和大小
		m_VS.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_VS.SetWindowText(_T(" VS"));

		m_rBtnWithd.SetBitmaps( IDB_BITMAP_P2PBUTTON_1 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_1 , RGB(255, 255, 255) );
		m_rBtnWithd.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnWithd.SetWindowText("提现") ;
		m_rBtnWithd.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnWithd.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnWithd.SizeToContent();

		m_rBtnRech.SetBitmaps( IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 0) , IDB_BITMAP_P2PBUTTON_2 , RGB(255, 255, 255) );
		m_rBtnRech.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnRech.SetWindowText("充值") ;
		m_rBtnRech.SetFontEx(20 , _T("微软雅黑"));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnRech.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnRech.SizeToContent();

		m_BonusListBox.InsertStr(0,this->GetSafeHwnd());
		m_BonusListBox.SetIndexInage(0 , IDB_BITMAP_P2PBUTTON_2, IDB_BITMAP_P2PBUT);
		m_BonusListBox.SetIndexString(0 , _T("51800-68"), _T("接"), _T("1000SMC"), _T("接"));//

		AddListaddrDataBox();
		QueryNotDrawBalance();
		OnListPool();
		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_P2P_UI ) ;
		//m_BonusListBox.InsertStr(1,this->GetSafeHwnd());
		//m_BonusListBox.SetIndexInage(1 , IDB_BITMAP_P2PBUTTON_2, IDB_BITMAP_P2PBUT);
		//m_BonusListBox.SetIndexString(1 , _T("51801-68"), _T("接1"), _T("1001SMC"), _T("接1"));//


		//m_RecordListBox.InsertStr(0,this->GetSafeHwnd());
		//m_RecordListBox.SetIndexInage(0 , IDB_BITMAP_P2PBUTTON_2, IDB_BITMAP_P2PBUT);
		//m_RecordListBox.SetIndexString(0 , _T("51800-68"), _T("接"), _T("1000SMC"), _T("接"));//
	}
	return bRes ;
}
LRESULT CP2PDlg::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	CString str;
	str.Format(_T("adc:%d") , (int)lParam ) ;
	AfxMessageBox(str);
	return 1 ;
}

void CP2PDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		CWnd *pW = (CWnd*)GetDlgItem(IDC_LIST_BONUS);
		if( NULL != pW ) {	
			pW->SetWindowPos(NULL ,454, 80 , 432 , 178 , SWP_SHOWWINDOW);
		}
		pW = (CWnd*)GetDlgItem(IDC_LIST_RECORD);
		if( NULL != pW ) {	
			pW->SetWindowPos(NULL ,10, 352 , 877 , 135 , SWP_SHOWWINDOW);
		}
	}

}


void CP2PDlg::OnCbnSelchangeComboAddres()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	int sel = m_addrbook.GetCurSel();
	m_addrbook.GetLBText(sel,text);

	//m_addrbook.GetWindowText(text) ;
	if(text!=_T(""))
	{
		CString strCommand,strShowData;

		CString strCond;
		strCond.Format(_T(" address = '%s' "), text);
		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		strCommand.Format(_T("%s %s %s"),_T("getappaccinfo") , theApp.m_betScritptid ,addrsql.RegID);
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(strShowData.GetString(), root)) 
			return  ;

		int pos = strShowData.Find("FreeValues");
		INT64 nMoney = 0;
		if (pos >0)
		{
			nMoney = root["FreeValues"].asInt64() ;
		}
		double money = (nMoney*1.0/COIN);
		strShowData.Format(_T("%.8f"),money);

		((CStatic*)GetDlgItem(IDC_STATIC_BALANCE))->SetWindowText(strShowData);
		Invalidate();
	}
}

void CP2PDlg::InsertComboxIitem()
{
	CPostMsg postmsg;
	if (!theApp.m_UiSendDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());

	CString addressd;
	addressd.Format(_T("%s"),addr.address);

	int item = m_addrbook.GetCount();
	m_addrbook.InsertString(item,addressd);
}

BOOL CP2PDlg::AddListaddrDataBox(){

	map<CString,uistruct::LISTADDR_t> m_mapAddrInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), &m_mapAddrInfo);

	if ( 0 == m_mapAddrInfo.size() ) return FALSE ;

	//清除ComBox控件
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->ResetContent();
	//加载到ComBox控件
	int nItem = 0;
	std::map<CString,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_mapAddrInfo.begin() ; const_it != m_mapAddrInfo.end() ; const_it++ ) {

		((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->InsertString(nItem , const_it->first );
		nItem++;
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ADDRES))->SetCurSel(0);

	CString address;
	int sel = m_addrbook.GetCurSel();
	m_addrbook.GetLBText(sel,address);
	OnCbnSelchangeComboAddres();
	return TRUE ;
}
LRESULT CP2PDlg::OnShowListCtrol( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_NEWADDRESS:
		{
			InsertComboxIitem();
			break;
		}
		break;
	default:
		break;

	}
	return 0 ;
}
void  CP2PDlg::QueryNotDrawBalance()
{
	CString strCond;
	strCond.Format(_T(" state = 2 "));

	double   nmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_p2p_quiz") , _T("amount") , strCond) ;
	CString srtShow =_T("");
	if (nmoney <0)
	{
		srtShow = _T("0.0");
	}else{
		srtShow.Format(_T("%.3lf"),nmoney);
	}
	((CStatic*)GetDlgItem(IDC_STATIC_NOT_DRAW))->SetWindowText(srtShow);
	Invalidate();

}

void CP2PDlg::OnBnClickedButtonWithd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	CString strShowData ;

	
	CString addr;
	int sel = m_addrbook.GetCurSel();
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CString strCommand , strMaddress , strMoney;

	string strContractData = m_P2PBetHelp.GetAppAccountMoneyContract(addr.GetString(),1,1);

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	int pos = strShowData.Find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		CString strHash;
		strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		strTip.Format( _T("恭喜提现成功!\n%s") , root["hash"].asCString() ) ;
	}else{
		strTip.Format( _T("提现失败!") ) ;
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}


void CP2PDlg::OnBnClickedButtonRech()
{
	// TODO: 在此添加控件通知处理程序代码

	CReCharge outdlg;
	if ( IDOK != outdlg.DoModal()){
		return;
	}

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	if (theApp.m_strAddress == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strShowData ;
	CString addr;
	int sel = m_addrbook.GetCurSel();
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	CString strCommand , strMaddress , strMoney;

	string strContractData = m_P2PBetHelp.GetReChangContract();

	INT64 strTxFee = theApp.m_P2PBetCfg.GetAppAmountnFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),addr.GetString(),strContractData,0,strTxFee,REAL_MONEY(atof(theApp.m_strAddress) ));
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	int pos = strShowData.Find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		CString strHash;
		strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		strTip.Format( _T("恭喜充值成功!\n%s") , root["hash"].asCString() ) ;
	}else{
		strTip.Format( _T("充值失败!") ) ;
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}

void CP2PDlg::SendBet(int rewardnum)
{
	// TODO: 在此添加控件通知处理程序代码  PacketP2PSendContract


	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}

	CString addr;
	int sel = m_addrbook.GetCurSel();
	m_addrbook.GetLBText(sel,addr);

	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("地址不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	//// 查询地址是否激活
	CString strCond;
	strCond.Format(_T(" address = '%s' "), addr);
	uistruct::LISTADDR_t addrsql;
	int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;
	if (!addrsql.bSign)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("账户未激活不能发生合约交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	char strTemp[34];
	memset(strTemp , 0 , 34 );
	memcpy(strTemp , UiFun::Rnd32() , 32 );
	strTemp[32] =rewardnum ;


	string temp(strTemp,strTemp+33);
	int aa = temp.length() ;
	CString strCommand;
	strCommand.Format(_T("%s %s"),_T("gethash") , strTemp );
	CStringA strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	int pos = strShowData.Find("hash");
	if ( pos < 0 ) return ;

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;

	CString strHash ;
	strHash.Format(_T("%s") ,  root["hash"].asCString() ) ;


	string strContractData;
	string strRamdHash = CSoyPayHelp::getInstance()->GetReverseHash(strHash.GetString());
	strContractData = m_P2PBetHelp.PacketP2PSendContract((INT64)REAL_MONEY(atof(strTxMoney)),OUT_HEIGHT ,strRamdHash );

	INT64 strTxFee = theApp.m_P2PBetCfg.SendBetFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("小费不足") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData.c_str(),strShowData);

	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	BOOL bRes = FALSE ;
	CString strTip;
	pos = strShowData.Find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		CString strHash ;
		strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		strTip.Format( _T("恭喜发送赌约成功!\n%s") , root["hash"].asCString() ) ;
	}else{
		strTip.Format( _T("发送赌约失败!") ) ;
	}

	//保存到数据库
	if ( bRes ) {
		uistruct::P2P_QUIZ_RECORD_t p2pbetrecord ;
		memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_QUIZ_RECORD_t));
		SYSTEMTIME curTime ;
		memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
		GetLocalTime( &curTime ) ;
		CString strSendTime;
		strSendTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		p2pbetrecord.send_time = UiFun::SystemTimeToTimet(curTime);
		p2pbetrecord.time_out  = OUT_HEIGHT ;
		memcpy(p2pbetrecord.tx_hash ,root["hash"].asCString() , sizeof(p2pbetrecord.tx_hash));
		p2pbetrecord.tx_hash[64] = '\0';
		memcpy(p2pbetrecord.left_addr ,addr , sizeof(p2pbetrecord.left_addr));
		p2pbetrecord.amount = atof(strTxMoney) ;
		memcpy(p2pbetrecord.content ,strTemp , sizeof(p2pbetrecord.content));

		p2pbetrecord.actor  = 0 ;
		p2pbetrecord.state  = 0 ;
		//插入到数据库
		CString strSourceData;
		strSourceData.Format(_T("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'") , \
			strSendTime , _T("") , p2pbetrecord.time_out , \
			p2pbetrecord.tx_hash ,  p2pbetrecord.left_addr , p2pbetrecord.right_addr ,p2pbetrecord.amount);

		strSourceData.AppendFormat(",'%s' ,'%d','%d','%d','%d','%s','%d'",p2pbetrecord.content ,p2pbetrecord.actor ,p2pbetrecord.confirmed ,p2pbetrecord.height ,p2pbetrecord.state ,\
			p2pbetrecord.relate_hash ,p2pbetrecord.guess_num ) ;

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData.GetString();
		pDatabase.strTabName =  _T("p2p_bet_record");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();
		CString strShow;
		strShow.Format(_T("%s"),strTemp.c_str());
		postmsg.SetData(strShow);
		theApp.m_MsgQueue.push(postmsg);
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}

void CP2PDlg::OnBnClickedButtonMale()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strDisplay;
	strDisplay.Format(_T("确定要发送赌约妹--->金额:%s"), strTxMoney);
	COut outdlg(NULL, strDisplay,100);
	if ( IDOK != outdlg.DoModal()){
		return;
	}
	SendBet(0);
}


void CP2PDlg::OnBnClickedButtonWoman()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxMoney;
	GetDlgItem(IDC_EDIT_MONEY)->GetWindowText(strTxMoney) ;
	if (strTxMoney == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("金额不能为空") , _T("提示") , MB_ICONINFORMATION ) ;
		return ;
	}
	CString strDisplay;
	strDisplay.Format(_T("确定要发送赌约哥--->金额:%s"), strTxMoney);
	COut outdlg(NULL, strDisplay,100);
	if ( IDOK != outdlg.DoModal()){
		return;
	}
	SendBet(1);
}
void CP2PDlg::OnListPool()
{
	uistruct::P2PLIST pPoolList;
	theApp.m_SqliteDeal.GetP2PQuizPoolList(_T(" 1=1 "), &pPoolList);
	if (pPoolList.size() == 0)
	{
		return ;
	}

	int nSubIdx = 0 , i = 0 ;
	CString strShowData = _T("");
	std::vector<uistruct::LISTP2POOL_T>::const_iterator const_it;
	for ( const_it = pPoolList.begin() ; const_it != pPoolList.end() ; const_it++ ) {

		string nValue = const_it->data;
		uistruct::DBBET_DATA DBbet;
		memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
		memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

		CString addr,money;
		std::vector<unsigned char> vSendid;
		vSendid.assign(DBbet.sendbetid,DBbet.sendbetid+sizeof(DBbet.sendbetid));

		CString regid = CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);
		CString strCond;
		strCond.Format(_T(" reg_id = '%s' "), regid);
		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		addr.Format(_T("%s"),addrsql.address);

		double dmoney = (DBbet.money*1.0)/COIN;
		money.Format(_T("%.4f"),dmoney);
		m_BonusListBox.InsertStr(i,this->GetSafeHwnd());
		m_BonusListBox.SetIndexInage(i , IDB_BITMAP_P2PBUTTON_2, IDB_BITMAP_P2PBUT);
		m_BonusListBox.SetIndexString(i , addr, _T("接"), money, _T("接"));


		//nSubIdx = 0;
		//CString strOrder("");
		//strOrder.Format(_T("%d"), i+1);
		//m_listCtrl.InsertItem(i,strOrder);

		//uistruct::LISTADDR_t address = const_it->second;
		//strShowData.Format(_T("%s") ,address.Label) ;
		//m_listCtrl.SetItemText( i , ++nSubIdx, strShowData) ;
		//m_listCtrl.SetItemData(item , (DWORD_PTR)&(*const_it)) ;


		//strShowData.Format(_T("%s") ,address.address) ;
		//m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		//if (address.bSign == 1)
		//{
		//	strShowData.Format(_T("已激活")) ;
		//}else{
		//	strShowData.Format(_T("未激活")) ;
		//}

		//m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;
		//strShowData.Format(_T("%.8f") , address.fMoney ) ;
		//m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		//if (address.nColdDig== 1)
		//{
		//	strShowData.Format(_T("支持")) ;
		//}else{
		//	strShowData.Format(_T("不支持")) ;
		//}
		//m_listCtrl.SetItemText(i , ++nSubIdx , strShowData ) ;

		i++;
	}
}