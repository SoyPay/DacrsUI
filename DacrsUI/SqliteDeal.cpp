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
	string strTableName = "sqlite_master";
	string strCondition = "type='table' and name = 't_wallet_address'";

	if(!GetTableCountItem(strTableName, strCondition)) 
	{
		string createSQL="CREATE TABLE t_wallet_address(address TEXT PRIMARY KEY, reg_id TEXT, money DOUBLE, cold_dig INT, sign INT, label TEXT)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_wallet_address failed\n");
			return FALSE;
		}
	}
	strCondition = "type='table' and name= 't_common_address'";
	if(!GetTableCountItem(strTableName, strCondition)) 
	{
		string createSQL="CREATE TABLE t_common_address(reg_id TEXT, app_id TEXT, keyid integer PRIMARY KEY autoincrement)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_common_address failed\n");
			return FALSE;
		}
	}

	strCondition = "type='table' and name= 't_chain_tip'";
	if(!GetTableCountItem(strTableName, strCondition))
	{
		string createSQL="CREATE TABLE t_chain_tip(block_hash TEXT PRIMARY KEY);";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_chain_tip failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_address_book'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		string createSQL="CREATE TABLE t_address_book(Label TEXT,address TEXT PRIMARY KEY)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_address_book failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_dark_record'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		string createSQL= "CREATE TABLE t_dark_record(send_time TEXT,recv_time TEXT,tx_hash TEXT PRIMARY KEY,left_addr TEXT,right_addr TEXT,amount INT,actor INT,confirmed INT ,state INT,relate_hash TEXT)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_dark_record failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_p2p_quiz'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		string createSQL="CREATE TABLE t_p2p_quiz(send_time TEXT,recv_time TEXT,time_out INT,tx_hash TEXT PRIMARY KEY, left_addr TEXT, right_addr TEXT, amount double, content TEXT, actor INT, comfirmed INT, height INT, state INT, relate_hash TEXT, guess_num INT,deleteflag INT)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}else if (!IsExistField(_T("t_p2p_quiz"),_T("deleteflag"),_T("1=1")))
	{
		string createSQL="alter table t_p2p_quiz add column deleteflag INT default 0";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_quiz_pool'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
			string createSQL="CREATE TABLE t_quiz_pool(hash TEXT PRIMARY KEY,send_acct_id TEXT,total_amount INT,height INT,acceptid TEXT,accepthash TEXT,guess INT,state INT)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}else if (!IsExistField(_T("t_quiz_pool"),_T("acceptid"),_T("1=1")))
	{
		DeleteTable(_T("t_quiz_pool"));
		string createSQL="CREATE TABLE t_quiz_pool(hash TEXT PRIMARY KEY,send_acct_id TEXT,total_amount INT,height INT,acceptid TEXT,accepthash TEXT,guess INT,state INT)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_quiz_pool failed\n");
			return FALSE;
		}
	}

	strCondition = "type='table' and name='t_transaction'";
	if(!GetTableCountItem(strTableName, strCondition))
	{
		string createSQL= "CREATE TABLE t_transaction(hash TEXT PRIMARY KEY, tx_type TEXT,version INT, src_addr TEXT, pub_key TEXT, miner_pub_key TEXT, fees DOUBLE, height INT, des_addr TEXT, money DOUBLE, contract TEXT, confirm_height INT, confirmed_time INT, block_hash TEXT, state INT)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}

	if (!CreatRedPacketTable())
	{
		LogPrint("INFO", "Create table redpacket failed\n");
		return FALSE;
	}
	return TRUE ;
}

