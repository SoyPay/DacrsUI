#include "StdAfx.h"
#include "DacrsUI.h"
#include "SqliteDeal.h"
#include "CommonStruct.h"
#include "MyMutex.h"

CSqliteDeal::CSqliteDeal(void)
{
	m_pSqliteWrite = NULL;
	m_pCs = new CCriticalSection();

}

CSqliteDeal::~CSqliteDeal(void)
{
	if ( NULL != m_pSqliteWrite ) {
       sqlite3_close(m_pSqliteWrite);
	   m_pSqliteWrite = NULL ;
	}
	if( NULL != m_pCs ) {
		delete m_pCs;
		m_pCs = NULL;
	}

}

//初始化检测表是否存在，不存在则创建表
BOOL CSqliteDeal::InitializationDB(){
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	CString strTableName(_T("sqlite_master"));
	CString strCondition(_T("type='table' and name = 't_wallet_address'"));
	if(!GetTableCountItem(strTableName, strCondition))
	{
		CString createSQL(_T("CREATE TABLE t_wallet_address(address TEXT, reg_id TEXT, money DOUBLE, cold_dig INT, sign INT, label TEXT)"));
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_wallet_address failed\n");
			return FALSE;
		}
	}
	strCondition = _T("type='table' and name= 't_chain_tip'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		CString createSQL(_T("CREATE TABLE t_chain_tip(block_hash TEXT);"));
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_chain_tip failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_address_book'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		CString createSQL(_T("CREATE TABLE t_address_book(Label TEXT,address TEXT)"));
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_address_book failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_dark_record'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		CString createSQL(_T("CREATE TABLE t_dark_record(send_time TEXT,recv_time TEXT,tx_hash TEXT,left_addr TEXT,right_addr TEXT,amount INT,actor INT,confirmed INT ,state INT,relate_hash TEXT)"));
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_dark_record failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_p2p_quiz'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		CString createSQL(_T("CREATE TABLE t_p2p_quiz(send_time TEXT,recv_time TEXT,time_out INT,tx_hash TEXT, left_addr TEXT, right_addr TEXT, amount INT, content TEXT, actor INT, comfirmed INT, height INT, state INT, relate_hash TEXT, guess_num INT)"));
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_quiz_pool'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		CString createSQL(_T("CREATE TABLE t_quiz_pool(hash TEXT, data TEXT)"));
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_transaction'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		CString createSQL(_T("CREATE TABLE t_transaction(hash TEXT, tx_type TEXT,version INT, src_addr TEXT, pub_key TEXT, miner_pub_key TEXT, fees DOUBLE, height INT, des_addr TEXT, money DOUBLE, contract TEXT, confirm_height INT, confirmed_time INT, block_hash TEXT, state INT)"));
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}
	return TRUE ;
}

//获取数据库连接
sqlite3** CSqliteDeal::GetDBConnect() 
{
	LOCK(m_pCs);
	CString strPath;
	strPath.Format(_T("%s\\db\\data.db") , theApp.str_InsPath); 
	if(NULL == m_pSqliteWrite)
	{
		int ret = sqlite3_open_v2( UiFun::MbcsToUtf8(strPath), &m_pSqliteWrite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX, NULL);
		if ( 0 != ret) {   //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
			sqlite3_close(m_pSqliteWrite);
			m_pSqliteWrite = NULL ;
			LogPrint("INFO", "Open DataBase %s failed!\n", strPath.GetString());
			return FALSE ;
		}
		LogPrint("INFO", "Open DataBase %s succeed!\n", strPath.GetString());
	}
	
	return &m_pSqliteWrite;
}
//开启事务
BOOL CSqliteDeal::BeginDBTransaction()
{
	sqlite3** ppDb = GetDBConnect();
	LOCK(m_pCs);
	char *pzErrorMsg;
	int nResult = sqlite3_exec(*ppDb, "begin;", 0,0, &pzErrorMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "nResult:%d error:%s\n", nResult, pzErrorMsg);
		sqlite3_close(*ppDb);  
		sqlite3_free(pzErrorMsg);  
		*ppDb = NULL ;
		return FALSE ;
	}
	return TRUE;
}

