#pragma once
#include "CommonStruct.h"

class CSqliteDeal
{
public:
	CSqliteDeal(void);
public:
	~CSqliteDeal(void);
private:
	sqlite3 *   m_pSqlitedb;  //
	char    *   m_pzErrMsg ;  //错误信息
	char    **  m_pResult;    //结果
	int         m_nRow;       //行数
	int         m_nCol;       //列数
public:
	int    FindDB(const CString strTabName , const CString strP, const CString strSource );
	int    FindINTDB(const CString strTabName , const CString strP, const CString strSource );
	int    GetTableCount(const CString strTabName);
	int    FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::P2P_BET_RECORD_t * pResult );
	int	   FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::DARK_RECORD *darkrecord );
	int	   FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::TRANSRECORDLIST* pListInfo);
	int	   FindDB(const CString strTabName , const CString strP, const CString strSource,uistruct::LISTADDR_t *listaddr );
	BOOL   InsertData(const CString strTabName ,const CString strSourceData );
	BOOL   EmptyTabData(const CString strTabName );
	BOOL   Updatabase(const CString strTabName , const CString strSourceData , const CString strW );
	BOOL   DeleteData(const CString strTabName, const CString strSourceData , const CString strW);
	BOOL   CreateTabe(const CString strTabName,const CString strFiled);
	BOOL   IsExistTabe(const CString strTabName);
	string GetColSum(const CString strTabName , const CString strP, const CString strSource);
	string GetColSum(const CString strTabName,const CString filed);
public:
	BOOL   OpenSqlite(CString strPath) ;    //打开数据库 
public:
	BOOL   GetListaddrData(map<CString,uistruct::LISTADDR_t>* pListInfo); 
	BOOL   GetRevtransactionDatta(uistruct::TRANSRECORDLIST* pListInfo); 
	BOOL   GetRecorBetData(uistruct::P2PBETRECORDLIST* pListInfo); 
	BOOL  GetRecorDarkData(uistruct::DARKRECORDLIST* pListInfo);
	BOOL  GetRecorP2Pool(uistruct::P2PLIST* pListInfo);
	BOOL  isExistTx(CString tablename,CString filed ,CString txhash);
	BOOL  isinBlock();
	void  UpdataAllTableData(BOOL flag);
	void UpdataAllTable();
public:
	BOOL   UpdataP2pBetRecord();
	BOOL   UpdataDarkRecord();
public:
//		CRITICAL_SECTION        cs_UpDataResult ;  //数据库获取结果
};
