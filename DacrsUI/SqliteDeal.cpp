#include "StdAfx.h"
#include "DacrsUI.h"
#include "SqliteDeal.h"
#include "CommonStruct.h"
#include "MyMutex.h"

CSqliteDeal::CSqliteDeal(void)
{
	m_pSqliteRead = NULL;
	m_pSqliteWrite = NULL;
	//m_pzErrMsg  = NULL;
	//m_pResult   = NULL;
	//m_nRow = -1 ;
	//m_nCol = -1 ;
	m_pCs = new CCriticalSection();
	//��ʼ���ٽ�
	InitializeCriticalSection(&(cs_UpDataResult) ) ;
}

CSqliteDeal::~CSqliteDeal(void)
{
	if( NULL != m_pSqliteRead ) {
		//delete m_pSqliteRead;
		//m_pSqliteRead = NULL;
		sqlite3_close(m_pSqliteRead);
		m_pSqliteRead = NULL ;
	}
	if ( NULL != m_pSqliteWrite ) {
       sqlite3_close(m_pSqliteWrite);
	   m_pSqliteWrite = NULL ;
	}
	if( NULL != m_pCs ) {
		delete m_pCs;
		m_pCs = NULL;
	}

}

BOOL CSqliteDeal::OpenSqlite(CString strPath, BOOL bOperateFlag)
{
   if ( NULL != m_pSqliteWrite ) TRUE;
   ///////////////////////////////////////
   CString strDbPath ;
   strDbPath.Format(_T("%s\\db\\data.db") , strPath );
   if(bOperateFlag) {
	   EnterCriticalSection( &cs_UpDataResult) ;
	   int ret = sqlite3_open_v2( UiFun::MbcsToUtf8(strDbPath), &m_pSqliteWrite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX, NULL);
	   if ( 0 != ret) {   //��ָ�������ݿ��ļ�,��������ڽ�����һ��ͬ�������ݿ��ļ�
		   sqlite3_close(m_pSqliteWrite);
		   m_pSqliteWrite = NULL ;
		   return FALSE ;
	   }
	   LeaveCriticalSection (&cs_UpDataResult);
   }   
   else{
	   int ret = sqlite3_open_v2( UiFun::MbcsToUtf8(strDbPath), &m_pSqliteRead, SQLITE_OPEN_READONLY  | SQLITE_OPEN_NOMUTEX, NULL);
	   if ( 0 != ret) {   //��ָ�������ݿ��ļ�,��������ڽ�����һ��ͬ�������ݿ��ļ�
		   sqlite3_close(m_pSqliteWrite);
		   m_pSqliteRead = NULL ;
		   return FALSE ;
	   }
   }
   return TRUE ;
}
/***************************************
strTabName: ����
strP: Ҫ�Ƚϵ��ֶ���
strSource:Ҫ�Ƚϵ��ֶ�ֵ
����������Ŀ
***************************************/
int CSqliteDeal::FindDB(const CString strTabName ,const CString strP, const CString strSource )
{
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char *pzErrMsg;
	CString strSql = _T("SELECT * FROM ") + strTabName + _T(" WHERE ") + strSource + _T(" ='") + strP + _T("'");

//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		 sqlite3_close(m_pSqliteRead);  
		 sqlite3_free(pzErrMsg);  
		 m_pSqliteRead = NULL ;
		 return -1 ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return nRow;
}
int  CSqliteDeal::FindInDB(const CString strTabName , const CString strP, const CString strSource ){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}
	char    **ppResult;		//���
	int      nRow;			//����
	int      nCol;			//����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM ") + strTabName + _T(" WHERE ") + strSource + _T(" = ") + strP;

//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return -1 ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return nRow;
}
int CSqliteDeal::GetTableCount(const CString strTabName){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM ") + strTabName;

//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return -1 ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return nRow;
}
BOOL CSqliteDeal::DeleteData(const CString strTabName,const CString strSourceData , const CString strW){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteWrite ) {
		if ( !OpenSqlite(theApp.str_InsPath, TRUE) ) return -1 ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ
	CString strSql = _T("delete from ") + strTabName + _T(" WHERE ") + strSourceData + _T(" ='") + strW + _T("'");

//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteWrite, strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteWrite);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteWrite = NULL ;
		return -1 ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return nRow ;
}
BOOL  CSqliteDeal::CreateTabe(const CString strTabName,const CString strFiled){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteWrite ) {
		if ( !OpenSqlite(theApp.str_InsPath, TRUE) ) return -1 ;
	}
	char    *pzErrMsg;    //������Ϣ
	CString strSql = _T("create table  ") + strTabName + _T("(") + strFiled + _T(");");

//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_exec( m_pSqliteWrite , strSql.GetBuffer() , NULL , NULL , &pzErrMsg );
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteWrite);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteWrite = NULL ;
		return FALSE ;
	}
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return TRUE ;
}

BOOL  CSqliteDeal::IsExistTabe(const CString strTabName){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}
	CString strSql = _T("select count(type) from sqlite_master where type='table' and name =");
	strSql.AppendFormat("'%s'",strTabName);


	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

