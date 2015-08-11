// BetRecord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "BetRecord.h"
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
// CBetRecord �Ի���

IMPLEMENT_DYNAMIC(CBetRecord, CDialogEx)

CBetRecord::CBetRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBetRecord::IDD, pParent)
{
	m_pBmp = NULL ;
	m_pagecount = 0;
	m_curpage = 0;
	m_PoolList.clear();
	m_pagesize = 4;
}

CBetRecord::~CBetRecord()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CBetRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_ListBox);
	DDX_Control(pDX, IDC_BUTTON_UP, m_rBtnUp);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_rBtnNext);
	DDX_Control(pDX ,IDC_STATIC_COUNT_PAGE ,m_sCountpage ) ;
}


BEGIN_MESSAGE_MAP(CBetRecord, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UP, &CBetRecord::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CBetRecord::OnBnClickedButtonNext)
	ON_LBN_DBLCLK(IDC_LIST_BOX, &CBetRecord::OnLbnDblclkListBox)
END_MESSAGE_MAP()


// CBetRecord ��Ϣ�������
void CBetRecord::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CBetRecord::OnEraseBkgnd(CDC* pDC)
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


int CBetRecord::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid(IDB_BITMAP_BET_RECORD);
	return 0;
}


BOOL CBetRecord::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogEx::Create(lpszTemplateName, pParentWnd);
	if ( bRes ){
		UpdateData(0);
	
		m_sCountpage.SetFont(90, _T("����"));				//������ʾ����ʹ�С
		m_sCountpage.SetTextColor(RGB(0,0,0));			    //������ɫ	
		m_sCountpage.SetWindowText(_T("��:"));

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

		GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T("��:0"));
	}
	return bRes;
}


void CBetRecord::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( NULL != GetSafeHwnd() ) {
		CWnd *pst = GetDlgItem( IDC_LIST_BOX ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL ,0 , 33 , 875, 130  ,SWP_SHOWWINDOW ) ; 
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
void CBetRecord::Showlistbox(CString address)
{
	//// �������ݿ����Ƿ���ڴ˼�¼
	//m_ListBox.DeleteAllIndex();
	m_PoolList.clear();
	m_curpage = 0;
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(_T(""));
	string conditon;
	conditon = strprintf("right_addr ='%s' and (actor = 1 or actor = 2) order by recv_time desc" , address);

	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon.c_str() ,&m_PoolList ) ;

	m_pagecount = (m_PoolList.size()%m_pagesize)==0?(m_PoolList.size()/m_pagesize):(m_PoolList.size()/m_pagesize)+1;

	string temp;
	temp= strprintf("��:%d",m_pagecount);
	GetDlgItem(IDC_STATIC_COUNT_PAGE)->SetWindowText(temp.c_str());
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(_T(""));
	Invalidate();
	m_ListBox.DeleteAllIndex();
	OnShowPagePool(1);
}


