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

#define MSG_USER_ACCEPTRECOD_UI			    WM_USER+103	    //���µ�AcceptBetRecod����
#define MSG_USER_DARK_UI			        WM_USER+104	    //���µ�Dark����
#define MSG_USER_MAIN_UI			    WM_USER+105	    //���µ�Mian����

#define MSG_USER_SENDRECOD_UI			    WM_USER+106	    //���µ�SendRecord����
#define MSG_USER_SIGNACCEPT_UI			    WM_USER+107	    //���µ�SignAccountsDlg���� 1131
#define MSG_USER_TRANSRECORD_UI			    WM_USER+108	    //���µ�TransRecord����
#define MSG_USER_BETPOOL_UI			        WM_USER+109	    //���µ�CAcceptBetDlg����
#define MSG_USER_GET_SERVER_DATA            WM_USER+110     //�ӷ�������ȡ���� 1134
#define MSG_USER_GET_UPDATABASE             WM_USER+111     //�����ݿ��л�ȡ����
#define MSG_USER_SHOW_INIT                  WM_USER+112     //��ʾ��������ʼ�� 
#define MSG_USER_GET_TIPBLOCK_DATA          WM_USER+113     //���·�����bloktip hash 
#define MSG_USER_SENDDARK_UI			    WM_USER+114	    //���µ�����Dark����
#define MSG_USER_ACCEPTDARK_UI			    WM_USER+115	    //���µ�����Dark����
#define MSG_USER_SENDBET_UI			        WM_USER+116	    //���µ�����p2p����
#define MSG_USER_GETAPPM_UI			        WM_USER+116	    //���µ����ֽ���
#define MSG_USER_UP_PROGRESS			    WM_USER+117	    //���½�����  1131
#define MSG_USER_INSERT_DATA                WM_USER+118	    //�������ݵ����ݿ�
#define MSG_USER_UPDATA_DATA                WM_USER+119	    //�������ݵ����ݿ�
#define MSG_USER_BLOCKSTATE_UI			    WM_USER+120	    //���µ����ֽ���  1138
#define MSG_USER_SIGUSER_UI			        WM_USER+121	    //���µ�ע��Ӧ��  1138
#define MSG_USER_SEND_UI			        WM_USER+122    //���µ�ת��  1138
#define MSG_USER_RECIVE_UI			        WM_USER+122    //���µ�����ҳ��

#define MSG_USER_QUITTHREAD			        WM_USER+200	    //�˳��߳�


#define MSG_USER_END			        WM_USER+800	    //�˳��߳�

//enum
enum {
	LANGUAGE_CN = 0x01,
	LANGUAGE_EN ,
} ;
typedef enum tagDialogType{
	DIALOG_MYWALLET     = 0x01,    //�ҵ�Ǯ�� 
	DIALOG_P2P_BET       ,         //P2p��Լ
	DIALOG_GUARANTEE     ,         //��������  
	DIALOG_PRESS         ,         //��ѹ����
	DIALOG_SIGN_ACCOUNTS ,         //ע���ʺ�
	DIALOG_SIGN_USE      ,         //ע��Ӧ��
	DIALOG_TRANSFER      ,         //ת��
	DIALOG_TRANSFER_RECORD,        //���׼�¼
	DIALOG_SEND_RECORD   ,         //���ͼ�¼
	DIALOG_ACCEPTBET_RECORD   ,         //���ͼ�¼
	DIALOG_SEND_PRESS         ,         //���͵�ѹ����
	DIALOG_ACCEPT_PRESS         ,         //���͵�ѹ����
	DIALOG_BLOCK_STATE        ,         //״̬
} DialogType ;
 

typedef enum tagUiDataType{
	DATA_MYWALLET     = 0x001,   //�ҵ�Ǯ�� 
	DATA_P2P_BET       ,         //P2p��Լ
	DATA_GUARANTEE     ,         //��������  
	DATA_PRESS         ,         //��ѹ����
	DATA_SIGN_ACCOUNTS ,         //ע���ʺ�
	DATA_SIGN_USE      ,         //ע��Ӧ��
	DATA_TRANSFER      ,         //ת��
	DATA_TRANSFER_RECORD,        //���׼�¼
	DATA_SEND_RECORD   ,         //���ͼ�¼
} UiDataType ;

typedef enum tagUiType{
	UI_TRANSIONRECORD     = 0x01,   //���׼�¼ 
	UI_SENDP2P_RECORD       ,       //P2p��Լ
	UI_ACCEPTP2P_RECORD     ,       //���ܶ�Լ��¼  
	UI_DARK_RECORD          ,       //��ѹ����
} UiType ;

CString txTypeArray[] = { "NULL_TXTYPE", "REWARD_TX", "REG_ACCT_TX", "COMMON_TX", "CONTRACT_TX", "REG_APP_TX"};

