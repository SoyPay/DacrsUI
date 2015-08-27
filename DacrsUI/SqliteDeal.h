#pragma once
#include "CommonStruct.h"

typedef int (*CallBackFunc)(void * para, int n_column, char ** column_value, char ** column_name);

class CSqliteDeal
{
public:
	CSqliteDeal(void);
public:
	~CSqliteDeal(void);
private:
	sqlite3 *   m_pSqliteWrite; //Ð´Á¬½Ó
	CCriticalSection  * m_pCs;

private:
	void   CheckFailedCode(int retCode);

public:
	void   UpdataAllTableData();

public:
	sqlite3**   GetDBConnect();
	BOOL       InitializationDB();
	BOOL       CreatRedPacketTable();
	BOOL	   BeginDBTransaction();
	BOOL	   CommitDbTransaction();
	BOOL	   ExcuteSQL(sqlite3** ppDb, CallBackFunc pFunc, string strSQL, void *pPara);
	int		   GetTableCountItem(const string &strTabName ,const string &strCondition);
	int		   DeleteTableItem(const string &strTabName, const string &strCondition);
	BOOL       InsertTableItem(const string &strTabName ,const string &strSourceData);
	BOOL       ClearTableData(const string &strTabName);
	BOOL       UpdateTableItem(const string &strTabName , const string &strSourceData , const string &strW);
	int        GetWalletAddressItem(const string &strCond, uistruct::LISTADDR_t *pAddr);
	int        GetWalletAddressList(const string &strCondition, map<string,uistruct::LISTADDR_t> *pListInfo);
	int        GetP2PQuizRecordItem(const string &strCondition, uistruct::P2P_QUIZ_RECORD_t * p2pQuizRecord);
	int        GetP2PQuizRecordList(const string &strCondition, uistruct::P2PBETRECORDLIST * p2pQuizRecordList);
	int        GetP2PQuizPoolItem(const string &strCondition, uistruct::LISTP2POOL_T* pPoolItem);
	int        GetP2PQuizPoolList(const string &strCondition, uistruct::P2PLIST* pPoolList);
	int        GetTipBlockHash(char *pBlockHash);
	BOOL       IsBlockTipInChain();
	double     GetTableItemSum(const string &strTabName, const string &stdFieldName, const string &strCond);
	BOOL       GetAddressBookItem(const string &strCond, uistruct::ADDRBOOK_t *pAddrBook);
	BOOL       GetAddressBookList(const string &strCond, map<string,uistruct::ADDRBOOK_t>* pAddrBookMap);
	BOOL       GetTransactionItem(const string &strCond, uistruct::REVTRANSACTION_t *pTxItem);
	BOOL       GetTransactionList(const string &strCond, uistruct::TRANSRECORDLIST *pTxList);
	BOOL       IsExistTx(string tablename,string filed ,string txhash);
	int        GetRedPacketSendItem(const string &strCondition, uistruct::REDPACKETSEND_t * RedPackeSendRecord);
	int        GetRedPacketSendRecordList(const string &strCondition, uistruct::REDPACKETSENDLIST * RedPackeSendRecordList);
	int        GetRedPacketGrabItem(const string &strCondition, uistruct::REDPACKETGRAB_t * RedPackeGrabRecord);
	int        GetRedPacketGrabRecordList(const string &strCondition, uistruct::REDPACKETGRABLIST * RedPackeGrabRecordList);
	int        GetRedPacketPoolItem(const string &strCondition, uistruct::REDPACKETPOOL_t * RedPackePoolRecord);
	int        GetRedPacketPoolRecordList(const string &strCondition, uistruct::REDPACKETPOOLLIST * RedPackePoolRecordList);
	BOOL       IsExistField(const string tablename,const string filed , const string &strCond);
	BOOL       DeleteTable(const string tablename);
	int       GetCommonWalletAddressItem(const string &strCondition, uistruct::COMMONLISTADDR_t *pAddr);
	int       GetCommonWalletAddressList(const string &strCondition, map<int,uistruct::COMMONLISTADDR_t> *pListInfo);
};


extern int GetTransactionData( void *para, int n_column, char **column_value, char **column_name);