//	EnterCriticalSection( &cs_UpDataResult) ;
	bool tableIsExisted = FALSE;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return FALSE ;
	}
	int nIndex = nCol;
	CString strValue ;
	int result = atoi(ppResult[nIndex] ) ;
	tableIsExisted = result;
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return tableIsExisted ;
}
int   CSqliteDeal::FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::P2P_BET_RECORD_t * p2pbetrecord ){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM ") + strTabName + _T(" WHERE ") + strSource + _T(" ='") + strP + _T("'");

//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return -1 ;
	}
	int nIndex = nCol;
	CString strValue ;
	for(int j = 0; j < nCol; j++ ){
		switch (j)
		{
		case 0:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				SYSTEMTIME curTime ;
				sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
				p2pbetrecord->sendtime = UiFun::SystemTimeToTimet(curTime);
			}
			break;
		case 1:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				SYSTEMTIME curTime ;
				sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
				p2pbetrecord->recvtime = atoi(strValue);
			}
			break;
		case 2:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				p2pbetrecord->timeout =atoi(strValue) ;
			}
			break;
		case 3:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(p2pbetrecord->tx_hash,strValue,sizeof(p2pbetrecord->tx_hash));
			}
			break;
		case 4:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(p2pbetrecord->left_addr,strValue,sizeof(p2pbetrecord->left_addr));
			}
			break;
		case 5:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(p2pbetrecord->right_addr,strValue,sizeof(p2pbetrecord->right_addr));
			}
			break;
		case 6:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				p2pbetrecord->amount = atol(strValue);
			}
			break;
		case 7:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(p2pbetrecord->content,strValue,sizeof(p2pbetrecord->content));
			}
			break;
		case 8:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				p2pbetrecord->actor = atoi(strValue);
			}
			break;
		case 9:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				p2pbetrecord->comfirmed = atoi(strValue);
			}
			break;
		case 10:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				p2pbetrecord->height = atoi(strValue);
			}
			break;
		case 11:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				p2pbetrecord->state = atoi(strValue);
			}
			break;
		case 12:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(p2pbetrecord->relate_hash,strValue,sizeof(p2pbetrecord->relate_hash));
			}
			break;
		case 13:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				p2pbetrecord->guessnum = atoi(strValue);
			}
			break;
		}
		++nIndex ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return nRow ;
}
/***************************************
strTabName: ����
strSourceData: Ҫ���������
***************************************/
BOOL CSqliteDeal::InsertData(const CString strTabName ,const CString strSourceData )
{
	LOCK(m_pCs);
	if ( NULL == m_pSqliteWrite ) {
		if ( !OpenSqlite(theApp.str_InsPath, TRUE) ) return FALSE ;
	}

	char    *pzErrMsg;    //������Ϣ
	CString strSql = _T("INSERT INTO ") + strTabName + _T(" VALUES( ") + strSourceData + _T(" )") ;
	TRACE("sql:%s\n", strSql.GetBuffer());
	int nResult = sqlite3_exec(m_pSqliteWrite , strSql.GetBuffer() , NULL , NULL , &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		TRACE("nResult:%d error:%s\n", nResult, pzErrMsg);
		sqlite3_close(m_pSqliteWrite);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteWrite = NULL ;
		return FALSE ;
	}
	return TRUE ;
}
BOOL CSqliteDeal::EmptyTabData(const CString strTabName )
{
	LOCK(m_pCs);
	if ( NULL == m_pSqliteWrite ) {
		if ( !OpenSqlite(theApp.str_InsPath, TRUE)) return FALSE ;
	}
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("DELETE FROM ") + strTabName ;
	int nResult = sqlite3_exec( m_pSqliteWrite , strSql.GetBuffer() , NULL , NULL , &pzErrMsg );
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteWrite);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteWrite = NULL ;
		return FALSE ;
	}
	return TRUE ;
}
BOOL CSqliteDeal::Updatabase(const CString strTabName , const CString strSourceData , const CString strW )
{
	LOCK(m_pCs);
	if ( NULL == m_pSqliteWrite ) {
		if ( !OpenSqlite(theApp.str_InsPath, TRUE)) return FALSE ;
	}
	char    *pzErrMsg;    //������Ϣ
	CString strSql = _T("UPDATE ") + strTabName + _T(" SET ") + strSourceData + _T(" WHERE ") + strW ;
	int nResult = sqlite3_exec( m_pSqliteWrite , strSql.GetBuffer() , NULL , NULL , &pzErrMsg );
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteWrite);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteWrite = NULL ;
		return FALSE ;
	}
	return TRUE ;
}

BOOL CSqliteDeal::GetListaddrData(map<CString,uistruct::LISTADDR_t> *pListInfo)
{
	LOCK(m_pCs);
	if (NULL == pListInfo ) return FALSE ;
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return FALSE ;
	}
	pListInfo->clear() ;

	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ
	CString strSql = _T("SELECT * FROM MYWALLET");
//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return FALSE ;
	}
	int nIndex = nCol;
	CString strValue ;
	uistruct::LISTADDR_t listdata;
	for(int i = 0; i < nRow ; i++) {
		memset(&listdata , 0 , sizeof(uistruct::LISTADDR_t));
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(listdata.address , strValue , sizeof(listdata.address));
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(listdata.RegID , strValue , sizeof(listdata.RegID));
				}
				break;
			case 2:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					listdata.fMoney = atof(strValue);
				}
				break;
			case 3:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					listdata.nColdDig = atoi(strValue) ;
				}
				break;
			case 4:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					listdata.bSign = atoi(strValue) ;
				}
			case 5:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(listdata.Lebel , strValue , sizeof(listdata.Lebel));
				}
				break;
			}
			++nIndex ;
		}
		//pListInfo->push_back(listdata) ;
		CString key;
		key.Format(_T("%s"),listdata.address);
		(*pListInfo)[key] = listdata;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return TRUE ;
}
BOOL CSqliteDeal::GetRevtransactionDatta(uistruct::TRANSRECORDLIST* pListInfo)
{
	LOCK(m_pCs);
	if (NULL == pListInfo ) return FALSE ;
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath, TRUE)) return FALSE ;
	}
	pListInfo->clear() ;

	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM revtransaction");
