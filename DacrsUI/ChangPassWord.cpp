// ChangPassWord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "ChangPassWord.h"
#include "afxdialogex.h"


// CChangPassWord �Ի���

IMPLEMENT_DYNAMIC(CChangPassWord, CDialogBase)

CChangPassWord::CChangPassWord(CWnd* pParent /*=NULL*/)
	: CDialogBase(CChangPassWord::IDD, pParent)
{

}

CChangPassWord::~CChangPassWord()
{
}

void CChangPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICODLE, m_oldpassword);
	DDX_Control(pDX, IDC_STATICNEW, m_newpassword);	
	DDX_Control(pDX, IDC_STATICCOMFIRED, m_comfiredpassword);	
	DDX_Control(pDX, IDC_STATIC4, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CChangPassWord, CDialogBase)
	ON_BN_CLICKED(IDOK, &CChangPassWord::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CChangPassWord::OnBnClickedClose)
END_MESSAGE_MAP()


// CChangPassWord ��Ϣ�������


void CChangPassWord::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString inputOld = _T("");
	CString confiedNew= _T("");
	CString confiedPassWord = _T("");
	GetDlgItem(IDC_EDIT_OLDPASSWORD)->GetWindowText(inputOld);
	GetDlgItem(IDC_EDIT_NEWPASSWORD)->GetWindowText(confiedNew);
	GetDlgItem(IDC_EDIT_CONFIREDPASSWORD)->GetWindowText(confiedPassWord);
	int pos = confiedNew.Find(_T(" "));
	if (pos >=0)
	{
		UiFun::MessageBoxEx(_T("�����������в����пո�") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	if (inputOld == _T(""))
	{
		UiFun::MessageBoxEx(_T("�����������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (confiedNew == _T(""))
	{
		UiFun::MessageBoxEx(_T("������������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (confiedPassWord == _T(""))
	{
		UiFun::MessageBoxEx(_T("������ȷ��������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (IsAllDigtal(confiedNew))
	{
		UiFun::MessageBoxEx(_T("���벻��ȫ������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	if (confiedNew.GetLength()<6)
	{
		UiFun::MessageBoxEx(_T("�������������λ") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	if (strcmp(confiedPassWord,confiedNew) != 0)
	{
		UiFun::MessageBoxEx(_T("ȷ��������������벻һ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	string strCommand;
	strCommand = strprintf("%s %s %s",_T("walletpassphrasechange"),inputOld,confiedNew);

	Json::Value root; 
	if (!CSoyPayHelp::getInstance()->SendRpc(strCommand.c_str(),root))
	{
		TRACE("rpccmd walletpassphrasechange error");
		return;
	}

	bool isEntryp = root["chgpwd"].asBool();
	if (!isEntryp)
	{
		UiFun::MessageBoxEx(_T("��������벻��ȷ,����������") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}else{
		UiFun::MessageBoxEx(_T("��ϲ�޸�����ɹ�") , _T("��ʾ") ,MFB_OK|MFB_TIP );
	}
	
	CDialogBase::OnOK();
}


void CChangPassWord::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}


BOOL CChangPassWord::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_headText.SetFont(100, _T("΢���ź�"));
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
	//m_rBtnOk.SetFontEx(12 , _T("����"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT2 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText("ȡ ��") ;
	//m_rBtnCancel.SetFontEx(12 , _T("����"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
