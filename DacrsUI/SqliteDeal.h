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
	BOOL   UpdateDarkRecord();

public:
	sqlite3**   GetDBConnect();
	BOOL       InitializationDB();
	BOOL       CreatRedPacketTable();
	BOOL	   BeginDBTransaction();
	BOOL	   CommitDbTransaction();
	BOOL	   ExcuteSQL(sqlite3** ppDb, CallBackFunc pFunc, CString strSQL, void *pPara);
	int		   GetTableCountItem(const CString &strTabName ,const CString &strCondition);
	int		   DeleteTableItem(const CString &strTabName, const CString &strCondition);
	BOOL       InsertTableItem(const CString &strTabName ,const CString &strSourceData);
	BOOL       ClearTableData(const CString &strTabName);
	BOOL       UpdateTableItem(const CString &strTabName , const CString &strSourceData , const CString &strW);
	int        GetWalletAddressItem(const CString &strCond, uistruct::LISTADDR_t *pAddr);
	int        GetWalletAddressList(const CString &strCondition, map<CString,uistruct::LISTADDR_t> *pListInfo);
	int        GetP2PQuizRecordItem(const CString &strCondition, uistruct::P2P_QUIZ_RECORD_t * p2pQuizRecord);
	int        GetP2PQuizRecordList(const CString &strCondition, uistruct::P2PBETRECORDLIST * p2pQuizRecordList);
	int        GetP2PQuizPoolItem(const CString &strCondition, uistruct::LISTP2POOL_T* pPoolItem);
	int        GetP2PQuizPoolList(const CString &strCondition, uistruct::P2PLIST* pPoolList);
	int        GetTipBlockHash(char *pBlockHash);
	BOOL       IsBlockTipInChain();
	double     GetTableItemSum(const CString &strTabName, const CString &stdFieldName, const CString &strCond);
	BOOL       GetAddressBookItem(const CString &strCond, uistruct::ADDRBOOK_t *pAddrBook);
	BOOL       GetAddressBookList(const CString &strCond, map<CString,uistruct::ADDRBOOK_t>* pAddrBookMap);
	BOOL       GetTransactionItem(const CString &strCond, uistruct::REVTRANSACTION_t *pTxItem);
	BOOL       GetTransactionList(const CString &strCond, uistruct::TRANSRECORDLIST *pTxList);
	BOOL       IsExistTx(CString tablename,CString filed ,CString txhash);
	int        GetRedPacketSendItem(const CString &strCondition, uistruct::REDPACKETSEND_t * RedPackeSendRecord);
	int        GetRedPacketSendRecordList(const CString &strCondition, uistruct::REDPACKETSENDLIST * RedPackeSendRecordList);
	int        GetRedPacketGrabItem(const CString &strCondition, uistruct::REDPACKETGRAB_t * RedPackeGrabRecord);
	int        GetRedPacketGrabRecordList(const CString &strCondition, uistruct::REDPACKETGRABLIST * RedPackeGrabRecordList);
	int        GetRedPacketPoolItem(const CString &strCondition, uistruct::REDPACKETPOOL_t * RedPackePoolRecord);
	int        GetRedPacketPoolRecordList(const CString &strCondition, uistruct::REDPACKETPOOLLIST * RedPackePoolRecordList);
	BOOL       IsExistField(const CString tablename,const CString filed , const CString &strCond);
	BOOL       DeleteTable(const CString tablename);
};


extern int GetTransactionData( void *para, int n_column, char **column_value, char **column_name);

