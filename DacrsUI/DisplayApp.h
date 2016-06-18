#pragma once
#include "afxcmn.h"

#include <map>
#include "UiHead.h"
#include "json\json.h"
#include <fstream>
#include "ListCtrlCl.h"
#include "StaticTrans.h"
// CDisplayApp 对话框

class CDisplayApp : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplayApp)

public:
	CDisplayApp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDisplayApp();

// 对话框数据
	enum { IDD = IDD_DIALOG_APP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//CListCtrl m_LargeList;
	//CListCtrl m_Commctrl;
	//CListCtrl m_gameCtrl;
	
public:
	//	CSplitButton m_split_button;
	void InitListCtrl();
	BOOL  Download(const CString& strFileURLInServer, //待下载文件的URL
		const CString & strFileLocalFullPath);
	void InitAppListFromNetWork();
	void ReadLoalConfigFile(const string& strFilePath);
	void ReadURLConfigFile(const string& strUrlFilePath,const string& localpath);
	void DownLoadAllappPic(const string& strFilePath);
	void DownLoadPic(const string&UrpPath ,const string& strFilePath);
	void SetTypeCount();
	void ReadLargeApp(const Json::Value &root,map<string ,ADD_APP_DATA>& mapdata);
	void ReadCommApp(const Json::Value &root,map<string ,ADD_APP_DATA>& mapdata);
	void ReadGamesApp(const Json::Value &root,map<string ,ADD_APP_DATA>& mapdata);
	ADD_APP_DATA GetAppValue(const Json::Value &root);
	int GetAllCtrlHeight();
	void InitConfigAndList();
	void WriteCoinfig(const string& strFilePath);
	void GetObj(ADD_APP_DATA data,Json::Value &root);
	void GetAppUrlValue(int type,string key,ADD_APP_DATA &retdata);
	void MoidfyListAndSaveToFile(int type,string key,ADD_APP_DATA ModifyData);
	bool UnZipFile(string unzipfilename,string zipfilepath);
	bool DownLoadFile(const string&UrpPath ,const string& strFilePath);
	void CloseProcess();
public:
	CListCtrlCl m_Commctrl;
	CListCtrlCl m_gameCtrl;
	CListCtrlCl m_LargeList;  
	CStaticTrans     m_commstatic;
	CStaticTrans     m_gamestatic;
	int comm_app_count;
	int game_app_count;
	int comm_app_Line;
	int game_app_Line;
	map<string ,ADD_APP_DATA> m_commapp_list;
	map<string ,ADD_APP_DATA> m_gameapp_list;
	map<string ,ADD_APP_DATA> m_Largeapp_list;

	map<string ,ADD_APP_DATA> m_commappUrl_list;
	map<string ,ADD_APP_DATA> m_gameappUrl_list;
	map<string ,ADD_APP_DATA> m_LargeappUrl_list;
	vector<PROCESS_INFORMATION> m_process;
public:
	int largerHeight;
	int smallHeight;
	int largerWith;
	int smallWith;
public:
	int ctrid;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT onBnCLick( WPARAM wParam, LPARAM lParam );
	void updateListCtrlPos();
	string m_apppath;
	string m_configName;
	string m_urlpath;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};
