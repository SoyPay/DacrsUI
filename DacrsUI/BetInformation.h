#pragma once


// CBetInformation �Ի���

class CBetInformation : public CDialogEx
{
	DECLARE_DYNAMIC(CBetInformation)

public:
	CBetInformation(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBetInformation();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	void ShowBetRecordDetail(CString jsontx);
};