void  CBetRecord::OnShowPagePool(int page)
{
	if (page >m_pagecount || page == m_curpage || page <= 0)
	{
		return;
	}


	m_ListBox.DeleteAllIndex();
	string strpage;
	strpage= strprintf("%d",page);
	GetDlgItem(IDC_EDIT_PAGE)->SetWindowText(strpage.c_str());

	m_curpage = page;
	int index = (page-1)*m_pagesize;
	unsigned int count = (m_PoolList.size() -index)>=m_pagesize?m_pagesize:(m_PoolList.size() -index);

	string dmoney,reward,result,guess,Sendaddr,address;
	string sendTime,reciveTime;
	
	char buffer[1024] ={0};
	int i = 0;
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	for (unsigned int k = index;k< (index+count) && k < m_PoolList.size();k++)
	{
		uistruct::P2P_QUIZ_RECORD_t const_it = m_PoolList.at(k);
		
		dmoney =strprintf("%.4f",const_it.amount);

		if (const_it.guess_num == 1)
		{
			guess = "��";
		}else
		{
			guess = "��";
		}
		m_ListBox.InsertStr(i,this->GetSafeHwnd());
		m_ListBox.SetIndexBackCol(i , 0 , RGB(242,32,32));
		m_ListBox.SetIndexBackCol(i , 4 , RGB(242,32,32));

		

		if (const_it.send_time == 0)
		{
			sendTime = "--";
		}else{
			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(const_it.send_time);
			sendTime =strprintf("%02d-%02d %02d:%02d:%02d", curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		}
		if (const_it.recv_time == 0)
		{
			reciveTime = "--";
		}else{
			SYSTEMTIME rTime =UiFun::Time_tToSystemTime(const_it.recv_time);
			reciveTime =strprintf("%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
		}

		reward =strprintf("%.4f", const_it.amount);
		
		///˵��������
		if (const_it.state == 2)
		{
			int rewardnum = (int)const_it.content[32];
			if (const_it.guess_num == const_it.content[32])
			{
				m_ListBox.SetIndexBackCol(i , 6 , RGB(242,32,32));

				reward =strprintf("+%.4f", const_it.amount);
			}else
			{
				m_ListBox.SetIndexBackCol(i , 6 , RGB(1,127,1));
				reward =strprintf("-%.4f", const_it.amount);
			}


			if (const_it.content[32] == 1)
			{
				result = "��";
			}else
			{
				result = "��";
			}
			m_ListBox.SetIndexString(i , const_it.left_addr.c_str(),const_it.right_addr.c_str(),sendTime.c_str(),reciveTime.c_str(), result.c_str(),guess.c_str(), reward.c_str());

		}else{
			if (const_it.height>0 &&(const_it.time_out + const_it.height)> theApp.blocktipheight&& theApp.IsSyncBlock)
			{
				m_ListBox.SetIndexString(i , const_it.left_addr.c_str(),const_it.right_addr.c_str(),sendTime.c_str(),reciveTime.c_str(), _T("δ����"),guess.c_str(),reward.c_str());
			}else if(theApp.IsSyncBlock && const_it.height != 0){
				m_ListBox.SetIndexBackCol(i , 6 , RGB(242,32,32));
				reward =strprintf("+%.4f", const_it.amount);
				m_ListBox.SetIndexString(i , const_it.left_addr.c_str(),const_it.right_addr.c_str(),sendTime.c_str(),reciveTime.c_str(), _T("��ʱ"),guess.c_str(),reward.c_str());
			}else{
				m_ListBox.SetIndexString(i , const_it.left_addr.c_str(),const_it.right_addr.c_str(),sendTime.c_str(),reciveTime.c_str(), _T("δ����"),guess.c_str(),reward.c_str());
			}
		}
		i++;
	}
}
BOOL CBetRecord::PreTranslateMessage(MSG* pMsg)
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

void CBetRecord::OnBnClickedButtonUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnShowPagePool((m_curpage-1));
}


void CBetRecord::OnBnClickedButtonNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnShowPagePool((m_curpage+1));
}


void CBetRecord::OnLbnDblclkListBox()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nSel=m_ListBox.GetCurSel(); 
	int count = ((m_curpage -1)*m_pagesize) + nSel;
	if (count <=m_PoolList.size())
	{
		uistruct::P2P_QUIZ_RECORD_t const_it = m_PoolList.at(count);
		string temp = const_it.ToJson();
		theApp.m_strAddress.Format(_T("%s") ,temp.c_str() ) ;
		CBetInformation dlg;
		dlg.DoModal();	
	}
	
}