BOOL   CSqliteDeal::CreatRedPacketTable()
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	string strCondition;
	string strTableName(_T("sqlite_master"));
	strCondition = "type='table' and name='t_red_packets_send'";
	if(!GetTableCountItem(strTableName, strCondition))
	{
		string createSQL = "CREATE TABLE t_red_packets_send(send_hash TEXT PRIMARY KEY,send_time INT ,total_amount double,packets_num  INT,send_acct_id TEXT, confirm_height INT,packet_type INT)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}

	strCondition = "type='table' and name='t_red_packets_grab'";
	if(!GetTableCountItem(strTableName, strCondition))
	{
		string createSQL="CREATE TABLE t_red_packets_grab(send_hash TEXT,grab_hash TEXT PRIMARY KEY,grab_time INT,lucky_amount double,send_acct_id TEXT,grab_acct_id TEXT,confirm_height INT,packet_type INT,lucky_fortune INT,total_amount double,packets_num  INT)";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}

	strCondition = _T("type='table' and name='t_red_packets_pool'");
	if(!GetTableCountItem(strTableName, strCondition))
	{
		string createSQL="CREATE TABLE t_red_packets_pool(send_hash TEXT PRIMARY KEY,send_acct_id TEXT,total_amount double,packets_num INT,packet_type INT, message TEXT,average_amount double )";
		if(!ExcuteSQL(pDBConn, NULL, createSQL, NULL))
		{
			LogPrint("INFO", "Create table t_p2p_quiz failed\n");
			return FALSE;
		}
	}else if (!IsExistField(_T("t_red_packets_pool"),_T("average_amount"),_T("1=1")))
	{
		DeleteTable(_T("t_red_packets_pool"));
		string createSQL="CREATE TABLE t_red_packets_pool(send_hash TEXT PRIMARY KEY,send_acct_id TEXT,total_amount double,packets_num INT,packet_type INT, message TEXT,average_amount double )";
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
	string strPath;
	strPath = strprintf("%s\\db\\data.db" , theApp.str_InsPath.c_str() );
	//strPath.Format(_T("%s\\data.db") , theApp.dbpath); 
	if(NULL == m_pSqliteWrite)
	{
		int ret = sqlite3_open_v2( UiFun::MbcsToUtf8(strPath.c_str()), &m_pSqliteWrite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX, NULL);
		if ( 0 != ret) {   //打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
			sqlite3_close(m_pSqliteWrite);
			m_pSqliteWrite = NULL ;
			LogPrint("INFO", "Open DataBase %s failed!\n", strPath.c_str());
			return FALSE ;
		}
		LogPrint("INFO", "Open DataBase %s succeed!\n", strPath.c_str());
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
BOOL CSqliteDeal::ExcuteSQL(sqlite3** ppDb, CallBackFunc pFunc, string strSQL, void *pPara)
{
	LOCK(m_pCs);
	char *pzErrMsg = NULL;
	int nResult = sqlite3_exec(*ppDb, strSQL.c_str(), pFunc, pPara, &pzErrMsg);
	TRACE("%s\n", strSQL.c_str());
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
	if(n_column != 15)
		return -1;


	if (column_value[0] == "")
	{
		p2pQuizRecord->send_time = 0;
	}else{
		SYSTEMTIME curTime ;
		sscanf_s(column_value[0],"%04d-%02d-%02d %02d:%02d:%02d",&curTime.wYear, &curTime.wMonth, &curTime.wDay, &curTime.wHour, &curTime.wMinute, &curTime.wSecond);
		p2pQuizRecord->send_time = UiFun::SystemTimeToTimet(curTime);
	}
	
	SYSTEMTIME recTime ;

	if (column_value[1] == "")
	{
		p2pQuizRecord->recv_time = 0;
	}else{
		sscanf_s(column_value[1],"%04d-%02d-%02d %02d:%02d:%02d",&recTime.wYear, &recTime.wMonth, &recTime.wDay, &recTime.wHour, &recTime.wMinute, &recTime.wSecond);
		p2pQuizRecord->recv_time = UiFun::SystemTimeToTimet(recTime);
	}
			
	p2pQuizRecord->time_out =atoi(column_value[2]) ;
	
	p2pQuizRecord->tx_hash = strprintf("%s",column_value[3]);
			
	p2pQuizRecord->left_addr =  strprintf("%s",column_value[4]);
	
	p2pQuizRecord->right_addr = strprintf("%s",column_value[5]); ;
	
	
	p2pQuizRecord->amount = strtod(column_value[6],NULL);
		
	memcpy(p2pQuizRecord->content,column_value[7],sizeof(p2pQuizRecord->content));
			
	p2pQuizRecord->actor = atoi(column_value[8]);
		
	p2pQuizRecord->confirmed = atoi(column_value[9]);
		
	p2pQuizRecord->height = atoi( column_value[10]);
		
	p2pQuizRecord->state = atoi( column_value[11] );
	
	p2pQuizRecord->relate_hash = strprintf("%s",column_value[12]);
			
	p2pQuizRecord->guess_num = atoi(column_value[13]);
	
	if (column_value[14] != NULL)
	{
		p2pQuizRecord->deleteflag = atoi(column_value[14]);
	}
	
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

	pAddr->address=strprintf("%s",column_value[0]);
;
	pAddr->RegID=strprintf("%s",column_value[1]);

	pAddr->fMoney = strtod(column_value[2],NULL);

	pAddr->nColdDig = atoi(column_value[3]) ;

	pAddr->bSign = atoi(column_value[4]) ;

	pAddr->Label =strprintf("%s",column_value[5]);

	return 0;
}
//获取某一个钱包地址
int CallGetCommonWalletAddressItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::COMMONLISTADDR_t *pAddr = (uistruct::COMMONLISTADDR_t *)para;
	if(NULL == column_value[0])
		return -1;
	if(n_column != 3)
		return -1;

	pAddr->reg_id=strprintf("%s",column_value[0]);
;
	pAddr->app_id=strprintf("%s",column_value[1]);

	pAddr->keyid = atoi(column_value[2]) ;

	return 0;
}
//获取钱包地址列表
int CallGetWalletAddressList(void *para, int n_column, char ** column_value, char ** column_name)
{
	map<string,uistruct::LISTADDR_t> *pListInfo = (map<string,uistruct::LISTADDR_t> *)para;
	if(n_column != 6)
		return -1;
	
	uistruct::LISTADDR_t listdata;
	
	if(CallGetWalletAddressItem(&listdata, n_column, column_value, column_name) < 0 )
		return -1;
	
	(*pListInfo)[listdata.address] = listdata;
	return 0;
}
//获取常用钱包地址列表
int CallGetCommonWalletAddressList(void *para, int n_column, char ** column_value, char ** column_name)
{
	map<int,uistruct::COMMONLISTADDR_t> *pListInfo = (map<int,uistruct::COMMONLISTADDR_t> *)para;
	if(n_column != 3)
		return -1;

	uistruct::COMMONLISTADDR_t listdata;
	
	if(CallGetCommonWalletAddressItem(&listdata, n_column, column_value, column_name) < 0 )
		return -1;
	
	(*pListInfo)[listdata.keyid] = listdata;
	return 0;
}
//获取quiz pool某一项
int CallGetP2PQuizPoolItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::LISTP2POOL_T* pPoolInfo = (uistruct::LISTP2POOL_T*)para;
	if(NULL == column_value[0])
		return -1;
	if(n_column != 8)
		return -1;
	string strValue ;
	strValue = strprintf("%s" , column_value[0]) ;
	pPoolInfo->hash = strValue;

	strValue= strprintf("%s" , column_value[1] ) ;
	pPoolInfo->sendbetid = strValue;

	sscanf_s(column_value[2],"%lld",&pPoolInfo->nPayMoney);
	pPoolInfo->outheight = atoi(column_value[3]);

	pPoolInfo->acceptid = strprintf("%s" , column_value[4] ) ;

	pPoolInfo->accepthash = strprintf("%s" , column_value[5] ) ;
	pPoolInfo->guess =atoi(column_value[6]);
	pPoolInfo->state = atoi(column_value[7]);
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
	string strValue;
	strValue = strprintf("%s" , column_value[0] ) ;
	root["hash"] = strValue;

	strValue = strprintf("%s" , column_value[1] ) ;
	root["txtype"] = strValue;

	root["ver"] = atoi(column_value[2]);

	strValue = strprintf("%s" , column_value[3] ) ;
	root["addr"] = strValue;

	strValue = strprintf("%s" , column_value[4] ) ;
	root["pubkey"] =  strValue;

	strValue= strprintf("%s" , column_value[5] ) ;
	root["miner_pubkey"] = strValue;

	root["fees"] = (INT64)(strtod (column_value[6], NULL)*100000000);

	root["height"] = atoi(column_value[7] );

	strValue =strprintf("%s" , column_value[8] ) ;
	root["desaddr"] = strValue;

	root["money"] = (INT64)(strtod ( column_value[9] , NULL)*100000000);

	strValue = strprintf("%s" , column_value[10] ) ;
	root["Contract"] =  strValue;

	root["confirmHeight"] = atoi(column_value[11]);

	root["confirmedtime"] = atoi( column_value[12]);

	strValue = strprintf("%s", column_value[13] ) ;
	root["blockhash"] = strValue;

	root["state"] = atoi(column_value[14]);

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
	string strValue;
	strValue = strprintf("%s" , column_value[0] );
	pAddrBook->label = strValue;

	strValue = strprintf("%s", column_value[1] ) ;
	pAddrBook->address = strValue;
	return 0;
}
//获取地址簿管理列表
int CallGetAddressBookList(void *para, int n_column, char ** column_value, char ** column_name)
{
	map<string,uistruct::ADDRBOOK_t>* pAddrBookMap = (map<string, uistruct::ADDRBOOK_t> *)para;
	uistruct::ADDRBOOK_t sAddrBook;
	if(CallGetAddressBookItem(&sAddrBook, n_column, column_value, column_name) < 0)
		return -1;
	(*pAddrBookMap)[sAddrBook.address] = sAddrBook;
	return 0;
}

//获取表的记录条数
int CSqliteDeal::GetTableCountItem(const string &strTabName ,const string &strCondition)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	int nCount(0);
	string strSQL="";
	strSQL = "SELECT count(*) as num FROM " + strTabName + _T(" WHERE ") + strCondition;
	//strSQL = "SELECT count(*) as num FROM ";
	//strSQL+ =strTabName;
	//strSQL+ =" WHERE ";
	//strSQL+ =strCondition;
	ExcuteSQL(pDBConn, &CallCountTableItem, strSQL, (void *)(&nCount));
	return nCount;
}

