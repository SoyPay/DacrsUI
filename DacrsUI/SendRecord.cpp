// SendRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SendRecord.h"
#include "afxdialogex.h"
#include "BetInformation.h"
#include "DacrsUIDlg.h"
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorksheet.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
// CSendRecord 对话框

IMPLEMENT_DYNAMIC(CSendRecord, CDialogEx)

CSendRecord::CSendRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendRecord::IDD, pParent)
{
	m_pBmp = NULL ;
	m_addr = _T("");
	m_pagecount = 0;
	m_curpage = 0;
	m_PoolList.clear();
	m_pagesize = 4;
}

CSendRecord::~CSendRecord()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CSendRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_listBox);
	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CSendRecord, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_MESSAGE( WM_BN_CLICK, &CSendRecord::onBnCLick)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UP, &CSendRecord::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CSendRecord::OnBnClickedButtonNext)
	ON_LBN_DBLCLK(IDC_LIST_BOX, &CSendRecord::OnLbnDblclkListBox)
END_MESSAGE_MAP()


// CSendRecord 消息处理程序
void CSendRecord::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CSendRecord::OnEraseBkgnd(CDC* pDC)
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


int CSendRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid(IDB_BITMAP_SEND_BET) ;
	return 0;
}


void CSendRecord::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		CWnd *pst = GetDlgItem( IDC_LIST_BOX ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,0 , 33 , 875,130 ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_UP ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-140 , 130+33 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875 - 115, 130+33 , 40, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_NEXT ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-70 , 130+33 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,875-50 ,130+36 , 50, 20  ,SWP_SHOWWINDOW ) ; 
		}
	}
}


BOOL CSendRecord::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ) { 
		UpdateData(0);

		m_sCountpage.SetFont(90, _T("黑体"));				//设置显示字体和大小
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //字体颜色	
		m_sCountpage.SetWindowText(_T("共:"));

		m_rBtnUp.SetBitmaps( IDB_BITMAP_UP , RGB(255, 255, 0) , IDB_BITMAP_UP , RGB(255, 255, 255) );
		m_rBtnUp.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnUp.SetWindowText("") ;
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnUp.SizeToContent();

		m_rBtnNext.SetBitmaps( IDB_BITMAP_NEXT , RGB(255, 255, 0) , IDB_BITMAP_NEXT , RGB(255, 255, 255) );
		m_rBtnNext.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNext.SetWindowText("") ;
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNext.SizeToContent();

		GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T("共:0"));
	}
	return bRes ;
}

