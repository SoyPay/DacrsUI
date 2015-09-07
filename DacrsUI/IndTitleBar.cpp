// IndTitleBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "IndTitleBar.h"
#include "DacrsUIDlg.h"


// CIndTitleBar �Ի���

IMPLEMENT_DYNAMIC(CIndTitleBar, CDialogBar)

	CIndTitleBar::CIndTitleBar()
{
	m_pBmp = NULL ;
	m_nButtonID = IDC_BUTTON_MAIN_UI ;
}

CIndTitleBar::~CIndTitleBar()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CIndTitleBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MAIN_UI	, m_rBtnMainUi );
	DDX_Control(pDX, IDC_BUTTON_SEND	, m_rBtnSend );
	DDX_Control(pDX, IDC_BUTTON_RECE	, m_rBtnRece );
	DDX_Control(pDX, IDC_BUTTON_TRAD_INFO	, m_rBtnTrad );
	DDX_Control(pDX, IDC_BUTTON_P2P	, m_rBtnP2p );
	DDX_Control(pDX, IDC_BUTTON_MORTGAGE	, m_rBtnMortgage );
	DDX_Control(pDX, IDC_BUTTON_IPO	, m_rBtnIpo );
	DDX_Control(pDX, IDC_BUTTON_ADDAPP	, m_rBtnAddApp );

	DDX_Control(pDX, IDC_BUTTON_CLOSE	, m_rBtnClose );
	DDX_Control(pDX, IDC_BUTTON_MIN	, m_rBtnMin );
	DDX_Control(pDX, IDC_MENUBAR	, m_rBtnMainMenu );
	DDX_Control(pDX, IDC_CN	, m_rCnButton );
}


BEGIN_MESSAGE_MAP(CIndTitleBar, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_MENUBAR, &CIndTitleBar::OnBnClickedMenubar)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_CN, &CIndTitleBar::OnBnClickedCn)
	ON_COMMAND(ID_CN,&CIndTitleBar::OnChinese)
	ON_COMMAND(ID_EN,&CIndTitleBar::OnEnglish)
	ON_UPDATE_COMMAND_UI(ID_EN, &CIndTitleBar::OnUpdateEn)
	ON_UPDATE_COMMAND_UI(ID_CN, &CIndTitleBar::OnUpdateCn)
END_MESSAGE_MAP()


// CIndTitleBar ��Ϣ�������
void CIndTitleBar::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CIndTitleBar::OnEraseBkgnd(CDC* pDC)
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

	return 1;
}

int CIndTitleBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid( IDB_BITMAP_BAR ) ;
	ModifyStyle(WS_BORDER,   0); 
	ModifyStyleEx(WS_EX_WINDOWEDGE,   0); 

	return 0;
}

void CIndTitleBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;
		CRect rc  ;
		GetClientRect( rc ) ;
		CButton *p_Button = (CButton*)GetDlgItem(IDC_BUTTON_MAIN_UI);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_SEND);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 80 , 15 , 0 , 0 , SWP_NOSIZE);
		}
		//
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_RECE);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 2*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_TRAD_INFO);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 3*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_P2P);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 4*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_MORTGAGE);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 5*80, 15, 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_IPO);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 6*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		p_Button = (CButton*)GetDlgItem(IDC_BUTTON_ADDAPP);
		if( NULL != p_Button ) {	
			CRect m_BtnRc ;
			p_Button->GetClientRect(&m_BtnRc);
			p_Button->SetWindowPos(NULL ,175 + 7*80, 15 , 0 , 0 , SWP_NOSIZE);
		}
		CRect rect ; 
		this->GetWindowRect(rect);
		//�ر�
		CButton *p_Close = (CButton*)GetDlgItem(IDC_BUTTON_CLOSE);
		if( NULL != p_Close ) {	
			CRect m_BtnRc ;
			p_Close->GetWindowRect(&m_BtnRc);
			p_Close->SetWindowPos(NULL ,rect.Width() - m_BtnRc.Width() - 5 /*rect.right-1*m_BtnRc.Width()-10*/ , 0 , 0 , 0 , SWP_NOSIZE);
		}
		CButton * p_Min = (CButton*)GetDlgItem(IDC_BUTTON_MIN);
		if( NULL != p_Min ) {	
			CRect m_BtnRc ;
			p_Min->GetWindowRect(&m_BtnRc);
			p_Min->SetWindowPos(NULL ,rect.Width() - 2*m_BtnRc.Width() - 8 /*rect.right-1*m_BtnRc.Width()-10*/ , 0 , 0 , 0 , SWP_NOSIZE);
		}
		
		CButton * m_rBtnMainMenu = (CButton*)GetDlgItem(IDC_MENUBAR);
		if( NULL != m_rBtnMainMenu ) {	
			CRect m_BtnRc ;
			m_rBtnMainMenu->GetWindowRect(&m_BtnRc);
			m_rBtnMainMenu->SetWindowPos(NULL ,rect.Width() - 3*m_BtnRc.Width() - 8 /*rect.right-1*m_BtnRc.Width()-10*/ , 0 , 0 , 0 , SWP_NOSIZE);
		}

		CButton * m_rBtnCn = (CButton*)GetDlgItem(IDC_CN);
		if( NULL != m_rBtnCn ) {	
			CRect m_BtnRc ;
			m_rBtnCn->GetWindowRect(&m_BtnRc);
			m_rBtnCn->SetWindowPos(NULL ,rect.Width() - 4*m_BtnRc.Width() - 8, 0 , 0 , 0 , SWP_NOSIZE);
		}

		CStatic * pImage = (CStatic*)GetDlgItem(IDC_STATIC_LOGO);
		if( NULL != pImage ) {	
			CRect m_BtnRc ;
			pImage->GetWindowRect(&m_BtnRc);
			pImage->SetWindowPos(NULL ,0 , -2 , 0 , 0 , SWP_NOSIZE);
		}
	}
}
void CIndTitleBar::MobileTip(UINT nButtonID , BOOL bFlag )
{
	if ( bFlag ) {   //��һ��
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI_2 , IDB_BITMAP_MAINUI_2 ,IDB_BITMAP_MAINUI_2);
	}
	if ( m_nButtonID == nButtonID  && !bFlag ) {
		return ;
	}
	switch(m_nButtonID)
	{
	case IDC_BUTTON_MAIN_UI:
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI_1,IDB_BITMAP_MAINUI_3,IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI_1);
		break;
	case IDC_BUTTON_SEND:
		m_rBtnSend.LoadBitmaps(IDB_BITMAP_SEND_1,IDB_BITMAP_SEND_3,IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_1);
		break;
	case IDC_BUTTON_RECE:
		m_rBtnRece.LoadBitmaps(IDB_BITMAP_REV_1,IDB_BITMAP_REV_3,IDB_BITMAP_REV_2,IDB_BITMAP_REV_1);
		break;
	case IDC_BUTTON_TRAD_INFO:
		m_rBtnTrad.LoadBitmaps(IDB_BITMAP_TRAD_1,IDB_BITMAP_TRAD_3,IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_1);
		break;
	case IDC_BUTTON_P2P:
		m_rBtnP2p.LoadBitmaps(IDB_BITMAP_YOUR_1,IDB_BITMAP_YOUR_3,IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_1);
		break;
	case IDC_BUTTON_MORTGAGE:
		m_rBtnMortgage.LoadBitmaps(IDB_BITMAP_GRAB_RP_1,IDB_BITMAP_GRAB_RP_3,IDB_BITMAP_GRAB_RP_2,IDB_BITMAP_GRAB_RP_1);
		break;
	case IDC_BUTTON_IPO:
		m_rBtnIpo.LoadBitmaps(IDB_BITMAP_IPO_1,IDB_BITMAP_IPO_3,IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_1);
		break;
	case IDC_BUTTON_ADDAPP:
		m_rBtnAddApp.LoadBitmaps(IDB_BITMAP_ADDAPP_1 , IDB_BITMAP_ADDAPP_3 ,IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_1);
		break;
	}
	switch(nButtonID)
	{
	case IDC_BUTTON_MAIN_UI:
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI_2 , IDB_BITMAP_MAINUI_2 ,IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI_2);
		break;
	case IDC_BUTTON_SEND:
		m_rBtnSend.LoadBitmaps(IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_2);
		break;
	case IDC_BUTTON_RECE:
		m_rBtnRece.LoadBitmaps(IDB_BITMAP_REV_2,IDB_BITMAP_REV_2,IDB_BITMAP_REV_2,IDB_BITMAP_REV_2);
		break;
	case IDC_BUTTON_TRAD_INFO:
		m_rBtnTrad.LoadBitmaps(IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_2);
		break;
	case IDC_BUTTON_P2P:
		m_rBtnP2p.LoadBitmaps(IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_2);
		break;
	case IDC_BUTTON_MORTGAGE:
		m_rBtnMortgage.LoadBitmaps(IDB_BITMAP_GRAB_RP_2,IDB_BITMAP_GRAB_RP_2,IDB_BITMAP_GRAB_RP_2,IDB_BITMAP_GRAB_RP_2);
		break;
	case IDC_BUTTON_IPO:
		m_rBtnIpo.LoadBitmaps(IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_2);
		break;
	case IDC_BUTTON_ADDAPP:
		m_rBtnAddApp.LoadBitmaps(IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_2);
		break;
	}
	m_nButtonID = nButtonID ;
	Invalidate();
}
BOOL CIndTitleBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		m_rBtnMainUi.LoadBitmaps(IDB_BITMAP_MAINUI_1,IDB_BITMAP_MAINUI_3,IDB_BITMAP_MAINUI_2,IDB_BITMAP_MAINUI_1);
		m_rBtnSend.LoadBitmaps(IDB_BITMAP_SEND_1,IDB_BITMAP_SEND_3,IDB_BITMAP_SEND_2,IDB_BITMAP_SEND_1);
		m_rBtnRece.LoadBitmaps(IDB_BITMAP_REV_1,IDB_BITMAP_REV_3,IDB_BITMAP_REV_2,IDB_BITMAP_REV_1);
		m_rBtnTrad.LoadBitmaps(IDB_BITMAP_TRAD_1,IDB_BITMAP_TRAD_3,IDB_BITMAP_TRAD_2,IDB_BITMAP_TRAD_1);
		m_rBtnP2p.LoadBitmaps(IDB_BITMAP_YOUR_1,IDB_BITMAP_YOUR_3,IDB_BITMAP_YOUR_2,IDB_BITMAP_YOUR_1);
		m_rBtnMortgage.LoadBitmaps(IDB_BITMAP_GRAB_RP_1,IDB_BITMAP_GRAB_RP_3,IDB_BITMAP_GRAB_RP_2,IDB_BITMAP_GRAB_RP_1);
		m_rBtnIpo.LoadBitmaps(IDB_BITMAP_IPO_1,IDB_BITMAP_IPO_3,IDB_BITMAP_IPO_2,IDB_BITMAP_IPO_1);
		m_rBtnAddApp.LoadBitmaps(IDB_BITMAP_ADDAPP_1 , IDB_BITMAP_ADDAPP_3 ,IDB_BITMAP_ADDAPP_2,IDB_BITMAP_ADDAPP_1);

		m_rBtnClose.LoadBitmaps(IDB_BITMAP_CLOSE , IDB_BITMAP_CLOSE2 ,IDB_BITMAP_CLOSE3,IDB_BITMAP_CLOSE);
		m_rBtnMin.LoadBitmaps(IDB_BITMAP_MIN , IDB_BITMAP_MIN2 ,IDB_BITMAP_MIN3,IDB_BITMAP_MIN);
		m_rBtnMainMenu.LoadBitmaps(IDB_BITMAP_NEMU1 , IDB_BITMAP_NEMU2 ,IDB_BITMAP_NEMU3,IDB_BITMAP_NEMU1);
		m_rCnButton.LoadBitmaps(IDB_BITMAP_NEMU1 , IDB_BITMAP_NEMU2 ,IDB_BITMAP_NEMU3,IDB_BITMAP_NEMU1);
		UpdateData(0);

		m_tooltip.Create(this); 
	    m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_MAIN_UI), UiFun::UI_LoadString("TITLEBAR_MODULE"  , "TITLEBAR_MAIN_INTERFACE",theApp.gsLanguage )); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_SEND), UiFun::UI_LoadString("TITLEBAR_MODULE"  , "TITLEBAR_SEND",theApp.gsLanguage )); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_RECE), UiFun::UI_LoadString("TITLEBAR_MODULE"  , "TITLEBAR_RECE",theApp.gsLanguage )); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_TRAD_INFO),UiFun::UI_LoadString("TITLEBAR_MODULE"  , "TITLEBAR_TRAD",theApp.gsLanguage )); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_P2P),UiFun::UI_LoadString("TITLEBAR_MODULE"  , "TITLEBAR_P2P",theApp.gsLanguage )); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_MORTGAGE), UiFun::UI_LoadString("TITLEBAR_MODULE"  , "TITLEBAR_MORTGAGE",theApp.gsLanguage )); 
		m_tooltip.AddTool((CButton   *)GetDlgItem(IDC_BUTTON_IPO), UiFun::UI_LoadString("TITLEBAR_MODULE"  , "TITLEBAR_IPO",theApp.gsLanguage )); 

		MobileTip(IDC_BUTTON_MAIN_UI , TRUE );

		newMenu.LoadMenu(IDR_MENU1);
		SetMenu(&newMenu);
		//ֻ���������˵��µĵ�һ�����ȫ����newMenu.ChangeMenuItem(&newMenu);
		newMenu.ChangeMenuItem(newMenu.GetSubMenu(0));

		//LanguageMenu.LoadMenu(IDR_MENU2);
		//SetMenu(&LanguageMenu);
		////ֻ���������˵��µĵ�һ�����ȫ����newMenu.ChangeMenuItem(&newMenu);
		//LanguageMenu.ChangeMenuItem(LanguageMenu.GetSubMenu(0));
		LanguageMenu.LoadMenu(IDR_MENU2);
		SetMenu(&LanguageMenu);

		CMenu *pPopup=LanguageMenu.GetSubMenu(0);
		if (theApp.language() == 1)
		{
			pPopup->CheckMenuItem(ID_CN, MF_BYCOMMAND|MF_CHECKED);
		}else{
			pPopup->CheckMenuItem(ID_EN, MF_BYCOMMAND|MF_CHECKED);
		}
	}
	return bRes ;
}