int CSqliteDeal::GetWalletAddressItem(const string &strCondition, uistruct::LISTADDR_t *pAddr)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	
	string strSQL="";
	strSQL = "SELECT * FROM t_wallet_address WHERE " + strCondition;
	ExcuteSQL(pDBConn, &CallGetWalletAddressItem, strSQL, (void *)(pAddr));
	return 0;

}
int CSqliteDeal::GetCommonWalletAddressItem(const string &strCondition, uistruct::COMMONLISTADDR_t *pAddr)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	
	string strSQL="";
	strSQL = "SELECT * FROM t_common_address WHERE " + strCondition;
	ExcuteSQL(pDBConn, &CallGetCommonWalletAddressItem, strSQL, (void *)(pAddr));
	return 0;
}
int CSqliteDeal::GetWalletAddressList(const string &strCondition, map<string,uistruct::LISTADDR_t> *pListInfo)
{
	sqlite3 ** pDBConn = GetDBConnect();
	string strSQL("");
	strSQL = "SELECT * FROM t_wallet_address WHERE " + strCondition;
	ExcuteSQL(pDBConn, &CallGetWalletAddressList, strSQL, (void *)pListInfo);
	return 0;
}
int CSqliteDeal::GetCommonWalletAddressList(const string &strCondition, map<int,uistruct::COMMONLISTADDR_t> *pListInfo)
{
	sqlite3 ** pDBConn = GetDBConnect();
	string strSQL("");
	strSQL = "SELECT * FROM t_common_address WHERE " + strCondition;
	ExcuteSQL(pDBConn, &CallGetCommonWalletAddressList, strSQL, (void *)pListInfo);
	return 0;
}
int CSqliteDeal::GetP2PQuizRecordItem(const string &strCondition, uistruct::P2P_QUIZ_RECORD_t * p2pQuizRecord)
{
	sqlite3 ** pDBConn = GetDBConnect();
	string strSQL("");
	strSQL ="SELECT * FROM t_p2p_quiz WHERE " + strCondition;
	ExcuteSQL(pDBConn, &CallGetP2PQuizRecordItem, strSQL, (void *)p2pQuizRecord);
	return 0;

}

