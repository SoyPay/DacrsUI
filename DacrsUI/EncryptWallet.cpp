// EncryptWallet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "EncryptWallet.h"
#include "afxdialogex.h"
#include "DacrsUIDlg.h"

// CEncryptWallet �Ի���

IMPLEMENT_DYNAMIC(CEncryptWallet, CDialogBase)

CEncryptWallet::CEncryptWallet(CWnd* pParent /*=NULL*/)
	: CDialogBase(CEncryptWallet::IDD, pParent)
{

}

CEncryptWallet::~CEncryptWallet()
{
}

void CEncryptWallet::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INPUTMIMA, m_password);
	DDX_Control(pDX, IDC_STATIC_CONFIREDMIMA, m_confiredpassword);	
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CEncryptWallet, CDialogBase)
	ON_BN_CLICKED(IDOK, &CEncryptWallet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CEncryptWallet::OnBnClickedClose)
END_MESSAGE_MAP()


// CEncryptWallet ��Ϣ�������
BOOL CEncryptWallet::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(90, _T("΢���ź�"));
	m_headText.SetTextColor(RGB(255,255,255));	

	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	CRect rect ;
	m_rBtnClose.GetClientRect(rect);

	RECT ret;
	GetWindowRect(&ret);
	m_rBtnClose.SetWindowPos(NULL ,(ret.right-ret.left)-rect.Width() , 2 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText("ȷ ��") ;
//	m_rBtnOk.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText("ȡ ��") ;
	//m_rBtnCancel.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CEncryptWallet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString inputE = _T("");
	CString confiedE = _T("");
	GetDlgItem(IDC_INPUTMIMA)->GetWindowText(inputE);
	GetDlgItem(IDC_CONFIREDMIMA)->GetWindowText(confiedE);

	int pos = inputE.Find(_T(" "));
	if (pos >=0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("���������в����пո�") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (inputE == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("��������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (confiedE == _T(""))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("������ȷ������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (inputE.GetLength()<6)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("�������������λ") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (IsAllDigtal(inputE))
	{
		::MessageBox( this->GetSafeHwnd() ,_T("���벻��ȫ������") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}
	if (strcmp(inputE,confiedE) != 0)
	{
		::MessageBox( this->GetSafeHwnd() ,_T("ȷ��������������벻һ��") , _T("��ʾ") , MB_ICONINFORMATION ) ;
		return;
	}

	string strCommand;
	strCommand = strprintf("%s %s",_T("encryptwallet"),inputE);
	string strShowData ="";
	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE(" rpccmd encryptwallet error");
		return;
	}

		
	bool isClose = FALSE; 
	bool isEntryp = root["encrypt"].asBool();
	isClose = isEntryp;
	if (isEntryp)
	{
		MessageBox(_T("Ǯ�����ܳɹ�����������Ǯ��"));

	}else
	{
		MessageBox(_T("Ǯ������ʧ��"));
	}

	CDialogBase::OnOK();
	if (isClose)
	{
		((CDacrsUIDlg*)(theApp.m_pMainWnd))->ClosWalletWind();
	}
}


void CEncryptWallet::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}