BOOL CIndTitleBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(pMsg->message) 
	{ 
	case   WM_MOUSEMOVE: 
		m_tooltip.RelayEvent(pMsg); 
	} 
	return CDialogBar::PreTranslateMessage(pMsg);
}
void CIndTitleBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	GetParent()->PostMessage(MSG_TITLE_LBUTTON_DOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialogBar::OnLButtonDown(nFlags, point);
}
void CIndTitleBar::OnBnClickedMenubar()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CBitmap pMenuBitmap;
	pMenuBitmap.LoadBitmap(IDB_BITMAP_MIN);//��Ҫ���ص�ͼ����Դ	

	RECT rect;
	GetDlgItem(IDC_MENUBAR)->GetWindowRect(&rect);//��ȡ�ؼ�����ȫƵ��λ��

	CMenu *pPopup=newMenu.GetSubMenu(0);
	if (theApp.HaveLocked)
	{
		pPopup->EnableMenuItem(ID__ENCRYPTWALLET,MF_GRAYED);
	}
	int b =pPopup->EnableMenuItem(ID__SET,MF_GRAYED);
	
	pPopup->SetMenuItemBitmaps(0,MF_BYPOSITION, &pMenuBitmap, &pMenuBitmap);
	//��ʾ�Ҽ��˵��������ര��ӵ�С�
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,rect.left,rect.bottom,this);
}