int CSqliteDeal::GetP2PQuizRecordList(const string &strCondition, uistruct::P2PBETRECORDLIST * p2pQuizRecordList)
{
	sqlite3 ** pDBConn = GetDBConnect();
	string strSQL("");
	strSQL = "SELECT * FROM t_p2p_quiz WHERE " + strCondition;
	ExcuteSQL(pDBConn, &CallGetP2PQuizRecordList, strSQL, (void *)p2pQuizRecordList);
	return 0;

}

int CSqliteDeal::GetP2PQuizPoolItem(const string &strCondition, uistruct::LISTP2POOL_T* pPoolItem)
{
	sqlite3 ** pDBConn = GetDBConnect();
	string strSQL("");
	strSQL = "SELECT * FROM t_quiz_pool WHERE " + strCondition;
	ExcuteSQL(pDBConn, &CallGetP2PQuizPoolItem, strSQL, (void *)pPoolItem);
	return 0;
}

int CSqliteDeal::GetP2PQuizPoolList(const string &strCondition, uistruct::P2PLIST* pPoolList)
{
	sqlite3 ** pDBConn = GetDBConnect();
	string strSQL("");
	strSQL = "SELECT * FROM t_quiz_pool WHERE " + strCondition;
	ExcuteSQL(pDBConn, &CallGetP2PQuizPoolList, strSQL, (void *)pPoolList);
	return 0;
}

int CSqliteDeal::GetTipBlockHash(char *pBlockHash)
{	
	sqlite3** pDBConn = GetDBConnect();
	string strSQL ="SELECT * FROM t_chain_tip";
	ExcuteSQL(pDBConn, &CallGetTipBlockHash, strSQL, (void *)pBlockHash);
	return 0;
}


//删除数据库表记录
int CSqliteDeal::DeleteTableItem(const string &strTabName, const string &strCondition)
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL =_T("DELETE FROM ") + strTabName + _T(" WHERE ") + strCondition;
	if (!ExcuteSQL(pDBConn, NULL, strSQL, NULL))
	{
		LogPrint("INFO","DeleteTableItem Error:%s\r\n",strSQL);
	}
	
	return 0;
}


//插入一条数据到表中
BOOL CSqliteDeal::InsertTableItem(const string &strTabName ,const string &strSourceData)
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL = strprintf("INSERT INTO %s VALUES( %s )",(LPSTR)(LPCTSTR)strTabName.c_str(), (LPSTR)(LPCTSTR)strSourceData.c_str());
	return ExcuteSQL(pDBConn , NULL, strSQL, NULL);
}

//清空表数据
BOOL CSqliteDeal::ClearTableData(const string &strTabName)
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL = "DELETE FROM " + strTabName;
	return ExcuteSQL(pDBConn , NULL, strSQL, NULL);
}

//修改数据项
BOOL CSqliteDeal::UpdateTableItem(const string &strTabName , const string &strSourceData , const string &strW)
{
	sqlite3 **pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL = "UPDATE " + strTabName + " SET " + strSourceData + " WHERE " + strW ;
	return ExcuteSQL(pDBConn , NULL, strSQL, NULL);
}

//判断blocktip是否还在主链上
BOOL CSqliteDeal::IsBlockTipInChain()
{
	char cTipBlockHash[35];
	memset(cTipBlockHash, 0, 35);
	GetTipBlockHash(cTipBlockHash);
	string strShowData, strCommand;
	if(strlen(cTipBlockHash) == 0){
		strCommand = strprintf("%s %d","getblock" ,-1 );
	}else{
		strCommand = strprintf("%s %s",_T("getblock") ,cTipBlockHash );
	}
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("IsBlockTipInChain rpccmd listaddr error");
		return false;
	}


	int height = root["height"].asInt();
	strCommand = strprintf("%s %d","getblockhash" ,height );
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("IsBlockTipInChain rpccmd getblockhash error");
		return false;
	}

	string newblock =root["hash"].asString(); 
	if (!strcmp(cTipBlockHash, newblock.c_str()))
	{
		return FALSE;
	}

	return TRUE ;
}

