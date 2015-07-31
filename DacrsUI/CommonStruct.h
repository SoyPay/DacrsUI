#pragma once
#include <Windows.h>
#include <fstream>
#include <Tlhelp32.h>  
#include <shlwapi.h>
#include <afxmt.h >
#pragma comment(lib,"Shlwapi.lib")
#include <string>
#include <vector>
#include "json\json.h"
#include "RpcCmd.h"
#include "UiHead.h"
//#include "sqlite3.h"
//#include "RPCDlg.h"

using namespace std;
typedef long long  int64_t;
static const ULONGLONG COIN = 100000000;
#define REAL_MONEY(x) (INT64)((x)*COIN)

#define BEGIN(a)            ((char*)&(a))
#define END(a)              ((char*)&((&(a))[1]))

#define WM_SEND_RPC_DATA		WM_USER+1
#define WM_SHOW_RECV_DATA		WM_USER+2
#define WM_SOCKET_RECEIVE		WM_USER+3
#define WM_SOCKET_CLOSE			WM_USER+4
#define WM_SOCKET_CONNECT		WM_USER+5
#define WM_SHOW_SEND_DATA		WM_USER+6
#define WM_UPDATE_HEIGHT		WM_USER+7
#define WM_GET_ACCOUNTINFO		WM_USER+8
#define WM_LIST_TX				WM_USER+9
#define WM_GET_ACCLIST			WM_USER+10
#define WM_GET_BET_RANDOM_DATA	WM_USER+11
#define WM_UPDATE				WM_USER+12
#define WM_UPWALLET             WM_USER+13
#define WM_REVTRANSACTION       WM_USER+14
#define WM_P2P_BET_RECORD       WM_USER+15
#define WM_SHOW_OPENBET			WM_USER+17
#define WM_DARK_RECORD          WM_USER+18
#define WM_UP_BETPOOL           WM_USER+19
#define WM_UP_BlLOCKTIP         WM_USER+22
#define WM_UP_ADDRESS           WM_USER+23
#define WM_APP_TRANSATION       WM_USER+24
#define WM_UP_NEWADDRESS        WM_USER+25
#define WM_UP_ADDRBOOK          WM_USER+26
#define WM_UP_DELETERBOOK       WM_USER+27
#define WM_SYNC_TRANSACTION     WM_USER+28
#define WM_INSERT               WM_USER+29
#define WM_REDPACKETPOOL        WM_USER+30
#define WM_LOCKSTATE            WM_USER+31
#define WM_RELEASETX            WM_USER+32
#define WM_REMOVETX             WM_USER+33
#define WM_CONNECTNET           WM_USER+34



#define WM_CLOSEAPP				WM_USER+35
#define WINDOW_TAG				_T("Dacrs-UIDemo")

typedef enum
{
	emRPCCommand = 1,
	emBaseFunction,
	emRegisterScript,
	emContract,
	emGetAccData,
	emGetAccountInfo,
	emGetRegScriptHash,
	emGetRegScriptID,
	emGetBetRandomData,
	emListTx,
	emNoType
}emSendDataType;

#define DEFINE_SINGLETON(cls)\
 private:\
 static std::auto_ptr<cls> m_pInstance;\
 protected:\
 cls(){}\
 public:\
 ~cls(){}\
 static cls* getInstance(){\
 if(!m_pInstance.get()){\
 m_pInstance = std::auto_ptr<cls>(new cls());\
 }\
 return m_pInstance.get();\
	}

#define IMPLEMENT_SINGLETON(cls) \
	std::auto_ptr<cls> cls::m_pInstance(NULL);

typedef enum 
{
	emSesureTx=1,
	emDarkTx,
	emAnonymTx,
	emP2PBet,
	emLottery,
	emNonType,
}emBusinessType;

//权限结构
typedef struct _AUTHORIZATE
{
	_AUTHORIZATE()
	{
		nAuthorizeTime = 0;
		nMaxMoneyPerTime = 0;
		nMaxMoneyTotal = 0;
		nMaxMoneyPerDay = 0;
	}

	int nAuthorizeTime;
	double nMaxMoneyPerTime;
	double nMaxMoneyTotal;
	double nMaxMoneyPerDay;
	string strUserData;
}AUTHORIZATE;

