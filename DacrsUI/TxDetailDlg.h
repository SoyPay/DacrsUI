#pragma once


// CTxDetailDlg 对话框

class CTxDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTxDetailDlg)

public:
	CTxDetailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTxDetailDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOWTXDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ShowTxDetail(CString jsontx);
	virtual BOOL OnInitDialog();
};