void CIndTitleBar::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(lpMeasureItemStruct->CtlType==ODT_MENU)//��������ǲ˵�
		newMenu.MeasureItem(lpMeasureItemStruct);//����CNewMenu���MeasureItem��Ա����
	else
	CDialogBar::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CIndTitleBar::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(lpDrawItemStruct->CtlType==ODT_MENU )
		newMenu.DrawItem(lpDrawItemStruct);
	else
	CDialogBar::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CIndTitleBar::OnBnClickedCn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RECT rect;
	GetDlgItem(IDC_CN)->GetWindowRect(&rect);//��ȡ�ؼ�����ȫƵ��λ��

	CMenu *pPopup=LanguageMenu.GetSubMenu(0);

	//pPopup->CheckMenuItem(ID_CN,MF_CHECKED);

	//��ʾ�Ҽ��˵��������ര��ӵ�С�
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,rect.left,rect.bottom,this);
}
void CIndTitleBar::OnChinese()
{
	/// Ӣ�Ľ���
   if (theApp.gsLanguage == 2)
   {
	   theApp.gsLanguage = 1;
	   Setlanguage(theApp.gsLanguage);
	  UiFun::MessageBoxEx(_T("�޸����Ի�������������Ǯ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
	  ((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
	 
   }
}
void CIndTitleBar::OnEnglish()
{
	/// ���Ľ���
	if (theApp.gsLanguage == 1)
	{
		theApp.gsLanguage = 2;
		Setlanguage(theApp.gsLanguage);
		UiFun::MessageBoxEx(_T("�޸����Ի�������������Ǯ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
	}
}



void CIndTitleBar::OnUpdateEn(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CMenu *pPopup=LanguageMenu.GetSubMenu(0);
	if (pPopup->GetMenuState(ID_CN,MF_CHECKED) == MF_CHECKED)
	{
		pPopup->CheckMenuItem(ID_CN, MF_BYCOMMAND|MF_USECHECKBITMAPS );
	}

	pPopup->CheckMenuItem(ID_EN, MF_BYCOMMAND|MF_CHECKED);
}


void CIndTitleBar::OnUpdateCn(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CMenu *pPopup=LanguageMenu.GetSubMenu(0);
	if (pPopup->GetMenuState(ID_EN,MF_CHECKED) == MF_CHECKED)
	{
		pPopup->CheckMenuItem(ID_EN, MF_BYCOMMAND|MF_USECHECKBITMAPS );
	}

	pPopup->CheckMenuItem(ID_CN, MF_BYCOMMAND|MF_CHECKED);
}
void CIndTitleBar::Setlanguage(int index)
{
	// ȡĬ����������
	string strTemp = strprintf("%d",index);;
	string  strG;
	string strAppIni = theApp.str_InsPath;// + (CString)LANGUAGE_FILE;
	strAppIni += LANGUAGE_FILE;
	::WritePrivateProfileString("Language1","gsLanguage",strTemp.c_str(),(LPCTSTR)strAppIni.c_str());
}