//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return FALSE ;
	}

	//�����ݿ�
	int nIndex = nCol;
	CString strValue  ;
	uistruct::REVTRANSACTION_t listdata;
	for(int i = 0; i < nRow ; i++) {
		Json::Value root;
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["hash"] = strValue.GetString();
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["txtype"] = strValue.GetString();
				}
				break;
			case 2:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["ver"] = atoi(strValue);
				}
				break;
			case 3:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["addr"] = strValue.GetString();
				}
				break;
			case 4:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["pubkey"] =  strValue.GetString();
				}
				break;
			case 5:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["miner_pubkey"] = strValue.GetString();
				}
				break;
			case 6:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ; 
					root["fees"] = (INT64)(strtod (strValue, NULL)*100000000);
				}
				break;
			case 7:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["height"] = atoi(strValue);
				}
				break;
			case 8:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["desaddr"] = strValue.GetString();
				}
				break;
			case 9:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["money"] = (INT64)(strtod (strValue, NULL)*100000000);
				}
				break;
			case 10:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["Contract"] =  strValue.GetString();
				}
				break;
			case 11:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["confirmedHeight"] = atoi(strValue);
				}
				break;
			case 12:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["confirmedtime"] = atoi(strValue);
				}
				break;
			case 13:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["blockhash"] = strValue.GetString();
				}
			case 14:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["state"] = atoi(strValue);
				}
				break;
			}
			++nIndex ;
		}
		listdata.JsonToStruct(root.toStyledString());
		pListInfo->push_back(listdata) ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return TRUE ;

}
BOOL  CSqliteDeal::GetRecorBetData(uistruct::P2PBETRECORDLIST* pListInfo)
{
	LOCK(m_pCs);
	if (NULL == pListInfo ) return FALSE ;
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return FALSE ;
	}
	pListInfo->clear() ;
	
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM p2p_bet_record");
//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return FALSE ;
	}

	//�����ݿ�
	int nIndex = nCol;
	CString strValue ;
	uistruct::P2P_BET_RECORD_t p2pbetrecord;
	for(int i = 0; i < nRow ; i++) {
		memset(&p2pbetrecord , 0 , sizeof(uistruct::P2P_BET_RECORD_t));
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					SYSTEMTIME curTime ;
					sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
					p2pbetrecord.sendtime = UiFun::SystemTimeToTimet(curTime);
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					SYSTEMTIME curTime ;
					sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
					p2pbetrecord.recvtime = atoi(strValue);
				}
				break;
			case 2:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pbetrecord.timeout =atoi(strValue) ;
				}
				break;
			case 3:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(p2pbetrecord.tx_hash,strValue,sizeof(p2pbetrecord.tx_hash));
				}
				break;
			case 4:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(p2pbetrecord.left_addr,strValue,sizeof(p2pbetrecord.left_addr));
				}
				break;
			case 5:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(p2pbetrecord.right_addr,strValue,sizeof(p2pbetrecord.right_addr));
				}
				break;
			case 6:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pbetrecord.amount = atol(strValue);
				}
				break;
			case 7:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(p2pbetrecord.content,strValue,sizeof(p2pbetrecord.content));
				}
				break;
			case 8:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pbetrecord.actor = atoi(strValue);
				}
				break;
			case 9:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pbetrecord.comfirmed = atoi(strValue);
				}
				break;
			case 10:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pbetrecord.height = atoi(strValue);
				}
				break;
			case 11:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pbetrecord.state = atoi(strValue);
				}
				break;
			case 12:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(p2pbetrecord.relate_hash,strValue,sizeof(p2pbetrecord.relate_hash));
				}
				break;
			case 13:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pbetrecord.guessnum = atoi(strValue);
				}
				break;
			}
			++nIndex ;
		}
		pListInfo->push_back(p2pbetrecord) ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return TRUE ;
}
BOOL  CSqliteDeal::UpdataP2pBetRecord()
{
	LOCK(m_pCs);
	uistruct::P2PBETRECORDLIST pTransaction;
	GetRecorBetData(&pTransaction);
	if (0 == pTransaction.size() ) return FALSE ;

	vector<string> vSignHash;
	std::vector<uistruct::P2P_BET_RECORD_t>::const_iterator const_it;
	for (const_it = pTransaction.begin() ; const_it != pTransaction.end() ; const_it++ ) {
		vSignHash.push_back(const_it->tx_hash);
	}
	if (vSignHash.size() == 0)
	{
		return true;
	}
	Json::Reader reader; 
	Json::Value root;
	root["method"] = "getappkeyvalue";

	Json::Value item;
	for ( size_t i = 0;i< vSignHash.size(); i++ )
	{
		item.append(vSignHash.at(i));
	}
	root["params"].append(theApp.m_betScritptid.GetBuffer());
	root["params"].append(item);

	CString strData ,strShowData ;
	
	strData.Format(_T("%s") , root.toStyledString().c_str() ) ;

	CSoyPayHelp::getInstance()->SendContacrRpc(strData , strShowData);
	if (strShowData.Find("key") <0)
	{
		return false;
	}
	if (!reader.parse(strShowData.GetString(), root)) 
		return FALSE;
	int size = root.size();
	for ( int index =0; index < size; ++index )
	{
		CString txhash = root[index]["key"].asCString();
		CString nValue = root[index]["value"].asCString();
		INT64   nTime  = root[index]["confirmedtime"].asInt64() ;
		int     nConfirmHeight = root[index]["confirmHeight"].asInt() ;
		uistruct::DBBET_DATA DBbet;
		memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue.GetString());
		uistruct::P2P_BET_RECORD_t  betrecord;
		//theApp.cs_SqlData.Lock();
		//int nItem =  theApp.m_SqliteDeal.FindDB(_T("p2p_bet_record") , txhash ,_T("tx_hash"),&betrecord ) ;
		//theApp.cs_SqlData.Unlock();
		int nItem =FindDB(_T("p2p_bet_record") , txhash ,_T("tx_hash"),&betrecord ) ;
		if (vTemp.size() == 0)  /// ����������Ӧ�����ݿ��б�ɾ����,������Ӷ���,˵���Ѿ��Ҷ���
		{
			if (nItem != 0 && (betrecord.state == 1 || betrecord.state == 4 || betrecord.state == 5))
			{
				CString strField,strWhere;
				strField.Format(_T("state = %d ") , 2) ;
				strWhere.Format(_T("tx_hash = '%s'") , txhash );

				//��������
				if ( !Updatabase(_T("p2p_bet_record") , strField , strWhere )) {
					TRACE(_T("p2p_bet_record:��������ʧ��!  Hash: %s") , txhash );
				}
			}
			continue;
		}
		memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

		if (DBbet.betstate == 0x00 && betrecord.state ==4)   //���ڽӶ�
		{
			continue;
		}

		if (DBbet.betstate == 0x01 && betrecord.state ==5)   //���ڽҶ�
		{
			continue;
		}
		int nComfirmed = 0 ;
		if ( 0 != nTime ) {
           nComfirmed = 1 ;
		}

		int state = DBbet.betstate;
		string txaccepthash;
		if ( state == 1)
		{
			std::vector<unsigned char> vTemp;// = CSoyPayHelp::getInstance()->ParseHex(string(DBbet.accepthash,DBbet.accepthash+sizeof(DBbet.accepthash)));
			vTemp.assign(DBbet.accepthash,DBbet.accepthash+sizeof(DBbet.accepthash));
			reverse(vTemp.begin(),vTemp.end());
			txaccepthash = CSoyPayHelp::getInstance()->HexStr(vTemp);
			CString strCommand;
			strCommand.Format(_T("%s %s"),_T("gettxdetail") ,txaccepthash.c_str() );
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	
			Json::Value root1;
			if (!reader.parse(strShowData.GetString(), root1)) 
				continue;
			int npos = strShowData.Find("confirmHeight");
			int confirh = 0;
			if ( npos >= 0 ) { //
				confirh = root1["confirmHeight"].asInt() ;    //���ױ�ȷ�ϵĸ߶�
			}
			strCommand.Format(_T("%s"),_T("getinfo"));
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (!reader.parse(strShowData.GetString(), root1)) 
				continue;
			int curheight =root1["blocks"].asInt();
			if (curheight >=(confirh +DBbet.hight ))
			{
				state = 3;                              ///˵����ʱ��
			}
		}
		//�������ݿ�  update p2p_bet_record set timeout=15 where actor=0; 
		CString strSql , strField  , strWhere;
		vTemp.clear();
		vTemp.assign(DBbet.acceptbetid,DBbet.acceptbetid+sizeof(DBbet.acceptbetid));
		string acceptaddr = CSoyPayHelp::getInstance()->HexStr(vTemp);
		CString strCommand;
		strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,acceptaddr.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		Json::Value rootinfo;
		if (!reader.parse(strShowData.GetString(), rootinfo)) 
			continue;
		CString acceptbase;
		if (strShowData.Find("Address") >= 0)
		{
			acceptbase =  rootinfo["Address"].asCString();
		}
		strField.Format(_T("recvtime = %ld , ") , nTime) ;
		strField.AppendFormat(" right_addr = '%s' ,",acceptbase );
		strField.AppendFormat(_T("comfirmed = %d ,height = %d ,state = %d ,relate_hash = '%s' ,guessnum = %d ") ,nComfirmed ,nConfirmHeight ,state ,txaccepthash.c_str() ,(int)DBbet.acceptebetdata ) ;
		strWhere.Format(_T("tx_hash = '%s'") , txhash );

		//��������
		if ( !Updatabase(_T("p2p_bet_record") , strField , strWhere )) {
			TRACE(_T("p2p_bet_record:��������ʧ��!  Hash: %s") , txhash );
		}
	}
	return true;
}
BOOL   CSqliteDeal::GetRecorP2Pool(uistruct::P2PLIST* pListInfo)
{
	LOCK(m_pCs);
	if (NULL == pListInfo ) return FALSE ;
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return FALSE ;
	}
	pListInfo->clear() ;


	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM p2ppool");