//*****************************担保合约结构************************************
#define ACCOUNT_ID_SIZE 6
#define MAX_ARBITRATOR 3
#define MAX_ACCOUNT_LEN 20
#define HASH_SIZE	32
#pragma pack(1)
typedef enum
{
	emFirstStep = 1,
	emSecondStep,
	emThirdStep,
	emLastStep,
	emInvalidStep
}enumSesureTrade;

typedef enum
{
	TX_BUYTRADE = 0x01,
	TX_SELLERTRADE = 0x02,
	TX_BUYERCONFIM = 0x03,
	TX_BUYERCANCEL = 0x04,
	TX_InvalidStep,
}enumDarkTx;

typedef enum
{
	emP2PBetASend = 1,
	emP2PBetBAccept,
	emP2PBetAExpose,
	emP2PBetBExpose,
	emInvalidP2PBet
}enumP2PBet;

typedef struct tag_INT64 {
	unsigned char data[8];
} Int64;

typedef struct tagACCOUNT_ID
{
	char accounid[ACCOUNT_ID_SIZE];
}ACCOUNT_ID;


typedef struct  {
	unsigned char nType;					//!<类型
	unsigned char nArbitratorCount;			//!<仲裁者个数
	ACCOUNT_ID 	buyer;						//!<买家ID（采用6字节的账户ID）
	ACCOUNT_ID seller;						//!<卖家ID（采用6字节的账户ID）
	ACCOUNT_ID arbitrator[MAX_ARBITRATOR];	//!<仲裁者ID（采用6字节的账户ID）
	long nHeight;							//!<超时绝对高度
	Int64 nFineMoney;						//!<卖家违约后最大罚款金额
	Int64 nPayMoney;						//!<买家向卖家支付的金额
	Int64 nFee;								//!<仲裁手续费
	Int64 ndeposit;							//!<仲裁押金,申诉时从仲裁者扣除的押金(如果仲裁不响应钱归买家否则买家退还给仲裁者)
}FIRST_CONTRACT;

typedef struct {
	unsigned char nType;					//!<交易类型
	unsigned char hash[HASH_SIZE];			//!<上一个交易包的哈希
} NEXT_CONTRACT;

typedef struct {
	unsigned char nType;					//!<交易类型
	unsigned char hash[HASH_SIZE];			//!<上一个交易的哈希
	Int64 nMinus;
}ARBIT_RES_CONTRACT;

//暗黑币
typedef struct  {
	unsigned char	dnType;					//!<类型
	char			seller[6];				//!<卖家ID（采用6字节的账户ID）
	ULONGLONG		nPayMoney;				//!<买家向卖家支付的金额

}DARK_FIRST_CONTRACT;

typedef struct {
	unsigned char dnType;					//!<交易类型
	char hash[32];							//!<上一个交易包的哈希
}DARK_NEXT_CONTRACT;


//匿名交易
typedef struct  
{
	char 			account[6];				//!<接受钱的ID（采用6字节的账户ID）
	ULONGLONG		nReciMoney;				//!<	收到钱的金额
} ACCOUNT_INFO;

struct BASE_ANONYM_CONTRACT
{
	char 			szSender[6];			//!<转账人ID（采用6字节的账户ID）
	int				nHeight;				//!<超时绝对高度
	ULONGLONG		nPayMoney;				//!<转账的人支付的金额
	unsigned short	len;                    //!<接受钱账户信息长度
};

struct ANONYM_CONTRACT:public BASE_ANONYM_CONTRACT
{
	vector<ACCOUNT_INFO> vAccInfo;
};

//P2P赌博
typedef struct {
	unsigned char type;
	ULONGLONG     money;
	unsigned short hight;
	unsigned char dhash[32];
} SEND_DATA;

typedef struct {
	unsigned char type;
	ULONGLONG money;
	unsigned char data;
	unsigned char txhash[32];		//发起对赌的哈希，也是对赌数据的关键字
} ACCEPT_DATA;