#pragma pack(push)
#pragma pack(1)
namespace uistruct {
	//listaddr�ṹ
	typedef struct LISTADDR{   
		char    address[34+1] ;   //address
		char    RegID[34]   ;   //RegID
		double  fMoney      ;   //���
		int     nColdDig    ;   //�Ƿ�֧�����ڿ� 1:����֧��  �� 0:����֧��
		int     bSign       ;   //�Ƿ�ע��       1:��ע��    ,  0:ûע��
		char    Lebel[34]       ;
	}LISTADDR_t;
	typedef std::vector<LISTADDR_t> LISTADDRLIST ;
	//��ʼ���������ṹ��
	typedef struct INITSERVER{   
		char    sType[32] ;   //����
		char    msg[1000] ;   //����
	}INITSERVER_t;

	//���Ͷ�Լ �� ���ն�Լ
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
		time_t sendtime       ;     //����ʱ��
		time_t recvtime       ;     //����ʱ��
		int    timeout        ;     //��ʱ
		char   tx_hash[64+1]    ;      //����Լhash
		char   left_addr[34+1]  ;		//����Լ��ַ
		char   right_addr[34+1] ;		//���ܶ�Լ��ַ
		INT64  amount  ;				//���
		char   content[33]    ;       //���� 
		int    actor  ;               // 0 �����Լ 1 ���ܶ�Լ 2 ��ʹ�����Լ���ǽ��ܶ�Լ
		int    comfirmed      ;        // �Ƿ���ȷ��
		int    height         ;           //ȷ���߶�
		int    state          ;         //0 ����״̬ 1 �Ӷ�״̬ 2 �Ҷ�״̬ 3 �Ѿ������Ҷ�ʱ�� 4 ���ڽӶ� 5���ڽҶ�
		char   relate_hash[64+1];          //�Ӷ�hash
		int guessnum;      //�Ӳµ����� 
	}P2P_BET_RECORD_t;

	typedef std::vector<P2P_BET_RECORD_t> P2PBETRECORDLIST ;

	typedef struct {                 //��ԼӦ�����ݿ�ṹ
		unsigned char betstate;        //00δ�Ӷ� 01�Ӷ�
		INT64 money;
		unsigned short hight;
		unsigned char dhash[32];//32 + one byte checksum
		unsigned char accepthash[32];//32 + one byte checksum
		unsigned char sendbetid[6];
		unsigned char acceptbetid[6];
		unsigned char acceptebetdata;

	}DBBET_DATA;
	typedef std::vector<DBBET_DATA> DBBETDATALIST ;
	//���׼�¼
	typedef struct REVTRANSACTION{
		CString    txhash  ;   //hash
		string    txtype ;  //����
		int       ver;
		string    addr   ;  //
		string    pubkey;
		string	  miner_pubkey;
		double    fees       ;
		int       height     ;  // 
		string    desaddr   ;
		double   money        ;
		string   Contract  ;
		int      confirmedHeight ; //ȷ��ʱ��
		int      confirmedtime ; //ȷ��ʱ��
		string     blockhash      ;  //1:Ϊȷ��״̬   0:ΪδΪȷ��״̬
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
			txtype = "";  //����
			ver = 0;
			addr  = "" ;  //
			 pubkey  = "";
			  miner_pubkey  = "";
			  fees = 0.0      ;
			 height = 0    ;  // 
			desaddr  = ""   ;
			 money =0.0       ;
			 Contract  = ""  ;
			confirmedHeight = 0 ; //ȷ��ʱ��
			confirmedtime = 0; //ȷ��ʱ��
			blockhash  = ""      ;  //1:Ϊȷ��״̬   0:ΪδΪȷ��״̬
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

	//// ���ܵ�Ѻ���׼�¼
	typedef struct {
		time_t sendtime       ;     //����ʱ��
		time_t recvtime       ;     //����ʱ��
		char   tx_hash[64+1]    ;      //���͵�Ѻhash
		char   left_addr[34+1]  ;		//���͵�Ѻ��ַ
		char   right_addr[34+1] ;		//���ܵ�Ѻ��ַ
		INT64  amount  ;				//���
		int    actor  ;               // 0 ����Ѻ 1 ���ܵ�Ѻ 2 ��ʹ����Ѻ ���ǽ��ܵ�Ѻ
		int    comfirmed      ;        // �Ƿ���ȷ�� 00 û��ȷ�� 01 ��ȷ��
		int    state          ;         //0 ����״̬ 1 �ӵ�Ѻ״̬ 2 ȷ��״̬ 3 �Ѿ�ȡ������ 4 ���ڽӵ�Ѻ 5 ����ȷ�� 6 ����ȡ������
		char   relate_hash[64+1];          //�ӵ�Ѻhash 
	}DARK_RECORD;

	typedef std::vector<DARK_RECORD> DARKRECORDLIST ;
	typedef struct  {
		bool flag;                          // 00 ��Ѻ����Ϊ���� 01��Ѻ�����Ѿ�����
		char 	buyer[6];						//!<���ID������6�ֽڵ��˻�ID��
		char seller[6];       //!<����ID������6�ֽڵ��˻�ID��
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
	int     RandNumber();   //����һ��1-6�������

	CString UI_LoadString( CString secID , CString szID , UINT language ) ;  //�����ַ���ID�����ַ���

	HBITMAP GlobalBkgndBMP(UINT nIDBitmap);

	CString MbcsToUtf8(const char *file) ;

}
#endif