double CSqliteDeal::GetTableItemSum(const string &strTabName, const string &stdFieldName, const string &strCond)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL=strprintf("SELECT sum(%s) as total FROM %s WHERE %s",(LPSTR)(LPCTSTR)stdFieldName.c_str(), (LPSTR)(LPCTSTR)strTabName.c_str(), (LPSTR)(LPCTSTR)strCond.c_str());
	double dSum(0);
	if(!ExcuteSQL(pDBConn , &CallGetTableItemSum, strSQL, (void*)(&dSum)))
		return -1;
	 return dSum;
}

BOOL CSqliteDeal::GetAddressBookItem(const string &strCond, uistruct::ADDRBOOK_t *pAddrBook)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL= strprintf("SELECT * FROM t_address_book WHERE %s",(LPSTR)(LPCTSTR)strCond.c_str());
	return ExcuteSQL(pDBConn , &CallGetAddressBookItem, strSQL, (void*)(pAddrBook));
}

BOOL CSqliteDeal::GetAddressBookList(const string &strCond, map<string,uistruct::ADDRBOOK_t>* pAddrBookMap)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL =strprintf("SELECT * FROM t_address_book WHERE %s",(LPSTR)(LPCTSTR)strCond.c_str());
	return ExcuteSQL(pDBConn , &CallGetAddressBookList, strSQL, (void*)(pAddrBookMap));
}


BOOL CSqliteDeal::GetTransactionItem(const string &strCond, uistruct::REVTRANSACTION_t *pTxItem)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL=strprintf("SELECT * FROM t_transaction WHERE %s",(LPSTR)(LPCTSTR)strCond.c_str());
	return ExcuteSQL(pDBConn , &CallGetTransactionItem, strSQL, (void*)(pTxItem));
}

BOOL CSqliteDeal::GetTransactionList(const string &strCond, uistruct::TRANSRECORDLIST *pTxList)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL= strprintf("SELECT * FROM t_transaction WHERE %s",(LPSTR)(LPCTSTR)strCond.c_str());
	return ExcuteSQL(pDBConn , &CallGetTransactionList, strSQL, (void*)(pTxList));
}


