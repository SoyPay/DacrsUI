// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include "DacrsUIDlg.h"
#include <afxinet.h>

// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogBar)

CMainDlg::CMainDlg()
{
	m_pBmp = NULL ;
	m_url.clear();
}

CMainDlg::~CMainDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	if (CMFCVisualManager::GetInstance() != NULL)
	{
		delete CMFCVisualManager::GetInstance();
	}
	v_linkCtrl.InternalRelease();
	v_linkCtrl.ExternalRelease();
	v_linkCtrl.OnFinalRelease();
	v_linkCtrl.DestroyWindow();

	v_linkCtrl1.InternalRelease();
	v_linkCtrl1.ExternalRelease();
	v_linkCtrl1.OnFinalRelease();
	v_linkCtrl1.DestroyWindow();

	v_linkCtrl2.InternalRelease();
	v_linkCtrl2.ExternalRelease();
	v_linkCtrl2.OnFinalRelease();
	v_linkCtrl2.DestroyWindow();

	v_linkCtrl3.InternalRelease();
	v_linkCtrl3.ExternalRelease();
	v_linkCtrl3.OnFinalRelease();
	v_linkCtrl3.DestroyWindow();
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALLTXDETAIL , m_rBtnAllTxdetail);
	DDX_Control(pDX, IDC_TX_JY2 , m_strTrading);
	DDX_Control(pDX, IDC_TX_JY3 , m_strTrading2);
	DDX_Control(pDX, IDC_TX_JY4 , m_strTrading3);
	DDX_Control(pDX, IDC_TX_JY5 , m_strTrading4);
	DDX_Control(pDX, IDC_TX_JY7 , m_strTrading5);

	DDX_Control(pDX, IDC_TX1 , m_strTx1);
	DDX_Control(pDX, IDC_TX2 , m_strTx2);
	DDX_Control(pDX, IDC_TX3 , m_strTx3);
	DDX_Control(pDX, IDC_TX4 , m_strTx4);
	DDX_Control(pDX, IDC_TX5 , m_strTx5);

	DDX_Control(pDX, IDC_STATIC_AMOUNT , m_strOver);
	DDX_Control(pDX, IDC_STATIC_NOTCOF , m_strOking);
	DDX_Control(pDX, IDC_STATIC_COUNT , m_strTranNum);

	DDX_Control(pDX, IDC_MFCLINK1, v_linkCtrl);
	DDX_Control(pDX, IDC_MFCLINK3, v_linkCtrl1);
	DDX_Control(pDX, IDC_MFCLINK4, v_linkCtrl2);
	DDX_Control(pDX, IDC_MFCLINK5, v_linkCtrl3);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogBar)
	ON_BN_CLICKED(IDC_ALLTXDETAIL, &CMainDlg::OnBnClickedAlltxdetail)
	ON_MESSAGE(MSG_USER_MAIN_UI , &CMainDlg::OnShowListCtorl )
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

void CMainDlg::SetBkBmpNid( UINT nBitmapIn ) 
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

void CMainDlg::OnBnClickedAlltxdetail()
{
	// TODO: 在此添加控件通知处理程序代码
	CDacrsUIDlg* pDlg = (CDacrsUIDlg*)GetParent();
	if ( NULL != pDlg ) {
		pDlg->ShowDialog(CTradDlg::IDD) ;
		pDlg->ShowStateTip(IDC_BUTTON_TRAD_INFO);
	}
}