//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return FALSE ;
	}

	//�����ݿ�
	int nIndex = nCol;
	CString strValue ;
	uistruct::LISTP2POOL_T p2pool;
	for(int i = 0; i < nRow ; i++) {
		memset(&p2pool , 0 , sizeof(uistruct::LISTP2POOL_T));
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pool.hash = strValue;
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					p2pool.data = strValue;
				}
				break;
			}
			++nIndex ;
		}
		pListInfo->push_back(p2pool) ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return TRUE ;
}
BOOL  CSqliteDeal::GetRecorDarkData(uistruct::DARKRECORDLIST* pListInfo)
{
	LOCK(m_pCs);
	if (NULL == pListInfo ) return FALSE ;
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return FALSE ;
	}
	pListInfo->clear() ;

	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM dark_record");
//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return FALSE ;
	}

	//�����ݿ�
	int nIndex = nCol;
	CString strValue ;
	uistruct::DARK_RECORD darkrecord;
	for(int i = 0; i < nRow ; i++) {
		memset(&darkrecord , 0 , sizeof(uistruct::DARK_RECORD));
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					SYSTEMTIME curTime ;
					sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
					darkrecord.sendtime = UiFun::SystemTimeToTimet(curTime);
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					SYSTEMTIME curTime ;
					sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
					darkrecord.recvtime = atoi(strValue);
				}
				break;
			case 2:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(darkrecord.tx_hash,strValue,sizeof(darkrecord.tx_hash));
				}
				break;
			case 3:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(darkrecord.left_addr,strValue,sizeof(darkrecord.left_addr));
				}
				break;
			case 4:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(darkrecord.right_addr,strValue,sizeof(darkrecord.right_addr));
				}
				break;
			case 5:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					darkrecord.amount = atol(strValue);
				}
				break;
			case 6:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					darkrecord.actor = atoi(strValue);
				}
				break;
			case 7:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					darkrecord.comfirmed = atoi(strValue);
				}
				break;
			case 8:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					darkrecord.state = atoi(strValue);
				}
				break;
			case 9:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(darkrecord.relate_hash,strValue,sizeof(darkrecord.relate_hash));
				}
				break;
			}
			++nIndex ;
		}
		pListInfo->push_back(darkrecord) ;
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return TRUE ;
}