BOOL CSqliteDeal::IsExistTx(string tablename,string filed ,string txhash){
	LOCK(m_pCs);
	string strCommand,strShowData;
	strCommand = strprintf("%s %s",_T("gettxdetail") ,txhash );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	BOOL flag = true;
	if (strShowData == _T("")){
		flag = false;
	}
	if(strShowData.find("hash") < 0){		
		flag = false;
	}
	if (!flag)
	{
		string strCond;
		strCond = strprintf(" %s='%s' ", filed, txhash);
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
		strFullPath.Format(_T("%s\\db\\data.db") , theApp.str_InsPath.c_str() );
		//strFullPath.Format(_T("%s\\data.db") , theApp.dbpath); 
		if(!DeleteFile((LPCTSTR)strFullPath))
			LogPrint("INFO", "删除数据库文件失败：%s\n", strFullPath.GetBuffer());
		exit(-1);
	}

}
//todo 重构
void  CSqliteDeal::UpdataAllTableData(){

	//LOCK(m_pCs);
	///// 不用更新所所有的数据库表
	uistruct::P2PBETRECORDLIST pTransaction;
	string conditon = strprintf("height >=%d",theApp.m_syncHight);
	GetP2PQuizRecordList(conditon, &pTransaction);

	//if (0 == pTransaction.size() ) return  ;

	vector<string> vSignHash;
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	for (const_it = pTransaction.begin() ; const_it != pTransaction.end() ; const_it++ ) {
		string strCondition("");
		if (const_it->actor == 0)  // 发赌者
		{
			strCondition=strprintf(" hash = '%s'", const_it->tx_hash.c_str());
			uistruct::REVTRANSACTION_t pTxItem;
			GetTransactionItem(strCondition, &pTxItem);
			if (pTxItem.txhash == "")
			{
				strCondition=strprintf(" tx_hash='%s'", const_it->tx_hash.c_str());
				//DeleteTableItem(_T("t_p2p_quiz"),strCondition);
				string strField;
				strField="deleteflag=1";
				if ( !UpdateTableItem(_T("t_p2p_quiz") ,strField,strCondition )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , const_it->tx_hash.c_str() );
				}
			}else if(pTxItem.confirmedHeight == 0){
				//更新数据
				string strField,strCond;
				strField +=strprintf("send_time='%s',recv_time = '%s',comfirmed = %d,height =%d,state = %d ,relate_hash = '%s' ,guess_num = %d,right_addr ='%s'" ,"" ,"",0,0,0,"",0,"" ) ;
				strCond=strprintf(" tx_hash='%s'", const_it->tx_hash);
				if ( !UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , const_it->tx_hash.c_str() );
				}
			}
		}else if (const_it->actor == 1)  /// 接赌约者
		{
			strCondition=strprintf(" hash = '%s'", const_it->relate_hash.c_str());
			uistruct::REVTRANSACTION_t pTxItem;
			GetTransactionItem(strCondition, &pTxItem);
			if (pTxItem.txhash == "")
			{
				strCondition=strprintf("relate_hash = '%s'",const_it->relate_hash.c_str());
			//	DeleteTableItem(_T("t_p2p_quiz"),strCondition);
				string strField;
				strField="deleteflag=1";
				if ( !UpdateTableItem(_T("t_p2p_quiz") ,strField,strCondition )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , const_it->tx_hash.c_str() );
				}
			}else if(pTxItem.confirmedHeight == 0){
				//更新数据
				string strField,strCond;
				strField+=strprintf("recv_time = '%s',comfirmed = %d,height =%d,state = %d" ,"",0,0,4) ;
				strCond=strprintf("relate_hash = '%s'",const_it->relate_hash.c_str());
				if ( !UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , const_it->tx_hash.c_str() );
				}
			}
		}else if (const_it->actor == 2)  /// 即是接赌者又是发赌约者
		{
			strCondition=strprintf(" hash = '%s'", const_it->tx_hash.c_str());
			uistruct::REVTRANSACTION_t pTxItem;
			GetTransactionItem(strCondition, &pTxItem);
			uistruct::REVTRANSACTION_t pTxItem1;
			strCondition=strprintf(" hash = '%s'",const_it->relate_hash.c_str());
			GetTransactionItem(strCondition, &pTxItem1);
			if (pTxItem.txhash =="")
			{
				strCondition=strprintf(" tx_hash='%s'", const_it->tx_hash.c_str());
				//DeleteTableItem(_T("t_p2p_quiz"),strCondition);
				string strField;
				strField="deleteflag=1";
				if ( !UpdateTableItem(_T("t_p2p_quiz") ,strField,strCondition )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , const_it->tx_hash.c_str() );
				}
			}else if(pTxItem.confirmedHeight == 0){
				//更新数据
				string strField,strCond;
				strField+=strprintf("send_time='%s',recv_time = '%s',comfirmed = %d,height =%d,state = %d ,relate_hash = '%s' ,guess_num = %d,right_addr ='%s'" ,"" ,"",0,0,0,"",0,"" ) ;
				strCond=strprintf(" tx_hash='%s'", const_it->tx_hash.c_str());
				if ( !UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , const_it->tx_hash.c_str() );
				}
			}else{
			 
				if (pTxItem1.txhash == "")
				{
					string strField,strCond;
					strField+=strprintf("recv_time = '%s',comfirmed = %d,state = %d ,relate_hash = '%s' ,guess_num = %d,right_addr ='%s',actor=%d" ,"",0,0,"",0,"",0 ) ;
					strCond=strprintf(" tx_hash='%s'", const_it->tx_hash.c_str());
					if ( !UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
						TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , const_it->tx_hash.c_str() );
					}
				}else if(pTxItem1.confirmedHeight == 0){
					//更新数据
					string strField,strCond;
					strField+=strprintf("recv_time = '%s',state = %d,height =%d " ,"" ,4,pTxItem.confirmedHeight) ;
					strCond=strprintf(" tx_hash='%s'", const_it->tx_hash.c_str());
					if ( !UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
						TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , const_it->tx_hash.c_str() );
					}

				}
			}
		}
	}

	
	uistruct::REDPACKETSENDLIST RedPackeSendRecordList;
	conditon = strprintf("confirm_height >=%d",theApp.m_syncHight);
	GetRedPacketSendRecordList(conditon, &RedPackeSendRecordList);
	std::vector<uistruct::REDPACKETSEND_t>::const_iterator const_it1;
	for (const_it1 = RedPackeSendRecordList.begin() ; const_it1 != RedPackeSendRecordList.end() ; const_it1++ ) {
		string strCondition="";
		strCondition= strprintf(" hash = '%s'", const_it1->send_hash.c_str());

		uistruct::REVTRANSACTION_t pTxItem;
		GetTransactionItem(strCondition, &pTxItem);
		if (pTxItem.txhash == _T(""))
		{
			strCondition = strprintf(" send_hash = '%s' ", const_it1->send_hash.c_str());
			DeleteTableItem(_T("t_red_packets_send"),strCondition);
		}else if(pTxItem.confirmedHeight == 0){
			//更新数据
			string strField,strCond;
			strField += strprintf("send_time=%d,confirm_height = %d" ,0 ,0 ) ;
			strCond= strprintf(" send_hash='%s' ", const_it1->send_hash.c_str());
			if ( !UpdateTableItem(_T("t_red_packets_send") ,strField,strCond )) {
				TRACE(_T("t_red_packets_send:更新数据失败!  Hash: %s") , const_it1->send_hash.c_str() );
			}
		}

		//int nItem =GetTableCountItem(_T("t_transaction") ,strCondition);
		//if (nItem == 0)
		//{
		//	strCondition.Format(" send_hash = '%s' ", const_it1->send_hash);
		//	DeleteTableItem(_T("t_red_packets_send"),strCondition);
		//}
	}

	uistruct::REDPACKETGRABLIST RedPackeGrabRecordList;
	conditon = strprintf("confirm_height >=%d",theApp.m_syncHight);
	GetRedPacketGrabRecordList(conditon, &RedPackeGrabRecordList);
	std::vector<uistruct::REDPACKETGRAB_t>::const_iterator const_it2;
	for (const_it2 = RedPackeGrabRecordList.begin() ; const_it2 != RedPackeGrabRecordList.end() ; const_it2++ ) {
		string strCondition(_T(""));
		strCondition =strprintf(" hash = '%s'", const_it2->grab_hash.c_str());
		uistruct::REVTRANSACTION_t pTxItem;
		GetTransactionItem(strCondition, &pTxItem);
		if (pTxItem.txhash == _T(""))
		{
			strCondition= strprintf(" grab_hash = '%s' ", const_it2->grab_hash.c_str());
			DeleteTableItem(_T("t_red_packets_grab"),strCondition);
		}else if(pTxItem.confirmedHeight == 0){
			//更新数据
			string strField,strCond;
			strField = strprintf("grab_time=%d,confirm_height = %d,lucky_fortune = 0,lucky_amount=%lf" ,0 ,0,0.0 ) ;
			strCond= strprintf(" grab_hash='%s' ", const_it2->grab_hash.c_str());
			if ( !UpdateTableItem(_T("t_red_packets_grab") ,strField,strCond )) {
				TRACE(_T("t_red_packets_grab:更新数据失败!  Hash: %s") , const_it2->grab_hash.c_str() );
			}
		}
		//int nItem =GetTableCountItem(_T("t_transaction") ,strCondition);
		//if (nItem == 0)
		//{
		//	strCondition.Format(" grab_hash = '%s' ", const_it2->grab_hash);
		//	DeleteTableItem(_T("t_red_packets_grab"),strCondition);
		//}
	}

}
//获取record
int CallGetRedPackeSendRecordItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::REDPACKETSEND_t * RedPacketRecord =  (uistruct::REDPACKETSEND_t *)para;
	if(NULL == column_value[0])
		return -1;
	if(n_column != 7)
		return -1;


	string strValue ;
	strValue =strprintf("%s" , column_value[0] ) ;
	RedPacketRecord->send_hash = strValue;

	RedPacketRecord->send_time = atoi(column_value[1]);

	RedPacketRecord->amount = strtod(column_value[2],NULL);

	RedPacketRecord->packet_num = atoi(column_value[3]);

	strValue = strprintf("%s" , column_value[4] ) ;
	RedPacketRecord->send_acc_id = strValue;

	RedPacketRecord->confirm_height = atoi(column_value[5]);

	RedPacketRecord->packet_type = atoi(column_value[6]);
	return 0;
}
//获取交易列表
int CallGetRedPackeSendRecordList(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::REDPACKETSENDLIST* pListInfo = (uistruct::REDPACKETSENDLIST*) para;
	uistruct::REDPACKETSEND_t sTxInfo;
	if(CallGetRedPackeSendRecordItem((void *)(&sTxInfo), n_column, column_value, column_name) < 0)
	{
		return -1;
	}
	pListInfo->push_back(sTxInfo);
	return 0;
}

