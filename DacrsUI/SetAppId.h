#pragma once


// CSetAppId �Ի���

class CSetAppId : public CDialogEx
{
	DECLARE_DYNAMIC(CSetAppId)

public:
	CSetAppId(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetAppId();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETAPPID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetDaluft();
	virtual BOOL OnInitDialog();
	bool IsRegId(const char* pData);
};