BOOL CSqliteDeal::UpdataDarkRecord(){
	LOCK(m_pCs);
	uistruct::DARKRECORDLIST pTransaction;
	GetRecorDarkData(&pTransaction);
	if (0 == pTransaction.size() ) return FALSE ;

	vector<string> vSignHash;
	std::vector<uistruct::DARK_RECORD>::const_iterator const_it;
	for (const_it = pTransaction.begin() ; const_it != pTransaction.end() ; const_it++ ) {
		vSignHash.push_back(const_it->tx_hash);
	}
	if (vSignHash.size() == 0)
	{
		return true;
	}
	Json::Reader reader; 
	Json::Value root;
	root["method"] = "getappkeyvalue";

	Json::Value item;
	for ( size_t i = 0;i< vSignHash.size(); i++ )
	{
		item.append(vSignHash.at(i));
	}
	root["params"].append(theApp.m_darkScritptid.GetBuffer());
	root["params"].append(item);

	CString strData ,strShowData ;

	strData.Format(_T("%s") , root.toStyledString().c_str() ) ;

	CSoyPayHelp::getInstance()->SendContacrRpc(strData , strShowData);
	if (strShowData.Find("key") <0)
	{
		return false;
	}
	if (!reader.parse(strShowData.GetString(), root)) 
		return FALSE;
	int size = root.size();
	for ( int index =0; index < size; ++index )
	{
		CString txhash = root[index]["key"].asCString();
		CString nValue = root[index]["value"].asCString();
		INT64   nTime  = root[index]["confirmedtime"].asInt64() ;

		uistruct::DARK_DATA_DB DBbet;
		memset(&DBbet , 0 , sizeof(uistruct::DARK_DATA_DB));
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue.GetString());
		uistruct::DARK_RECORD  betrecord;
		//theApp.cs_SqlData.Lock();
		//int nItem =  theApp.m_SqliteDeal.FindDB(_T("dark_record") , txhash ,_T("tx_hash"),&betrecord ) ;
		//theApp.cs_SqlData.Unlock();

		int nItem =  FindDB(_T("dark_record") , txhash ,_T("tx_hash"),&betrecord ) ;
		if (vTemp.size() == 0)  /// ����������Ӧ�����ݿ��б�ɾ����,ȷ���ջ���
		{
			if (nItem != 0 && (betrecord.state == 1 || betrecord.state == 5))
			{
				CString strField,strWhere;
				strField.Format(_T("state = %d ") , 2) ;
				strWhere.Format(_T("tx_hash = '%s'") , txhash );

				//��������
				if ( !Updatabase(_T("dark_record") , strField , strWhere )) {
					TRACE(_T("p2p_bet_record:��������ʧ��!  Hash: %s") , txhash );
				}
			}else if (nItem != 0 && betrecord.state == 6)
			{
				CString strField,strWhere;
				strField.Format(_T("state = %d ") , 3) ;
				strWhere.Format(_T("tx_hash = '%s'") , txhash );

				//��������
				if ( !Updatabase(_T("dark_record") , strField , strWhere )) {
					TRACE(_T("p2p_bet_record:��������ʧ��!  Hash: %s") , txhash );
				}
			}
			continue;
		}
		memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

		if (DBbet.flag && betrecord.state == 5)   //����ȷ�϶�����
		{
			continue;
		}
		int nComfirmed = 0 ;
		if ( 0 != nTime ) {
			nComfirmed = 1 ;
		}

		vTemp.clear();
		vTemp.assign(DBbet.seller,DBbet.seller+sizeof(DBbet.seller));
		string acceptaddr = CSoyPayHelp::getInstance()->HexStr(vTemp);
		CString strCommand;
		strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,acceptaddr.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		Json::Value rootinfo;
		if (!reader.parse(strShowData.GetString(), rootinfo)) 
			continue;
		CString selleraddr;
		if (strShowData.Find("Address") >= 0)
		{
			selleraddr =  rootinfo["Address"].asCString();
		}
		vTemp.clear();
		vTemp.assign(DBbet.buyer,DBbet.buyer+sizeof(DBbet.buyer));
		acceptaddr = CSoyPayHelp::getInstance()->HexStr(vTemp);
	
		strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,acceptaddr.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	
		if (!reader.parse(strShowData.GetString(), rootinfo)) 
			continue;
		CString buyeraddr;
		if (strShowData.Find("Address") >= 0)
		{
			buyeraddr =  rootinfo["Address"].asCString();
		}

		CString strField,strWhere;
		strField.Format(_T("recvtime = %ld ") , nTime) ;
		strField.AppendFormat(",comfirmed = %d, state = %d ",nComfirmed,(int)DBbet.flag) ;
		strField.AppendFormat(" ,left_addr ='%s',right_addr = '%s'",buyeraddr,selleraddr);
		strWhere.Format(_T("tx_hash = '%s'") , txhash );

		//��������
		if ( !Updatabase(_T("dark_record") , strField , strWhere )) {
			TRACE(_T("p2p_bet_record:��������ʧ��!  Hash: %s") , txhash );
		}
	}
	return true;
}
int   CSqliteDeal::FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::DARK_RECORD *darkrecord ){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}

	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM ") + strTabName + _T(" WHERE ") + strSource + _T(" ='") + strP + _T("'");