//获取record
int CallGetRedPackeGrabRecordItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::REDPACKETGRAB_t * RedPacketRecord =  (uistruct::REDPACKETGRAB_t *)para;
	if(NULL == column_value[0])
		return -1;
	if(n_column != 11)
		return -1;


	string strValue ;
	strValue = strprintf("%s", column_value[0] ) ;
	RedPacketRecord->send_hash = strValue;

	strValue =strprintf("%s" , column_value[1] ) ;
	RedPacketRecord->grab_hash = strValue;

	RedPacketRecord->grab_time= atoi(column_value[2] );

	RedPacketRecord->lucky_amount = strtod(column_value[3],NULL);

	strValue =strprintf("%s", column_value[4] ) ;
	RedPacketRecord->send_acc_id = strValue;

	strValue =strprintf("%s" , column_value[5] ) ;
	RedPacketRecord->grab_acct_id = strValue;

	RedPacketRecord->confirm_height = atoi(column_value[6]);

	RedPacketRecord->packet_type = atoi(column_value[7]);

	RedPacketRecord->lucky_fortune = atoi(column_value[8]);

	RedPacketRecord->total_amount = strtod(column_value[9],NULL);

	RedPacketRecord->total_num = atoi(column_value[10]);

	return 0;
}
//获取交易列表
int CallGetRedPackeGrabRecordList(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::REDPACKETGRABLIST* pListInfo = (uistruct::REDPACKETGRABLIST*) para;
	uistruct::REDPACKETGRAB_t sTxInfo;
	if(CallGetRedPackeGrabRecordItem((void *)(&sTxInfo), n_column, column_value, column_name) < 0)
	{
		return -1;
	}
	pListInfo->push_back(sTxInfo);
	return 0;
}