typedef struct {
	unsigned char type;
	unsigned char txhash[32];		//发起对赌的哈希，也是对赌数据的关键字
	unsigned char accepthash[32];		//接赌对赌的哈希，也是对赌数据的关键字
	unsigned char dhash[33];        //32 个随机数 + 中奖数
	int out_heitht;
} OPEN_DATA;

typedef struct {
	unsigned char systype;               //0xff
	unsigned char type;            // 0x01 提现  02 充值  03 提现一定的金额
	unsigned char typeaddr;            // 0x01 regid 0x02 base58
    ULONGLONG     money;
} APPACC;

typedef struct {
	unsigned char type;
	unsigned char num[15];
	unsigned char numlen;
	ULONGLONG money;
}ORDER_DATA;

////		抢红包
typedef struct {
	unsigned char nType;
	int64_t money;
	long number;
	unsigned char message[200];
}RED_PACKET;

typedef struct {
	unsigned char nType;
	unsigned char redhash[32];
}ACCEPT_RED_PACKET;


enum TXTYPE{
	TX_SENDBET = 0x01,
	TX_ACCEPTBET = 0x02,
	TX_OPENBET = 0x03,
	TX_WITHDRAWAL = 0x04,
};

enum REDPACKETTYPE{
	TX_COMM_SENDREDPACKET = 0x01,
	TX_COMM_ACCEPTREDPACKET = 0x02,
	TX_SPECIAL_SENDREDPACKET = 0x03,
	TX_SPECIAL_ACCEPTREDPACKET = 0x04
};

enum OPERATE{
	OP_SYSTEMACC = 0x00,
	OP_APPACC = 0x01,
	OP_NULL = 0x02,
};

typedef struct {
	unsigned char type;
}DRAW_DATA;

#pragma pack()





const signed char p_util_hexdigit[256] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1, -1, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0xa, 0xb, 0xc, 0xd, 0xe,
	0xf, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, };

inline signed char HexDigit(char c)
{
	return p_util_hexdigit[(unsigned char)c];
}



class CSoyPayHelp
{
	DEFINE_SINGLETON(CSoyPayHelp)
	static const int		REG_ID_SIZE = 6;
//	CRPCDlg*		 m_pRPCDlg;		
	CRpcCmd mRpcCmd;

	
private:
	string ParseRecvData(const char*pRecvData);

public:
   //CRPCDlg * pdlg,
	void InitialRpcCmd(const string &severip,const string	 &sendPreHeadstr,const string &sendendHeadstr,const string &rpcport)
	{
		mRpcCmd.SetHeadString(sendPreHeadstr,sendendHeadstr);
		mRpcCmd.SetPort(atoi(rpcport.c_str()));
		mRpcCmd.SetSeverIp(severip);
		//m_pRPCDlg = pdlg;
	}

	int SendRpc(string cmd,string &rev);
	int SendContacrRpc(string cmd,string &rev);
	int SendRpcDlg(string cmd,string &rev);
	void Jiexi(Json::Value &obj, HTREEITEM hItemRoot,CTreeCtrl&m_rpccommand);
	void Onexpandtree(CTreeCtrl& m_tree) ;  //展开所有节点
	double GetAccountBalance(CString addr);



	string	GetReverseHash(const string& strHash);
	string	GetHexData(const char*pData,size_t nLen);
	string	ParseTxHashFromJson(const string& strJsonData);
	string	ParseScriptIDFromJson(const string& strJsonData);
	string  EncodeBase64(const unsigned char* pch, size_t len);
	string  EncodeBase64(const string& str);
	string	CreateContractTx(const string& strScript,const string vAddr ,const string& strContract,
			int nHeight,int64_t nFee,int64_t money);
	string	SignContractTx(const string& strRawTxHash);
	string	CreateScriptTx(const char* pAccount,bool bPath,const char* pScript,int64_t nFee,int nHeight,
				int nAuthTime=0,int64_t nMoneyPerTime=0,int64_t nMoneyTotal=0,int64_t nMoneyPerDay=0,const string& strUserData="");
	string	CreateScriptAccountTx(const char* pAccount,int64_t nFee,bool bIsMiner);
	string	CreateNomalTx(const char* pFrom,const char* pTo,int64_t nMoney,int64_t nFee,int nHeight);
	bool	GetErrorMsg(const string& strRecvData,string& strErrorMsg);
	string	Setgenerate(bool bStart);
	string  GetFullRegID(const string& strRegID);
	string  GetNotFullRegID(std::vector<unsigned char>strRegID);