//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return -1 ;
	}
	int nIndex = nCol;
	CString strValue ;
	for(int j = 0; j < nCol; j++ ){
		switch (j)
		{
		case 0:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				SYSTEMTIME curTime ;
				sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
				darkrecord->sendtime = UiFun::SystemTimeToTimet(curTime);
			}
			break;
		case 1:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				SYSTEMTIME curTime ;
				sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
				darkrecord->recvtime = atoi(strValue);
			}
			break;
		case 2:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(darkrecord->tx_hash,strValue,sizeof(darkrecord->tx_hash));
			}
			break;
		case 3:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(darkrecord->left_addr,strValue,sizeof(darkrecord->left_addr));
			}
			break;
		case 4:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(darkrecord->right_addr,strValue,sizeof(darkrecord->right_addr));
			}
			break;
		case 5:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				darkrecord->amount = atol(strValue);
			}
			break;
		case 6:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				darkrecord->actor = atoi(strValue);
			}
			break;
		case 7:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				darkrecord->comfirmed = atoi(strValue);
			}
			break;
		case 8:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				darkrecord->state = atoi(strValue);
			}
			break;
		case 9:
			{
				strValue.Format(_T("%s") , ppResult[nIndex] ) ;
				memcpy(darkrecord->relate_hash,strValue,sizeof(darkrecord->relate_hash));
			}
			break;
		}
		++nIndex ;
	}
	sqlite3_free_table(ppResult);
	//LeaveCriticalSection (&cs_UpDataResult) ;
	return nRow ;
}
BOOL   CSqliteDeal::isExistTx(CString tablename,CString filed ,CString txhash){
	LOCK(m_pCs);
	CString strCommand,strShowData;
	strCommand.Format(_T("%s %s"),_T("gettxdetail") ,txhash );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	BOOL flag = true;
	if (strShowData == _T("")){
		flag = false;
	}
	if(strShowData.Find("hash") < 0){		
		flag = false;
	}
	if (!flag)
	{
		if (DeleteData(tablename,filed,txhash))
		{
			return flag;
		}
		
	}
	return flag;
}
BOOL CSqliteDeal::isinBlock(){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return FALSE ;
	}

	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ


	CString strSql = _T("SELECT * FROM tip_block");
//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "isinBlock error%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		
		return FALSE ;
	}
	
	//�����ݿ�
	bool flag = true;
	int nIndex = nCol;

	if (nCol == 0)
	{
		flag = false;
	}
	CString strCommand , strValue ,strShowData ;
	for(int i = 0; i < nRow ; i++) {
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;

					if(strValue == _T("")){
						strCommand.Format(_T("%s %d"),_T("getblock") ,-1 );
					}else{
						strCommand.Format(_T("%s %s"),_T("getblock") ,strValue );
					}					
					CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
					if (strShowData == _T("")){
						flag = false;
						break;
					}
					if(strShowData.Find("hash") < 0){		
						flag = false;
						break;
					}
					Json::Reader reader;  
					Json::Value root; 
					if (!reader.parse(strShowData.GetString(), root)) 
					{		
						flag = false;
						break;
					}

					int height = root["height"].asInt();
					strCommand.Format(_T("%s %d"),_T("getblockhash") ,height );
					CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
					if (strShowData == _T("")){
						flag = false;
						break;
					}
					if(strShowData.Find("hash") < 0){		
						flag = false;
						break;
					}
					if (!reader.parse(strShowData.GetString(), root)) 
					{		
						flag = false;
						break;
					}
					CString newblock = root["hash"].asCString();
					if (!strcmp(strValue,newblock))
					{
						flag = false;
						break;
					}
				}
				break;
			}
			++nIndex ;
		}
	}
	sqlite3_free_table(ppResult);
