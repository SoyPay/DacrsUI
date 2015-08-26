// GrabCommRedPacket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "GrabCommRedPacket.h"
#include "afxdialogex.h"


// CGrabCommRedPacket �Ի���

IMPLEMENT_DYNAMIC(CGrabCommRedPacket, CDialogEx)

CGrabCommRedPacket::CGrabCommRedPacket(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrabCommRedPacket::IDD, pParent)
{
	m_pBmp = NULL ;
	m_pagecount = 0;
	m_curpage = 0;
	m_PoolList.clear();
	m_pagesize = 4;
	m_addr = _T("");
	m_walletmoney = _T("");
}

CGrabCommRedPacket::~CGrabCommRedPacket()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CGrabCommRedPacket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_BonusListBox);
	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CGrabCommRedPacket, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UP, &CGrabCommRedPacket::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CGrabCommRedPacket::OnBnClickedButtonNext)
	ON_MESSAGE( WM_BN_CLICK, &CGrabCommRedPacket::onBnCLick)
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR()
	ON_LBN_DBLCLK(IDC_LIST_BOX, &CGrabCommRedPacket::OnLbnDblclkListBox)
END_MESSAGE_MAP()


// CGrabCommRedPacket ��Ϣ�������
void CGrabCommRedPacket::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CGrabCommRedPacket::OnEraseBkgnd(CDC* pDC)
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


int CGrabCommRedPacket::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid(IDB_BITMAP_REDPACKETPOOL);
	return 0;
}


BOOL CGrabCommRedPacket::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
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


void CGrabCommRedPacket::OnSize(UINT nType, int cx, int cy)
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
void CGrabCommRedPacket::Showlistbox()
{
	//// �������ݿ����Ƿ���ڴ˼�¼
//	m_PoolList.clear();
	m_curpage = 0;
//	theApp.m_SqliteDeal.GetRedPacketPoolRecordList(_T(" packet_type = 1 order by average_amount desc"), &m_PoolList);
	m_pagecount = (m_PoolList.size()%m_pagesize)==0?(m_PoolList.size()/m_pagesize):(m_PoolList.size()/m_pagesize)+1;

	string temp;
	temp = strprintf("��:%d",m_pagecount);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_BonusListBox.DeleteAllIndex();
	Invalidate();
	OnShowPagePool(1);
}


void  CGrabCommRedPacket::OnShowPagePool(int page)
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
	string strShow;
	string txhash, line;
	string temp;
	for (unsigned int k = index;k< (index+count);k++)
	{
		uistruct::REDPACKETPOOL_t const_it = m_PoolList.at(k);


		strmoney= strprintf("%.8f",const_it.total_amount);
		money = strprintf("%.4f",const_it.total_amount);

		temp = const_it.send_hash.substr(0,6);
		line = strprintf("%s",temp);
		if (const_it.packet_type == 1)
		{
			strShow ="��ͨ���";
		}else if (const_it.packet_type == 2)
		{
			strShow="�������";
		}
		m_BonusListBox.InsertStr(i,this->GetSafeHwnd());
		m_BonusListBox.SetotherIndexInage(i , IDB_BITMAP_GRAB_RED);
		 m_BonusListBox.SetIndexString(i , line.c_str(),const_it.send_acc_id.c_str(), _T("��"), money.c_str(), const_it.send_hash.c_str(),strmoney.c_str());
		//m_BonusListBox.SetIndexInage(i , IDB_BITMAP_P2P_LISTBOX_BUT);
		//m_BonusListBox.SetIndexString(i , line,const_it.send_acc_id, money,strShow,_T("��"),const_it.send_hash,strmoney);
		i++;
	}
}
BOOL CGrabCommRedPacket::PreTranslateMessage(MSG* pMsg)
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
					UiFun::MessageBoxEx(_T("��������,����������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
				}
				return TRUE;
			}
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CGrabCommRedPacket::OnBnClickedButtonUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnShowPagePool((m_curpage-1));
}


void CGrabCommRedPacket::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnShowPagePool((m_curpage+1));
}
LRESULT CGrabCommRedPacket::onBnCLick( WPARAM wParam, LPARAM lParam )
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
			if (pPoolList.packet_type == 1)
			{
				AcceptRedPacketComm(pPoolList.send_hash.c_str(),pPoolList);
			}
		}

	}
	return 0;

}

