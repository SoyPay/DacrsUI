// GrabSpecalRedPacket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "GrabSpecalRedPacket.h"
#include "afxdialogex.h"


// CGrabSpecalRedPacket �Ի���

IMPLEMENT_DYNAMIC(CGrabSpecalRedPacket, CDialogEx)

CGrabSpecalRedPacket::CGrabSpecalRedPacket(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrabSpecalRedPacket::IDD, pParent)
{
	m_pBmp = NULL ;
	m_pagecount = 0;
	m_curpage = 0;
	m_PoolList.clear();
	m_pagesize = 4;
	m_addr = _T("");
	m_walletmoney = _T("");
	m_balance = _T("");
}

CGrabSpecalRedPacket::~CGrabSpecalRedPacket()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CGrabSpecalRedPacket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_BonusListBox);
	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CGrabSpecalRedPacket, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UP, &CGrabSpecalRedPacket::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CGrabSpecalRedPacket::OnBnClickedButtonNext)
	ON_MESSAGE( WM_BN_CLICK, &CGrabSpecalRedPacket::onBnCLick)
	ON_WM_CTLCOLOR()
	ON_LBN_DBLCLK(IDC_LIST_BOX, &CGrabSpecalRedPacket::OnLbnDblclkListBox)
END_MESSAGE_MAP()


// CGrabSpecalRedPacket ��Ϣ�������
void CGrabSpecalRedPacket::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CGrabSpecalRedPacket::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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


int CGrabSpecalRedPacket::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid(IDB_BITMAP_REDPACKETPOOL);
	return 0;
}


BOOL CGrabSpecalRedPacket::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ){
		UpdateData(0);

		m_sCountpage.SetFont(90, _T("����"));				//������ʾ����ʹ�С
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //������ɫ	
		m_sCountpage.SetWindowText(_T("��:"));

		m_rBtnUp.SetBitmaps( IDB_BITMAP_RED_UP , RGB(255, 255, 0) , IDB_BITMAP_RED_UP , RGB(255, 255, 255) );
		m_rBtnUp.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnUp.SetWindowText("") ;
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnUp.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnUp.SizeToContent();

		m_rBtnNext.SetBitmaps( IDB_BITMAP_RED_NEX , RGB(255, 255, 0) , IDB_BITMAP_RED_NEX , RGB(255, 255, 255) );
		m_rBtnNext.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNext.SetWindowText("") ;
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNext.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNext.SizeToContent();

		GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T("��:0"));
	}

	/// listbox ������ɫ
	m_hbrush = CreateSolidBrush(RGB(255,236,229));
	return bRes;
}


void CGrabSpecalRedPacket::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( NULL != GetSafeHwnd() ) {
		CWnd *pst = GetDlgItem( IDC_LIST_BOX ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,0 , 25 , 424, 120  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_UP ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,424-140 , 120+25 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_EDIT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,424 - 115, 120+25 , 40, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_BUTTON_NEXT ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,424-70 , 120+25 , 20, 20  ,SWP_SHOWWINDOW ) ; 
		}
		pst = GetDlgItem( IDC_STATIC_COUNT_PAGE ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,424-50 ,120+28 , 50, 20  ,SWP_SHOWWINDOW ) ; 
		}
	}
}
void CGrabSpecalRedPacket::Showlistbox()
{
	//// �������ݿ����Ƿ���ڴ˼�¼
	m_BonusListBox.DeleteAllIndex();
	m_PoolList.clear();
	m_curpage = 0;
	theApp.m_SqliteDeal.GetRedPacketPoolRecordList(_T(" packet_type = 2 "), &m_PoolList);
	m_pagecount = (m_PoolList.size()%m_pagesize)==0?(m_PoolList.size()/m_pagesize):(m_PoolList.size()/m_pagesize)+1;

	string temp;
	temp =strprintf("��:%d",m_pagecount);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_BonusListBox.DeleteAllIndex();
	OnShowPagePool(1);
}