//	LeaveCriticalSection (&cs_UpDataResult) ;
	return flag ;
}
void  CSqliteDeal::UpdataAllTableData(BOOL flag){
	LOCK(m_pCs);
	///// ���ø��������е����ݿ��
	if (flag)
	{
		return ;
	}
	//// ���½������ݿ��
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return  ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM revtransaction");
	//int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&pResult,&m_nRow,&m_nCol,&m_pzErrMsg);
	//if ( nResult != SQLITE_OK ){
	//	sqlite3_close(m_pSqliteRead);  
	//	sqlite3_free(m_pzErrMsg);  
	//	m_pSqliteRead = NULL ;
	//	return  ;
	//}
	//�����ݿ�
	//int nIndex = m_nCol;
	//int nTempRow = m_nRow;
	//int nTempCol = m_nCol;
	//CString strCommand , strValue ,strShowData ;
	//for(int i = 0; i < nTempRow ; i++) {
	//		strValue.Format(_T("%s") , pResult[nIndex] ) ;
	//		isExistTx(_T("revtransaction"),_T("hash"),strValue);
	//		nIndex +=  nTempCol;
	//}
	//sqlite3_free_table(pResult);


	strSql = _T("SELECT * FROM dark_record");
	 int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "UpdataAllTableData error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return  ;
	}
	//�����ݿ�
	int nIndex = nCol;
	int nTempRow = nRow;
	int nTempCol = nCol;
	CString strCommand , strValue ,strShowData ;
	for(int i = 0; i < nTempRow ; i++) {
			strValue.Format(_T("%s") , ppResult[nIndex+2] ) ;
			isExistTx(_T("dark_record"),_T("tx_hash"),strValue);
			nIndex +=nTempCol ;
	}
	sqlite3_free_table(ppResult);

	strSql = _T("SELECT * FROM p2p_bet_record");
	 nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "UpdataAllTableData select p2p_bet_record error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return  ;
	}
	//�����ݿ�
	 nIndex = nCol;
	 nTempRow = nRow;
	 nTempCol = nCol;
	for(int i = 0; i < nTempRow ; i++) {
		strValue.Format(_T("%s") , ppResult[nIndex+3] ) ;
		isExistTx(_T("p2p_bet_record"),_T("tx_hash"),strValue);
		nIndex +=nTempCol ;
	}
	sqlite3_free_table(ppResult);

	//EmptyTabData(_T("MYWALLET"));
	//theApp.UpdataAddressData();
}
void  CSqliteDeal::UpdataAllTable(){
	LOCK(m_pCs);
	CString filed =_T("");
	if (!IsExistTabe(_T("MYWALLET")))
	{
		filed = _T("address TEXT,regid TEXT,money double,coldig INT,sign INT,Lebel TEXT");
		CreateTabe(_T("MYWALLET"),filed);
	}
	if (!IsExistTabe(_T("p2p_bet_record")))
	{
		filed = _T("sendtime TEXT,recvtime  TEXT,timeout integer,tx_hash TEXT,left_addr TEXT,right_addr TEXT,\
				   amount integer,content TEXT,actor integer,comfirmed integer,height integer,state integer,\
				   relate_hash TEXT,guessnum integer");
		CreateTabe(_T("p2p_bet_record"),filed);
	}
	if (!IsExistTabe(_T("revtransaction")))
	{
		filed = _T(" hash text, txtype text,ver INT, addr text, pubkey text,miner_pubkey text,fees double,height INT, desaddr text,money double, Contract text,confirmHeight INT,confirmedtime INT,blockhash text,state INT");
		CreateTabe(_T("revtransaction"),filed);
	}
	if (!IsExistTabe(_T("dark_record")))
	{
		filed = _T("sendtime text,recvtime text,tx_hash text,left_addr text,right_addr text,amount integer,actor integer,comfirmed integer ,state integer,relate_hash text");
			CreateTabe(_T("dark_record"),filed);
	}
	if (!IsExistTabe(_T("tip_block")))
	{
		filed = _T("blockhash text");
		CreateTabe(_T("tip_block"),filed);
	}
	if (!IsExistTabe(_T("p2ppool")))
	{
		filed = _T("hash text,data text");
		CreateTabe(_T("p2ppool"),filed);
	}
	if (!IsExistTabe(_T("addrbook")))
	{
		filed = _T("Lebel TEXT,address TEXT");
		CreateTabe(_T("addrbook"),filed);
	}
}

