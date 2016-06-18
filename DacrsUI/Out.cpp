// Out.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "Out.h"
#include "afxdialogex.h"


// COut �Ի���

IMPLEMENT_DYNAMIC(COut, CDialogEx)

COut::COut(CWnd* pParent /*=NULL*/, CString strDisplay, int nFontSize,CString strok,CString strNo ,bool showlink)
	: CDialogEx(COut::IDD, pParent)
{
	m_strDisplay = strDisplay;
	m_nFontSize = nFontSize;
	m_strok = strok;
	m_strno = strNo;
	m_showLink = showlink;
	v_linkCtrl = NULL;
}

COut::~COut()
{
	if (v_linkCtrl != NULL)
	{
		delete v_linkCtrl;
		v_linkCtrl = NULL;
	}
}

void COut::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_Text);
	DDX_Control(pDX, IDC_BUTTON_OK, m_rBtnOK);
	DDX_Control(pDX, IDC_BUTTON_NO, m_rBtnNO);
}


BEGIN_MESSAGE_MAP(COut, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_NO, &COut::OnBnClickedButtonNo)
	ON_BN_CLICKED(IDC_BUTTON_OK, &COut::OnBnClickedButtonOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// COut ��Ϣ��������


BOOL COut::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��
	UpdateData(FALSE);
	m_Text.SetFont(m_nFontSize, _T("����"));	
	m_Text.SetTextColor(RGB(0,0,0));			    //������ɫ	
	if(_T("") == m_strDisplay) 
	{
		m_Text.SetWindowText(UiFun::UI_LoadString("OUT_MODULE" , "OUT_OUTAPP" ,theApp.gsLanguage)) ;
	}else
	{
		m_Text.SetWindowText(m_strDisplay.GetString());
	}
	//m_Text.SetWindowPos(NULL ,143 , 55 , 0 , 0 , SWP_NOSIZE);

	m_rBtnOK.SetBitmaps( IDB_BITMAP_OUTBUTTON , RGB(255, 255, 0) , IDB_BITMAP_OUTBUTTON , RGB(255, 255, 255) );
	m_rBtnOK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	if (m_strok == _T(""))
	{
		m_rBtnOK.SetWindowText(UiFun::UI_LoadString("OUT_MODULE" , "OUT_CONFIRM" ,theApp.gsLanguage)) ;
	}else{
		m_rBtnOK.SetWindowText(m_strok) ;
	}
	
	m_rBtnOK.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnOK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOK.SizeToContent();

	m_rBtnNO.SetBitmaps( IDB_BITMAP_OUTBUTTON , RGB(255, 255, 0) , IDB_BITMAP_OUTBUTTON , RGB(255, 255, 255) );
	m_rBtnNO.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	if (m_strno == _T(""))
	{
		m_rBtnNO.SetWindowText(UiFun::UI_LoadString("OUT_MODULE" , "OUT_CANCEL" ,theApp.gsLanguage)) ;
	}else{
		m_rBtnNO.SetWindowText(m_strno) ;
	}
	m_rBtnNO.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnNO.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnNO.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnNO.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnNO.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnNO.SizeToContent();


	SetBkBmpNid( IDB_BITMAP_DLG_BALCK ) ;
	m_fontGrid.CreatePointFont(100,_T("������"));

	v_linkCtrl->SetWindowText(_T(""));
	if (m_showLink)
	{
		m_rBtnOK.ShowWindow(SW_HIDE);
		onShowLink();
		CRect rc ;
		GetClientRect( rc ) ;
		CWnd *pst = GetDlgItem( IDC_BUTTON_NO ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL ,(rc.Width()/100)*45 , (rc.Height()/100)*100+15, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		}

		pst = GetDlgItem( IDC_MFCLINK1 ) ;
		if ( NULL != pst ) {
	
			pst->SetWindowPos( NULL ,(rc.Width()/100)*32 , (rc.Height()/100)*80, 200, 30  ,SWP_SHOWWINDOW ) ; 
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void COut::SetBkBmpNid( UINT nBitmapIn ) 
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


HBRUSH COut::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	/*if (nCtlColor == CTLCOLOR_STATIC)
	{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_fontGrid);
	hbr = (HBRUSH)CreateSolidBrush(RGB(240,240,240));
	}*/
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL COut::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	CRect   rect; 
	GetClientRect(&rect); 

	if(m_pBmp   !=   NULL) { 
		BITMAP   bm; 
		CDC   dcMem; 
		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
		pDC->StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return TRUE;
}


void COut::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴�������Ϣ�����������
}


LRESULT COut::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	UINT nResult = CDialog::OnNcHitTest(point);
	return nResult  == HTCLIENT ? HTCAPTION : nResult;//���������ڿͻ���ô���ڵĻ��Ͱ������ɱ�����

	return CDialogEx::OnNcHitTest(point);
}


void COut::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND, SC_MOVE, 0);//����һ���ƶ�����Ϣ
	CDialogEx::OnLButtonDown(nFlags, point);
}

void COut::OnBnClickedButtonNo()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	EndDialog(IDCANCEL);
}

void COut::OnBnClickedButtonOk()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	EndDialog(IDOK);
}
void  COut::onShowLink()
{
	v_linkCtrl->SetWindowText(UiFun::UI_LoadString("OUT_MODULE" , "OUT_WHY_XP" ,theApp.gsLanguage));
	v_linkCtrl->SetURL("www.baidu.com");
}



int COut::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ�������ר�õĴ�������
	if (v_linkCtrl == NULL)
	{
		v_linkCtrl = new CMFCLinkCtrl;
		v_linkCtrl->Create(_T(""), 
			WS_VISIBLE, 
			CRect(5, 5, 30, 20), 
			this, 
			IDC_MFCLINK1);
	}
	return 0;
}