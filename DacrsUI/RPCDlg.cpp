// RPCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "RPCDlg.h"
#include "afxdialogex.h"
#include "CommonStruct.h"

// CRPCDlg 对话框

IMPLEMENT_DYNAMIC(CRPCDlg, CDialogEx)

CRPCDlg::CRPCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRPCDlg::IDD, pParent)
{

}

CRPCDlg::~CRPCDlg()
{
}

void CRPCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_RPC, m_rpccommand);
	DDX_Control(pDX, IDC_COMBO_INPUT, m_comboxinput);
}


BEGIN_MESSAGE_MAP(CRPCDlg, CDialogEx)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_MESSAGE(WM_SHOW_SEND_DATA,OnShowSendData)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CRPCDlg::OnBnClickedBtnClear)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CRPCDlg 消息处理程序
BOOL CRPCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rcClient;
	GetClientRect(&rcClient);

	//m_rpccommand.SetBkColor(RGB(205,225,243));
	//CEdit* pShowInfoEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOWINFO);
	//ASSERT(pShowInfoEdit);
	//pShowInfoEdit->MoveWindow(5,10,rcClient.Width()-60,rcClient.Height()-130);

	CTreeCtrl* pShowInfoEdit = (CTreeCtrl*)GetDlgItem(IDC_TREE_RPC);
	ASSERT(pShowInfoEdit);
	pShowInfoEdit->MoveWindow(5,10,rcClient.Width()-60,rcClient.Height()-130);

	CStatic* pStaticInput = (CStatic*)GetDlgItem(IDC_STATIC_INPUT);
	ASSERT(pStaticInput);
	pStaticInput->MoveWindow(5,rcClient.Height()-100,40,20);

	CComboBox* pInputEdit = (CComboBox*)GetDlgItem(IDC_COMBO_INPUT);
	ASSERT(pInputEdit);
	pInputEdit->MoveWindow(50,rcClient.Height()-103,rcClient.Width()-160,25);

	CButton* pBtnClear = (CButton*)GetDlgItem(IDC_BTN_CLEAR);
	ASSERT(pBtnClear);
	pBtnClear->MoveWindow(rcClient.Width()-95,rcClient.Height()-103,30,25);

	AutoCombo.Init(&m_comboxinput);

	CStringA configpath = "";
	configpath.AppendFormat("%s",theApp.str_InsPath);
	configpath.AppendFormat("\\%s","dacrsclient.conf");

	if (PathFileExistsA(configpath))
	{
		CJsonConfigHelp::getInstance()->AddItemString(configpath,AutoCombo);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CRPCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		//if (pMsg->hwnd == GetDlgItem(IDC_COMBO_INPUT)->GetSafeHwnd())
		if(m_comboxinput.IsChild(GetFocus()))
		{
			CString strCommand;
			GetDlgItem(IDC_COMBO_INPUT)->GetWindowText(strCommand);
			GetDlgItem(IDC_COMBO_INPUT)->SetWindowText(_T(""));

			CStringA strSendData,strretshow;
			RPCCommandToJson(strCommand,strSendData);
			CSoyPayHelp::getInstance()->SendRpcDlg(strSendData,strretshow);
			ShowJson(strCommand,strretshow);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CRPCDlg::OnBnClickedBtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	//GetDlgItem(IDC_EDIT_SHOWINFO)->SetWindowText(_T(""));
	m_rpccommand.DeleteAllItems();
}
LRESULT CRPCDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	UpdateEditContent(str,_T("RecvData:"));
	return TRUE;
}
LRESULT CRPCDlg::OnShowSendData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	UpdateEditContent(str,_T("SendData:"));
	return TRUE;
}
void CRPCDlg::UpdateEditContent(const CStringA& strNewData,const CString& strKeyWord)
{
	/*CString strShow;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOWINFO);
	pEdit->GetWindowText(strShow);
	if(strShow.GetLength()> 1024*10)
	strShow.Empty();
	strShow += _T("\r\n");
	strShow += strKeyWord;
	strShow += _T("\r\n");
	strShow += CString(strNewData.GetString());
	pEdit->SetWindowText(strShow);

	int nLines = pEdit->GetLineCount();
	pEdit->LineScroll(nLines);*/
}


void CRPCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;
		CRect rc ;
		GetClientRect( rc ) ;
		CWnd *pst = GetDlgItem( IDC_TREE_RPC ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL , 0 , 0 , rc.Width(), 90*rc.Height()/div ,SWP_SHOWWINDOW ) ;
		}
		pst = GetDlgItem( IDC_STATIC_INPUT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , 0 , 94*rc.Height()/div , 20*rc.Width()/div, rect.Height() ,SWP_SHOWWINDOW ) ;
		}
		pst = GetDlgItem( IDC_COMBO_INPUT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (20*rc.Width()/div)+5  , 94*rc.Height()/div - 2 , 65*rc.Width()/div, 65*rc.Width()/div ,SWP_SHOWWINDOW ) ;
		}
		pst = GetDlgItem( IDC_BTN_CLEAR ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , 90*rc.Width()/div  , 94*rc.Height()/div - 5 , rect.Width(), rect.Height() ,SWP_SHOWWINDOW ) ;
		}
	}
}

void CRPCDlg::ShowJson(CString rpcname,CString jsomstr){
	if (jsomstr == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(jsomstr.GetString(), root)) 
		return ;
	m_rpccommand.Expand(m_rpccommand.GetRootItem(),TVE_COLLAPSE);
	HTREEITEM hItemRoot = m_rpccommand.InsertItem(rpcname,TVI_ROOT);
	CSoyPayHelp::getInstance()->Jiexi(root,hItemRoot,m_rpccommand);
	m_rpccommand.Expand(hItemRoot,TVE_EXPAND);
}