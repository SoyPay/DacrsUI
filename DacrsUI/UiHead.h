#ifndef __FB_UIHEAD__
#define __FB_UIHEAD__

#include <afxdb.h>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <time.h>
#include <math.h>
#include <string>
#include "json\json.h"

using namespace std;
//#define
#define LANGUAGE_FILE			"\\Config\\Language.ini"
#define LANGUAGE_FILE_CN		"\\Config\\Language_CN.ini"
#define LANGUAGE_FILE_EN		"\\Config\\Language_EN.ini"

#define MSG_USER_ACCEPTRECOD_UI			    WM_USER+103	    //更新到AcceptBetRecod界面
#define MSG_USER_DARK_UI			        WM_USER+104	    //更新到Dark界面
#define MSG_USER_MAIN_UI			    WM_USER+105	    //更新到Mian界面

#define MSG_USER_SENDRECOD_UI			    WM_USER+106	    //更新到SendRecord界面
#define MSG_USER_SIGNACCEPT_UI			    WM_USER+107	    //更新到SignAccountsDlg界面 1131
#define MSG_USER_TRANSRECORD_UI			    WM_USER+108	    //更新到TransRecord界面
#define MSG_USER_BETPOOL_UI			        WM_USER+109	    //更新到CAcceptBetDlg界面
#define MSG_USER_GET_SERVER_DATA            WM_USER+110     //从服务器获取数据 1134
#define MSG_USER_GET_UPDATABASE             WM_USER+111     //从数据库中获取数据
#define MSG_USER_SHOW_INIT                  WM_USER+112     //显示服务器初始化 
#define MSG_USER_GET_TIPBLOCK_DATA          WM_USER+113     //更新服务器bloktip hash 
#define MSG_USER_SENDDARK_UI			    WM_USER+114	    //更新到发送Dark界面
#define MSG_USER_ACCEPTDARK_UI			    WM_USER+115	    //更新到接受Dark界面
#define MSG_USER_SENDBET_UI			        WM_USER+116	    //更新到接受p2p界面
#define MSG_USER_GETAPPM_UI			        WM_USER+116	    //更新到提现界面
#define MSG_USER_UP_PROGRESS			    WM_USER+117	    //更新进度条  1131
#define MSG_USER_INSERT_DATA                WM_USER+118	    //插入数据到数据库
#define MSG_USER_UPDATA_DATA                WM_USER+119	    //更新数据到数据库
#define MSG_USER_BLOCKSTATE_UI			    WM_USER+120	    //更新到提现界面  1138
#define MSG_USER_SIGUSER_UI			        WM_USER+121	    //更新到注册应用  1138
#define MSG_USER_SEND_UI			        WM_USER+122    //更新到转账  1138
#define MSG_USER_RECIVE_UI			        WM_USER+122    //更新到接受页面

#define MSG_USER_QUITTHREAD			        WM_USER+200	    //退出线程


#define MSG_USER_END			        WM_USER+800	    //退出线程

//enum
enum {
	LANGUAGE_CN = 0x01,
	LANGUAGE_EN ,
} ;
typedef enum tagDialogType{
	DIALOG_MYWALLET     = 0x01,    //我的钱包 
	DIALOG_P2P_BET       ,         //P2p赌约
	DIALOG_GUARANTEE     ,         //担保交易  
	DIALOG_PRESS         ,         //抵压交易
	DIALOG_SIGN_ACCOUNTS ,         //注册帐号
	DIALOG_SIGN_USE      ,         //注册应用
	DIALOG_TRANSFER      ,         //转帐
	DIALOG_TRANSFER_RECORD,        //交易记录
	DIALOG_SEND_RECORD   ,         //发送记录
	DIALOG_ACCEPTBET_RECORD   ,         //发送记录
	DIALOG_SEND_PRESS         ,         //发送抵压交易
	DIALOG_ACCEPT_PRESS         ,         //发送抵压交易
	DIALOG_BLOCK_STATE        ,         //状态
} DialogType ;
 

typedef enum tagUiDataType{
	DATA_MYWALLET     = 0x001,   //我的钱包 
	DATA_P2P_BET       ,         //P2p赌约
	DATA_GUARANTEE     ,         //担保交易  
	DATA_PRESS         ,         //抵压交易
	DATA_SIGN_ACCOUNTS ,         //注册帐号
	DATA_SIGN_USE      ,         //注册应用
	DATA_TRANSFER      ,         //转帐
	DATA_TRANSFER_RECORD,        //交易记录
	DATA_SEND_RECORD   ,         //发送记录
} UiDataType ;

typedef enum tagUiType{
	UI_TRANSIONRECORD     = 0x01,   //交易记录 
	UI_SENDP2P_RECORD       ,       //P2p赌约
	UI_ACCEPTP2P_RECORD     ,       //接受赌约记录  
	UI_DARK_RECORD          ,       //抵压交易
} UiType ;

