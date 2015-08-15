// MessageBoxEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxwin.h"
#include "DacrsUI.h"
#include "MessageBoxEx.h"
#include "afxdialogex.h"


// CMessageBoxEx �Ի���

IMPLEMENT_DYNAMIC(CMessageBoxEx, CDialogBase)

CMessageBoxEx::CMessageBoxEx(CString strText,CString strTip ,UINT uType ,CWnd* pParent /*=NULL*/)
	: CDialogBase(CMessageBoxEx::IDD, pParent)
{
	m_Text = strText;
	m_Tip  = strTip ;
	m_uType = uType ;
	m_pBmp = NULL;
//	SetBkBmpNid( IDB_BITMAP_MESSAGE_H) ;
	
}

CMessageBoxEx::~CMessageBoxEx()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CMessageBoxEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUIT, m_quit);
	DDX_Control(pDX, IDC_OK, m_OK);
	DDX_Control(pDX, IDC_CANCEL, m_cancel);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_strText);
	DDX_Control(pDX, IDC_TIP, m_strTip);
}


BEGIN_MESSAGE_MAP(CMessageBoxEx, CDialogBase)
	ON_BN_CLICKED(IDC_OK, &CMessageBoxEx::OnBnClickedOk)
	ON_BN_CLICKED(IDC_QUIT, &CMessageBoxEx::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_CANCEL, &CMessageBoxEx::OnBnClickedCancel)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMessageBoxEx ��Ϣ�������
/*
void CMessageBoxEx::SetBkBmpNid( UINT nBitmapIn ) 
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
*/
void CMessageBoxEx::SetBitmap(UINT uBmpResource)
{
	HBITMAP hBitmap; //���meaasgebox�����е�ͼƬ 
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_IMAGE); 
	//����ͼƬID 
	hBitmap = (HBITMAP)LoadImage(    
	AfxGetInstanceHandle(), 
	MAKEINTRESOURCE(uBmpResource), 
	IMAGE_BITMAP, 
	0, 
	0, 
	LR_LOADMAP3DCOLORS); 
	pStatic->ModifyStyle(0xF, SS_BITMAP|SS_CENTERIMAGE); 
	pStatic->SetBitmap(hBitmap); 
}
void  CMessageBoxEx::ReplaceEditText()
{
	string retTemp ;
	string textTemp = strprintf("%s",m_Text);
	int nPosn =textTemp.find("\n");
	int nPosr =textTemp.find("\r");
	int row = 1;
	while(nPosn >=0)
	{
		if ((nPosn-nPosr)==1 && nPosr >=0)
		{
			retTemp +=textTemp.substr(0,nPosn+1);
			textTemp=textTemp.substr(nPosn+1);
			row++;
		}else{
			if (nPosn >=0)
			{
				textTemp.replace (nPosn,1,"\r\n");
			}
		}
		nPosn =textTemp.find("\n");
		nPosr =textTemp.find("\r");
	}
	retTemp+=textTemp;
	m_Text=retTemp.c_str();
}
BOOL CMessageBoxEx::OnInitDialog()
{
	CDialogBase::OnInitDialog();
	ReplaceEditText();
//	CDialogBase::SetBkBmpNid(IDB_BITMAP_RED_BK, CDialogBase::m_pBmp) ;
//	CDialogBase::SetBkBmpNid(IDB_BITMAP_RED_TITLE, CDialogBase::m_HeadBmp);

	int nIconLeftMargin = 25;    //ͼ�������ʾ����߾�
	int nIconTopMargin = 45;     //ͼ�������ʾ���ϱ߾�
	int nBtBottomMargin = 10;    //��ť�����ʾ���±߾�
	int nBtRightMargin = 15;     //��ť�����ʾ���ұ߾�

	int nLeftMargin = 75;    //����static��߾�
	int nRightMargin = 55;   //����static�ұ߾�
    int nBottomMargin = 65;  //����static�±߾�
	int nStaticHeigh = 0;    //����static�߶�
	int nStaticWidth = 0;    //����static���
	int nDialogWidth = 0;
	int nDialogHight = 0;
	CClientDC dc(this);   
	//�������ݳ��ȼ�����ʾ��Ĵ�С
	int nRow = 1;
	CSize nFontSize = dc.GetTextExtent(m_Text);
	if(nFontSize.cx > MAX_WIDTH - nLeftMargin -nRightMargin) {
		nDialogWidth = MAX_WIDTH;
		nStaticWidth = MAX_WIDTH - nLeftMargin -nRightMargin;
		
	}
	else {
		nDialogWidth = nFontSize.cx + nLeftMargin + nRightMargin;	
		nStaticWidth = nFontSize.cx;
		
	}
	int nPos = m_Text.ReverseFind('\n');
	if(nPos != -1) {
		nRow = m_Text.Replace("\n", "\n");
		CString substr = m_Text.Right(m_Text.GetLength()-nPos);
		CSize nSubstrSize = dc.GetTextExtent(substr);
		if(nSubstrSize.cx % nStaticWidth == 0) {
			nRow +=  nSubstrSize.cx / nStaticWidth;
		}else {
			nRow +=  nSubstrSize.cx / nStaticWidth + 1;
		}
		
	}else {
		nRow =  nFontSize.cx / nStaticWidth;
	}
	nStaticHeigh = nRow * (nFontSize.cy);
	nDialogHight = nStaticHeigh + nBottomMargin + nIconTopMargin;
	

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);
	
	if (theApp.m_pMainWnd != NULL)
	{
		CRect rect;
		theApp.m_pMainWnd->GetWindowRect(&rect);
		theApp.m_pMainWnd->ClientToScreen(rect);
		MoveWindow((rect.left + rect.right)/2 - nDialogWidth/2,(rect.top + rect.bottom)/2 - nDialogHight/2 ,nDialogWidth, nDialogHight, FALSE);
	}else
	{
		MoveWindow(0,5 ,nDialogWidth, nDialogHight, FALSE);
	}

	CenterWindow();
	
	m_fontGrid.CreatePointFont(100,_T("������"));

	m_quit.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_quit.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_quit.SetWindowText("") ;
	m_quit.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_quit.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_quit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_quit.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_quit.SizeToContent();

	CRect titleRect ;
	m_quit.GetClientRect(titleRect);

	RECT ret;
	GetWindowRect(&ret);
	m_quit.SetWindowPos(NULL ,(ret.right-ret.left)-titleRect.Width() , 2 , 0 , 0 , SWP_NOSIZE); 

	//ͼƬ
	if ( (m_uType & MFB_TIP) == MFB_TIP ) {
		SetBitmap(IDB_BITMAP_TIP);
	}else if ( (m_uType & MFB_ERROR) == MFB_ERROR ) {
		SetBitmap(IDB_BITMAP_ERROR);
	}else if ( (m_uType & MFB_WARNING) == MFB_WARNING ) {
		SetBitmap(IDB_BITMAP_WARNING);
	}
	GetDlgItem(IDC_IMAGE)->MoveWindow(nIconLeftMargin , nIconTopMargin , 39 , 38);

	//��ʾ��Ϣλ��
	m_strTip.SetFont(100, _T("΢���ź�"));				//������ʾ����ʹ�С
	m_strTip.SetTextColor(RGB(255,255,255));	
	m_strTip.MoveWindow(2,2,381/4,145/5);
	m_strTip.SetWindowText(m_Tip);

	//��ʾ�ı���λ��
	//m_strText.SetFont(100, _T("����"));				//������ʾ����ʹ�С
	//m_strText.SetTextColor(RGB(0,0,0));	
	m_strText.MoveWindow(nLeftMargin, nIconTopMargin+10, nStaticWidth, nStaticHeigh);
	m_strText.SetWindowText(m_Text);

	if ( (m_uType & MFB_OKCANCEL) == MFB_OKCANCEL ) {
		  m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_OK.SetWindowText("ȷ��") ;
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_OK.SizeToContent();
		   m_OK.MoveWindow(nDialogWidth-nBtRightMargin-56-56-10, nDialogHight-nBtBottomMargin-23, 56, 23);

		   m_cancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_cancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_cancel.SetWindowText("ȡ��") ;
		  
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_cancel.SizeToContent();
		   //��ťλ��
		   m_cancel.MoveWindow(nDialogWidth-nBtRightMargin-56,nDialogHight-nBtBottomMargin-23, 56, 23);
	}
	else if ( (m_uType & MFB_YESNO) == MFB_YESNO ) {
		   m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_OK.SetWindowText("��") ;
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_OK.SizeToContent();
		   //�ǰ�ťλ��
		   m_OK.MoveWindow(nDialogWidth-nBtRightMargin-56-56-10, nDialogHight-nBtBottomMargin-23, 56, 23);;
		   //��ť
		   m_cancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_cancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_cancel.SetWindowText("��") ;
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_cancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_cancel.SizeToContent();
		   //��ťλ��
		   m_cancel.MoveWindow(nDialogWidth-nBtRightMargin-56,nDialogHight-nBtBottomMargin-23, 56, 23);
	}
	else if ( (m_uType & MFB_OK) == MFB_OK ) {
		   m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_OK.SetWindowText("ȷ��") ;
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_OK.SizeToContent();
		   m_OK.MoveWindow(nDialogWidth-nBtRightMargin-56, nDialogHight-nBtBottomMargin-23,56,23);
		   m_cancel.ShowWindow(SW_HIDE);
	} 

	else if ( (m_uType & MFB_YES) == MFB_YES ) {
		   m_OK.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		   m_OK.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		   m_OK.SetWindowText("��") ;
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
		   m_OK.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
		   m_OK.SizeToContent();
		   m_OK.MoveWindow(nDialogWidth-nBtRightMargin-56,nDialogHight-nBtBottomMargin-23, 56, 23);
		   m_cancel.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CMessageBoxEx::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if((m_uType & MFB_OK) == MFB_OK){
		EndDialog(IDOK);
	}
	else if((m_uType & MFB_YES) == MFB_YES) {
		EndDialog(IDYES);
	}
	CDialogBase::OnClose();
}