//提交事务
BOOL CSqliteDeal::CommitDbTransaction()
{
	sqlite3** ppDb = GetDBConnect();
	LOCK(m_pCs);
	char    *pzErrMsg;    //错误信息
	int nResult = sqlite3_exec(*ppDb, "commit;", 0,0, &pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "nResult:%d error:%s\n", nResult, pzErrMsg);
		sqlite3_close(*ppDb);  
		sqlite3_free(pzErrMsg);  
		*ppDb = NULL ;
		return FALSE ;
	}
	LogPrint("INFO", "sync tx: begin database transaction\n")
	
	return TRUE;
}
//执行SQL语句
BOOL CSqliteDeal::ExcuteSQL(sqlite3** ppDb, CallBackFunc pFunc, CString strSQL, void *pPara)
{
	LOCK(m_pCs);
	char *pzErrMsg = NULL;
	int nResult = sqlite3_exec(*ppDb, strSQL.GetString(), pFunc, pPara, &pzErrMsg);
	TRACE("%s\n", strSQL);
	if ( nResult != SQLITE_OK ){
		if(pzErrMsg != NULL) {
			LogPrint("INFO", "call ExcuteSQL retCode:%d, error:%s,SQL:%s\n", nResult, pzErrMsg, strSQL);
		}else {
			LogPrint("INFO", "call ExcuteSQL error retCode:%d, SQL:%s\n", nResult, strSQL);
		}
		
		sqlite3_close(*ppDb);
		sqlite3_free(pzErrMsg);
		*ppDb = NULL ;
		CheckFailedCode(nResult);
		return FALSE ;
	}
	return TRUE;
}
//获取p2p quiz record
int CallGetP2PQuizRecordItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::P2P_QUIZ_RECORD_t * p2pQuizRecord =  (uistruct::P2P_QUIZ_RECORD_t *)para;
	if(NULL == column_value[0])
		return -1;
	if(n_column != 14)
		return -1;

	CString strValue ;
	strValue.Format(_T("%s") , column_value[0] ) ;
	SYSTEMTIME curTime ;
	sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
	p2pQuizRecord->send_time = UiFun::SystemTimeToTimet(curTime);
			
	strValue.Format(_T("%s") , column_value[1] ) ;
	sscanf(strValue,"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
	p2pQuizRecord->recv_time = atoi(strValue);
			
	strValue.Format(_T("%s") , column_value[2] ) ;
	p2pQuizRecord->time_out =atoi(strValue) ;
	
	strValue.Format(_T("%s") , column_value[3]) ;
	memcpy(p2pQuizRecord->tx_hash,strValue,sizeof(p2pQuizRecord->tx_hash));
			
	strValue.Format(_T("%s") , column_value[4] ) ;
	memcpy(p2pQuizRecord->left_addr,strValue,sizeof(p2pQuizRecord->left_addr));
	
	strValue.Format(_T("%s") , column_value[5] ) ;
	memcpy(p2pQuizRecord->right_addr,strValue,sizeof(p2pQuizRecord->right_addr));
	
	strValue.Format(_T("%s") , column_value[6] ) ;
	p2pQuizRecord->amount = atol(strValue);
		
	strValue.Format(_T("%s") , column_value[7] ) ;
	memcpy(p2pQuizRecord->content,strValue,sizeof(p2pQuizRecord->content));
			
	strValue.Format(_T("%s") , column_value[8] ) ;
	p2pQuizRecord->actor = atoi(strValue);
		
	strValue.Format(_T("%s") , column_value[9] ) ;
	p2pQuizRecord->confirmed = atoi(strValue);
		
	strValue.Format(_T("%s") , column_value[10] ) ;
	p2pQuizRecord->height = atoi(strValue);
		
	strValue.Format(_T("%s") , column_value[11] ) ;
	p2pQuizRecord->state = atoi(strValue);
	
	strValue.Format(_T("%s") , column_value[12] ) ;
	memcpy(p2pQuizRecord->relate_hash, strValue, sizeof(p2pQuizRecord->relate_hash));
			
	strValue.Format(_T("%s") , column_value[13] ) ;
	p2pQuizRecord->guess_num = atoi(strValue);
		
	return 0;
}
//获取所有p2p quiz record列表
int CallGetP2PQuizRecordList(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::P2PBETRECORDLIST* pP2PBetRecordList = (uistruct::P2PBETRECORDLIST*)para;
	uistruct::P2P_QUIZ_RECORD_t p2pQuizReord;
	if(CallGetP2PQuizRecordItem(&p2pQuizReord,  n_column,  column_value,  column_name)<0)
		return -1; 
	pP2PBetRecordList->push_back(p2pQuizReord);
	return 0;
}
//获取某一个钱包地址
int CallGetWalletAddressItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::LISTADDR_t *pAddr = (uistruct::LISTADDR_t *)para;
	if(NULL == column_value[0])
		return -1;
	if(n_column != 6)
		return -1;
	CString strValue(_T(""));

	strValue.Format(_T("%s") , column_value[0]) ;
	memcpy(pAddr->address , strValue , sizeof(pAddr->address));

	strValue.Format(_T("%s") , column_value[1]) ;
	memcpy(pAddr->RegID , strValue , sizeof(pAddr->RegID));

	strValue.Format(_T("%s") , column_value[2]) ;
	pAddr->fMoney = atof(strValue);

	strValue.Format(_T("%s") , column_value[3]) ;
	pAddr->nColdDig = atoi(strValue) ;

	strValue.Format(_T("%s") , column_value[4]) ;
	pAddr->bSign = atoi(strValue) ;

	strValue.Format(_T("%s") , column_value[5]) ;
	memcpy(pAddr->Label , strValue.GetBuffer() , sizeof(pAddr->Label));

	return 0;
}
//获取钱包地址列表
int CallGetWalletAddressList(void *para, int n_column, char ** column_value, char ** column_name)
{
	map<CString,uistruct::LISTADDR_t> *pListInfo = (map<CString,uistruct::LISTADDR_t> *)para;
	if(n_column != 6)
		return -1;
	CString strValue(_T(""));
	uistruct::LISTADDR_t listdata;
	
	if(CallGetWalletAddressItem(&listdata, n_column, column_value, column_name) < 0 )
		return -1;
	
	CString key;
	key.Format(_T("%s"), listdata.address);
	(*pListInfo)[key] = listdata;
	return 0;
}
//获取quiz pool某一项
int CallGetP2PQuizPoolItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::LISTP2POOL_T* pPoolInfo = (uistruct::LISTP2POOL_T*)para;
	if(NULL == column_value[0])
		return -1;
	if(n_column != 2)
		return -1;
	CString strValue ;
	strValue.Format(_T("%s") , column_value[0]) ;
	pPoolInfo->hash = strValue;

	strValue.Format(_T("%s") , column_value[1] ) ;
	pPoolInfo->data = strValue;
	return 0;
}
//获取quiz pool列表
int CallGetP2PQuizPoolList(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::P2PLIST* pListInfo = (uistruct::P2PLIST *)para;
	uistruct::LISTP2POOL_T sPool;
	if(CallGetP2PQuizPoolItem((void*)(&sPool), n_column, column_value, column_name)<0)
		return -1;
	pListInfo->push_back(sPool);
	return 0;
}
//获取某个交易信息
int CallGetTransactionItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::REVTRANSACTION_t *pTxItem =  (uistruct::REVTRANSACTION_t*) para;
	if(NULL == column_value[0])
		return -1;
	
	if(n_column != 15)
		return -1;
	Json::Value root;
	CString strValue;
	strValue.Format(_T("%s") , column_value[0] ) ;
	root["hash"] = strValue.GetString();

	strValue.Format(_T("%s") , column_value[1] ) ;
	root["txtype"] = strValue.GetString();

	strValue.Format(_T("%s") , column_value[2] ) ;
	root["ver"] = atoi(strValue);

	strValue.Format(_T("%s") , column_value[3] ) ;
	root["addr"] = strValue.GetString();

	strValue.Format(_T("%s") , column_value[4] ) ;
	root["pubkey"] =  strValue.GetString();

	strValue.Format(_T("%s") , column_value[5] ) ;
	root["miner_pubkey"] = strValue.GetString();

	strValue.Format(_T("%s") , column_value[6] ) ; 
	root["fees"] = (INT64)(strtod (strValue, NULL)*100000000);

	strValue.Format(_T("%s") , column_value[7] ) ;
	root["height"] = atoi(strValue);

	strValue.Format(_T("%s") , column_value[8] ) ;
	root["desaddr"] = strValue.GetString();

	strValue.Format(_T("%s") , column_value[9] ) ;
	root["money"] = (INT64)(strtod (strValue, NULL)*100000000);

	strValue.Format(_T("%s") , column_value[10] ) ;
	root["Contract"] =  strValue.GetString();

	strValue.Format(_T("%s") , column_value[11] ) ;
	root["confirmedHeight"] = atoi(strValue);

	strValue.Format(_T("%s") , column_value[12] ) ;
	root["confirmedtime"] = atoi(strValue);

	strValue.Format(_T("%s") , column_value[13] ) ;
	root["blockhash"] = strValue.GetString();

	strValue.Format(_T("%s") , column_value[14] ) ;
	root["state"] = atoi(strValue);

	pTxItem->JsonToStruct(root.toStyledString());
	
	return 0;

}
//获取交易列表
int CallGetTransactionList(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::TRANSRECORDLIST* pListInfo = (uistruct::TRANSRECORDLIST*) para;
	uistruct::REVTRANSACTION_t sTxInfo;
	if(CallGetTransactionItem((void *)(&sTxInfo), n_column, column_value, column_name) < 0)
	{
		return -1;
	}
	pListInfo->push_back(sTxInfo);
	return 0;
}
//获取表记录总条数
int CallCountTableItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	if(NULL == column_value[0])
		return -1;
	if(1 == n_column && !strcmp(column_name[0],"num"))
		*(int *)para = atoi(column_value[0]);
	return 0;
}
//获取tip block hash
int CallGetTipBlockHash(void *para, int n_column, char ** column_value, char ** column_name)
{

	if(NULL == column_value[0])
		return -1;
	CString strValue;
	if(n_column != 1)
	{
		return -1;
	}
	memcpy(para, column_value[0], strlen(column_value[0]));
	return 0;
}
//获取指定表某个字段的总和
int CallGetTableItemSum(void *para, int n_column, char ** column_value, char ** column_name)
{
	if(NULL == column_value[0])
		return 0;
	
	if(1 == n_column && !strcmp(column_name[0],"total"))
		*(double *)para = strtod (column_value[0], NULL);
	return 0;

}
//获取地址簿管理某一项
int CallGetAddressBookItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::ADDRBOOK_t *pAddrBook = (uistruct::ADDRBOOK_t *)para;
	if(n_column != 2)
		return -1;
	CString strValue;
	strValue.Format(_T("%s") , column_value[0] );
	pAddrBook->label = strValue;

	strValue.Format(_T("%s") , column_value[1] ) ;
	pAddrBook->address = strValue;
	return 0;
}
//获取地址簿管理列表
int CallGetAddressBookList(void *para, int n_column, char ** column_value, char ** column_name)
{
	map<CString,uistruct::ADDRBOOK_t>* pAddrBookMap = (map<CString, uistruct::ADDRBOOK_t> *)para;
	uistruct::ADDRBOOK_t sAddrBook;
	if(CallGetAddressBookItem(&sAddrBook, n_column, column_value, column_name) < 0)
		return -1;
	(*pAddrBookMap)[sAddrBook.address] = sAddrBook;
	return 0;
}

