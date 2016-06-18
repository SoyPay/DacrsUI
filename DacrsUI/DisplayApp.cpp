// DisplayApp.cpp : 实现文件
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DisplayApp.h"
#include "afxdialogex.h"
#include <afxinet.h>
#include  <io.h>
#include "zip\zip.h"
#include "zip\unzip.h"
// CDisplayApp 对话框

IMPLEMENT_DYNAMIC(CDisplayApp, CDialogEx)

CDisplayApp::CDisplayApp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplayApp::IDD, pParent)
{
	comm_app_count = 0;
	game_app_count = 0;
	largerHeight = 136;
	smallHeight = 68;
	largerWith = 282;
	smallWith = 282 ;
	ctrid = 2000;
	comm_app_Line = 0;
	game_app_Line = 0;
	m_apppath = "";
	m_configName = "";
	m_urlpath = "";
}

CDisplayApp::~CDisplayApp()
{
	CloseProcess();
}

void CDisplayApp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LARGE, m_LargeList);
	DDX_Control(pDX, IDC_LIST_COMM, m_Commctrl);
	DDX_Control(pDX, IDC_LIST_GAME, m_gameCtrl);
}

BEGIN_MESSAGE_MAP(CDisplayApp, CDialogEx)
	ON_WM_SIZE()
	ON_MESSAGE( WM_BN_CLICK, &CDisplayApp::onBnCLick)
END_MESSAGE_MAP()


