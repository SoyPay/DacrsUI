#pragma once


// CRedPacketList 对话框

class CRedPacketList : public CDialogEx
{
	DECLARE_DYNAMIC(CRedPacketList)

public:
	CRedPacketList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRedPacketList();

// 对话框数据
	enum { IDD = IDD_DIALOG_REDPACKETLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowTxDetail(CString txhash);
	CListCtrl   m_listCtrl;
};