//获取表的记录条数
int CSqliteDeal::GetTableCountItem(const CString &strTabName ,const CString &strCondition)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	int nCount(0);
	CString strSQL(_T(""));
	strSQL = _T("SELECT count(*) as num FROM ") + strTabName + _T(" WHERE ") + strCondition;
	ExcuteSQL(pDBConn, &CallCountTableItem, strSQL, (void *)(&nCount));
	return nCount;
}

int CSqliteDeal::GetWalletAddressItem(const CString &strCondition, uistruct::LISTADDR_t *pAddr)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	
	CString strSQL(_T(""));
	strSQL = _T("SELECT * FROM t_wallet_address WHERE ") + strCondition;
	ExcuteSQL(pDBConn, &CallGetWalletAddressItem, strSQL, (void *)(pAddr));
	return 0;

}

int CSqliteDeal::GetWalletAddressList(const CString &strCondition, map<CString,uistruct::LISTADDR_t> *pListInfo)
{
	sqlite3 ** pDBConn = GetDBConnect();
	CString strSQL(_T(""));
	strSQL = _T("SELECT * FROM t_wallet_address WHERE ") + strCondition;
	ExcuteSQL(pDBConn, &CallGetWalletAddressList, strSQL, (void *)pListInfo);
	return 0;
}

