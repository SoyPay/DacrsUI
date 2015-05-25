// SendRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "SendRecord.h"
#include "afxdialogex.h"


// CSendRecord 对话框

IMPLEMENT_DYNAMIC(CSendRecord, CDialogEx)

CSendRecord::CSendRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendRecord::IDD, pParent)
{
	m_pBmp = NULL ;
	m_addr = _T("");
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
}


BEGIN_MESSAGE_MAP(CSendRecord, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_MESSAGE( WM_BN_CLICK, &CSendRecord::onBnCLick)
	ON_WM_SIZE()
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
			pst->SetWindowPos( NULL ,0 , 33 , 875, 185  ,SWP_SHOWWINDOW ) ; 
		}
	}
}


BOOL CSendRecord::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ) { 
		UpdateData(0);

		//m_listBox.InsertStr(0,this->GetSafeHwnd());
		//m_listBox.SetIndexInage(0 , IDB_BITMAP_P2P_LISTBOX_BUT);
		//m_listBox.SetIndexBackCol(0, 0, RGB(242,32,32));
		//m_listBox.SetIndexString(0 , _T("durCyWC8MTdQdpCo9QXZ5wxSyL9jtyDFri"), _T("2011-01-01 01:01:01"), _T("10000"), _T("接"));
	}
	return bRes ;
}

LRESULT CSendRecord::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	List_SendAppendData* pinf = m_listBox.GetAppendDataInfo((int)wParam);
	if ( NULL != pinf ) { 
		CString hash = pinf->pstr;
		OpenBet(hash);
	}
	
	return 0;

}
void CSendRecord::Showlistbox(CString address)
{
	//// 查找数据库中是否存在此记录
	m_addr = address;
	m_listBox.ResetContent();
	CString conditon;
	conditon.Format(_T("left_addr ='%s' and (actor = 0 or actor = 2)") , address);
	uistruct::P2PBETRECORDLIST  pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon ,&pPoolItem ) ;
	if (pPoolItem.size() != 0) ///此记录不存在,插入记录
	{
		int i = 0;
		std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
		for (const_it = pPoolItem.begin();const_it!=pPoolItem.end();const_it++)
		{
			CString dmoney,reward,result;

			m_listBox.InsertStr(i,this->GetSafeHwnd());
			m_listBox.SetIndexInage(i , IDB_BITMAP_P2P_LISTBOX_BUT);
			m_listBox.SetIndexBackCol(i, 0, RGB(242,32,32));
			///说明开奖了
			if (const_it->state == 2 || const_it->state == 1)
			{
				SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it->send_time);
				CString strTime;
				strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
				if (const_it->guess_num == const_it->content[32])
				{
					reward.Format(_T("+%.4f"),const_it->amount);
				}else
				{
					reward.Format(_T("-%.4f"),const_it->amount);
				}
				if (const_it->content[32] == 0)
				{
					result.Format(_T("%s"),"妹");
				}else
				{
					result.Format(_T("%s"),"哥");
				}
				CString recaddr;
				recaddr.Format(_T("%s"),const_it->right_addr);
				if (const_it->state == 2)
				{
					m_listBox.SetIndexString(i , recaddr,strTime, reward, _T("已开"),const_it->tx_hash);
				}else{
					m_listBox.SetIndexString(i , recaddr,strTime, reward, _T("开"),const_it->tx_hash);
				}
				

			}else
			{
				reward.Format(_T("%.4f"),const_it->amount);
				m_listBox.SetIndexString(i , _T("--"),_T("--"), reward,_T("未接奖"),const_it->tx_hash);
			}
			i++;
		}
	}
}
void CSendRecord::OpenBet(CString txhash)
{

	if (m_addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("请选择地址") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	CString conditon;
	conditon.Format(_T("tx_hash ='%s'") , txhash );
	uistruct::P2P_QUIZ_RECORD_t pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
	if (strlen(pPoolItem.tx_hash) == 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("数据库中无此记录") , _T("提示") , MB_ICONINFORMATION ) ;
	}

	CString randnumber,strShowData,txaccepthash;
	randnumber.Format(_T("%s"),pPoolItem.content);
	txaccepthash.Format(_T("%s"),pPoolItem.relate_hash);
	string sendhash = CSoyPayHelp::getInstance()->GetReverseHash(txhash.GetBuffer());
	string accepthash = CSoyPayHelp::getInstance()->GetReverseHash(txaccepthash.GetBuffer());
	
	string number;
	number.assign(pPoolItem.content,pPoolItem.content+sizeof(pPoolItem.content));
	string strContractData = m_P2PBetHelp.PacketP2PExposeContract(sendhash,number,accepthash,pPoolItem.time_out);

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid.GetBuffer(),m_addr.GetString(),strContractData,0,theApp.m_P2PBetCfg.OpenBetnFee,0);
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
		CString strHash ;
		strHash.Format(_T("'%s'") , root["hash"].asCString() );
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		strTip.Format( _T("恭喜开奖成功!\n%s") , root["hash"].asCString() ) ;
	}else{
		strTip.Format( _T("开奖失败!") ) ;
	}

	//保存到数据库
	if ( bRes ) {

		//插入到交易记录数据库

		//// 查找数据库中是否存在此记录
		CString conditon;
		conditon.Format(_T("tx_hash ='%s'") , txhash );
		uistruct::P2P_QUIZ_RECORD_t pPoolItem;
		int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
		if (strlen(pPoolItem.tx_hash) == 0) ///此记录不存在,插入记录
		{
			CString strSourceData  , strW ;
			strSourceData.Format(_T("state = %d") , 5  ) ;
			strW.Format(_T("tx_hash = '%s'") , txhash ) ;

			uistruct::DATABASEINFO_t DatabaseInfo;
			DatabaseInfo.strSource = strSourceData.GetString();
			DatabaseInfo.strWhere = strW.GetString() ;
			DatabaseInfo.strTabName = _T("p2p_bet_record");
			CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			string strtemp = DatabaseInfo.ToJson();
			CString pstr;
			pstr.Format(_T("%s"),strtemp.c_str());
			postmsg.SetData(pstr);
			theApp.m_MsgQueue.push(postmsg);


		}
	}
	::MessageBox( this->GetSafeHwnd() ,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}