int	CSqliteDeal::FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::TRANSRECORDLIST* pListInfo){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ


	CString strSql = _T("SELECT * FROM ") + strTabName + _T(" WHERE ") + strSource + _T(" =") + strP;

	pListInfo->clear() ;
//	EnterCriticalSection( &cs_UpDataResult) ;
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "FindDB error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return FALSE ;
	}

	//�����ݿ�
	int nIndex = nCol;
	CString strValue  ;
	uistruct::REVTRANSACTION_t listdata;
	for(int i = 0; i < nRow ; i++) {
		Json::Value root;
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["hash"] = strValue.GetString();
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["txtype"] = strValue.GetString();
				}
				break;
			case 2:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["ver"] = atoi(strValue);
				}
				break;
			case 3:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["addr"] = strValue.GetString();
				}
				break;
			case 4:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["pubkey"] =  strValue.GetString();
				}
				break;
			case 5:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["miner_pubkey"] = strValue.GetString();
				}
				break;
			case 6:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ; 
					root["fees"] = (INT64)(strtod (strValue, NULL)*100000000);
				}
				break;
			case 7:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["height"] = atoi(strValue);
				}
				break;
			case 8:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["desaddr"] = strValue.GetString();
				}
				break;
			case 9:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["money"] = (INT64)(strtod (strValue, NULL)*100000000);
				}
				break;
			case 10:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["Contract"] =  strValue.GetString();
				}
				break;
			case 11:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["confirmHeight"] = atoi(strValue);
				}
				break;
			case 12:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["confirmedtime"] = atoi(strValue);
				}
				break;
			case 13:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["blockhash"] = strValue.GetString();
				}
				break;
			case 14:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					root["state"] = atoi(strValue);
				}
				break;
			}
			++nIndex ;
		}
		listdata.JsonToStruct(root.toStyledString());
		pListInfo->push_back(listdata) ;
	}
	sqlite3_free_table(ppResult);
	//LeaveCriticalSection (&cs_UpDataResult) ;
	return TRUE ;

}
int	 CSqliteDeal::FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::LISTADDR_t *listaddr ){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ


	CString strSql = _T("SELECT * FROM ") + strTabName + _T(" WHERE ") + strSource + _T(" ='") + strP+ _T("'") ;

	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "FindDB2 error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return 0 ;
	}

	int nIndex = nCol;
	CString strValue ;
	for(int i = 0; i < nRow ; i++) {
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(listaddr->address , strValue , sizeof(listaddr->address));
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(listaddr->RegID , strValue , sizeof(listaddr->RegID));
				}
				break;
			case 2:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					listaddr->fMoney = atof(strValue);
				}
				break;
			case 3:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					listaddr->nColdDig = atoi(strValue) ;
				}
				break;
			case 4:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					listaddr->bSign = atoi(strValue) ;
				}
			case 5:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					memcpy(listaddr->Lebel , strValue , sizeof(listaddr->Lebel));
				}
				break;
			}
			++nIndex ;
		}
	}
	sqlite3_free_table(ppResult);
	return nRow;
}
string CSqliteDeal::GetColSum(const CString strTabName , const CString strP, const CString strSource){
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return "" ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT sum(money) FROM ") + strTabName + _T(" WHERE ") + strSource + _T(" =") + strP;

	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(), &ppResult, &nRow, &nCol, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "GetColSum error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return "" ;
	}
	int nIndex = nCol;
	CString strValue  ;
	strValue.Format(_T("%s"),ppResult[nIndex]);
	sqlite3_free_table(ppResult);
	return strValue.GetString() ;
}
string  CSqliteDeal::GetColSum(const CString strTabName,const CString filed)
{
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return "" ;
	}
	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT sum(")+filed+_T(") FROM ") + strTabName;

	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "GetColSum2 error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return "" ;
	}
	int nIndex = nCol;
	CString strValue  ;
	strValue.Format(_T("%s"),ppResult[nIndex]);
	sqlite3_free_table(ppResult);
	return strValue.GetString() ;

}

BOOL  CSqliteDeal::GetaddrBookData(map<CString,uistruct::ADDRBOOK_t>* pListInfo)
{
	LOCK(m_pCs);
	if (NULL == pListInfo ) return FALSE ;
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return FALSE ;
	}
	pListInfo->clear() ;

	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM addrbook");
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "GetaddrBookData error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return FALSE ;
	}
	int nIndex = nCol;
	CString strValue ;
	uistruct::ADDRBOOK_t listdata;
	for(int i = 0; i < nRow ; i++) {
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] );
					listdata.lebel = strValue;
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					listdata.address = strValue;
				}
				break;
			}
			++nIndex ;
		}
		CString key;
		key.Format(_T("%s"),listdata.address);
		(*pListInfo)[key] = listdata;
	}
	sqlite3_free_table(ppResult);
	return TRUE ;
}
int	 CSqliteDeal::FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::ADDRBOOKLIST *listaddr )
{
	LOCK(m_pCs);
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return -1 ;
	}

	char    **  ppResult;    //���
	int         nRow;       //����
	int         nCol;       //����
	char    *pzErrMsg;    //������Ϣ

	CString strSql = _T("SELECT * FROM ") + strTabName + _T(" WHERE ") + strSource + _T(" ='") + strP+ _T("'") ;

	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "FindDB3 error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return 0 ;
	}

	int nIndex = nCol;
	CString strValue ;
	uistruct::ADDRBOOK_t listdata;
	for(int i = 0; i < nRow ; i++) {
		for(int j = 0; j < nCol; j++ ){
			switch (j)
			{
			case 0:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] );
					listdata.lebel = strValue;
				}
				break;
			case 1:
				{
					strValue.Format(_T("%s") , ppResult[nIndex] ) ;
					listdata.address = strValue;
				}
				break;
			}
			++nIndex ;
		}
		listaddr->push_back(listdata);
	}
	sqlite3_free_table(ppResult);
	return nRow;
}