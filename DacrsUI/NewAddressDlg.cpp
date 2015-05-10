// NewAddressDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "NewAddressDlg.h"
#include "afxdialogex.h"

// CNewAddressDlg �Ի���

IMPLEMENT_DYNAMIC(CNewAddressDlg, CDialogEx)

CNewAddressDlg::CNewAddressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewAddressDlg::IDD, pParent)
{
	m_pBmp = NULL ; 
}

CNewAddressDlg::~CNewAddressDlg()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CNewAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewAddressDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CNewAddressDlg::OnBnClickedButton1)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CNewAddressDlg ��Ϣ�������

void CNewAddressDlg::SetBkBmpNid( UINT nBitmapIn ) 
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


void CNewAddressDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CDC memDC;  
	memDC.CreateCompatibleDC(&dc);  
	CRect rc;  
	GetClientRect(&rc);  

	HBITMAP hOldbmp = (HBITMAP)memDC.SelectObject(m_pBmp); 
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);  
	memDC.SelectObject(hOldbmp);  
	memDC.DeleteDC();  
}


void CNewAddressDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strCommand;
	int nCold = 0;
	if( ((CButton*)GetDlgItem(IDC_RADIO_YES))->GetCheck() ) {
		strCommand.Format(_T("%s"),_T("getnewaddress true"));
		nCold = 1;
	}else if ( ((CButton*)GetDlgItem(IDC_RADIO_NO))->GetCheck()){
		strCommand.Format(_T("%s"),_T("getnewaddress"));
	}
	CStringA strShowData ;
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	int pos = strShowData.Find("addr");
	if ( pos < 0 ) return ;

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;
	CString addr = root["addr"].asCString();

	CString Leble;
	GetDlgItem(IDC_EDIT_Leble)->GetWindowText(Leble);

	CString strSourceData;
	strSourceData.Format(_T("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'") , addr ,"",0.0 ,nCold ,0,Leble) ;
	uistruct::DATABASEINFO_t   pDatabase;
	pDatabase.strSource = strSourceData.GetString();
	pDatabase.strTabName =  _T("MYWALLET");
	CPostMsg postmsg(MSG_USER_INSERT_DATA,0);
	string  strTemp = pDatabase.ToJson();
	postmsg.SetData(strTemp.c_str());
	theApp.m_MsgQueue.push(postmsg);

	strCommand.Format(_T("��ϲ�����µ�ַ%s"),addr);
	::MessageBox( this->GetSafeHwnd() ,_T("��ϲ�����µ�ַ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
}


BOOL CNewAddressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBkBmpNid(IDB_BITMAP_DLG_BJ);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