int CSqliteDeal::GetP2PQuizRecordItem(const CString &strCondition, uistruct::P2P_QUIZ_RECORD_t * p2pQuizRecord)
{
	sqlite3 ** pDBConn = GetDBConnect();
	CString strSQL(_T(""));
	strSQL = _T("SELECT * FROM t_p2p_quiz WHERE ") + strCondition;
	ExcuteSQL(pDBConn, &CallGetP2PQuizRecordItem, strSQL, (void *)p2pQuizRecord);
	return 0;

}

int CSqliteDeal::GetP2PQuizRecordList(const CString &strCondition, uistruct::P2PBETRECORDLIST * p2pQuizRecordList)
{
	sqlite3 ** pDBConn = GetDBConnect();
	CString strSQL(_T(""));
	strSQL = _T("SELECT * FROM t_p2p_quiz WHERE ") + strCondition;
	ExcuteSQL(pDBConn, &CallGetP2PQuizRecordList, strSQL, (void *)p2pQuizRecordList);
	return 0;

}

int CSqliteDeal::GetP2PQuizPoolItem(const CString &strCondition, uistruct::LISTP2POOL_T* pPoolItem)
{
	sqlite3 ** pDBConn = GetDBConnect();
	CString strSQL(_T(""));
	strSQL = _T("SELECT * FROM t_p2p_quiz WHERE ") + strCondition;
	ExcuteSQL(pDBConn, &CallGetP2PQuizPoolItem, strSQL, (void *)pPoolItem);
	return 0;
}