//获取record
int CallGetRedPackePoolRecordItem(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::REDPACKETPOOL_t * RedPacketRecord =  (uistruct::REDPACKETPOOL_t *)para;
	if(NULL == column_value[0])
		return -1;
	if(n_column != 7)
		return -1;


	string strValue ;
	strValue = strprintf("%s" , column_value[0] ) ;
	RedPacketRecord->send_hash = strValue;

	strValue =strprintf("%s" , column_value[1] ) ;
	RedPacketRecord->send_acc_id= strValue;

	RedPacketRecord->total_amount = strtod(column_value[2],NULL);

	RedPacketRecord->packets_num = atoi(column_value[3]);

	RedPacketRecord->packet_type = atoi(column_value[4] );

	strValue= strprintf("%s", column_value[5] ) ;
	RedPacketRecord->message = strValue;
	return 0;
}
//获取交易列表
int CallGetRedPackePoolRecordList(void *para, int n_column, char ** column_value, char ** column_name)
{
	uistruct::REDPACKETPOOLLIST* pListInfo = (uistruct::REDPACKETPOOLLIST*) para;
	uistruct::REDPACKETPOOL_t sTxInfo;
	if(CallGetRedPackePoolRecordItem((void *)(&sTxInfo), n_column, column_value, column_name) < 0)
	{
		return -1;
	}
	pListInfo->push_back(sTxInfo);
	return 0;
}
int CSqliteDeal::GetRedPacketSendItem(const string &strCondition, uistruct::REDPACKETSEND_t * RedPackeSendRecord){

	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL = strprintf("SELECT * FROM t_red_packets_send WHERE %s",(LPSTR)(LPCTSTR)strCondition.c_str());
	return ExcuteSQL(pDBConn , &CallGetRedPackeSendRecordItem, strSQL, (void*)(RedPackeSendRecord));
}

int CSqliteDeal::GetRedPacketSendRecordList(const string &strCondition, uistruct::REDPACKETSENDLIST * RedPackeSendRecordList){

	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL = strprintf("SELECT * FROM t_red_packets_send WHERE %s",(LPSTR)(LPCTSTR)strCondition.c_str());
	return ExcuteSQL(pDBConn , &CallGetRedPackeSendRecordList, strSQL, (void*)(RedPackeSendRecordList));
}

int CSqliteDeal::GetRedPacketGrabItem(const string &strCondition, uistruct::REDPACKETGRAB_t * RedPackeGrabRecord){
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL =strprintf("SELECT * FROM t_red_packets_grab WHERE %s",(LPSTR)(LPCTSTR)strCondition.c_str());
	return ExcuteSQL(pDBConn , &CallGetRedPackeGrabRecordItem, strSQL, (void*)(RedPackeGrabRecord));
}

int CSqliteDeal::GetRedPacketGrabRecordList(const string &strCondition, uistruct::REDPACKETGRABLIST * RedPackeGrabRecordList){
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL= strprintf("SELECT * FROM t_red_packets_grab WHERE %s",(LPSTR)(LPCTSTR)strCondition.c_str());
	return ExcuteSQL(pDBConn , &CallGetRedPackeGrabRecordList, strSQL, (void*)(RedPackeGrabRecordList));
}

int CSqliteDeal::GetRedPacketPoolItem(const string &strCondition, uistruct::REDPACKETPOOL_t * RedPackePoolRecord){
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL= strprintf("SELECT * FROM t_red_packets_pool WHERE %s",(LPSTR)(LPCTSTR)strCondition.c_str());
	return ExcuteSQL(pDBConn , &CallGetRedPackePoolRecordItem, strSQL, (void*)(RedPackePoolRecord));
}
int CSqliteDeal::GetRedPacketPoolRecordList(const string &strCondition, uistruct::REDPACKETPOOLLIST * RedPackePoolRecordList){

	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL= strprintf("SELECT * FROM t_red_packets_pool WHERE %s",(LPSTR)(LPCTSTR)strCondition.c_str());
	return ExcuteSQL(pDBConn , &CallGetRedPackePoolRecordList, strSQL, (void*)(RedPackePoolRecordList));

}

int CallTableItem(void *para, int n_column, char ** column_value, char ** column_name)
{

	if(NULL == column_value[0])
		return 0;

	for (int i = 0;i<n_column;i++)
	{
		if (strcmp((char*)para ,column_name[0]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

BOOL CSqliteDeal::IsExistField(const string tablename,const string filed , const string &strCond)
{
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL =strprintf("SELECT * FROM %s",tablename);

	//strSQL =strprintf("SELECT * FROM USER_TAB_COLUMNS WHERE TABLE_NAME = '%s' AND COLUMN_NAME = '%s'",tablename,filed);
	BOOL isExist = false;
	char **pazResult;
	char *zErrMsg = NULL;
	int nRow, nCol;
	int rc = sqlite3_get_table(*pDBConn, strSQL.c_str(), &pazResult, &nRow, &nCol, &zErrMsg);
	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		return FALSE;
	}
	for(int i=0; i<nCol;++i) {
		string strValue ;
		strValue = strprintf("%s" , pazResult[i] ) ;
		if (strcmp(filed.c_str(),strValue.c_str()) == 0)
		{
			isExist = TRUE;
			break;
		}
	}
	sqlite3_free_table(pazResult);
	return isExist;
}

BOOL CSqliteDeal::DeleteTable(const string tablename){
	sqlite3 ** pDBConn = GetDBConnect(); //获取数据库连接
	string strSQL("");
	strSQL = strprintf("drop table %s",(LPSTR)(LPCTSTR)tablename.c_str());
	return ExcuteSQL(pDBConn , &CallTableItem, strSQL, NULL);
}