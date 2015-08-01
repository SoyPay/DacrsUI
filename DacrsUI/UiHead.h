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

#define MSG_USER_MAIN_UI			        WM_USER+105	    //���µ�Mian����
#define MSG_USER_TRANSRECORD_UI			    WM_USER+106	    //���µ�TransRecord����
#define MSG_USER_GET_UPDATABASE             WM_USER+107     //�����ݿ��л�ȡ����
#define MSG_USER_SHOW_INIT                  WM_USER+108    //��ʾ��������ʼ�� 
#define MSG_USER_UP_PROGRESS			    WM_USER+109	    //���½�����  1131
#define MSG_USER_INSERT_DATA                WM_USER+110    //�������ݵ����ݿ�
#define MSG_USER_UPDATA_DATA                WM_USER+111	    //�������ݵ����ݿ�
#define MSG_USER_SEND_UI			        WM_USER+112    //���µ�ת��  1138
#define MSG_USER_RECIVE_UI			        WM_USER+113    //���µ�����ҳ��
#define MSG_USER_STARTPROCESS_UI			WM_USER+114    //���������Ľ���
#define MSG_USER_UPDATA_UI			        WM_USER+115   //���µ�����ҳ��
#define MSG_USER_P2P_UI			            WM_USER+116	    //���µ�P2P����
#define MSG_USER_REDPACKET_UI			    WM_USER+117	    //���µ�P2P����

#define WM_SHOWTASK                        (WM_USER +118) 

#define MSG_USER_QUITTHREAD			        WM_USER+200	    //�˳��߳�
#define MSG_USER_OUT                        WM_USER+201     //�˳����


#define MSG_USER_END			        WM_USER+800	    //�˳��߳�
#define WM_BN_CLICK						WM_USER+801
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
		string    address ;   //address
		string    RegID   ;   //RegID
		double  fMoney      ;   //���
		int     nColdDig    ;   //�Ƿ�֧�����ڿ� 1:����֧��  �� 0:����֧��
		int     bSign       ;   //�Ƿ�ע��       1:��ע��    ,  0:ûע��
		string    Label       ;
		LISTADDR(){
			address="";
			RegID="";
			Label="";
			fMoney = 0.0;
			nColdDig =0;
			bSign = 0;
		}
		string ToJson(){
			Json::Value root;
			root["address"] = address;
			root["RegID"] = RegID;
			root["fMoney"] = fMoney;
			root["nColdDig"] = nColdDig;
			root["bSign"] = bSign;
			root["Label"] = Label;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;
			address = root["address"].asString();
			
			RegID =  root["RegID"].asString();
			this->fMoney = root["fMoney"].asDouble();
			this->nColdDig = root["nColdDig"].asInt();
			this->bSign = root["bSign"].asInt();

			Label=root["Label"].asString();
			return true;
		}
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
	
	typedef struct P2P_QUIZ_RECORD {
		time_t send_time       ;     //����ʱ��
		time_t recv_time       ;     //����ʱ��
		int    time_out        ;     //��ʱ
		string   tx_hash    ;      //����Լhash
		string   left_addr  ;		//����Լ��ַ
		string   right_addr ;		//���ܶ�Լ��ַ
		double  amount  ;				//���
		char   content[33]    ;       //���� 
		int    actor  ;               // 0 �����Լ 1 ���ܶ�Լ 2 ��ʹ�����Լ���ǽ��ܶ�Լ
		int    confirmed      ;        // �Ƿ���ȷ��
		int    height         ;           //ȷ���߶�
		int    state          ;         //0 ����״̬ 1 �Ӷ�״̬ 2 �Ҷ�״̬ 3 �Ѿ������Ҷ�ʱ�� 4 ���ڽӶ� 5���ڽҶ�
		string   relate_hash;          //�Ӷ�hash
		int    guess_num;      //�Ӳµ�����
		P2P_QUIZ_RECORD(){
			send_time = 0;
			recv_time = 0;
			time_out = 0;
			tx_hash = "";
			left_addr ="";
			right_addr = "";
			amount = 0;
			memset(content, 0, 33);
			actor = 0;
			confirmed =0;
			height = 0;
			state = 0;
			relate_hash = "";
			guess_num = 0;
		}
		string ToJson(){
			Json::Value root;
			root["send_time"] = send_time;
			root["recv_time"] = recv_time;
			root["time_out"] = time_out;
			root["tx_hash"] = tx_hash;
			root["left_addr"] = left_addr;
			root["right_addr"] = right_addr;
			root["amount"] = amount;
			root["content"] = content;
			root["actor"] = actor;
			root["confirmed"] = confirmed;
			root["height"] = height;
			root["relate_hash"] = relate_hash;
			root["state"] = state;
			root["guess_num"] = guess_num;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;

			this->send_time = root["send_time"].asInt() ;
			this->recv_time = root["recv_time"].asInt();
			this->time_out = root["time_out"].asInt();

			tx_hash = root["tx_hash"].asString();
		
			left_addr = root["left_addr"].asString();
	
			right_addr = root["right_addr"].asString();
	
			this->amount = root["amount"].asDouble();

			CString temp = root["content"].asCString();
			memcpy(content,temp,sizeof(content));
	
			this->actor = root["actor"].asInt();
			this->confirmed = root["confirmed"].asInt();
			this->height = root["height"].asInt();

			relate_hash = root["relate_hash"].asString();
			
		
			this->state = root["state"].asInt();
			this->guess_num = root["guess_num"].asInt() ;

			return true;
		}
	}P2P_QUIZ_RECORD_t;

	typedef std::vector<P2P_QUIZ_RECORD_t> P2PBETRECORDLIST ;

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
		string    txhash  ;   //hash
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
		string   regid;
		string   desregid;
		string ToJson(){
			Json::Value root;
			root["hash"] = txhash;
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
			root["confirmHeight"] = confirmedHeight;
			root["confirmedtime"] = confirmedtime;
			root["blockhash"] = blockhash;
			root["state"] = state;
			root["regid"] = regid;
			root["desregid"] = desregid;
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

			this->txhash = root["hash"].asString() ;  
			this->txtype = root["txtype"].asString(); 
			this->ver = root["ver"].asInt();
			this->addr = root["addr"].asString()  ;
			this->height  =root["height"].asInt()   ;

			int pos = -1;
			pos = json.find("money"); 
			if( pos>=0)
			{
				this->money = (root["money"].asDouble()*1.0)/100000000 ;
			}
			
			pos = json.find("pubkey"); 
			if(pos>=0)
			this->pubkey = root["pubkey"].asString();

			pos = json.find("miner_pubkey") ;
			if(pos >=0)
			this->miner_pubkey = root["miner_pubkey"].asString();

			pos = json.find("fees") ;
			if(pos >=0)
			{
				this->fees = (root["fees"].asDouble()*1.0)/100000000 ;
			}
			
			pos = json.find("desaddr") ;
			if(pos >=0)
			this->desaddr =root["desaddr"].asString()  ;

			pos = json.find("Contract") ;
			if(pos >=0)
			this->Contract = root["Contract"] .asString() ;

			pos = json.find("confirmHeight") ;
			if(pos >=0)
			this->confirmedHeight = root["confirmHeight"].asInt() ; 

			pos = json.find("confirmedtime") ;
			if(pos >=0)
			this->confirmedtime = root["confirmedtime"].asInt();  

			pos = json.find("blockhash") ;
			if(pos >=0)
			this->blockhash = root["blockhash"].asString()     ;

			pos = json.find("state") ;
			if(pos >=0)
			this->state = root["state"].asInt();

			pos = json.find("regid");
			if(pos >=0)
				this->regid = root["regid"].asString();

			 pos = json.find("desregid");
			if(pos >=0)
				this->desregid = root["desregid"].asString();

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
		int    tips ;
		INT64  high  ;
		string   hash;
		int connections;
		string ToJson(){
			Json::Value root;
			root["type"] = type;
			root["tips"] = tips;
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
		this->tips = root["tips"].asInt();
		this->high = root["high"].asInt64();
		this->hash = root["hash"].asString();
		this->connections = root["connections"].asInt();
		return true;
	}
	}BLOCKCHANGED_t;

	typedef struct DATABASEINFO  {
		string strSource ;
		string strWhere  ;
		string  strTabName  ;
		string  strcutjson ;
		string ToJson(){
			Json::Value root;
			root["strSource"] = strSource;
			root["strWhere"] = strWhere;
			root["strTabName"] = strTabName;
			root["strcutjson"] = strcutjson;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;
			int pos = json.find("strSource");
			if (pos >= 0)
			this->strSource = root["strSource"].asString();

			this->strWhere = root["strWhere"].asString();
			this->strTabName = root["strTabName"].asString();
			this->strcutjson = root["strcutjson"].asString();
			return true;
		}
	}DATABASEINFO_t;

	typedef struct LISTP2POOL{   
		string   hash ;   //address
		string   sendbetid ;   //RegID
		INT64   nPayMoney;
		int     outheight;
		string ToJson(){
			Json::Value root;
			root["hash"] = hash;
			root["sendbetid"] = sendbetid;
			root["money"] = nPayMoney;
			root["height"] = outheight;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;

			this->hash = root["hash"].asString();
			this->sendbetid = root["data"].asString();
			this->nPayMoney = root["money"].asInt64();
			this->outheight=root["height"].asInt();
			return true;
		}
	}LISTP2POOL_T;
	typedef std::vector<LISTP2POOL_T> P2PLIST ;

	typedef struct MINDLG{   
		string   money ;   //address
		string   unconfirmmoney ;   //RegID
		string   itemcount;
		string   addr1;
		string   addr2;
		string   addr3;
		string   addr4;
		string   addr5;
		MINDLG(){
			money = "" ;   //address
			unconfirmmoney = "";   //RegID
			itemcount = "";
			addr1 = "";
			addr2 = "";
			addr3 = "";
			addr4 = "";
			addr5 = "";
		}
		string ToJson(){
			Json::Value root;
			root["money"] = money;
			root["unconfirmmoney"] = unconfirmmoney;
			root["itemcount"] = itemcount;
			root["addr1"] = addr1;
			root["addr2"] = addr2;
			root["addr3"] = addr3;
			root["addr4"] = addr4;
			root["addr5"] = addr5;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;

			this->money = root["money"].asString();
			this->unconfirmmoney = root["unconfirmmoney"].asString();
			this->itemcount = root["itemcount"].asString();
			this->addr1 = root["addr1"].asString();
			this->addr2 = root["addr2"].asString();
			this->addr3 = root["addr3"].asString();
			this->addr4 = root["addr4"].asString();
			this->addr5 = root["addr5"].asString();
			return true;
		}
	}MINDLG_T;

	typedef struct ADDRBOOK{   
		string    label;   //address
		string    address  ;   //RegID
		ADDRBOOK(){
			label = _T("");
			address = _T("");
		}
		string ToJson(){
			Json::Value root;
			root["address"] = address;
			root["label"] = label;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;
			this->label = root["label"].asString();
			this->address = root["address"].asString();
			return true;
		}
	}ADDRBOOK_t;
	typedef std::vector<ADDRBOOK_t> ADDRBOOKLIST ;

	typedef struct REDPACKETSEND{   
		string    send_hash;   //address
		int        send_time;   //RegID
		double     amount  ;				//���
		int        packet_num;
		string    send_acc_id;
		int        confirm_height;
		int        packet_type;       //1:��ͨ���  2:�������
		REDPACKETSEND(){
			send_hash ="";   //address
			send_time = 0;;   //RegID
			amount = 0.0  ;				//���
			packet_num = 0;
			send_acc_id = "";
			confirm_height = 0;
			packet_type = 0;  
		}
		string ToJson(){
			Json::Value root;
			root["send_hash"] = send_hash;
			root["send_time"] = send_time;
			root["amount"] = amount;
			root["packet_num"] = packet_num;
			root["send_acc_id"] = send_acc_id;
			root["confirm_height"] = confirm_height;
			root["packet_type"] = packet_type;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;
			this->send_hash = root["send_hash"].asString();
			this->send_time = root["send_time"].asInt();
			this->amount = root["amount"].asDouble();
			this->packet_num = root["packet_num"].asInt();
			this->send_acc_id = root["send_acc_id"].asString();
			this->confirm_height = root["confirm_height"].asInt();
			this->packet_type = root["packet_type"].asInt();
			return true;
		}
	}REDPACKETSEND_t;
	typedef std::vector<REDPACKETSEND_t> REDPACKETSENDLIST ;

	typedef struct REDPACKETGRAB{   
		string    send_hash;   //address
		string    grab_hash;
		int        grab_time;   //RegID
		double     lucky_amount  ;				//���
		string    send_acc_id;
		string    grab_acct_id;
		int        confirm_height;
		int        packet_type;       //1:��ͨ���  2:�������
		int        lucky_fortune;     /// 1 ��ͨ 2 ������
		double     total_amount;
		int         total_num;
		REDPACKETGRAB(){
			send_hash ="";   //address
			grab_hash = "";
			grab_time = 0;;   //RegID
			lucky_amount = 0.0  ;				//���
			send_acc_id = "";
			confirm_height = 0;
			packet_type = 0;  
			total_amount = 0.0;
			total_num = 0;
		}
		string ToJson(){
			Json::Value root;
			root["send_hash"] = send_hash;
			root["grab_hash"] = grab_hash;
			root["grab_time"] = grab_time;
			root["lucky_amount"] = lucky_amount;
			root["send_acc_id"] = send_acc_id;
			root["grab_acct_id"] = grab_acct_id;
			root["confirm_height"] = confirm_height;
			root["packet_type"] = packet_type;
			root["lucky_fortune"] = lucky_fortune;
			root["total_amount"] = total_amount;
			root["total_num"] = total_num;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;
			this->send_hash = root["label"].asString();
			this->grab_hash = root["grab_hash"].asString();
			this->grab_time = root["address"].asInt();
			this->lucky_amount = root["lucky_amount"].asDouble();
			this->send_acc_id = root["send_acc_id"].asString();
			this->grab_acct_id = root["grab_acct_id"].asString();
			this->confirm_height = root["confirm_height"].asInt();
			this->packet_type = root["packet_type"].asInt();
			this->lucky_fortune = root["lucky_fortune"].asInt();
			this->total_amount = root["total_amount"].asDouble();
			this->total_num = root["total_amount"].asInt();
			return true;
		}
	}REDPACKETGRAB_t;
	typedef std::vector<REDPACKETGRAB_t> REDPACKETGRABLIST ;

	typedef struct REDPACKETPOOL{   
		string    send_hash;   //address
		double     total_amount  ;				//���
		string    send_acc_id;
		int        packets_num;
		int        packet_type;       //1:��ͨ���  2:�������
		string    message;
		double     average_amout;
		REDPACKETPOOL(){
			send_hash ="";   //address
			total_amount = 0.0  ;				//���
			send_acc_id = "";
			packet_type = 0;  
			average_amout =0.0;
		}
		string ToJson(){
			Json::Value root;
			root["send_hash"] = send_hash;
			root["total_amount"] = total_amount;
			root["send_acc_id"] = send_acc_id;
			root["packets_num"] = packets_num;
			root["packet_type"] = packet_type;
			root["message"] = message;
			root["average_amout"] = average_amout;
			return root.toStyledString();
		}
		bool JsonToStruct(string json){
			Json::Reader reader;  
			Json::Value root; 
			if (!reader.parse(json, root)) 
				return false ;
			this->send_hash = root["label"].asString();
			this->total_amount = root["address"].asDouble();
			this->send_acc_id = root["send_acc_id"].asString();
			this->packets_num = root["packets_num"].asInt();
			this->packet_type = root["packet_type"].asInt();
			this->message = root["message"].asString();
			this->average_amout = root["average_amout"].asDouble();
			return true;
		}
	}REDPACKETPOOL_t;
	typedef std::vector<REDPACKETPOOL_t> REDPACKETPOOLLIST ;

	typedef struct {
		char          sendRedid[6] ;        //// ����Ǯ����id
		INT64         amount;                     //����Ǯ���Ľ��
		short           number;                       /// ����Ǯ���ĸ���
		char          type;                        /// �������
		int          timeout;                       //�����ʱʱ��
		bool          fover;                        // ��ʾ�Ƿ�����
		unsigned char message[200];                 //�����
		short           takennum;                       // ��ʾ�ڼ������
	}RED_PACKETDB;

	typedef struct {
		char regid[6];
		INT64 amount;
	}USER_INFO;

	#define MAX_COUNT       100
	typedef struct {
		RED_PACKETDB dbdata;
		USER_INFO userinfo[MAX_COUNT];
	}RED_DATA;
	typedef std::vector<RED_DATA> RED_DATALIST ;
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


	std::string& trimleft(std::string &s) ;

	std::string& trimright(std::string &s) ;
}
#endif