int CSqliteDeal::GetP2PQuizPoolList(const CString &strCondition, uistruct::P2PLIST* pPoolList)
{
	sqlite3 ** pDBConn = GetDBConnect();
	CString strSQL(_T(""));
	strSQL = _T("SELECT * FROM t_p2p_quiz WHERE ") + strCondition;
	ExcuteSQL(pDBConn, &CallGetP2PQuizPoolList, strSQL, (void *)pPoolList);
	return 0;
}

int CSqliteDeal::GetTipBlockHash(char *pBlockHash)
{	
	sqlite3** pDBConn = GetDBConnect();
	CString strSQL = _T("SELECT * FROM t_chain_tip");
	ExcuteSQL(pDBConn, &CallGetTipBlockHash, strSQL, (void *)pBlockHash);
	return 0;
}


//删除数据库表记录
int CSqliteDeal::DeleteTableItem(const CString &strTabName, const CString &strCondition)
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL(_T(""));
	strSQL =_T("DELETE FROM ") + strTabName + _T(" WHERE ") + strCondition;
	ExcuteSQL(pDBConn, NULL, strSQL, NULL);
	return 0;
}


//插入一条数据到表中
BOOL CSqliteDeal::InsertTableItem(const CString &strTabName ,const CString &strSourceData)
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL(_T(""));
	strSQL.Format(_T("INSERT INTO %s VALUES( %s )"),(LPSTR)(LPCTSTR)strTabName, (LPSTR)(LPCTSTR)strSourceData);
	return ExcuteSQL(pDBConn , NULL, strSQL, NULL);
}

