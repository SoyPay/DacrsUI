#pragma once


// CRedPacketList �Ի���

class CRedPacketList : public CDialogEx
{
	DECLARE_DYNAMIC(CRedPacketList)

public:
	CRedPacketList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRedPacketList();

// �Ի�������
	enum { IDD = IDD_DIALOG_REDPACKETLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowTxDetail(CString txhash);
	CListCtrl   m_listCtrl;
};