// CDisplayApp 消息处理程序
BOOL  CDisplayApp::Download(const CString& strFileURLInServer, //待下载文件的URL
	const CString & strFileLocalFullPath)//存放到本地的路径
{
	ASSERT(strFileURLInServer != "");
	ASSERT(strFileLocalFullPath != "");
	CInternetSession session(_T("test"));
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	CString strServer, strObject;
	INTERNET_PORT wPort;

	DWORD dwType;
	const int nTimeOut = 2000;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //重试之间的等待延时
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //重试次数
	char* pszBuffer = NULL; 

	try
	{
		AfxParseURL(strFileURLInServer, dwType, strServer, strObject, wPort);
		pHttpConnection = session.GetHttpConnection(strServer, wPort);
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
		if(pHttpFile->SendRequest() == FALSE)
			return false;
		DWORD dwStateCode;

		pHttpFile->QueryInfoStatusCode(dwStateCode);
		if(dwStateCode == HTTP_STATUS_OK)
		{
			HANDLE hFile = CreateFile(strFileLocalFullPath, GENERIC_WRITE,
				FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
				NULL);  //创建本地文件
			if(hFile == INVALID_HANDLE_VALUE)
			{
				pHttpFile->Close();
				pHttpConnection->Close();
				session.Close();
				return false;
			}

			char szInfoBuffer[1000];  //返回消息
			DWORD dwFileSize = 0;   //文件长度
			DWORD dwInfoBufferSize = sizeof(szInfoBuffer);
			BOOL bResult = FALSE;
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
				(void*)szInfoBuffer,&dwInfoBufferSize,NULL);

			dwFileSize = atoi(szInfoBuffer);
			const int BUFFER_LENGTH = 1024 * 10;
			pszBuffer = new char[BUFFER_LENGTH];  //读取文件的缓冲
			DWORD dwWrite, dwTotalWrite;
			dwWrite = dwTotalWrite = 0;
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //读取服务器上数据

			while(nRead > 0)
			{
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);  //写到本地文件
				dwTotalWrite += dwWrite;
				nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH);
			}

			delete[]pszBuffer;
			pszBuffer = NULL;
			CloseHandle(hFile);
		}
		else
		{
			delete[]pszBuffer;
			pszBuffer = NULL;
			if(pHttpFile != NULL)
			{
				pHttpFile->Close();
				delete pHttpFile;
				pHttpFile = NULL;
			}
			if(pHttpConnection != NULL)
			{
				pHttpConnection->Close();
				delete pHttpConnection;
				pHttpConnection = NULL;
			}
			session.Close();
			return false;
		}
	}
	catch(...)
	{
		delete[]pszBuffer;
		pszBuffer = NULL;
		if(pHttpFile != NULL)
		{
			pHttpFile->Close();
			delete pHttpFile;
			pHttpFile = NULL;
		}
		if(pHttpConnection != NULL)
		{
			pHttpConnection->Close();
			delete pHttpConnection;
			pHttpConnection = NULL;
		}
		session.Close();
		return false;
	}

	if(pHttpFile != NULL)
		pHttpFile->Close();
	if(pHttpConnection != NULL)
		pHttpConnection->Close();
	session.Close();
	return true;
}
void CDisplayApp::InitAppListFromNetWork()
{
	m_configName ="app.config";
	if (theApp.language() == 2)
	{
		m_apppath =theApp.str_InsPath +"\\English";
		m_urlpath = "http://www.dacrs.com/updata/english";
	}else{
		m_apppath =theApp.str_InsPath +"\\Chinese";
		m_urlpath = "http://www.dacrs.com/updata/chinese";
	}
	if (!PathIsDirectory(m_apppath.c_str()))
	{
		::CreateDirectory(m_apppath.c_str(), NULL);
	}

	string strFile =m_apppath +"\\app.config";
	if( (_access( strFile.c_str(), 0 )) == -1 )
	{
		HANDLE  handle= CreateFile(strFile.c_str(),GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
		if(handle==INVALID_HANDLE_VALUE){
			UiFun::MessageBoxEx(_T("创建文件失败\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
		}else
		{
			CloseHandle(handle);
		}
	}
	///从网上读取配置文件
	ReadLoalConfigFile(strFile);
	//读取本地配置文件
	string urlpath = m_urlpath +"/app.config";
	string urlpathTemp =m_apppath +"\\apptemp.config";
	ReadURLConfigFile(urlpath,urlpathTemp);
	/// 比较本地文件和网上文件,并且保存到本地文件
	InitConfigAndList();
	/// 设置各个控件的个数
	SetTypeCount();

	/// 下载应用程序的图片
	DownLoadAllappPic(m_apppath);
}
void CDisplayApp::ReadLoalConfigFile(const string& strFilePath)
{
	ifstream ifs;
	ifs.open(strFilePath,ios::in);
	if (!ifs.is_open())
	{
		return ;
	}
	//ifs.read()
	Json::Reader reader;  
	Json::Value root;  
	if (reader.parse(ifs, root,false))
	{
		ReadLargeApp(root,m_Largeapp_list);
		ReadCommApp(root,m_commapp_list);
		ReadGamesApp(root,m_gameapp_list);
	}
	ifs.close();
}
void CDisplayApp::ReadURLConfigFile(const string& strUrlFilePath,const string& localpath)
{

	//if (Download(strUrlFilePath.c_str(),localpath.c_str()))
	if (DownLoadFile(strUrlFilePath.c_str(),localpath.c_str()))
	{
		ifstream ifs;
		ifs.open(localpath,ios::in);
		if (!ifs.is_open())
		{
			return ;
		}
		//ifs.read()
		Json::Reader reader;  
		Json::Value root;  
		if (reader.parse(ifs, root,false))
		{
			ReadLargeApp(root,m_LargeappUrl_list);
			ReadCommApp(root,m_commappUrl_list);
			ReadGamesApp(root,m_gameappUrl_list);
		}
		ifs.close();
		///删除下载的临时文件
		DeleteFile(localpath.c_str());
	}else{
		UiFun::MessageBoxEx(_T("下载配置文件失败\r\n") , _T("提示") ,MFB_OK|MFB_ERROR );
	}
}

bool CDisplayApp::DownLoadFile(const string&UrpPath ,const string& strFilePath)
{  
	CInternetSession session;
	std::string strHtml;

	try
	{
		CHttpFile* pfile = (CHttpFile*)session.OpenURL(UrpPath.c_str(),1,INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_RELOAD,NULL,0);

		DWORD dwStatusCode;    
		pfile->QueryInfoStatusCode(dwStatusCode);    
		if(dwStatusCode == HTTP_STATUS_OK)    
		{    
			char strBuff[1025] = {0};
			while ((pfile->Read((void*)strBuff, 1024)) > 0) 
			{ 
				strHtml += strBuff; 
			} 
		}
		else
		{
			return false;
		}

		pfile->Close();    
		delete pfile;    
		session.Close();
	}
	catch (CException* e)
	{
		e;//消除警告
		return false;
	}

	if (!strHtml.empty())
	{
		ofstream outfile(strFilePath);  
		if (!outfile.is_open())
		{
			return false;
		}
		outfile<<strHtml;
		outfile.close();
	}
	return true;
}

void CDisplayApp::DownLoadAllappPic(const string& strFilePath)
{
	string path = strFilePath;

	map<string ,ADD_APP_DATA>::const_iterator item = m_LargeappUrl_list.begin();

	for (;item != m_LargeappUrl_list.end();item++)
	{
		string apppath =path ;
		apppath +=strprintf("\\%s",item->first);
		if (!PathIsDirectory(apppath.c_str()))
		{
			::CreateDirectory(apppath.c_str(), NULL);
		}
		string urlpath = m_urlpath + strprintf("/%s",item->first);
		DownLoadPic(urlpath,apppath);
	}

	item = m_gameappUrl_list.begin();

	for (;item != m_gameappUrl_list.end();item++)
	{
		string apppath =path ;
		apppath +=strprintf("\\%s",item->first);
		if (!PathIsDirectory(apppath.c_str()))
		{
			::CreateDirectory(apppath.c_str(), NULL);
		}
		string urlpath = m_urlpath  + strprintf("/%s",item->first);
		DownLoadPic(urlpath,apppath);
	}

	item = m_commappUrl_list.begin();

	for (;item != m_commappUrl_list.end();item++)
	{
		string apppath =path ;
		apppath +=strprintf("\\%s",item->first);
		if (!PathIsDirectory(apppath.c_str()))
		{
			::CreateDirectory(apppath.c_str(), NULL);
		}
		string urlpath = m_urlpath + strprintf("/%s",item->first);
		DownLoadPic(urlpath,apppath);
	}
}

//string button_out_pic ini.bmp;      // 初始按钮图片名字
//string button_in_pic Install.bmp;     // 未安装应用鼠标放上去的图片
//string Installbutton_in_pic  UnInstall.bmp;     // 安装应用鼠标放上去的图片
//string Installbutton_in_pic  click.bmp;     // 未安装应用鼠标点击图片

void CDisplayApp::DownLoadPic(const string&UrpPath ,const string& strFilePath)
{
	string picPath = strFilePath;
	picPath +="\\ini.bmp";
	string downloadpathe = UrpPath +"/ini.bmp";
	if(_access(picPath.c_str(),0) ==-1)
		Download(downloadpathe.c_str(),picPath.c_str());
	picPath ="";
	picPath = strFilePath +"\\Install.bmp";
	downloadpathe = UrpPath +"/Install.bmp";
	if(_access(picPath.c_str(),0) ==-1)
		Download(downloadpathe.c_str(),picPath.c_str());
	picPath ="";
	picPath = strFilePath +"\\UnInstall.bmp";
	downloadpathe = UrpPath +"/UnInstall.bmp";
	if(_access(picPath.c_str(),0) ==-1)
		Download(downloadpathe.c_str(),picPath.c_str());
	picPath ="";
	picPath = strFilePath +"\\click.bmp";
	downloadpathe = UrpPath +"/click.bmp";
	if(_access(picPath.c_str(),0) ==-1)
		Download(downloadpathe.c_str(),picPath.c_str());
}
void CDisplayApp::SetTypeCount()
{
	comm_app_count =m_commapp_list.size() ;
	game_app_count = m_gameapp_list.size();

	comm_app_Line =  (comm_app_count%3) ==0?(comm_app_count/3):(comm_app_count/3+1);
	game_app_Line =(game_app_count%3) ==0?(game_app_count/3):(game_app_count/3+1);;
}
void CDisplayApp::ReadLargeApp(const Json::Value &root,map<string ,ADD_APP_DATA>& mapdata)
{
	Json::Value arrayTags = root["lagerapp"];
	if (arrayTags.isNull())
	{
		return;
	}
	for(unsigned int i=0; i<arrayTags.size();++i)
	{
		ADD_APP_DATA appdata = GetAppValue(arrayTags[i]);
		mapdata[appdata.appname] = appdata;
	}
}
void CDisplayApp::ReadCommApp(const Json::Value &root,map<string ,ADD_APP_DATA>& mapdata)
{
	Json::Value arrayTags = root["commapp"];
	if (arrayTags.isNull())
	{
		return;
	}
	for(unsigned int i=0; i<arrayTags.size();++i)
	{
		ADD_APP_DATA appdata = GetAppValue(arrayTags[i]);
		mapdata[appdata.appname] = appdata;
	}
}
void CDisplayApp::ReadGamesApp(const Json::Value &root,map<string ,ADD_APP_DATA>& mapdata)
{
	Json::Value arrayTags = root["gameapp"];
	if (arrayTags.isNull())
	{
		return;
	}
	for(unsigned int i=0; i<arrayTags.size();++i)
	{
		ADD_APP_DATA appdata = GetAppValue(arrayTags[i]);
		mapdata[appdata.appname] = appdata;
	}
}
ADD_APP_DATA CDisplayApp::GetAppValue(const Json::Value &root)
{
	ADD_APP_DATA ret;
	ret.appname = root["appname"].asString();
	ret.isLagerPic = root["isLagerPic"].asBool();
	ret.isInstall =  root["isInstall"].asBool();
	ret.type =  root["type"].asInt();
	ret.version =root["version"].asInt();
	return ret;
}
int CDisplayApp::GetAllCtrlHeight()
{
	int height = 0;

	height = 30 + largerHeight;
	height += 20 + 20;
	height += (smallHeight+5)*comm_app_Line +10;
	height += 20 + 20;
	height += (smallHeight+5)*game_app_Line+10;

	height += 50;
	return height;
}

void CDisplayApp::updateListCtrlPos()
{
	m_Commctrl.updateListCtrlButtonPos();
	m_gameCtrl.updateListCtrlButtonPos();
}
void CDisplayApp::InitConfigAndList()
{
	bool flag = false;       /// 是否要将配置文件写到文件中
	map<string ,ADD_APP_DATA>::const_iterator item = m_commappUrl_list.begin();
	for (;item != m_commappUrl_list.end();item++)
	{
		if (m_commapp_list.size() ==0)
		{
			m_commapp_list[item->first] = item->second;
			flag = true;
		}else{
			map<string ,ADD_APP_DATA>::const_iterator itfind = m_commapp_list.find(item->first);
			if (itfind == m_commapp_list.end())
			{
				m_commapp_list[item->first] = item->second;
				flag = true;
			}
		}

	}

	item = m_commapp_list.begin();

	for (;item != m_commapp_list.end();item)
	{
		if (m_commappUrl_list.size() ==0)
		{
			m_commapp_list.clear();
			flag = true;
			break;
		}
		map<string ,ADD_APP_DATA>::const_iterator itfind = m_commappUrl_list.find(item->first);
		if (itfind == m_commappUrl_list.end())
		{
			item =m_commapp_list.erase(itfind);
			flag = true;
		}else{
			item++;
		}
	}

	item = m_gameappUrl_list.begin();
	for (;item != m_gameappUrl_list.end();item++)
	{
		if (m_gameapp_list.size() ==0)
		{
			m_gameapp_list[item->first] = item->second;
			flag = true;
		}else{
			map<string ,ADD_APP_DATA>::const_iterator itfind = m_gameapp_list.find(item->first);
			if (itfind == m_gameapp_list.end())
			{
				m_gameapp_list[item->first] = item->second;
				flag = true;
			}
		}

	}

	item = m_gameapp_list.begin();

	for (;item != m_gameapp_list.end();item)
	{
		if (m_gameappUrl_list.size() ==0)
		{
			m_gameapp_list.clear();
			flag = true;
			break;
		}
		map<string ,ADD_APP_DATA>::const_iterator itfind = m_gameappUrl_list.find(item->first);
		if (itfind == m_gameappUrl_list.end())
		{
			item =m_gameapp_list.erase(itfind);
			flag = true;
		}else{
			item++;
		}
	}

	/// 大图片
	item = m_LargeappUrl_list.begin();
	for (;item != m_LargeappUrl_list.end();item++)
	{
		if (m_Largeapp_list.size() ==0)
		{
			m_Largeapp_list[item->first] = item->second;
			flag = true;
		}else{
			map<string ,ADD_APP_DATA>::const_iterator itfind = m_Largeapp_list.find(item->first);
			if (itfind == m_Largeapp_list.end())
			{
				m_Largeapp_list[item->first] = item->second;
				flag = true;
			}
		}

	}

	item = m_Largeapp_list.begin();

	for (;item != m_Largeapp_list.end();item)
	{
		if (m_LargeappUrl_list.size() ==0)
		{
			m_Largeapp_list.clear();
			flag = true;
			break;
		}
		map<string ,ADD_APP_DATA>::const_iterator itfind = m_LargeappUrl_list.find(item->first);
		if (itfind == m_LargeappUrl_list.end())
		{
			item =m_Largeapp_list.erase(itfind);
			flag = true;
		}else{
			item++;
		}
	}

	if (flag)
	{
		string strFile=m_apppath;
		strFile += "\\" +m_configName;

		WriteCoinfig(strFile);
	}
}
void CDisplayApp::WriteCoinfig(const string& strFilePath)
{
	Json::Value root;
	Json::Value arraycomm;
	Json::Value arraygame;
	Json::Value arraylager;
	Json::Value item;
	map<string ,ADD_APP_DATA>::const_iterator it = m_commapp_list.begin();
	for (;it!= m_commapp_list.end();it++)
	{
		GetObj(it->second,item);
		root["commapp"].append(item);
	}

	it = m_gameapp_list.begin();
	for (;it!= m_gameapp_list.end();it++)
	{
		GetObj(it->second,item);
		root["lagerapp"].append(item);
	}

	it = m_Largeapp_list.begin();
	for (;it!= m_Largeapp_list.end();it++)
	{
		GetObj(it->second,item);
		root["lagerapp"].append(item);
	}

	ofstream outfile(strFilePath);  
	if (!outfile.is_open())
	{
		return;
	}
	string strfile = root.toStyledString();
	outfile<<strfile;
	outfile.close();
	//CStdioFile  File;
	//if (File.Open((LPCTSTR)(LPSTR)strFilePath.c_str(),CFile::modeWrite | CFile::modeCreate))
	//{
	//	string strfile = root.toStyledString();
	//	File.WriteString(strfile.c_str());
	//	File.Close();
	//}
	//

}
void CDisplayApp::GetObj(ADD_APP_DATA data,Json::Value &root)
{
	root["appname"]=data.appname;
	root["isLagerPic"]=data.isLagerPic;
	root["isInstall"]=data.isInstall;
	root["type"]=data.type;
	root["version"]=data.version;
}

void CDisplayApp::GetAppUrlValue(int type,string key,ADD_APP_DATA &retdata)
{
	if (type == 0)
	{
		if (m_LargeappUrl_list.size() ==0 )
		{
			return ;
		}
		map<string ,ADD_APP_DATA>::const_iterator itfind = m_LargeappUrl_list.find(key);
		if (itfind != m_LargeappUrl_list.end())
		{
			ADD_APP_DATA itdata = itfind->second;
			retdata =itdata;
		}
	}else if (type == 1)
	{
		if (m_commappUrl_list.size() ==0 )
		{
			return ;
		}
		map<string ,ADD_APP_DATA>::const_iterator itfind = m_commappUrl_list.find(key);
		if (itfind != m_commappUrl_list.end())
		{
			ADD_APP_DATA itdata = itfind->second;
			retdata =itdata;
		}
	}else if (type == 2)
	{
		if (m_gameappUrl_list.size() ==0 )
		{
			return ;
		}
		map<string ,ADD_APP_DATA>::const_iterator itfind = m_gameappUrl_list.find(key);
		if (itfind != m_gameappUrl_list.end())
		{
			ADD_APP_DATA itdata = itfind->second;
			retdata =itdata;
		}
	}

}
void CDisplayApp::MoidfyListAndSaveToFile(int type,string key,ADD_APP_DATA ModifyData)
{
	if (type == 0)
	{

		map<string ,ADD_APP_DATA>::const_iterator itfind = m_Largeapp_list.find(key);
		if (itfind != m_Largeapp_list.end())
		{
			m_Largeapp_list[itfind->first] = ModifyData;
		}
	}else if (type == 1)
	{
		map<string ,ADD_APP_DATA>::const_iterator itfind = m_commapp_list.find(key);
		if (itfind != m_commapp_list.end())
		{
			m_commapp_list[itfind->first] = ModifyData;
		}
	}else if (type == 2)
	{
		map<string ,ADD_APP_DATA>::const_iterator itfind = m_gameapp_list.find(key);
		if (itfind != m_gameapp_list.end())
		{
			m_gameapp_list[itfind->first] = ModifyData;
		}
	}

	string strFile=m_apppath;
	strFile +="\\"+m_configName;
	WriteCoinfig(strFile);
}
LRESULT CDisplayApp::onBnCLick( WPARAM wParam, LPARAM lParam )
{
	CButtonCtrl *button = (CButtonCtrl*)lParam;
	ADD_APP_DATA itemdata =(ADD_APP_DATA)button->m_pData;
	if (itemdata.isInstall)
	{
		ADD_APP_DATA BtData;
		GetAppUrlValue(itemdata.type,itemdata.appname,BtData);
		if (BtData.appname != "" && BtData.version != itemdata.version)
		{
			if ( IDYES == UiFun::MessageBoxEx(_T("此程序有更新的版本,是否要下载") , UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_YESNO|MFB_TIP ) )
			{
				string dowloufilepath = m_apppath + "\\"+strprintf("%s",itemdata.appname);
				string dowloufileName = dowloufilepath + "\\"+strprintf("%s.zip",itemdata.appname);

				string dowlouUrlPaht = m_urlpath +"/"+strprintf("%s",itemdata.appname);
				string dowlouUrName = dowlouUrlPaht +"/"+strprintf("%s.zip",itemdata.appname);
				if (Download(dowlouUrName.c_str(),dowloufileName.c_str()))
				{
					UnZipFile(dowloufileName,dowloufilepath);
					button->m_pData.version = BtData.version;
					MoidfyListAndSaveToFile(itemdata.type ,itemdata.appname,button->m_pData);
				}
			}
		}else{
			string appfileexe = m_apppath + "\\"+strprintf("%s",itemdata.appname)+strprintf("\\%s",itemdata.appname)+".exe";
			/// 配置文件安装了,但是找不到exe，重新设置配置文件没有安装
			if (_access(appfileexe.c_str(),0) == -1)
			{
				button->m_pData.isInstall = false;
				MoidfyListAndSaveToFile(itemdata.type ,itemdata.appname,button->m_pData);
				return 0;
			}
			PROCESS_INFORMATION		app_pi; 
			STARTUPINFOA si; 
			memset(&si, 0, sizeof(STARTUPINFO));  
			si.cb = sizeof(STARTUPINFO);  
			si.dwFlags = STARTF_USESHOWWINDOW;  
			si.wShowWindow =SW_HIDE;//SW_HIDE; //SW_SHOW;  
			if(!CreateProcessA(NULL,(LPSTR)appfileexe.c_str(),NULL,NULL,FALSE,0,NULL,NULL,&si,&app_pi))   
			{  
				int n = GetLastError();
				AfxMessageBox(_T("CreateProcessA sever error!"));
				LogPrint("INFO", "开启服务端程序失败\n");
				exit(1);  
			}  
			CloseHandle(app_pi.hProcess);
			CloseHandle(app_pi.hThread);
			m_process.push_back(app_pi);
			//HANDLE hProcessHandle; 
			//hProcessHandle = ::OpenProcess( PROCESS_ALL_ACCESS, FALSE, app_pi.dwProcessId );
			//TerminateProcess( hProcessHandle,0);
		}
	}else{  /// 下载可执行程序
		string dowloufilepath = m_apppath + "\\"+strprintf("%s",itemdata.appname);
		string dowloufileName = dowloufilepath + "\\"+strprintf("%s.zip",itemdata.appname);

		string dowlouUrlPaht = m_urlpath +"/"+strprintf("%s",itemdata.appname);
		string dowlouUrName = dowlouUrlPaht +"/"+strprintf("%s.zip",itemdata.appname);
		if (Download(dowlouUrName.c_str(),dowloufileName.c_str()))
		{
			UnZipFile(dowloufileName,dowloufilepath);
			///删除下载的临时文件
			DeleteFile(dowloufileName.c_str());
			button->m_pData.isInstall = true;
			MoidfyListAndSaveToFile(itemdata.type ,itemdata.appname,button->m_pData);
		}
	}
	return 0;

}

bool CDisplayApp::UnZipFile(string unzipfilename,string zipfilepath)
{
	HZIP hz; DWORD writ;

	// EXAMPLE 2 - unzip it with the names suggested in the zip
	hz = OpenZip(unzipfilename.c_str(),0);
	string path = zipfilepath +"\\";
	SetUnzipBaseDir(hz,path.c_str());
	ZIPENTRY ze; GetZipItem(hz,-1,&ze);
	int numitems=ze.index;
	for (int zi=0; zi<numitems; zi++)
	{ GetZipItem(hz,zi,&ze);
	UnzipItem(hz,zi,ze.name);
	}
	CloseZip(hz);
	return true;
}
void CDisplayApp::CloseProcess()
{
	vector<PROCESS_INFORMATION>::iterator it = m_process.begin();
	for (;it!=m_process.end();it++)
	{
		HANDLE hProcessHandle; 
		hProcessHandle = ::OpenProcess( PROCESS_ALL_ACCESS, FALSE, it->dwProcessId );
		if (hProcessHandle != NULL)
		{
			TerminateProcess( hProcessHandle,0);
		}

	}
}

void CDisplayApp::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int CurHeight = 0;
	if (::IsWindow(m_LargeList.m_hWnd))
	{
		m_LargeList.LockWindowUpdate();

		//int with = (largerWith+7)*3;
		int with = (largerWith+7)*3;
		m_LargeList.MoveWindow(20, 30, with, largerHeight);
		CurHeight = 30 + largerHeight;

		CRect rect;
		GetClientRect(&rect);

		int w = cx - 2 - ::GetSystemMetrics(SM_CXVSCROLL);
		int total_cx = 0;
		int nColNum = 5;

		m_LargeList.UnlockWindowUpdate();
	}

	//if (m_commstatic.m_hWnd)
	//{
	//	m_commstatic.LockWindowUpdate();
	//	m_commstatic.MoveWindow(20, CurHeight+20, 70, 20);
	//	CurHeight += 20 + 20;
	//	m_commstatic.UnlockWindowUpdate();
	//}

	if (::IsWindow(m_Commctrl.m_hWnd))
	{
		m_Commctrl.LockWindowUpdate();

		int line = (m_commapp_list.size()%3) ==0?(m_commapp_list.size()/3):(m_commapp_list.size()/3+1);
		int height = (smallHeight+5)*line;
		m_Commctrl.MoveWindow(20, CurHeight+10, (smallWith+7)*3, height);
		CurHeight +=10 + height;

		CRect rect;
		GetClientRect(&rect);

		int w = cx - 2 - ::GetSystemMetrics(SM_CXVSCROLL);
		int total_cx = 0;
		int nColNum = 5;

		m_Commctrl.UnlockWindowUpdate();
	}

	//if (m_gamestatic.m_hWnd)
	//{
	//	m_gamestatic.LockWindowUpdate();
	//	m_gamestatic.MoveWindow(20, CurHeight+20,60, 20);
	//	CurHeight += 20 + 20;
	//	m_gamestatic.UnlockWindowUpdate();
	//}

	if (::IsWindow(m_gameCtrl.m_hWnd))
	{
		m_gameCtrl.LockWindowUpdate();

		int line = (m_gameapp_list.size()%3) ==0?(m_gameapp_list.size()/3):(m_gameapp_list.size()/3+1);
		int height = (smallHeight+5)*line;
		m_gameCtrl.MoveWindow(20, CurHeight+10, (smallWith+7)*3, height);
		CurHeight += 10 + height;

		CRect rect;
		GetClientRect(&rect);

		int w = cx - 2 - ::GetSystemMetrics(SM_CXVSCROLL);
		int total_cx = 0;
		int nColNum = 5;
		m_gameCtrl.UnlockWindowUpdate();
	}
}

void CDisplayApp::InitListCtrl()
{

	// set column width according to window rect
	int sizeL = 0;
	int countL = 0;
	if (m_Largeapp_list.size() <3 && m_Largeapp_list.size() >0)
	{
		if (m_Largeapp_list.size() == 2)
		{
			sizeL = (m_Largeapp_list.size()*282*3)/2;
		}else{
			sizeL = m_Largeapp_list.size()*282*3;
		}
		countL = m_Largeapp_list.size();
	}else if (m_Largeapp_list.size() >3)
	{
		sizeL = 282;
		countL = 3;
	}

	for( int i = 0 ; i < countL ; i++  ) {
		m_LargeList.InsertColumn(i,_T("tim"),LVCFMT_CENTER,sizeL);
	}
	for( int i = 0 ; i < 3 ; i++  ) {
		m_Commctrl.InsertColumn(i,_T("tim"),LVCFMT_CENTER,282);
	}

	for( int i = 0 ; i < 3 ; i++  ) {
		m_gameCtrl.InsertColumn(i,_T("tim"),LVCFMT_CENTER,282);
	}

	//m_apppath = "D:\\cppwork\\dacrs_ui_new\\bin\\应用程序";
	m_LargeList.SetRowHeigt(136);
	m_gameCtrl.SetRowHeigt(68);
	m_Commctrl.SetRowHeigt(68);
	int ncount = 0;
	int  nRow = m_LargeList.InsertItem(0, _T(""));

	map<string ,ADD_APP_DATA>::const_iterator it = m_Largeapp_list.begin();
	for (;it != m_Largeapp_list.end();it++)
	{
		ADD_APP_DATA data = it->second; 
		string filepathe;
		filepathe = m_apppath +"\\"+data.appname+"\\ini.bmp";

		CString FileName;
		FileName.AppendFormat(_T("%s"),filepathe.c_str());
		//image.Load(FileName,CXIMAGE_FORMAT_PNG|CXIMAGE_FORMAT_JPG|CXIMAGE_SUPPORT_BMP|CXIMAGE_SUPPORT_GIF|CXIMAGE_SUPPORT_ICO);
		HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);  
		//HBITMAP hbmp = image.MakeBitmap();

		m_LargeList.createItemButton(nRow,ncount++,this->GetSafeHwnd(),_T(""),&data,hbmp , RGB(255, 255, 255) , hbmp , RGB(255, 255, 255));
	}
	//m_LargeList.release();

	map<string ,ADD_APP_DATA>::const_iterator item = m_commapp_list.begin();
	ncount = 0;
	int nSubIdx =0;
	int CurRow = m_Commctrl.InsertItem(0, _T(""));

	int index = 0;
	for (;item != m_commapp_list.end();item++)
	{
		int row = index/3;
		index++;
		if (row > CurRow)
		{
			CurRow +=1;
			m_Commctrl.InsertItem(CurRow, _T(""));
			ncount = 0;
		}
		ADD_APP_DATA data = item->second; 
		string filepathe;
		filepathe = m_apppath +"\\"+data.appname+"\\ini.bmp";

		CString FileName;
		FileName.AppendFormat(_T("%s"),filepathe.c_str());

		//image.Load(filepathe,CXIMAGE_FORMAT_PNG|CXIMAGE_FORMAT_JPG|CXIMAGE_SUPPORT_BMP|CXIMAGE_SUPPORT_GIF|CXIMAGE_SUPPORT_ICO);
		HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);  
		//HBITMAP hbmp = image.MakeBitmap();

		HBITMAP hbmp2;
		if (data.isInstall)
		{
			FileName.Format(_T("%s\\%s\\Install.bmp"),m_apppath.c_str(),data.appname.c_str());
			//image.Load(filepathe,CXIMAGE_FORMAT_PNG|CXIMAGE_FORMAT_JPG|CXIMAGE_SUPPORT_BMP|CXIMAGE_SUPPORT_GIF|CXIMAGE_SUPPORT_ICO);
			hbmp2 = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
		}else{
			FileName.Format(_T("%s\\%s\\UnInstall.bmp"),m_apppath.c_str(),data.appname.c_str());
			//image.Load(filepathe,CXIMAGE_FORMAT_PNG|CXIMAGE_FORMAT_JPG|CXIMAGE_SUPPORT_BMP|CXIMAGE_SUPPORT_GIF|CXIMAGE_SUPPORT_ICO);
			hbmp2 = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
		}
		//hbmp2 = image.MakeBitmap();
		m_Commctrl.createItemButton(CurRow,ncount++,this->GetSafeHwnd(),_T(""),&data,hbmp2 , RGB(255, 255, 255) , hbmp , RGB(255, 255, 255));
	}

	map<string ,ADD_APP_DATA>::const_iterator item1 = m_gameapp_list.begin();
	ncount = 0;

	CurRow = m_gameCtrl.InsertItem(0, _T(""));
	index = 0;
	for (;item1 != m_gameapp_list.end();item1++)
	{
		int row = index/3;
		index++;
		if (row > CurRow)
		{
			CurRow +=1;
			m_gameCtrl.InsertItem(CurRow, _T(""));
			ncount = 0;
		}

		ADD_APP_DATA data = item1->second; 
		string filepathe;
		filepathe = m_apppath+"\\"+data.appname+"\\ini.bmp";

		CString FileName;
		FileName.AppendFormat(_T("%s"),filepathe.c_str());

		//image.Load(filepathe,CXIMAGE_FORMAT_PNG|CXIMAGE_FORMAT_JPG|CXIMAGE_SUPPORT_BMP|CXIMAGE_SUPPORT_GIF|CXIMAGE_SUPPORT_ICO);
		HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);  
		//HBITMAP hbmp = image.MakeBitmap();

		HBITMAP hbmp2;
		if (data.isInstall)
		{
			FileName.Format(_T("%s\\%s\\Install.bmp"),m_apppath.c_str(),data.appname.c_str());
			//image.Load(filepathe,CXIMAGE_FORMAT_PNG|CXIMAGE_FORMAT_JPG|CXIMAGE_SUPPORT_BMP|CXIMAGE_SUPPORT_GIF|CXIMAGE_SUPPORT_ICO);
			hbmp2 = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
		}else{
			FileName.Format(_T("%s\\%s\\UnInstall.bmp"),m_apppath.c_str(),data.appname.c_str());
			//image.Load(filepathe,CXIMAGE_FORMAT_PNG|CXIMAGE_FORMAT_JPG|CXIMAGE_SUPPORT_BMP|CXIMAGE_SUPPORT_GIF|CXIMAGE_SUPPORT_ICO);
			hbmp2 = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
		}
		m_gameCtrl.createItemButton(CurRow,ncount++,this->GetSafeHwnd(),_T(""),&data,hbmp2 , RGB(255, 255, 255) , hbmp , RGB(255, 255, 255));
	}

}
BOOL CDisplayApp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	InitAppListFromNetWork();

	InitListCtrl();

	if (m_gameapp_list.size() == 0)
	{
		//m_gamestatic.ShowWindow(SW_HIDE);
		m_gameCtrl.ShowWindow(SW_HIDE);
	}

	if (m_commapp_list.size() == 0)
	{
		m_commstatic.ShowWindow(SW_HIDE);
		m_Commctrl.ShowWindow(SW_HIDE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