//清空表数据
BOOL CSqliteDeal::ClearTableData(const CString &strTabName)
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL = _T("DELETE FROM ") + strTabName;
	return ExcuteSQL(pDBConn , NULL, strSQL, NULL);
}

//修改数据项
BOOL CSqliteDeal::UpdateTableItem(const CString &strTabName , const CString &strSourceData , const CString &strW)
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL = _T("UPDATE ") + strTabName + _T(" SET ") + strSourceData + _T(" WHERE ") + strW ;
	return ExcuteSQL(pDBConn , NULL, strSQL, NULL);
}

//判断blocktip是否还在主链上
BOOL CSqliteDeal::IsBlockTipInChain()
{
	char cTipBlockHash[35];
	memset(cTipBlockHash, 0, 35);
	GetTipBlockHash(cTipBlockHash);
	CString strShowData, strCommand;
	if(strlen(cTipBlockHash) == 0){
		strCommand.Format(_T("%s %d"),_T("getblock") ,-1 );
	}else{
		strCommand.Format(_T("%s %s"),_T("getblock") ,cTipBlockHash );
	}					
	CSoyPayHelp::getInstance()->SendRpc(strCommand, strShowData);
	if (strShowData == _T("")){
		return FALSE;
	}
	if(strShowData.Find("hash") < 0){		
		return FALSE;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
	{		
		return FALSE;
	}

	int height = root["height"].asInt();
	strCommand.Format(_T("%s %d"),_T("getblockhash") ,height );
	CSoyPayHelp::getInstance()->SendRpc(strCommand, strShowData);
	if (strShowData == _T("")){
		return FALSE;
	}
	if(strShowData.Find("hash") < 0){		
		return FALSE;
	}
	if (!reader.parse(strShowData.GetString(), root)) 
	{		
		return FALSE;
	}
	CString newblock = root["hash"].asCString();
	if (!strcmp(cTipBlockHash, newblock))
	{
		return FALSE;
	}

	return TRUE ;
}

double CSqliteDeal::GetTableItemSum(const CString &strTabName, const CString &stdFieldName, const CString &strCond)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL(_T(""));
	strSQL.Format(_T("SELECT sum(%s) as total FROM %s WHERE %s"),(LPSTR)(LPCTSTR)stdFieldName, (LPSTR)(LPCTSTR)strTabName, (LPSTR)(LPCTSTR)strCond);
	double dSum(0);
	if(!ExcuteSQL(pDBConn , &CallGetTableItemSum, strSQL, (void*)(&dSum)))
		return -1;
	 return dSum;
}

BOOL CSqliteDeal::GetAddressBookItem(const CString &strCond, uistruct::ADDRBOOK_t *pAddrBook)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL(_T(""));
	strSQL.Format(_T("SELECT * FROM t_address_book WHERE %s"),(LPSTR)(LPCTSTR)strCond);
	return ExcuteSQL(pDBConn , &CallGetAddressBookItem, strSQL, (void*)(pAddrBook));
}

BOOL CSqliteDeal::GetAddressBookList(const CString &strCond, map<CString,uistruct::ADDRBOOK_t>* pAddrBookMap)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL(_T(""));
	strSQL.Format(_T("SELECT * FROM t_address_book WHERE %s"),(LPSTR)(LPCTSTR)strCond);
	return ExcuteSQL(pDBConn , &CallGetAddressBookList, strSQL, (void*)(pAddrBookMap));
}