void CBetRecord::GetExportCol(map<int,string> &item,uistruct::P2P_QUIZ_RECORD_t pitem)
{
	int i = 0;
	item[i++] = pitem.relate_hash;
	item[i++] = pitem.left_addr;
	item[i++] = pitem.right_addr;
	
	string amount;
	amount =strprintf("%.4f",pitem.amount);

	string guess;
	if (pitem.guess_num == 1)
	{
		guess = "��";
	}else
	{
		guess = "��";
	}


	string sendTime,reciveTime;
	if (pitem.send_time == 0)
	{
		sendTime = "--";
	}else{
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(pitem.send_time);
		sendTime = strprintf("%02d-%02d %02d:%02d:%02d", curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}
	if (pitem.recv_time == 0)
	{
		reciveTime = "--";
	}else{
		SYSTEMTIME rTime =UiFun::Time_tToSystemTime(pitem.recv_time);
		reciveTime = strprintf("%02d-%02d %02d:%02d:%02d", rTime.wMonth, rTime.wDay, rTime.wHour, rTime.wMinute, rTime.wSecond);
	}
	item[i++] = sendTime;
	item[i++] = reciveTime;

	string reward,result;
	if (pitem.state == 2)
	{
		int rewardnum = (int)pitem.content[32];
		if (pitem.guess_num == pitem.content[32])
		{
			reward =strprintf("+%.4f", pitem.amount);
		}else
		{
			reward =strprintf("-%.4f", pitem.amount);
		}

		if (pitem.content[32] == 1)
		{
			result = "��";
		}else
		{
			result = "��";
		}
		item[i++] = result;
		item[i++] = guess;
		item[i++] = reward;

	}else{
		if (pitem.height>0 &&(pitem.time_out + pitem.height)> theApp.blocktipheight&& theApp.IsSyncBlock)
		{
			item[i++] = "δ����";
			item[i++] = guess;
			item[i++] = amount;
		}else if(theApp.IsSyncBlock && pitem.height != 0){
			amount= strprintf("+%.4f", pitem.amount);
			item[i++] = "��ʱ";
			item[i++] = guess;
			item[i++] = amount;
		}else{
			item[i++] = "δ����";
			item[i++] = guess;
			item[i++] = amount;
		}
	}
}
void   CBetRecord::GetCellName(int nRow, int nCol, CString &strName)

{

	int nSeed = nCol;

	CString strRow;

	char cCell = 'A' + nCol - 1;



	strName.Format(_T("%c"), cCell);



	strRow.Format(_T( "%d "), nRow);

	strName += strRow;

}
void CBetRecord::OExportAcceptBetToexel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(FALSE,NULL,"�����ýӵ���¼",OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,"�ļ� (*.xls)|*.xls||");
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
	} listheadr[8]  = {
		{"�ӵ�hash" ,  70},
		{"������" ,    30},
		{"�ӵ���" ,    30},
		{"����ʱ��" ,  20}, 
		{"�ӵ�ʱ��" ,  20}, 
		{"����" ,10},
		{"�²�" ,10},
		{"���" , 50}

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

		MessageBox(_T("����ʧ�ܣ�"));

		return;

	}



	//Get   a   new   workbook.

	books = app.get_Workbooks();

	book = books.Add(covOptional);



	sheets = book.get_Worksheets();

	sheet = sheets.get_Item(COleVariant((short)1));



	////////////////////////////////////CListCtrl�ؼ�report���//////////////////////////////////////////////////////////

	//CHeaderCtrl   *pmyHeaderCtrl;

	//pmyHeaderCtrl = m_listCtrl.GetHeaderCtrl();//�˾�ȡ��CListCtrl�ؼ����б��^



	int   iRow,iCol;

	int   m_cols   =   8;

	int   m_rows = m_PoolList.size();

	HDITEM   hdi;

	TCHAR     lpBuffer[256];

	bool       fFound   =   false;



	hdi.mask   =   HDI_TEXT;

	hdi.pszText   =   lpBuffer;

	hdi.cchTextMax   =   256;

	CString   colname;

	CString strTemp;

	for(iCol=0;   iCol <m_cols;   iCol++)//���б�ı���ͷд��EXCEL

	{

		GetCellName(1 ,iCol + 1, colname);

		range   =   sheet.get_Range(COleVariant(colname),COleVariant(colname));

		//pmyHeaderCtrl-> GetItem(iCol,   &hdi);

		range.put_Value2(COleVariant(listheadr[iCol].name.c_str()));

		int   nWidth   = listheadr[iCol].size;  //m_listCtrl.GetColumnWidth(iCol)/6;

		//�õ���iCol+1��  

		range.AttachDispatch(range.get_Item(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);  

		//�����п� 

		range.put_ColumnWidth(_variant_t((long)nWidth));

	}

	range   =   sheet.get_Range(COleVariant( _T("A1 ")),   COleVariant(colname));

	range.put_RowHeight(_variant_t((long)50));//�����еĸ߶�

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

	string conditon;
	conditon = "actor = 1 or actor = 2 order by recv_time desc";

	uistruct::P2PBETRECORDLIST PoolList;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordList(conditon.c_str() ,&PoolList ) ;
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