LRESULT CSendRecord::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	List_SendAppendData* pinf = m_listBox.GetAppendDataInfo((int)wParam);
	if ( NULL != pinf ) { 
		CString hash = pinf->pstr;
		//OpenBet(hash);
	}
	
	return 0;

}
void CSendRecord::Showlistbox(CString address)
{
	//// 查找数据库中是否存在此记录
	m_addr = address;
	m_curpage = 0;
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T(""));
	m_PoolList.clear();
	string conditon;
	conditon= strprintf("left_addr ='%s' and (actor = 0 or actor = 2) order by send_time desc" , address);
	//uistruct::P2PBETRECORDLIST  pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon ,&m_PoolList ) ;
	m_pagecount = (m_PoolList.size()%m_pagesize)==0?(m_PoolList.size()/m_pagesize):(m_PoolList.size()/m_pagesize)+1;

	string temp;
	temp = strprintf("共:%d",m_pagecount);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_listBox.DeleteAllIndex();
	OnShowPagePool(1);

	OpenAcceptbet();
}
void CSendRecord::OpenBet(CString txhash)
{
	if (!theApp.IsSyncBlock )
	{
		
		UiFun::MessageBoxEx(_T("同步未完成,不能发送交易") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}

	if (m_addr == _T(""))
	{
		
		UiFun::MessageBoxEx(_T("请选择地址") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}
	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	string conditon;
	conditon= strprintf("tx_hash ='%s'" , txhash );
	uistruct::P2P_QUIZ_RECORD_t pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
	if (pPoolItem.tx_hash == "")
	{
		
		UiFun::MessageBoxEx(_T("数据库中无此记录") , _T("提示") ,MFB_OK|MFB_TIP );
	}

	string strCommand1;
	strCommand1 = strprintf("%s %s","gethash" , pPoolItem.content );
	string strShowData1 ;

	Json::Value root1; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand1,root1))
	{
		TRACE("OpenBet rpccmd gethash error");
		return;
	}

	string strHash1 =root1["hash"].asString();
	

	TRACE(_T("contect:%s\r\n"),pPoolItem.content);
	TRACE(_T("open:%s\r\n"),strHash1.c_str());

	string randnumber,strShowData,txaccepthash;
	randnumber = strprintf("%s",pPoolItem.content);
	txaccepthash = strprintf("%s",pPoolItem.relate_hash.c_str());
	string sendhash = CSoyPayHelp::getInstance()->GetReverseHash(txhash.GetString());
	string accepthash = CSoyPayHelp::getInstance()->GetReverseHash(txaccepthash);
	
	string number;
	number.assign(pPoolItem.content,pPoolItem.content+sizeof(pPoolItem.content));
	string strContractData = m_P2PBetHelp.PacketP2PExposeContract(sendhash,number,accepthash,pPoolItem.time_out);

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,m_addr.GetString(),strContractData,0,theApp.m_P2PBetCfg.OpenBetnFee,0);
	strShowData = "";
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash = root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜开奖成功!\n%s") , root["hash"].asCString() ) ;
		 strTip = "恭喜开奖成功，请等待1-2分钟确认交易\n" ;
	}else{
		strTip="开奖失败!" ;
	}

	//保存到数据库
	if ( bRes ) {

		//插入到交易记录数据库

		//// 查找数据库中是否存在此记录
		string conditon;
		conditon = strprintf("tx_hash ='%s'" , txhash );
		uistruct::P2P_QUIZ_RECORD_t pPoolItem;
		int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
		if (pPoolItem.tx_hash=="") ///此记录不存在,插入记录
		{
			string strSourceData  , strW ;
			strSourceData = strprintf("state = %d" , 5  ) ;
			strW =strprintf("tx_hash = '%s'" , txhash ) ;

			uistruct::DATABASEINFO_t DatabaseInfo;
			DatabaseInfo.strSource = strSourceData;
			DatabaseInfo.strWhere = strW;
			DatabaseInfo.strTabName = _T("p2p_bet_record");
			CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			string strtemp = DatabaseInfo.ToJson();

			postmsg.SetData(strtemp);
			theApp.m_MsgQueue.push(postmsg);


		}
	}

	UiFun::MessageBoxEx(strTip.c_str() , _T("提示") ,MFB_OK|MFB_TIP );
}
///开接赌了没有开奖的
void  CSendRecord::OpenAcceptbet()
{

	string conditon;
	conditon= "comfirmed=0 and state =1 and (actor = 0 or actor =2)";
	uistruct::P2PBETRECORDLIST       lList;;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon ,&lList ) ;

	for (unsigned int k = 0;k<lList.size();k++)
	{
		uistruct::P2P_QUIZ_RECORD_t const_it = lList.at(k);

		///说明开奖了
		if (const_it.state == 1)
		{
	
			if ((const_it.time_out + const_it.height)> theApp.blocktipheight && theApp.IsSyncBlock)
			{
				string strCond;
				uistruct::LISTADDR_t pAddr;
				strCond = strprintf("reg_id ='%s'",const_it.left_addr.c_str());
				theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &pAddr);
				double minfee = (theApp.m_P2PBetCfg.OpenBetnFee*1.0)/COIN;
				if (minfee > pAddr.fMoney)
				{
				
					UiFun::MessageBoxEx(_T("有些赌约未开奖,请先充值") , _T("提示") ,MFB_OK|MFB_TIP );
				}
				CString txhash;
				txhash.Format(_T("%s"),const_it.tx_hash.c_str());
				theApp.OpenBet(txhash);
			}
		}
	}
}
void  CSendRecord::OnShowPagePool(int page)
{
	if (page >m_pagecount || page == m_curpage || page <= 0)
	{
		return;
	}


	m_listBox.DeleteAllIndex();
	string strpage;
	strpage = strprintf("%d",page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());

	bool flag = false;
	m_curpage = page;
	int index = (page-1)*m_pagesize;
	unsigned int count = (m_PoolList.size() -index)>=m_pagesize?m_pagesize:(m_PoolList.size() -index);

	string SendTime;
	string dmoney,reward,result,guess;
	string strTime;
	string time;

	int i = 0;
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	for (unsigned int k = index;k< (index+count) && k<m_PoolList.size();k++)
	{
		uistruct::P2P_QUIZ_RECORD_t const_it = m_PoolList.at(k);


		if (const_it.send_time != 0)
		{
			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.send_time);
			SendTime =strprintf("%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		}else{
			SendTime = "--";
		}


		if (const_it.content[32] == 1)
		{
			result = "妹";
		}else
		{
			result = "哥";
		}
		reward =strprintf("%.4f",const_it.amount);

		m_listBox.InsertStr(i,this->GetSafeHwnd());
		m_listBox.SetIndexInage(i , IDB_BITMAP_P2PLIST);
		m_listBox.SetIndexBackCol(i, 1, RGB(242,32,32));
		m_listBox.SetIndexBackCol(i, 5, RGB(242,32,32));
		List_SendAppendData* pinf = m_listBox.GetAppendDataInfo(i);

		///说明开奖了
		if (const_it.state == 2 || const_it.state == 1)
		{
			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.recv_time);

			strTime =strprintf("%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		
			if (const_it.guess_num == 1)
			{
				guess = "妹";
			}else
			{
				guess = "哥";
			}
			if (const_it.state == 2)  /// 开奖
			{
				if (const_it.guess_num == const_it.content[32])
				{
					m_listBox.SetIndexBackCol(i, 6, RGB(1,127,1));
					reward =strprintf("-%.4f",const_it.amount);
				}else
				{
					m_listBox.SetIndexBackCol(i, 6, RGB(242,32,32));
					reward =strprintf("+%.4f",const_it.amount);
				}
			}

			
			time_t timeout =const_it.recv_time+const_it.time_out*60; 
			SYSTEMTIME curTimeOut =UiFun::Time_tToSystemTime(timeout);
			time =strprintf("%02d:%02d:%02d",curTimeOut.wHour, curTimeOut.wMinute, curTimeOut.wSecond);
		
			if (const_it.state == 2)
			{
				m_listBox.SetIndexString(i ,const_it.left_addr.c_str(), const_it.right_addr.c_str(),SendTime.c_str(),strTime.c_str(), result.c_str(),guess.c_str(), reward.c_str(),time.c_str(),_T("已开"),const_it.tx_hash.c_str());
			}else{
				if ((const_it.time_out + const_it.height)> theApp.blocktipheight && theApp.IsSyncBlock)
				{
					reward =strprintf("%.4f",const_it.amount);
					m_listBox.SetIndexString(i , const_it.left_addr.c_str(), const_it.right_addr.c_str(),SendTime.c_str(),strTime.c_str(), result.c_str(),_T("--"),reward.c_str(),time.c_str(), _T("待开"),const_it.tx_hash.c_str());
				}else if(theApp.IsSyncBlock && const_it.height != 0 &&(const_it.time_out + const_it.height)< theApp.blocktipheight){
					m_listBox.SetIndexBackCol(i, 6, RGB(1,127,1));
					reward =strprintf("-%.4f",const_it.amount);
					m_listBox.SetIndexString(i , const_it.left_addr.c_str(), const_it.right_addr.c_str(),SendTime.c_str(),strTime.c_str(), result.c_str(),guess.c_str(),reward.c_str(),time.c_str(), _T("超时"),const_it.tx_hash.c_str());
				}else{
					m_listBox.SetIndexString(i , const_it.left_addr.c_str(), const_it.right_addr.c_str(),SendTime.c_str(),strTime.c_str(), result.c_str(),_T("--"),reward.c_str(),time.c_str(), _T("待开"),const_it.tx_hash.c_str());
				}

			}


		}else
		{
			reward =strprintf("%.4f",const_it.amount);
			if (const_it.state == 0 &&(500 + const_it.height)> theApp.blocktipheight&& theApp.IsSyncBlock )
			{
				m_listBox.SetIndexString(i , const_it.left_addr.c_str(), const_it.right_addr.c_str(),SendTime.c_str(),_T("--"), result.c_str(),_T("--"),reward.c_str(),_T(""),_T("未接"),const_it.tx_hash.c_str());
			}else if(theApp.IsSyncBlock&&const_it.height !=0 && (500 + const_it.height)< theApp.blocktipheight){
				m_listBox.SetIndexString(i , const_it.left_addr.c_str(), const_it.right_addr.c_str(),SendTime.c_str(),_T("--"), result.c_str(),_T("--"),reward.c_str(),_T(""),_T("超时"),const_it.tx_hash.c_str());
			}else
			{
				m_listBox.SetIndexString(i , const_it.left_addr.c_str(), const_it.right_addr.c_str(),SendTime.c_str(),_T("--"), result.c_str(),_T("--"),reward.c_str(),_T(""),_T("未接"),const_it.tx_hash.c_str());
			}

		}
		i++;
	}
}
BOOL CSendRecord::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)  
	{   
		if(pMsg->wParam == VK_RETURN)  
		{  
			if (GetDlgItem(IDC_EDIT_PAGE) == this->GetFocus())
			{
				CString num;
				GetDlgItem(IDC_EDIT_PAGE)->GetWindowText(num);
				if (IsAllDigtal(num))
				{
					OnShowPagePool(atoi(num));
				}else
				{
					GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
					
					UiFun::MessageBoxEx(_T("输入有误,请输入数字") , _T("提示") ,MFB_OK|MFB_TIP );
				}
				return TRUE;
			}
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CSendRecord::OnBnClickedButtonUp()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowPagePool((m_curpage-1));
}


void CSendRecord::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	OnShowPagePool((m_curpage+1));
}


