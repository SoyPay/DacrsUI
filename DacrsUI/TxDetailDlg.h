#pragma once


// CTxDetailDlg �Ի���

class CTxDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTxDetailDlg)

public:
	CTxDetailDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTxDetailDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHOWTXDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void ShowTxDetail(CString jsontx);
	virtual BOOL OnInitDialog();
};