	template<typename T>
	std::string HexStr(const T itbegin, const T itend, bool fSpaces = false);

	template<typename T>
	inline std::string HexStr(const T& vch, bool fSpaces = false);

	
	

	vector<unsigned char> ParseHex(const char* psz);
	vector<unsigned char> ParseHex(const string& str);

	void LoadPng( CImage &img);

	string DisPlayMoney(string &strMoney);

	bool IsOSVersionBelowXp();
	void revert(char*pchar);
};

//*******************************担保交易************************************************
class CSesureTradeHelp
{
public:
	string PacketFirstContract(const string& strBuyID,const string& strSellID,const vector<string>& vArRegID,
		int nHeight,int64_t nFine,int64_t nPay,int64_t nFee,int64_t ndeposit);
	string PacketSecondContract(unsigned char* pHash);
	string PacketThirdContract(unsigned char* pHash);
	string PacketLastContract(unsigned char* pHash,int64_t nFine);

private:
	FIRST_CONTRACT		m_FirstContract;
	NEXT_CONTRACT		m_SecondContract;
	NEXT_CONTRACT		m_ThirdContract;
	ARBIT_RES_CONTRACT	m_LastContract;
};

//********************************暗黑币************************************
class CDarkTxHelp
{
public:
	string PacketBuyerContract(const string& strSellID,int64_t nMoney);
	string PacketNextContract(const string& strTxHash, unsigned char txTpye);

private:
	DARK_FIRST_CONTRACT	 m_FirstContract;
	DARK_NEXT_CONTRACT	 m_NextContract;
};

//********************************匿名交易************************************
class CAnonymTxHelp
{
public:
	string PackAnonymContract(const string& strSender,int nSendMoney,const vector<string>& vRecv,int nHeight);

private:
	ANONYM_CONTRACT	m_anonymContract;
};

//********************************P2P对赌************************************
class CP2PBetHelp
{
public:
	string PacketP2PSendContract(int64_t nMoney,int nHeight,const string& strRandomHash);
	string PacketP2PAcceptContract(int64_t nMoney, const string& strSendHash,char nData );
	string PacketP2PExposeContract(const string& SendHash,const string& strRandomHash,const string& AcceptHash,int outheight);
	string GetAppAccountMoneyContract(const string& straccid,int type,int typeaddr,int64_t nMoney);
	string GetReChangContract();
	string GetAppAccountSomeMoneyContract(const string& straccid,int type,int typeaddr,int64_t nMoney);
private:
	SEND_DATA		m_sendContract;
	ACCEPT_DATA		m_acceptContract;
	OPEN_DATA		m_openContract;
};

//********************************抢红包************************************
class CRedPacketHelp
{
public:
	string PacketSendCommContract(int64_t nMoney,int num,const string& message);
	string PacketAcceptCommContract(const string& strSendHash);
	string PacketSendSecpailContract(int64_t nMoney,int num,const string& message);
	string PacketAcceptSecpailContract(const string& strSendHash);
private:
	RED_PACKET		m_sendContract;
	ACCEPT_RED_PACKET		m_acceptContract;
};
















extern void ProductHttpHead(const string& configdir,const string& strCfgFileName,string &strPort,string& strPreHeadstr,string& strendHeadstr,string& struiport,int &netWork);
extern void RPCCommandToJson(const string& strRPCCommand,string& strSendData);


extern void ParseJsonAddrToMap(const CStringA& strValue,map<CString,CString>& mapAccount);
extern string ParseRecvData(const char*pRecvData);
extern void Setgenerate(bool bStart);
extern string GetFullRegID(const string& strRegID);
extern string GetCompressRegID(const string& strRegID);
extern void SendRPCData(const string& strData,emSendDataType emType);
extern bool CheckRegIDValid(const string& strRegID);
extern bool CheckHash(const CString& strRegID);