void  CGrabSpecalRedPacket::OnShowPagePool(int page)
{
	if (page >m_pagecount || page == m_curpage || page <= 0)
	{
		return;
	}


	m_BonusListBox.DeleteAllIndex();
	string strpage;
	strpage = strprintf("%d",page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());
	m_curpage = page;
	int index = (page-1)*m_pagesize;
	unsigned int count = (m_PoolList.size() -index)>=m_pagesize?m_pagesize:(m_PoolList.size() -index);
	int i =0;
	string strmoney;
	string money;
	string txhash, line;
	string strShow;
	string temp;
	for (unsigned int k = index;k< (index+count);k++)
	{
		uistruct::REDPACKETPOOL_t const_it = m_PoolList.at(k);


		strmoney=strprintf("%.8f",const_it.total_amount);
		money = strprintf("%.4f",const_it.total_amount);

		temp = const_it.send_hash.substr(0,6);
		line = strprintf("%s",temp.c_str());
		//line.Format(_T("%d"),(i+1));
		if (const_it.packet_type == 1)
		{
			strShow = "��ͨ���";
		}else if (const_it.packet_type == 2)
		{
			strShow = "�������";
		}
		m_BonusListBox.InsertStr(i,this->GetSafeHwnd());
		m_BonusListBox.SetotherIndexInage(i , IDB_BITMAP_GRAB_RED);
		m_BonusListBox.SetIndexString(i , line.c_str(),const_it.send_acc_id.c_str(), _T("��"), money.c_str(), const_it.send_hash.c_str(),strmoney.c_str());
		i++;
	}
}
BOOL CGrabSpecalRedPacket::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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
					::MessageBox( this->GetSafeHwnd() ,_T("��������,����������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
				}
				return TRUE;
			}
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGrabSpecalRedPacket::OnBnClickedButtonUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnShowPagePool((m_curpage-1));
}


void CGrabSpecalRedPacket::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnShowPagePool((m_curpage+1));
}
LRESULT CGrabSpecalRedPacket::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	List_AppendData* pinf = m_BonusListBox.GetAppendDataInfo((int)wParam);
	if ( NULL != pinf ) { 
		CString hash = pinf->pstr;
		string conditon;
		conditon = strprintf("send_hash='%s'",hash);
		uistruct::REDPACKETPOOL_t pPoolList;
		theApp.m_SqliteDeal.GetRedPacketPoolItem(conditon, &pPoolList);
		if (pPoolList.send_hash.length() != 0)
		{
			if (pPoolList.packet_type == 2)
			{
				AcceptRedPackeSpecail(pPoolList.send_hash.c_str(),pPoolList);
			}
		}

	}
	return 0;

}

