#pragma once


// CBetInformation 对话框

class CBetInformation : public CDialogEx
{
	DECLARE_DYNAMIC(CBetInformation)

public:
	CBetInformation(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBetInformation();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	void ShowBetRecordDetail(CString jsontx);
};