void CMainDlg::OnnitCtrlText()
{

	ClearCtrlText();
	CString strCommand,strShowData;
	strCommand.Format(_T("0"));
	theApp.cs_SqlData.Lock();
	string nmoney =  theApp.m_SqliteDeal.GetColSum(_T("MYWALLET") ,_T("money") ) ;
	theApp.cs_SqlData.Unlock();
	if (!strcmp(nmoney.c_str(),"(null)"))
	{
		m_strOver.SetWindowText(_T("0.0")) ;
	}else{
		m_strOver.SetWindowText(nmoney.c_str()) ;
	}

	strCommand.Format(_T("0"));
	theApp.cs_SqlData.Lock();
	 nmoney =  theApp.m_SqliteDeal.GetColSum(_T("revtransaction") , strCommand ,_T("confirmHeight")) ;
	theApp.cs_SqlData.Unlock();

	if (!strcmp(nmoney.c_str(),"(null)"))
	{
		GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(_T("0.0")) ;
	}else{
		GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(nmoney.c_str()) ;
	}
	

	theApp.cs_SqlData.Lock();
	int nItem =  theApp.m_SqliteDeal.GetTableCount(_T("revtransaction")) ;
	theApp.cs_SqlData.Unlock();

	strCommand.Format(_T("%d"),nItem);
	m_strTranNum.SetWindowText(strCommand) ;

	CString Where,strSource;
	//Where.Format(_T("'COMMON_TX' order by confirmedtime"));
	Where.Format(_T("'COMMON_TX' order by confirmedtime limit 5"));
	strSource.Format(_T("txtype"));
	uistruct::TRANSRECORDLIST pTransaction;
	theApp.cs_SqlData.Lock();
	theApp.m_SqliteDeal.FindDB(_T("revtransaction"),Where,strSource,&pTransaction);
	theApp.cs_SqlData.Unlock();

	int i = 1;
	strCommand.Format(_T("IDC_TX%d"),nItem);
	int item = IDC_TX1;
	int item1 = IDC_TX_JY2;
	if (pTransaction.size() != 0  ) {
		int nSubIdx = 0 , i = 0 ;
		CString strShowaddr ;
		std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
		for (const_it = pTransaction.begin() ; const_it != pTransaction.end()&&i<6 ; const_it++ ) {
			if (const_it->state == 1)
			{
				strSource.Format(_T("-%.8f"),const_it->money);
				strShowaddr.Format(_T("%s "),const_it->addr.c_str());
				i++;
			}else if (const_it->state == 2)
			{
				strSource.Format(_T("+%.8f"),const_it->money);
				strShowaddr.Format(_T("%s "),const_it->desaddr);
				i++;
			}
			if (const_it->state == 1)
			{
				if(i == 1)
				m_strTrading.SetTextColor(RGB(255,0,0));			//字体颜色
				if(i == 2)
				m_strTrading2.SetTextColor(RGB(255,0,0));
				if(i == 3)
					m_strTrading3.SetTextColor(RGB(255,0,0));			//字体颜色
				if(i == 4)
					m_strTrading4.SetTextColor(RGB(255,0,0));
				if(i == 5)
					m_strTrading5.SetTextColor(RGB(255,0,0));
			}else if (const_it->state == 2)
			{
				if(i == 1)
					m_strTrading.SetTextColor(RGB(166,162,247));			//字体颜色
				if(i == 2)
					m_strTrading2.SetTextColor(RGB(166,162,247));
				if(i == 3)
					m_strTrading3.SetTextColor(RGB(166,162,247));			//字体颜色
				if(i == 4)
					m_strTrading4.SetTextColor(RGB(166,162,247));
				if(i == 5)
					m_strTrading5.SetTextColor(RGB(166,162,247));

			}
			if(item <=IDC_TX5)
			GetDlgItem(item)->SetWindowText(strShowaddr) ;
			if(item1 <=IDC_TX_JY7)
			GetDlgItem(item1)->SetWindowText(strSource) ;
			item++;
			item1++;
		}
	}
}
void CMainDlg::SetCtrlText()
{

	ClearCtrlText();
	CPostMsg postmsg;
	if (!theApp.m_UiManDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::MINDLG_T maindlg; 
	string strTemp = postmsg.GetData();
	maindlg.JsonToStruct(strTemp.c_str());


	CString strCommand,strShowData;

	GetDlgItem(IDC_STATIC_AMOUNT)->SetWindowText(maindlg.money.c_str()) ;

	GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(maindlg.unconfirmmoney.c_str()) ;

	m_strTranNum.SetWindowText(maindlg.itemcount.c_str()) ;

	string addr1 = maindlg.addr1;
	uistruct::REVTRANSACTION_t temp;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		if (temp.state == 1)
		{
			strCommand.Format(_T("-%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.addr.c_str());
			m_strTrading.SetTextColor(RGB(255,0,0));
		}else if (temp.state == 2)
		{
			strCommand.Format(_T("+%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.desaddr);
			m_strTrading.SetTextColor(RGB(166,162,247));
		}

		GetDlgItem(IDC_TX1)->SetWindowText(strShowData) ;
		GetDlgItem(IDC_TX_JY2)->SetWindowText(strCommand) ;
	}
	
	addr1 = maindlg.addr2;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		if (temp.state == 1)
		{
			strCommand.Format(_T("-%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.addr.c_str());
			m_strTrading2.SetTextColor(RGB(255,0,0));
		}else if (temp.state == 2)
		{
			strCommand.Format(_T("+%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.desaddr);
			m_strTrading2.SetTextColor(RGB(166,162,247));
		}

		GetDlgItem(IDC_TX2)->SetWindowText(strShowData) ;
		GetDlgItem(IDC_TX_JY3)->SetWindowText(strCommand) ;
	}

	addr1 = maindlg.addr3;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		if (temp.state == 1)
		{
			strCommand.Format(_T("-%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.addr.c_str());
			m_strTrading3.SetTextColor(RGB(255,0,0));
		}else if (temp.state == 2)
		{
			strCommand.Format(_T("+%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.desaddr);
			m_strTrading3.SetTextColor(RGB(166,162,247));
		}

		GetDlgItem(IDC_TX3)->SetWindowText(strShowData) ;
		GetDlgItem(IDC_TX_JY4)->SetWindowText(strCommand) ;
	}
	addr1 = maindlg.addr4;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		if (temp.state == 1)
		{
			strCommand.Format(_T("-%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.addr.c_str());
			m_strTrading4.SetTextColor(RGB(255,0,0));
		}else if (temp.state == 2)
		{
			strCommand.Format(_T("+%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.desaddr);
			m_strTrading4.SetTextColor(RGB(166,162,247));
		}

		GetDlgItem(IDC_TX4)->SetWindowText(strShowData) ;
		GetDlgItem(IDC_TX_JY5)->SetWindowText(strCommand) ;
	}
	addr1 = maindlg.addr1;
	if (addr1 != "")
	{
		temp.JsonToStruct(addr1);
		if (temp.state == 1)
		{
			strCommand.Format(_T("-%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.addr.c_str());
			m_strTrading5.SetTextColor(RGB(255,0,0));
		}else if (temp.state == 2)
		{
			strCommand.Format(_T("+%.8f"),temp.money*COIN);
			strShowData.Format(_T("%s "),temp.desaddr);
			m_strTrading5.SetTextColor(RGB(166,162,247));
		}

		GetDlgItem(IDC_TX5)->SetWindowText(strShowData) ;
		GetDlgItem(IDC_TX_JY7)->SetWindowText(strCommand) ;
	}
	Invalidate();
}
BOOL CMainDlg::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: 在此添加专用代码和/或调用基类

	BOOL bRes =  CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnAllTxdetail.LoadBitmaps(IDB_BITMAP_ALLTRADE1,IDB_BITMAP_ALLTRADE1,IDB_BITMAP_ALLTRADE1,IDB_BITMAP_ALLTRADE1);
		UpdateData(0);
		ClearCtrlText();
		OnnitCtrlText();
		GetUrlServer();
		onnitLinkText();
		//m_strTx1.SetFont(120, _T("微软雅黑"));				//设置显示字体和大小
		//m_strTx1.SetTextColor(RGB(192,192,192));			    //字体颜色
		//m_strTx1.SetWindowText(_T("方斌")) ;
		//v_linkCtrl.SetURL(_T("www.hao123.com"));
		//v_linkCtrl.SetURLPrefix(_T("http://"));
		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_MAIN_UI ) ;
	}
	return bRes ;
}
LRESULT CMainDlg::OnShowListCtorl( WPARAM wParam, LPARAM lParam ) 
{
	//更新数据
	if (wParam == WM_UPWALLET)
	{
		GetDlgItem(IDC_STATIC_WALLET)->ShowWindow(SW_HIDE);
	}else{
		SetCtrlText();
	}
	
	return 0 ;
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetBkBmpNid( IDB_BITMAP_MAINUI_BJ ) ;
	m_strOver.SetFont(90, _T("Arial"));				//设置显示字体和大小
	m_strOking.SetFont(90, _T("Arial"));	   
	m_strTranNum.SetFont(90, _T("Arial"));	
	return 0;
}
bool CMainDlg::GetUrlServer()
{
	m_url.clear();
	CString url(_T("http://www.dacrs.com/dacrs/forumupdata.json"));    
	CInternetSession session;
	std::string strHtml;

	try
	{
		CHttpFile* pfile = (CHttpFile*)session.OpenURL(url,1,INTERNET_FLAG_TRANSFER_ASCII||INTERNET_FLAG_RELOAD,NULL,0);

		DWORD dwStatusCode;    
		pfile->QueryInfoStatusCode(dwStatusCode);    
		if(dwStatusCode == HTTP_STATUS_OK)    
		{    
			char strBuff[1025] = {0};
			while ((pfile->Read((void*)strBuff, 1024)) > 0) 
			{ 
				strHtml += strBuff; 
			} 
		}
		else
		{
			return false;
		}

		pfile->Close();    
		delete pfile;    
		session.Close();
	}
	catch (CException* e)
	{
		e;//消除警告
		return false;
	}

	if (strHtml.empty())
	{
		CStdioFile myFile;
		CString strLine; 
		if(myFile.Open(theApp.str_InsPath+_T("\\dacrsUpdate.json"), CFile::modeRead))
		{
			while(myFile.ReadString(strLine))
			{
				strHtml +=strLine.GetString();
			}
			//读取
			myFile.Close();
		}else{
			return false;
		}
	}else{
			//创建
			CStdioFile  File;
			File.Open(theApp.str_InsPath+_T("\\dacrsUpdate.json"),CFile::modeWrite | CFile::modeCreate);  
			File.WriteString(strHtml.c_str());
			File.Close();
	}


	Json::Reader reader;  
	Json::Value root; 

	if (reader.parse(strHtml, root)) 
	{
		int index = root.size();
		for (int i = 0;i <index;i++)
		{
			Json::Value  msgroot = root[i];
			CString key = msgroot["msn"].asCString();
			CString valuetemp = msgroot["url"].asCString();
			m_url[key] = valuetemp;
		}
		//strVersion = root["version"].asString();
		//ShellExecuteW(NULL, L"open", _T("http://bbs.dspay.org/portal.php"), NULL, NULL, SW_SHOWNORMAL);
		return true;
	}
	return false;
}

void CMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogBar::OnPaint()
}


BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
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
void CMainDlg::ClearCtrlText()
{

	GetDlgItem(IDC_STATIC_AMOUNT)->SetWindowText(_T("")) ;

	GetDlgItem(IDC_STATIC_NOTCOF)->SetWindowText(_T("")) ;

	m_strTranNum.SetWindowText(_T("")) ;

	GetDlgItem(IDC_TX1)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX2)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX3)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX4)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX5)->SetWindowText(_T("")) ;

	GetDlgItem(IDC_TX_JY2)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX_JY3)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX_JY4)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX_JY5)->SetWindowText(_T("")) ;
	GetDlgItem(IDC_TX_JY7)->SetWindowText(_T("")) ;
}

void CMainDlg::onnitLinkText()
{
   v_linkCtrl.SetWindowText(_T(""));
   v_linkCtrl1.SetWindowText(_T(""));
   v_linkCtrl2.SetWindowText(_T(""));
   v_linkCtrl3.SetWindowText(_T(""));
   int i = 1;
	map<CString,CString>::iterator it;
	for(it=m_url.begin();it!=m_url.end();++it)
	{
		if (i == 1)
		{
			v_linkCtrl.SetWindowText(it->first);
			v_linkCtrl.SetURL(it->second);
		}
		if (i == 2)
		{
			v_linkCtrl1.SetWindowText(it->first);
			v_linkCtrl1.SetURL(it->second);
		}
		if (i == 3)
		{
			v_linkCtrl2.SetWindowText(it->first);
			v_linkCtrl2.SetURL(it->second);
		}
		if (i == 4)
		{
			v_linkCtrl3.SetWindowText(it->first);
			v_linkCtrl3.SetURL(it->second);
			break;
		}
		i++;
	}
		
}