void CMessageBoxEx::OnBnClickedQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if((m_uType & MFB_CANCEL) == MFB_CANCEL){
		EndDialog(IDCANCEL);
	}
	else if((m_uType & MFB_NO) == MFB_NO) {
		EndDialog(IDNO);
	}else if ((m_uType & MFB_YES) == MFB_YES)
	{
		EndDialog(IDYES);
	}else if((m_uType & MFB_OK) == MFB_OK){
		EndDialog(IDOK);
	}
	CDialogBase::OnClose();
}

void CMessageBoxEx::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if((m_uType & MFB_CANCEL) == MFB_CANCEL){
		EndDialog(IDCANCEL);
	}
	else if((m_uType & MFB_NO) == MFB_NO) {
		EndDialog(IDNO);
	}
	CDialogBase::OnClose();
}

BOOL CMessageBoxEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*
	if((m_uType & MFB_BK_RED) == MFB_BK_RED) 
	{
		CRect   rect; 
		GetClientRect(&rect); 

		if(m_pBmp   !=   NULL) { 
			BITMAP   bm; 
			CDC   dcMem; 

			::GetObject(m_HeadBmp, sizeof(BITMAP),   (LPVOID)&bm); 
			dcMem.CreateCompatibleDC(NULL); 
			HBITMAP     pOldhBitmap   =(HBITMAP   )   dcMem.SelectObject(m_HeadBmp); 
			int height = bm.bmHeight;
			pDC->StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 
			dcMem.SelectObject(pOldhBitmap);

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); */
	CDialogBase::OnEraseBkgnd(pDC);

	return TRUE;
}

HBRUSH CMessageBoxEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	//if (nCtlColor == CTLCOLOR_STATIC)
	//{
	//	/*pDC->SetBkMode(TRANSPARENT);
	//	pDC->SelectObject(&m_fontGrid);
	//	hbr = (HBRUSH)CreateSolidBrush(RGB(224,224,224));*/
	//}
	return hbr;
}