BOOL CSqliteDeal::GetTransactionItem(const CString &strCond, uistruct::REVTRANSACTION_t *pTxItem)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL(_T(""));
	strSQL.Format(_T("SELECT * FROM t_transaction WHERE %s"),(LPSTR)(LPCTSTR)strCond);
	return ExcuteSQL(pDBConn , &CallGetTransactionItem, strSQL, (void*)(pTxItem));
}

BOOL CSqliteDeal::GetTransactionList(const CString &strCond, uistruct::TRANSRECORDLIST *pTxList)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	CString strSQL(_T(""));
	strSQL.Format(_T("SELECT * FROM t_transaction WHERE %s"),(LPSTR)(LPCTSTR)strCond);
	return ExcuteSQL(pDBConn , &CallGetTransactionList, strSQL, (void*)(pTxList));
}

//todo 重构
BOOL  CSqliteDeal::UpdateP2pBetRecord()
{
	LOCK(m_pCs);
	uistruct::P2PBETRECORDLIST pTransaction;
	GetP2PQuizRecordList(_T(" 1=1 "), &pTransaction);

	if (0 == pTransaction.size() ) return FALSE ;

	vector<string> vSignHash;
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
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
		uistruct::P2P_QUIZ_RECORD_t  betrecord;
		//theApp.cs_SqlData.Lock();
		//int nItem =  theApp.m_SqliteDeal.FindDB(_T("t_p2p_quiz") , txhash ,_T("tx_hash"),&betrecord ) ;
		//theApp.cs_SqlData.Unlock();
		CString strCond;
		strCond.Format(_T(" tx_hash='%s' "), txhash);
		int nItem =GetP2PQuizRecordItem(strCond, &betrecord ) ;
		if (vTemp.size() == 0)  /// 此条数据在应用数据库中被删除了,如果被接赌了,说明已经揭赌了
		{
			if (strlen(betrecord.left_addr) != 0 && (betrecord.state == 1 || betrecord.state == 4 || betrecord.state == 5))
			{
				CString strField,strWhere;
				strField.Format(_T("state = %d ") , 2) ;
				strWhere.Format(_T("tx_hash = '%s'") , txhash );

				//更新数据
				if ( !UpdateTableItem(_T("t_p2p_quiz") , strField , strWhere )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , txhash );
				}
			}
			continue;
		}
		memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

		if (DBbet.betstate == 0x00 && betrecord.state ==4)   //正在接赌
		{
			continue;
		}

		if (DBbet.betstate == 0x01 && betrecord.state ==5)   //正在揭赌
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
				confirh = root1["confirmHeight"].asInt() ;    //交易被确认的高度
			}
			strCommand.Format(_T("%s"),_T("getinfo"));
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (!reader.parse(strShowData.GetString(), root1)) 
				continue;
			int curheight =root1["blocks"].asInt();
			if (curheight >=(confirh +DBbet.hight ))
			{
				state = 3;                              ///说明超时了
			}
		}
		//更新数据库  update t_p2p_quiz set timeout=15 where actor=0; 
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

		//更新数据
		if ( !UpdateTableItem(_T("t_p2p_quiz") , strField , strWhere )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , txhash );
		}
	}
	return true;
}

//todo 重构
BOOL CSqliteDeal::UpdateDarkRecord(){
	LOCK(m_pCs);
	uistruct::DARKRECORDLIST pTransaction;
	//GetRecorDarkData(&pTransaction);
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
		//int nItem =  theApp.m_SqliteDeal.FindDB(_T("t_dark_record") , txhash ,_T("tx_hash"),&betrecord ) ;
		//theApp.cs_SqlData.Unlock();

	//	int nItem =  Get(_T("t_dark_record") , txhash ,_T("tx_hash"),&betrecord ) ;
		int nItem(0);
		if (vTemp.size() == 0)  /// 此条数据在应用数据库中被删除了,确认收货了
		{
			if (nItem != 0 && (betrecord.state == 1 || betrecord.state == 5))
			{
				CString strField,strWhere;
				strField.Format(_T("state = %d ") , 2) ;
				strWhere.Format(_T("tx_hash = '%s'") , txhash );

				//更新数据
				if ( !UpdateTableItem(_T("t_dark_record") , strField , strWhere )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , txhash );
				}
			}else if (nItem != 0 && betrecord.state == 6)
			{
				CString strField,strWhere;
				strField.Format(_T("state = %d ") , 3) ;
				strWhere.Format(_T("tx_hash = '%s'") , txhash );

				//更新数据
				if ( !UpdateTableItem(_T("t_dark_record") , strField , strWhere )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , txhash );
				}
			}
			continue;
		}
		memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

		if (DBbet.flag && betrecord.state == 5)   //正在确认定单中
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

		//更新数据
		if ( !UpdateTableItem(_T("t_dark_record") , strField , strWhere )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , txhash );
		}
	}
	return true;
}