void   CGrabCommRedPacket::AcceptRedPacketComm(CString sendhash,uistruct::REDPACKETPOOL_t pPoolList)
{
	string strshow= "����Ѿ�����,�뵽�˵�����ѡ��ָ�Ĭ������";
	if (!UiFun::IsCurrentAppId(theApp.m_redPacketScriptid.c_str(),theApp.m_neststcriptid.strNewSrcriptRedPacektid.c_str(),strshow))
	{
		return;
	}

	if (!theApp.IsSyncBlock )
	{
		UiFun::MessageBoxEx(_T("ͬ��δ���,���ܷ��ͽ���") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	if (!CheckRegIDValid( theApp.m_redPacketScriptid)) return ;

	if(pPoolList.message != "")
		UiFun::MessageBoxEx(pPoolList.message.c_str() , _T("��ʾ") ,MFB_OK|MFB_TIP );

	CString walletaddr = m_walletmoney;
	INT64 sub = (INT64)(strtod(walletaddr,NULL)*COIN) - theApp.m_RedPacketCfg.AcceptRedPacketCommFee;
	if (sub < 0)
	{
		UiFun::MessageBoxEx(_T("��Ǯ���˻����㸶С��,���ȳ�ֵ") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	CString addr = m_addr;
	if (addr == _T(""))
	{
		UiFun::MessageBoxEx(_T("��ַ����Ϊ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (strcmp(pPoolList.send_acc_id.c_str(),addr) == 0)
	{
		UiFun::MessageBoxEx(_T("�������ַ���������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (IsAcceptRedPacket(addr,pPoolList))
	{
		UiFun::MessageBoxEx(_T("�˵�ַ�Ѿ��������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	string strContractData,strHash;
	strHash= CSoyPayHelp::getInstance()->GetReverseHash(sendhash.GetString());
	strContractData = m_RedPacketHelp.PacketAcceptCommContract(strHash);

	INT64 strTxFee = theApp.m_RedPacketCfg.AcceptRedPacketCommFee;
	if (  strTxFee < 10000  ) {
		UiFun::MessageBoxEx(_T("С�Ѳ���") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return ;
	}
	string strShowData = _T("");
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
		string strHash = root["hash"].asCString() ;
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("��ϲ���Ͷ�Լ�ɹ�!\n%s") , root["hash"].asCString() ) ;
		strTip ="������ɹ�����ȴ�1-2����ȷ�Ͻ���\n" ;
	}else{
		strTip = "����ѱ���!" ;
	}

	//���浽���ݿ�
	if ( bRes ) {

		string txhash = root["hash"].asString();
		//���뵽���ݿ�
		string strSourceData;
		strSourceData = strprintf("'%s','%s','%d','%lf' , '%s' ,'%s' , '%d' , '%d','%d','%lf','%d'", \
			pPoolList.send_hash.c_str() ,txhash.c_str() , 0 ,0.0 ,  pPoolList.send_acc_id ,addr ,0,1,0,pPoolList.total_amount,pPoolList.packets_num);

		uistruct::DATABASEINFO_t   pDatabase;
		pDatabase.strSource = strSourceData;
		pDatabase.strTabName =  _T("t_red_packets_grab");
		CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
		string strTemp = pDatabase.ToJson();

		postmsg.SetData(strTemp);
		theApp.m_MsgQueue.push(postmsg);
	}
	UiFun::MessageBoxEx(strTip.c_str()  , _T("��ʾ") ,MFB_OK|MFB_TIP );
}

bool  CGrabCommRedPacket::IsAcceptRedPacket(CString account,uistruct::REDPACKETPOOL_t pPoolList)
{
	//for (int i =0;i < pPoolList.packets_num;i++)
	//{
		string strCommand,strShowData;
		strCommand = strprintf("%s %s",_T("gettxdetail") ,pPoolList.send_hash.c_str() );
		Json::Value root;
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("IsAcceptRedPacket rpccmd gettxdetail error");
			return false;
		}
		strShowData = root.toStyledString();
		
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
		strCommand = strprintf("%s %s %s",_T("getscriptdata") ,theApp.m_redPacketScriptid,keyValue.c_str() );
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("IsAcceptRedPacket rpccmd getscriptdata error");
			return false;
		}

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

	//}

	return false;
}
void   CGrabCommRedPacket::SetTxt(CString addr,CString strwallet)
{
	m_addr = addr;
	m_walletmoney = strwallet;
}


HBRUSH CGrabCommRedPacket::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CGrabCommRedPacket::OnLbnDblclkListBox()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel=m_BonusListBox.GetCurSel(); 
	int count = ((m_curpage -1)*m_pagesize) + nSel;
	if (count <=(int)m_PoolList.size())
	{
		uistruct::REDPACKETPOOL_t const_it = m_PoolList.at(count);
		string temp = "��ͨ���ID: ";
		string strShowid = const_it.send_hash.substr(0,30); 
		temp +=strprintf("%s" ,strShowid.c_str()) ;
		UiFun::MessageBoxEx(temp.c_str() , _T("��ʾ") ,MFB_OK|MFB_TIP );
	}
}
void CGrabCommRedPacket::ReadComRedPacketPool()
{
	if (theApp.m_readReadPacketPool)
	{
		m_PoolList.clear();
		theApp.m_SqliteDeal.GetRedPacketPoolRecordList(_T(" packet_type = 1 order by average_amount desc"), &m_PoolList);
		theApp.m_readReadPacketPool = false;
	}

	Showlistbox();
}