CString txTypeArray[] = { "NULL_TXTYPE", "REWARD_TX", "REG_ACCT_TX", "COMMON_TX", "CONTRACT_TX", "REG_APP_TX"};

#pragma pack(push)
#pragma pack(1)
namespace uistruct {
	//listaddr结构
	typedef struct LISTADDR{   
		char    address[34+1] ;   //address
		char    RegID[34]   ;   //RegID
		double  fMoney      ;   //金额
		int     nColdDig    ;   //是否支持冷挖矿 1:代表支持  ， 0:代表不支持
		int     bSign       ;   //是否注册       1:已注册    ,  0:没注册
		char    Lebel[34]       ;
	}LISTADDR_t;
	typedef std::vector<LISTADDR_t> LISTADDRLIST ;
	//初始化服务器结构体
	typedef struct INITSERVER{   
		char    sType[32] ;   //类型
		char    msg[1000] ;   //内容
	}INITSERVER_t;

	//发送赌约 和 接收赌约
	typedef struct DB_DATA{
		unsigned char betstate;
		INT64         money;
		unsigned int hight;
		unsigned char dhash[32];//32 + one byte checksum
		unsigned char accepthash[32];//32 + one byte checksum
		char     sendbetid[6];
		char     acceptbetid[6];
		unsigned char acceptebetdata;
	}DB_DATA_t;
	
	typedef struct P2P_BET_RECORD {
		time_t sendtime       ;     //发送时间
		time_t recvtime       ;     //接收时间
		int    timeout        ;     //超时
		char   tx_hash[64+1]    ;      //发赌约hash
		char   left_addr[34+1]  ;		//发赌约地址
		char   right_addr[34+1] ;		//接受赌约地址
		INT64  amount  ;				//金额
		char   content[33]    ;       //明文 
		int    actor  ;               // 0 发起赌约 1 接受赌约 2 即使发起赌约又是接受赌约
		int    comfirmed      ;        // 是否已确认
		int    height         ;           //确定高度
		int    state          ;         //0 发起状态 1 接赌状态 2 揭赌状态 3 已经超过揭赌时间 4 正在接赌 5正在揭赌
		char   relate_hash[64+1];          //接赌hash
		int guessnum;      //接猜的数字 
	}P2P_BET_RECORD_t;

	typedef std::vector<P2P_BET_RECORD_t> P2PBETRECORDLIST ;

	typedef struct {                 //赌约应用数据库结构
		unsigned char betstate;        //00未接赌 01接赌
		INT64 money;
		unsigned short hight;
		unsigned char dhash[32];//32 + one byte checksum
		unsigned char accepthash[32];//32 + one byte checksum
		unsigned char sendbetid[6];
		unsigned char acceptbetid[6];
		unsigned char acceptebetdata;

	}DBBET_DATA;
	typedef std::vector<DBBET_DATA> DBBETDATALIST ;
	//交易记录
	typedef struct REVTRANSACTION{
		CString    txhash  ;   //hash
		string    txtype ;  //类型
		int       ver;
		string    addr   ;  //
		string    pubkey;
		string	  miner_pubkey;
		double    fees       ;
		int       height     ;  // 
		string    desaddr   ;
		double   money        ;
		string   Contract  ;
		int      confirmedHeight ; //确认时间
		int      confirmedtime ; //确认时间
		string     blockhash      ;  //1:为确认状态   0:为未为确认状态
		int       state;
		string ToJson(){
			Json::Value root;
			root["hash"] = txhash.GetString();
			root["txtype"] = txtype;
			root["ver"] = ver;
			root["addr"] = addr;
			root["pubkey"] = pubkey;
			root["miner_pubkey"] = miner_pubkey;
			root["fees"] = fees;
			root["height"] = height;
			root["desaddr"] = desaddr;
			root["money"] = money;
			root["Contract"] = Contract;
			root["confirmedHeight"] = confirmedHeight;
			root["confirmedtime"] = confirmedtime;
			root["blockhash"] = blockhash;
			root["state"] = state;
			return root.toStyledString();
		}
		REVTRANSACTION(){
			txhash = _T("")  ;   
			txtype = "";  //类型
			ver = 0;
			addr  = "" ;  //
			 pubkey  = "";
			  miner_pubkey  = "";
			  fees = 0.0      ;
			 height = 0    ;  // 
			desaddr  = ""   ;
			 money =0.0       ;
			 Contract  = ""  ;
			confirmedHeight = 0 ; //确认时间
			confirmedtime = 0; //确认时间
			blockhash  = ""      ;  //1:为确认状态   0:为未为确认状态
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;

			this->txhash = root["hash"].asCString() ;  
			this->txtype = root["txtype"].asString(); 
			this->ver = root["ver"].asInt();
			this->addr = root["addr"].asString()  ;
			this->height  =root["height"].asInt()   ;

			if(json.find("money") >=0)
			this->money = (root["money"].asInt64()*1.0)/100000000 ;

			if(json.find("pubkey") >=0)
			this->pubkey = root["pubkey"].asString();
			if(json.find("miner_pubkey") >=0)
			this->miner_pubkey = root["miner_pubkey"].asString();
			if(json.find("fees") >=0)
			this->fees = (root["fees"].asInt64()*1.0)/100000000 ;     ;
			if(json.find("desaddr") >=0)
			this->desaddr =root["desaddr"].asString()  ;
			if(json.find("Contract") >=0)
			this->Contract = root["Contract"] .asString() ;
			if(json.find("confirmHeight") >=0)
			this->confirmedHeight = root["confirmHeight"].asInt() ; 
			if(json.find("confirmedtime") >=0)
			this->confirmedtime = root["confirmedtime"].asInt();  
			if(json.find("blockhash") >=0)
			this->blockhash = root["blockhash"].asString()     ;
			if(json.find("state") >=0)
			this->state = root["state"].asInt();
			return true;
		}
	}REVTRANSACTION_t;
	typedef std::vector<REVTRANSACTION_t> TRANSRECORDLIST ;