void   CGrabSpecalRedPacket::AcceptRedPackeSpecail(CString sendhash,uistruct::REDPACKETPOOL_t pPoolList)
{
	if (!theApp.IsSyncBlock )
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;


	CString walletaddr = m_walletmoney;
	INT64 sub = (INT64)(atof(walletaddr)*COIN) - theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee;
	if (sub < 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("��Ǯ���˻����㸶С��,���ȳ�ֵ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	if (atof(m_balance) < pPoolList.total_amount)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("��Ǯ���˻����С�ڽ���������,���ȳ�ֵ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	CString addr = m_addr;

	if (addr == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("��ַ����Ϊ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	if (strcmp(pPoolList.send_acc_id.c_str(),addr) == 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("�������ַ���������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAcceptRedPacket(addr,pPoolList))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("�˵�ַ�Ѿ��������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	string strContractData,strHash;
	strHash= CSoyPayHelp::getInstance()->GetReverseHash(sendhash.GetString());
	strContractData = m_RedPacketHelp.PacketAcceptSecpailContract(strHash);

	INT64 strTxFee = theApp.m_RedPacketCfg.AcceptRedPacketSpecailFee;
	if (  strTxFee < 10000  ) {
		::MessageBox( this->GetSafeHwnd() ,_T("С�Ѳ���") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return ;
	}
	string strShowData ="";
	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_redPacketScriptid,addr.GetString(),strContractData,0,strTxFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData =="")
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
		//���뵽���׼�¼���ݿ�
		string strHash = root["hash"].asString();
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("��ϲ���Ͷ�Լ�ɹ�!\n%s") , root["hash"].asCString() ) ;
		strTip ="������ɹ�����ȴ�1-2����ȷ�Ͻ���\n";
	}else{
		strTip ="����ѱ���!" ;
	}

	//���浽���ݿ�
	if ( bRes ) {

		string txhash = root["hash"].asString();
		//���뵽���ݿ�
		string strSourceData;
		strSourceData =strprintf("'%s','%s','%d','%lf' , '%s' ,'%s' , '%d' , '%d','%d','%lf','%d'" , \
			pPoolList.send_hash.c_str() ,txhash.c_str() , 0 ,0.0 ,pPoolList.send_acc_id , addr ,0,2,0,pPoolList.total_amount,pPoolList.packets_num);

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData;
		pDatabase.strTabName =  _T("t_red_packets_grab");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();

		postmsg.SetData(strTemp);
		theApp.m_MsgQueue.push(postmsg);
	}
	::MessageBox( this->GetSafeHwnd() ,strTip.c_str() , _T("��ʾ") , MB_ICONINFORMATION ) ;
}

bool  CGrabSpecalRedPacket::IsAcceptRedPacket(CString account,uistruct::REDPACKETPOOL_t pPoolList)
{
	//for (int i =0;i < pPoolList.packets_num;i++)
	//{
		string strCommand,strShowData;
		strCommand =strprintf("%s %s","gettxdetail" ,pPoolList.send_hash.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData =="")
		{
			return false;
		}
		Json::Reader reader; 
		Json::Value root;
		if (!reader.parse(strShowData, root)) 
			return false;
		int npos = strShowData.find("confirmHeight");
		int confirHeight = 1440;
		if ( npos >= 0 ) { //
			confirHeight += root["confirmHeight"].asInt() ;    //���ױ�ȷ�ϵĸ߶�
		}

		vector<unsigned char>vHash;
		CSoyPayHelp::getInstance()->revert((char*)&confirHeight);

		char key[4];
		memset(key,0,4);

		memcpy(key,&confirHeight,sizeof(confirHeight));
		vHash.assign(key,key+sizeof(key));
		string strKeyHex = CSoyPayHelp::getInstance()->HexStr(vHash);

		vHash =CSoyPayHelp::getInstance()->ParseHex(pPoolList.send_hash);
		reverse(vHash.begin(),vHash.end());
		string SendHash = CSoyPayHelp::getInstance()->HexStr(vHash);

		string keyValue;
		keyValue = strprintf("%s%s",strKeyHex.c_str(),SendHash.c_str());
		strCommand= strprintf("%s %s %s","getscriptdata" ,theApp.m_redPacketScriptid,keyValue.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == _T("") || strShowData.find("value") < 0)
			return false;

		if (!reader.parse(strShowData, root)) 
			return false;;

		string nValue = root["value"].asString();
		uistruct::RED_DATA redPacket;
		memset(&redPacket , 0 , sizeof(uistruct::RED_DATA));
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
		if (vTemp.size() <=0)
		{
			return false;
		}
		memcpy(&redPacket, &vTemp[0], sizeof(uistruct::RED_DATA));

		for (int i =0;i <redPacket.dbdata.takennum;i++)
		{
			uistruct::USER_INFO userinfo = redPacket.userinfo[i];
			std::vector<unsigned char> vSendid;
			vSendid.assign(userinfo.regid,userinfo.regid+sizeof(userinfo.regid));
			string regid  =CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);
			if (strcmp(account,regid.c_str()) == 0)
			{
				return true;
			}
		}

//	}

	return false;
}
void   CGrabSpecalRedPacket::SetTxt(CString addr,CString strwallet,CString strbalance)
{
	m_addr = addr;
	m_walletmoney = strwallet;
	m_balance = strbalance;
}

HBRUSH CGrabSpecalRedPacket::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	if (nCtlColor== CTLCOLOR_LISTBOX)
	{
		return m_hbrush; 
	} 
	else
	{
		return  CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


void CGrabSpecalRedPacket::OnLbnDblclkListBox()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel=m_BonusListBox.GetCurSel(); 
	int count = ((m_curpage -1)*m_pagesize) + nSel;
	if (count <=(int)m_PoolList.size())
	{
		uistruct::REDPACKETPOOL_t const_it = m_PoolList.at(count);
		string temp = "�������ID: ";
		string strShowid = const_it.send_hash.substr(0,30); 
		temp +=strprintf("%s" ,strShowid.c_str()) ;
		::MessageBox( this->GetSafeHwnd() ,temp.c_str() , _T("��ʾ") , MB_ICONINFORMATION ) ;
	}
}
