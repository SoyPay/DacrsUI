#if !defined(AFX_DLGVIEW_H__4BDEBFEB_9C39_44D2_B68C_CD551696D69C__INCLUDED_)
#define AFX_DLGVIEW_H__4BDEBFEB_9C39_44D2_B68C_CD551696D69C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgView.h : header file
//
#include <map>
#include "UiHead.h"
#include "json\json.h"
#include <fstream>
#include "ListCtrlCl.h"
#include "StaticTrans.h"
class CAddApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgView view

class CDlgView : public CScrollView
{
	friend class CAddApp;
protected:
	CDlgView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDlgView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDlgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDlgView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	static int m_nColWidths[];// 用于保存列宽度  
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void InitListCtrl();
public:
	//	CSplitButton m_split_button;

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
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIEW_H__4BDEBFEB_9C39_44D2_B68C_CD551696D69C__INCLUDED_)