void CSendRecord::OnLbnDblclkListBox()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel=m_listBox.GetCurSel(); 
	int count = ((m_curpage -1)*m_pagesize) + nSel;
	if (count <=(int)m_PoolList.size())
	{
		uistruct::P2P_QUIZ_RECORD_t const_it = m_PoolList.at(count);
		string temp = const_it.ToJson();
		theApp.m_strAddress.Format(_T("%s") ,temp.c_str() ) ;
		CBetInformation dlg;
		dlg.DoModal();	
	}
}



void CSendRecord::ExportSendBetRecordToexel()
{
	// TODO: 在此添加控件通知处理程序代码
	string conditon;
	conditon = "actor = 0 or actor = 2 order by recv_time desc";

	uistruct::P2PBETRECORDLIST PoolList;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon.c_str() ,&PoolList ) ;
	if (PoolList.size() == 0)
	{
		UiFun::MessageBoxEx(_T("没有记录可以导出！") , _T("提示") ,MFB_OK|MFB_TIP );
		return;
	}
	CFileDialog dlg(FALSE,NULL,"猜你妹发起记录",OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,"文件 (*.xls)|*.xls||");

	if (IDOK != dlg.DoModal())
	{
		return;
	}

	CString strFile = dlg.GetPathName();
	if (!((CDacrsUIDlg*)(theApp.m_pMainWnd))->GetFileName(strFile,_T(".xls")))
	{
		return;
	}

	struct LISTCol {
		string		name ;
		UINT		size ;
	} listheadr[10]  = {
		{"发起竞猜hash" ,  65},
		{"发起人" ,    10},
		{"接单人" ,   10},
		{"发起时间" ,  15}, 
		{"接单时间" ,  15}, 
		{"底牌" ,8},
		{"猜测" ,8},
		{"金额" ,20},
		{"超时",10},
		{"操作",10}

	};

	COleVariant

	covTrue((short)TRUE),

	covFalse((short)FALSE),

	covOptional((long)DISP_E_PARAMNOTFOUND,   VT_ERROR);

	CApplication   app;

	CWorkbooks   books;

	CWorkbook   book;

	CWorksheets   sheets;

	CWorksheet   sheet;

	CRange   range;

	CFont0   font;



	if (!app.CreateDispatch(_T("Excel.Application")))

	{

		
		UiFun::MessageBoxEx(_T("可能是没有装office 导致创建失败！") , _T("提示") ,MFB_OK|MFB_TIP );
		return;

	}



	//Get   a   new   workbook.

	books = app.get_Workbooks();

	book = books.Add(covOptional);



	sheets = book.get_Worksheets();

	sheet = sheets.get_Item(COleVariant((short)1));



	////////////////////////////////////CListCtrl控件report风格//////////////////////////////////////////////////////////

	//CHeaderCtrl   *pmyHeaderCtrl;

	//pmyHeaderCtrl = m_listCtrl.GetHeaderCtrl();//此句取得CListCtrl控件的列表頭



	int   iRow,iCol;

	int   m_cols   =   10;

	int   m_rows = PoolList.size();

	HDITEM   hdi;

	TCHAR     lpBuffer[256];

	bool       fFound   =   false;



	hdi.mask   =   HDI_TEXT;

	hdi.pszText   =   lpBuffer;

	hdi.cchTextMax   =   256;

	CString   colname;

	CString strTemp;

	for(iCol=0;   iCol <m_cols;   iCol++)//将列表的标题头写入EXCEL

	{

		UiFun::GetCellName(1 ,iCol + 1, colname);

		range   =   sheet.get_Range(COleVariant(colname),COleVariant(colname));

		//pmyHeaderCtrl-> GetItem(iCol,   &hdi);

		range.put_Value2(COleVariant(listheadr[iCol].name.c_str()));

		int   nWidth   = listheadr[iCol].size;  //m_listCtrl.GetColumnWidth(iCol)/6;

		//得到第iCol+1列  

		range.AttachDispatch(range.get_Item(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);  

		//设置列宽 

		range.put_ColumnWidth(_variant_t((long)nWidth));

	}

	range   =   sheet.get_Range(COleVariant( _T("A1 ")),   COleVariant(colname));

	range.put_RowHeight(_variant_t((long)50));//设置行的高度

	font = range.get_Font();

	font.put_Bold(covTrue);

	range.put_VerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108



	COleSafeArray   saRet;

	DWORD   numElements[]={m_rows,m_cols};       //5x2   element   array

	saRet.Create(VT_BSTR,   2,   numElements);

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range = range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));

	long   index[2];

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range   =   range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));


	int iLine = 0;
	iRow   =   1;
	iCol   =   1;

	vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator pitem = PoolList.begin();
	for(;pitem != PoolList.end();pitem++,iRow++)
	{
		map<int,string> item;
		GetExportCol(item,*pitem);
		for   (   iCol   =   1;   iCol   <=   m_cols;   iCol++)  

		{

			index[0]=iRow-1;

			index[1]=iCol-1;
			string strTemp =  item[iCol-1];
			CString   szTemp = strTemp.c_str();

			BSTR   bstr   =   szTemp.AllocSysString();

			saRet.PutElement(index,bstr);

			SysFreeString(bstr);

		}
	}


	range.put_Value2(COleVariant(saRet));


	saRet.Detach();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	book.SaveCopyAs(COleVariant(strFile));

	//       cellinterior.ReleaseDispatch();

	book.put_Saved(true);

	book.ReleaseDispatch();  

	books.ReleaseDispatch();  

	app.Quit();

	app.ReleaseDispatch();
}
void CSendRecord::GetExportCol(map<int,string> &item,uistruct::P2P_QUIZ_RECORD_t const_it)
{
	int i =0 ;
	item[i++] =const_it.tx_hash;
	item[i++] =const_it.left_addr;
	item[i++] =const_it.right_addr;

	string SendTime,result,reward,reciveTime,guess;

	if (const_it.send_time != 0)
	{
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.send_time);
		SendTime = strprintf("%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}else{
		SendTime = "--";
	}
	item[i++] =SendTime;

	if (const_it.content[32] == 1)
	{
		result = "妹";
	}else
	{
		result = "哥";
	}

	reward = strprintf("%.4f",const_it.amount);

	///说明开奖了
	if (const_it.state == 2 || const_it.state == 1)
	{
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.recv_time);

		reciveTime= strprintf("%02d-%02d %02d:%02d:%02d",curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

		if (const_it.guess_num == 1)
		{
			guess = "妹";
		}else
		{
			guess = "哥";
		}
		if (const_it.state == 2)  /// 开奖
		{
			if (const_it.guess_num == const_it.content[32])
			{
				reward =strprintf("-%.4f",const_it.amount);
			}else
			{
				reward =strprintf("+%.4f",const_it.amount);
			}
		}
			time_t timeout =const_it.recv_time+const_it.time_out*60; 
			SYSTEMTIME curTimeOut =UiFun::Time_tToSystemTime(timeout);
			string time =strprintf("%02d:%02d:%02d",curTimeOut.wHour, curTimeOut.wMinute, curTimeOut.wSecond);

			if (const_it.state == 2)
			{
				item[i++] =reciveTime;
				item[i++] =result;
				item[i++] =guess;
				item[i++] =reward;
				item[i++] =time;
				item[i++] ="已开";
			}else{
				if ((const_it.time_out + const_it.height)> theApp.blocktipheight && theApp.IsSyncBlock)
				{
					reward= strprintf("%.4f",const_it.amount);
					item[i++] =reciveTime;
					item[i++] =result;
					item[i++] ="--";
					item[i++] =reward;
					item[i++] =time;
					item[i++] ="待开";
				}else if(theApp.IsSyncBlock && const_it.height != 0 &&(const_it.time_out + const_it.height)< theApp.blocktipheight){
				reward= strprintf("-%.4f",const_it.amount);
				item[i++] =reciveTime;
				item[i++] =result;
				item[i++] =guess;
				item[i++] =reward;
				item[i++] =time;
				item[i++] ="超时";
				}else{
					reward= strprintf("%.4f",const_it.amount);
					item[i++] =reciveTime;
					item[i++] =result;
					item[i++] ="--";
					item[i++] =reward;
					item[i++] =time;
					item[i++] ="待开";
				}

			}
		}else
		{
			reward= strprintf("%.4f",const_it.amount);
			if (const_it.state == 0 &&(500 + const_it.height)> theApp.blocktipheight&& theApp.IsSyncBlock )
			{
				item[i++] ="--";
				item[i++] =result;
				item[i++] ="--";
				item[i++] =reward;
				item[i++] ="--";
				item[i++] ="未接";
			}else if(theApp.IsSyncBlock&&const_it.height !=0 && (500 + const_it.height)< theApp.blocktipheight){
				item[i++] ="--";
				item[i++] =result;
				item[i++] ="--";
				item[i++] =reward;
				item[i++] ="--";
				item[i++] ="超时";
			}else
			{
				item[i++] ="--";
				item[i++] =result;
				item[i++] ="--";
				item[i++] =reward;
				item[i++] ="--";
				item[i++] ="未接";
			}

		}
}