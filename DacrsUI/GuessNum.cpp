// GuessNum.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "GuessNum.h"
#include "afxdialogex.h"


// CGuessNum �Ի���

IMPLEMENT_DYNAMIC(CGuessNum, CDialogEx)

CGuessNum::CGuessNum(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGuessNum::IDD, pParent)
{

}

CGuessNum::~CGuessNum()
{
}

void CGuessNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGuessNum, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGuessNum::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGuessNum::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGuessNum ��Ϣ�������


void CGuessNum::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.m_strAddress=_T("");
	char guess = 0x00;
	guess = ((CButton *)GetDlgItem(IDC_RADIO_MAN))->GetCheck();
	theApp.m_strAddress.Format(_T("%d"),(int)guess);
	CDialogEx::OnOK();
}


void CGuessNum::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.m_strAddress=_T("");
	CDialogEx::OnCancel();
}


BOOL CGuessNum::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO_MAN);
	radio->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