	//// 接受抵押交易记录
	typedef struct {
		time_t sendtime       ;     //发送时间
		time_t recvtime       ;     //接收时间
		char   tx_hash[64+1]    ;      //发送抵押hash
		char   left_addr[34+1]  ;		//发送抵押地址
		char   right_addr[34+1] ;		//接受抵押地址
		INT64  amount  ;				//金额
		int    actor  ;               // 0 发抵押 1 接受抵押 2 即使发抵押 又是接受抵押
		int    comfirmed      ;        // 是否已确认 00 没有确认 01 已确认
		int    state          ;         //0 发起状态 1 接抵押状态 2 确认状态 3 已经取消订单 4 正在接抵押 5 正在确认 6 正在取消订单
		char   relate_hash[64+1];          //接抵押hash 
	}DARK_RECORD;

	typedef std::vector<DARK_RECORD> DARKRECORDLIST ;
	typedef struct  {
		bool flag;                          // 00 抵押交易为接受 01抵押交易已经接受
		char 	buyer[6];						//!<买家ID（采用6字节的账户ID）
		char seller[6];       //!<卖家ID（采用6字节的账户ID）
		INT64 nPayMoney;
	}DARK_DATA_DB;

	typedef struct BLOCKCHANGED  {
		string  type ;
		int    time ;
		INT64  high  ;
		string   hash;
		int connections;
		string ToJson(){
			Json::Value root;
			root["type"] = type;
			root["time"] = time;
			root["high"] = high;
			root["hash"] = hash;
			root["connections"] = connections;
			return root.toStyledString();
		}
	bool JsonToStruct(string json){
		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(json, root)) 
			return false ;

		this->type = root["type"].asString();
		this->time = root["time"].asInt();
		this->high = root["high"].asInt64();
		this->hash = root["hash"].asString();
		this->connections = root["connections"].asInt();
		return true;
	}
	}BLOCKCHANGED_t;

	typedef struct DATABASEINFO  {
		string strSource ;
		string strWhere  ;
		string  strTabName          ;
		string ToJson(){
			Json::Value root;
			root["strSource"] = strSource;
			root["strWhere"] = strWhere;
			root["strTabName"] = strTabName;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;
			if (json.find("strSource") >= 0)
			this->strSource = root["strSource"].asString();

			this->strWhere = root["strWhere"].asString();
			this->strTabName = root["strTabName"].asString();
			return true;
		}
	}DATABASEINFO_t;

	typedef struct LISTP2POOL{   
		string   hash ;   //address
		string   data ;   //RegID
		string ToJson(){
			Json::Value root;
			root["hash"] = hash;
			root["data"] = data;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;

			this->hash = root["hash"].asString();
			this->data = root["data"].asString();
			return true;
		}
	}LISTP2POOL_T;
	typedef std::vector<LISTP2POOL_T> P2PLIST ;
}
#pragma pack(pop)
//UiFun
namespace UiFun 
{
	UINT    MallocP( void * * p , UINT size );
	void    ReleaseP( void * p ) ;
	CString Rnd32() ;
	time_t  SystemTimeToTimet(const SYSTEMTIME& st) ;
	SYSTEMTIME Time_tToSystemTime(time_t t);
	int     RandNumber();   //生成一个1-6的随机数

	CString UI_LoadString( CString secID , CString szID , UINT language ) ;  //根据字符串ID加载字符串

	HBITMAP GlobalBkgndBMP(UINT nIDBitmap);

	CString MbcsToUtf8(const char *file) ;

}
#endif