BOOL CSqliteDeal::IsExistTx(CString tablename,CString filed ,CString txhash){
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
		CString strCond;
		strCond.Format(_T(" %s='%s' "), filed, txhash);
		if (DeleteTableItem(tablename, strCond))
		{
			return flag;
		}
		
	}
	return flag;
}

void CSqliteDeal::CheckFailedCode(int retCode)
{
	if(retCode == 11)
	{
		if ( NULL != m_pSqliteWrite ) {
			sqlite3_close(m_pSqliteWrite);
			m_pSqliteWrite = NULL ;
		}
		if( NULL != m_pCs ) {
			delete m_pCs;
			m_pCs = NULL;
		}
		CString strFullPath(_T(""));
		strFullPath.Format(_T("%s\\db\\data.db") , theApp.str_InsPath );
		if(!DeleteFile((LPCTSTR)strFullPath))
			LogPrint("INFO", "删除数据库文件失败：%s\n", strFullPath.GetBuffer());
		exit(-1);
	}

}
//todo 重构
void  CSqliteDeal::UpdataAllTableData(BOOL flag){
/*	
	LOCK(m_pCs);
	///// 不用更新所所有的数据库表
	if (flag)
	{
		return ;
	}
	//// 更新交易数据库表
	if ( NULL == m_pSqliteRead ) {
		if ( !OpenSqlite(theApp.str_InsPath) ) return  ;
	}
	char    **  ppResult;    //结果
	int         nRow;       //行数
	int         nCol;       //列数
	char    *pzErrMsg;    //错误信息

	CString strSql = _T("SELECT * FROM t_dark_record");
	int nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "UpdataAllTableData error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return  ;
	}
	//读数据库
	int nIndex = nCol;
	int nTempRow = nRow;
	int nTempCol = nCol;
	CString strCommand , strValue ,strShowData ;
	for(int i = 0; i < nTempRow ; i++) {
		strValue.Format(_T("%s") , ppResult[nIndex+2] ) ;
		isExistTx(_T("t_dark_record"),_T("tx_hash"),strValue);
		nIndex +=nTempCol ;
	}
	sqlite3_free_table(ppResult);

	strSql = _T("SELECT * FROM t_p2p_quiz");
	nResult = sqlite3_get_table(m_pSqliteRead,strSql.GetBuffer(),&ppResult,&nRow,&nCol,&pzErrMsg);
	if ( nResult != SQLITE_OK ){
		LogPrint("INFO", "UpdataAllTableData select t_p2p_quiz error:%s\n", pzErrMsg);
		sqlite3_close(m_pSqliteRead);  
		sqlite3_free(pzErrMsg);  
		m_pSqliteRead = NULL ;
		return  ;
	}
	//读数据库
	nIndex = nCol;
	nTempRow = nRow;
	nTempCol = nCol;
	for(int i = 0; i < nTempRow ; i++) {
		strValue.Format(_T("%s") , ppResult[nIndex+3] ) ;
		isExistTx(_T("t_p2p_quiz"),_T("tx_hash"),strValue);
		nIndex +=nTempCol ;
	}
	sqlite3_free_table(ppResult);

	*/
}