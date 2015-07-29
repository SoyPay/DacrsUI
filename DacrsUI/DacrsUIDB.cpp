
// DacrsUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"


#include <afxsock.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CDacrsUIApp::UpdateQuizPoolData()
{
	Json::Reader reader;  
	Json::Value root; 
	Json::Value root1;
	m_SqliteDeal.ClearTableData(_T("t_quiz_pool"));
	if(theApp.m_betScritptid != _T(""))
	{
		string strCommand;
		strCommand = strprintf("%s %s %s %s",_T("getscriptvalidedata"),theApp.m_betScritptid,_T("100"),_T("1"));
		string strShowData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		//m_AppID
		if (strShowData.find("key") < 0)
		{
			return;
		}
		if (!reader.parse(strShowData, root)) 
			return;
		int size = root.size();
		for ( int index =0; index < size; ++index )
		{
			string txhash = root[index]["key"].asString();
			txhash = txhash.substr(txhash.length()-64);
			string nValue = root[index]["value"].asString();
			uistruct::DBBET_DATA DBbet;
			memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);

			if (vTemp.size() <=0)
			{
				continue;
			}
			
			memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

			std::vector<unsigned char> vnTemp = CSoyPayHelp::getInstance()->ParseHex(txhash);
			reverse(vnTemp.begin(),vnTemp.end());
			string strTemp = CSoyPayHelp::getInstance()->HexStr(vnTemp);

			/// 查找数据库中此赌约是否正在接赌
			uistruct::P2P_QUIZ_RECORD_t  betrecord;
			string strCond;
			strCond =strprintf(" tx_hash = '%s' ", strTemp.c_str());

			int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(strCond, &betrecord );

			if (nItem != 0 && betrecord.state == 4) ////此赌约正在接赌，只是在block中没有确认
			{
				continue;
			}

			strCommand = strprintf("%s %s",_T("gettxdetail"),strTemp.c_str());
			string strShowData =_T("");
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (strShowData == _T(""))
			{
				return;
			}
			if (!reader.parse(strShowData, root1)) 
				return;
			int confirheight =root1["confirmHeight"].asInt();

			strCommand= strprintf("%s",_T("getinfo"));
			strShowData ="";
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (strShowData == _T(""))
			{
				return;
			}
			if (!reader.parse(strShowData, root1)) 
				return;
			int curheight =root1["blocks"].asInt();
			if(curheight >=(confirheight+500))
				continue;

			if (DBbet.betstate == 0x00)
			{
				std::vector<unsigned char> txTemp = CSoyPayHelp::getInstance()->ParseHex(txhash);
				reverse(txTemp.begin(),txTemp.end());
				string newTxhash =  CSoyPayHelp::getInstance()->HexStr(txTemp);
				string strSourceData;
				strSourceData=strprintf("'%s' , '%s'" , newTxhash.c_str(), nValue.c_str());
				m_SqliteDeal.InsertTableItem(_T("t_quiz_pool") ,strSourceData);
			}
		}
	}
}
void CDacrsUIApp::UpdateAddressData(){
	string strCommand;
	strCommand =strprintf("%s","listaddr");
	string strShowData =_T("");

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
	if (strShowData.find("addr") <0)
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) 
		return  ;

	uistruct::LISTADDR_t listaddr;
	for(unsigned int i = 0; i < root.size(); ++i){
		memset(&listaddr , 0 , sizeof(uistruct::LISTADDR_t));
		//address
		listaddr.address = root[i]["addr"].asString();
		//RegID
		listaddr.RegID = root[i]["regid"].asString();
		//金额
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//是否支持冷挖矿
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//是否注册GetLength();
		string strSign ;
		strSign = strprintf("%s" ,listaddr.RegID.c_str() ) ;
		if ( 3 <= strSign.length() ) {
			listaddr.bSign    = 1 ;
		}else{
			listaddr.bSign   = 0 ;
		}
		string strSourceData,feild;
		feild="address";
		strSourceData = strprintf("%s",listaddr.address.c_str());

		string strCond;
		strCond =strprintf(" address = '%s' ", listaddr.address.c_str());
		uistruct::LISTADDR_t addrsql;
		int item = m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		if (addrsql.address.length() == 0 )
		{
			strSourceData=strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , listaddr.address.c_str() ,listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Label.c_str() ) ;
			if (!m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strSourceData ) )
			{
				TRACE("Insert t_wallet_address error!\n");
			}
			
			string Temp = listaddr.ToJson();
			SendUIMsg((int)WM_UP_NEWADDRESS,Temp.c_str());
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				string strSourceData,strWhere;
				strSourceData = strprintf("reg_id = '%s', money = %.8f ,cold_dig =%d,sign =%d",listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere = strprintf("address = '%s'", listaddr.address.c_str()  ) ;
				if ( !m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}
		
				listaddr.Label=addrsql.Label;
				string Temp = listaddr.ToJson();
				SendUIMsg((int)WM_UP_ADDRESS,Temp.c_str());
				if (listaddr.bSign != addrsql.bSign)
				{
					/// p2pdlg 激活的地址要插入
					SendP2pMsg((int)WM_UP_NEWADDRESS,Temp.c_str());
				}
			}
		}
	}
}

void CDacrsUIApp::UpdateTransaction(string hash){

	string strCommand,strShowData;
	strCommand = strprintf("%s %s",_T("gettxdetail") ,hash.c_str());
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData.find("hash") < 0){		
		return;
	}

	//解析Json数据
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) {		
		return;
	}

	if (strShowData.find("blockhash") >= 0)
	{
		uistruct::REVTRANSACTION_t transcion;
		if (transcion.JsonToStruct(root.toStyledString()))
		{
			string strSourceData,strWhere;
			strSourceData = strprintf("confirm_height = %d , confirmed_time = %d ,block_hash ='%s'" ,transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str() ) ;
			strWhere = strprintf("hash = '%s'" , hash.c_str()) ;
			if ( !m_SqliteDeal.UpdateTableItem(_T("t_transaction") , strSourceData , strWhere ) ){
				TRACE(_T("update t_transaction failed\n"));
			}
		}
	}
}
void CDacrsUIApp::OpenBetRecord(vector<unsigned char> openbet,uistruct::REVTRANSACTION_t transcion)
{
	if(openbet.size()==0)
		return;
	OPEN_DATA Openbet;
	memset(&Openbet , 0 , sizeof(OPEN_DATA));
	memcpy(&Openbet, &openbet[0], sizeof(OPEN_DATA));

	vector<unsigned char>vHash;
	vHash.assign(Openbet.txhash,Openbet.txhash+sizeof(Openbet.txhash));
	reverse(vHash.begin(),vHash.end());
	string hexHash = CSoyPayHelp::getInstance()->HexStr(vHash);
	string strCond;
	strCond = strprintf(" tx_hash='%s' ", hexHash.c_str());
	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_p2p_quiz"), strCond) ;
	if (nItem != 0)
	{
		string strSourceData,strWhere;
		strSourceData = strprintf("comfirmed = %d ,height = %d,content = '%s',state=2" ,transcion.confirmedtime,transcion.confirmedHeight,Openbet.dhash ) ;
		strWhere = strprintf("tx_hash = '%s'" , hexHash.c_str() ) ;
		if ( !m_SqliteDeal.UpdateTableItem("t_p2p_quiz" , strSourceData , strWhere)){
			TRACE(_T("t_p2p_quiz数据更新失败!") );
			LogPrint("INFO","OpenBetRecord 更新失败:%s",hexHash.c_str() );
		}
		LogPrint("OPENBET","发赌约hash:%s  开奖hash:%s",hexHash.c_str(),transcion.txhash);
	}
}

void CDacrsUIApp::OpenBet(CString txhash,BOOL Flag)
{

	string m_addr = "";

	if (!CheckRegIDValid( theApp.m_betScritptid )) return ;

	string conditon;
	conditon = strprintf("tx_hash ='%s'" , txhash );
	uistruct::P2P_QUIZ_RECORD_t pPoolItem;
	int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;

	m_addr = pPoolItem.left_addr;
	if (m_addr == ""|| pPoolItem.state == 2) ///////地址不存在或者已经开奖
	{
		//::MessageBox(NULL,_T("请选择地址") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}
	if (pPoolItem.actor != 0 && pPoolItem.actor != 2)
	{
		return;
	}
	if (pPoolItem.tx_hash.length()== 0)
	{
		//::MessageBox(NULL ,_T("数据库中无此记录") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}

	if (!theApp.IsSyncBlock && Flag)
	{
		//::MessageBox(NULL ,_T("同步未完成,不能发送交易") , _T("提示") , MB_ICONINFORMATION ) ;
		return;
	}


	string strShowData = _T(""),txaccepthash;
	txaccepthash = strprintf("%s",pPoolItem.relate_hash.c_str());
	string sendhash = CSoyPayHelp::getInstance()->GetReverseHash(txhash.GetBuffer());
	string accepthash = CSoyPayHelp::getInstance()->GetReverseHash(txaccepthash);

	string number;
	number.assign(pPoolItem.content,pPoolItem.content+sizeof(pPoolItem.content));
	CP2PBetHelp m_P2PBetHelp;
	string strContractData = m_P2PBetHelp.PacketP2PExposeContract(sendhash,number,accepthash,pPoolItem.time_out);

	string strData = CSoyPayHelp::getInstance()->CreateContractTx( theApp.m_betScritptid,m_addr,strContractData,0,theApp.m_P2PBetCfg.OpenBetnFee,0);
	CSoyPayHelp::getInstance()->SendContacrRpc(strData,strShowData);

	if (strShowData == "")
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root;
	if (!reader.parse(strShowData, root)) 
		return  ;
	BOOL bRes = FALSE ;
	string strTip;
	int pos = strShowData.find("hash");

	if ( pos >=0 ) {
		//插入到交易记录数据库
		string strHash = root["hash"].asString() ;
		CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
		postmsg.SetData(strHash);
		theApp.m_MsgQueue.push(postmsg);
	}else{
		LogPrint("INFO","发赌约hash:%s  开奖失败:%s",txhash,strShowData);
	}

	if ( pos >=0 ) {
		bRes = TRUE ;
		//strTip.Format( _T("恭喜开奖成功!\n%s") , root["hash"].asCString() ) ;
		strTip = "恭喜开奖成功，请等待1-2分钟确认交易\n" ;
	}else{
		strTip = "余额不足,请充足!" ;
	}

	//保存到数据库
	if ( bRes ) {

		//插入到交易记录数据库

		//// 查找数据库中是否存在此记录
		string conditon;
		conditon = strprintf("tx_hash ='%s'" , txhash );
		uistruct::P2P_QUIZ_RECORD_t pPoolItem;
		int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(conditon ,&pPoolItem ) ;
		if (pPoolItem.tx_hash.length()!= 0) ///此记录不存在,插入记录
		{
			string strSourceData  , strW ;
			strSourceData = strprintf("state = %d" , 5  ) ;
			strW = strprintf("tx_hash = '%s'" , txhash ) ;

			uistruct::DATABASEINFO_t DatabaseInfo;
			DatabaseInfo.strSource = strSourceData;
			DatabaseInfo.strWhere = strW ;
			DatabaseInfo.strTabName = _T("p2p_bet_record");
			CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			string strtemp = DatabaseInfo.ToJson();

			postmsg.SetData(strtemp);
			theApp.m_MsgQueue.push(postmsg);


		}
	}
	//::MessageBox(NULL,strTip , _T("提示") , MB_ICONINFORMATION ) ;
}
void CDacrsUIApp::AcceptBetRecord(vector<unsigned char> acceptbet,uistruct::REVTRANSACTION_t transcion)
{
	if(acceptbet.size()==0)
		return;
	ACCEPT_DATA acceptcbet;
	memset(&acceptcbet , 0 , sizeof(ACCEPT_DATA));
	memcpy(&acceptcbet, &acceptbet[0], sizeof(ACCEPT_DATA));

	std::vector<unsigned char> txTemp;
	txTemp.assign(acceptcbet.txhash,acceptcbet.txhash+sizeof(acceptcbet.txhash));
	reverse(txTemp.begin(),txTemp.end());
	string SendTxhash =  CSoyPayHelp::getInstance()->HexStr(txTemp);


	SYSTEMTIME curTime =UiFun::Time_tToSystemTime(transcion.confirmedtime);
	string strTime;
	strTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

	string strCond,strField;
	strCond = strprintf(" tx_hash='%s' ", SendTxhash.c_str());
	
	strField +=strprintf(" right_addr = '%s' ,",transcion.regid.c_str() );
	strField+=strprintf("recv_time = '%s' ,height = %d ,state = %d ,relate_hash = '%s' ,guess_num = %d " ,strTime ,transcion.confirmedHeight ,1 ,transcion.txhash ,(int)acceptcbet.data) ;

	int item = m_SqliteDeal.GetTableCountItem(_T("t_p2p_quiz"),strCond);
	if (item != 0)
	{
		//更新数据
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , SendTxhash.c_str() );
			LogPrint("INFO","AcceptBetRecord 更新失败:%s",SendTxhash.c_str());
		}
	}else
	{
		string addr;
		addr = strprintf("address='%s'",transcion.addr.c_str());
		 item = m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),addr);
		 if (item != 0)
		 {
			 string strCommand = _T("");
			 strCommand = strprintf("%s %s",_T("gettxdetail"),SendTxhash.c_str());
			 string strShowData =_T("");
			 CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			 if (strShowData == "")
			 {
				 return;
			 }
			 uistruct::REVTRANSACTION_t sendtx;
			 sendtx.JsonToStruct(strShowData);

			 curTime =UiFun::Time_tToSystemTime(sendtx.confirmedtime);
			 string sendtrTime;
			 sendtrTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

			 string nValue = sendtx.Contract;
			 std::vector<unsigned char> vSendTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);

			 SEND_DATA sendbetdata;
			 memset(&sendbetdata,0,sizeof(SEND_DATA));
			 memcpy(&sendbetdata, &vSendTemp[0],sizeof(SEND_DATA));

			 double money = (acceptcbet.money*1.0)/COIN;
			 //插入新数据
			 string strSourceData ;
			 strSourceData = strprintf("'%s','%s','%d','%s' , '%s' , '%s' , '%lf'" , \
				 sendtrTime, strTime , sendbetdata.hight , \
				 SendTxhash.c_str() ,  sendtx.regid , transcion.regid ,money);

			 strSourceData +=strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d'","" ,1 ,0 ,transcion.confirmedHeight ,1 ,\
				 transcion.txhash ,(int)acceptcbet.data ) ;
			 if ( !m_SqliteDeal.InsertTableItem(_T("t_p2p_quiz") ,strSourceData)) {
				 TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , SendTxhash.c_str() );
				 LogPrint("INFO","AcceptBetRecord 插入失败:%s",SendTxhash.c_str());
			 }
		 }
	}


	OpenBet(SendTxhash.c_str(),FALSE);
}
void CDacrsUIApp::SendBetRecord(vector<unsigned char> sendbet,uistruct::REVTRANSACTION_t transcion)
{
	if(sendbet.size()==0)
		return;

	SEND_DATA sendbetdata;
	memset(&sendbetdata,0,sizeof(SEND_DATA));
	memcpy(&sendbetdata, &sendbet[0],sizeof(SEND_DATA));

	SYSTEMTIME curTime =UiFun::Time_tToSystemTime(transcion.confirmedtime);
	string strTime;
	strTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

	string strCond,strField;
	strCond = strprintf(" tx_hash='%s' ", transcion.txhash.c_str());
	strField += strprintf("send_time='%s',height = %d" ,strTime ,transcion.confirmedHeight ) ;

	/// 查找数据库中是否存在此记录
	int item = m_SqliteDeal.GetTableCountItem(_T("t_p2p_quiz"),strCond);
	if (item != 0)
	{
		//更新数据
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash.c_str() );
			LogPrint("INFO","SendBetRecord 更新失败:%s",transcion.txhash.c_str() );
		}
	}
	
}
void CDacrsUIApp::UpdateAppRecord(string txdetail){
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(txdetail))
	{
		
		if (!strcmp(transcion.desregid.c_str(),theApp.m_betScritptid.c_str()))
		{
			string nValue = transcion.Contract;
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
			if (vTemp[0] == TX_OPENBET)
			{
				OpenBetRecord(vTemp,transcion);
			}else if (vTemp[0] == TX_ACCEPTBET)
			{
				AcceptBetRecord(vTemp,transcion);
			}else if (vTemp[0] == TX_SENDBET)
			{
				SendBetRecord(vTemp,transcion);
			}
		}else  if (!strcmp(transcion.desregid.c_str(),theApp.m_redPacketScriptid.c_str())){ //// 接龙红包	
			string nValue = transcion.Contract;
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
			if (vTemp[0] == TX_COMM_SENDREDPACKET)
			{
				SendRePacketCommtRecord(vTemp,transcion);

			}else if (vTemp[0] == TX_COMM_ACCEPTREDPACKET)
			{
				AcceptRePacketCommtRecord(vTemp,transcion);
			}else if (vTemp[0] == TX_SPECIAL_SENDREDPACKET)
			{
				SendRePacketSpecailRecord(vTemp,transcion);
			}else if (vTemp[0] == TX_SPECIAL_ACCEPTREDPACKET)
			{
				AcceptRePacketSpecailRecord(vTemp,transcion);
			}
		}
	}
}
void CDacrsUIApp::InsertTransaction(string hash){
	string strCommand,strShowData;
	strCommand = strprintf("%s %s",_T("gettxdetail") ,hash.c_str() );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData.find("hash") < 0){		
		return;
	}

	//解析Json数据
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData, root)) {		
		return;
	}

	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(root.toStyledString()))
	{

		if (!strcmp("COMMON_TX",transcion.txtype.c_str()))
		{
			string conditon;
			conditon = strprintf("address = '%s'",transcion.addr.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			conditon = strprintf("address = '%s'",transcion.desaddr.c_str());
			int nItem1 =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			if (nItem1 !=0&&nItem != 0)
			{
				transcion.state = 3; 
			}else if (nItem != 0)
			{
				transcion.state = 1;        //// 扣钱
			}else if (nItem1 != 0)
			{
				transcion.state = 2;        ////加钱
			}
		}



		string strSourceData;
		strSourceData = strprintf("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d" , transcion.txhash.c_str() ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		m_SqliteDeal.InsertTableItem(_T("t_transaction") ,strSourceData ) ;

		//// 数据插入了，更新到交易详情界面
		CPostMsg Postmsg(MSG_USER_TRANSRECORD_UI,WM_INSERT);
		string temp =  transcion.ToJson();
		Postmsg.SetData(temp.c_str());	
		m_UiTxDetailQueue.push(Postmsg);
		DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_TRANSRECORD_UI ,WM_INSERT,0);
	}
}
void  CDacrsUIApp::InsertAddbook(uistruct::ADDRBOOK_t addr)
{
	string strSourceData;
	strSourceData = strprintf("'%s' , '%s' " , addr.label.c_str(), addr.address.c_str() ) ;
	m_SqliteDeal.InsertTableItem(_T("t_address_book") ,strSourceData ) ;
}
void  CDacrsUIApp::UpdateAddbook(uistruct::ADDRBOOK_t addr)
{
	string strSourceData,strWhere;
	strSourceData = strprintf("Label = '%s'" ,addr.label.c_str() ) ;
	strWhere =strprintf("address = '%s'" ,addr.address.c_str() ) ;
	if ( !m_SqliteDeal.UpdateTableItem(_T("t_address_book") , strSourceData , strWhere)){
		TRACE(_T("t_address_book数据更新失败!") );
	}
}
void CDacrsUIApp:: SyncTransaction(string obj)
{
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(obj))
	{
		/*CString strCond;
		strCond.Format(_T(" hash = '%s' "), transcion.txhash);
		int txitem = theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , strCond);
		if (txitem != 0)
		{
			return;
		}*/

		transcion.state = 0;
		if (!strcmp("COMMON_TX",transcion.txtype.c_str()))
		{
			string conditon;
			conditon = strprintf("address = '%s'",transcion.addr.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			conditon = strprintf("address = '%s'",transcion.desaddr.c_str());
			int nItem1 =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			if (nItem1 !=0&&nItem != 0)
			{
				transcion.state = 3; 
			}else if (nItem != 0)
			{
				transcion.state = 1;        //// 扣钱
			}else if (nItem1 != 0)
			{
				transcion.state = 2;        ////加钱
			}
		}

		string strSourceData;
		strSourceData = strprintf("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d", transcion.txhash.c_str() ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		m_SqliteDeal.InsertTableItem(_T("t_transaction") ,strSourceData );
	}
}
void CDacrsUIApp::ClearTransaction()
{
	m_SqliteDeal.ClearTableData(_T("t_transaction"));
}


void CDacrsUIApp::UpdateRedPacketPoolData()
{
	Json::Reader reader;  
	Json::Value root; 
	Json::Value root1;
	m_SqliteDeal.ClearTableData(_T("t_red_packets_pool"));
	if(theApp.m_redPacketScriptid != _T(""))
	{
		string strCommand;
		strCommand = strprintf("%s %s %s %s",_T("getscriptvalidedata"),theApp.m_redPacketScriptid,"100","1");
		string strShowData;		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		//m_AppID
		if (strShowData.find("key") < 0)
		{
			return;
		}
		if (!reader.parse(strShowData, root)) 
			return;
		int size = root.size();
		for ( int index =0; index < size; ++index )
		{
			string txhash = root[index]["key"].asString();
			txhash = txhash.substr(txhash.length()-64);
			string nValue = root[index]["value"].asString();
			uistruct::RED_DATA redPacket;
			memset(&redPacket , 0 , sizeof(uistruct::RED_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);

			if (vTemp.size() <0)
			{
				continue;
			}
			memcpy(&redPacket, &vTemp[0], sizeof(uistruct::RED_DATA));

			std::vector<unsigned char> txTemp = CSoyPayHelp::getInstance()->ParseHex(txhash);
			reverse(txTemp.begin(),txTemp.end());
			string newTxhash =  CSoyPayHelp::getInstance()->HexStr(txTemp);

			strCommand = strprintf("%s %s",_T("gettxdetail"),newTxhash.c_str());
			string strShowData = "";
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (strShowData == _T(""))
			{
				return;
			}
			if (!reader.parse(strShowData, root1)) 
				return;
			int confirheight =root1["confirmHeight"].asInt();

			if(blocktipheight >=(confirheight+1440))
				continue;

			if (redPacket.dbdata.fover== 0x00)
			{
				std::vector<unsigned char> vSendid;
				vSendid.assign(redPacket.dbdata.sendRedid,redPacket.dbdata.sendRedid+sizeof(redPacket.dbdata.sendRedid));
				string regid = CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);

				double money = (redPacket.dbdata.amount*1.0)/COIN;

				double ava_money = money/redPacket.dbdata.number;
				int packetype = redPacket.dbdata.type;
				if (packetype == 3)
				{
					packetype = 2;
				}
				string strSourceData;
				strSourceData =strprintf("'%s' , '%s','%lf','%d','%d','%s','%lf'" , newTxhash.c_str(),regid, money,redPacket.dbdata.number,packetype,redPacket.dbdata.message,ava_money);
				m_SqliteDeal.InsertTableItem(_T("t_red_packets_pool") ,strSourceData);
			}
		}
	}
}

void CDacrsUIApp::AcceptRePacketCommtRecord(vector<unsigned char> acceptRedPacket,uistruct::REVTRANSACTION_t transcion){

	if(acceptRedPacket.size()==0)
		return;

	string strCond,strField;
	strCond =strprintf(" grab_hash='%s' ", transcion.txhash.c_str());
	//strField.AppendFormat(_T("grab_time=%d,confirm_height = %d,lucky_fortune = 1") ,transcion.confirmedtime ,transcion.confirmedHeight ) ;

	/// 查找数据库中是否存在此记录
	//int item = m_SqliteDeal.GetTableCountItem(_T("t_red_packets_grab"),strCond);
	//if (item != 0)
	//{
	//	//更新数据
	//	if ( !m_SqliteDeal.UpdateTableItem(_T("t_red_packets_grab") ,strField,strCond )) {
	//		TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash );
	//	}
	//}

	int item = m_SqliteDeal.GetTableCountItem(_T("t_red_packets_grab"),strCond);
	if (item != 0)
	{
		ACCEPT_RED_PACKET acceptPacket;
		memset(&acceptPacket , 0 , sizeof(ACCEPT_RED_PACKET));
		memcpy(&acceptPacket, &acceptRedPacket[0], sizeof(ACCEPT_RED_PACKET));

		vector<unsigned char>vHash;
		vHash.assign(acceptPacket.redhash,acceptPacket.redhash+sizeof(acceptPacket.redhash));
		reverse(vHash.begin(),vHash.end());
		string SendHash = CSoyPayHelp::getInstance()->HexStr(vHash);

		string strCommand,strShowData;
		strCommand = strprintf("%s %s","gettxdetail" ,SendHash.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == "")
		{
			return;
		}
		Json::Reader reader; 
		Json::Value root;
		if (!reader.parse(strShowData, root)) 
			return;
		int npos = strShowData.find("confirmHeight");
		int confirHeight = 1440;
		if ( npos >= 0 ) { //
			confirHeight += root["confirmHeight"].asInt() ;    //交易被确认的高度
		}

		CSoyPayHelp::getInstance()->revert((char*)&confirHeight);

		char key[36];
		memset(key,0,36);

		memcpy(key,&confirHeight,sizeof(confirHeight));
		memcpy(&key[4],acceptPacket.redhash,sizeof(acceptPacket.redhash));

		vHash.assign(key,key+sizeof(key));
		string strKeyHex = CSoyPayHelp::getInstance()->HexStr(vHash);
		strCommand = strprintf("%s %s %s","getscriptdata" ,m_redPacketScriptid,strKeyHex.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == "" || strShowData.find("value") < 0)
			return;

		if (!reader.parse(strShowData, root)) 
			return;

		string nValue = root["value"].asString();
		uistruct::RED_DATA redPacket;
		memset(&redPacket , 0 , sizeof(uistruct::RED_DATA));
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
		if (vTemp.size() <=0)
		{
			return;
		}
		memcpy(&redPacket, &vTemp[0], sizeof(uistruct::RED_DATA));

		string strCond;
		strCond =strprintf(" address = '%s' ", transcion.addr.c_str());
		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		if (addrsql.address.length() == 0 )
		{
			return;
		}
		string regid = CSoyPayHelp::getInstance()->GetFullRegID(addrsql.RegID);
		INT64 GetAmout = GetRedPacketAmount(regid.c_str(),redPacket);
		double money = (GetAmout*1.0)/COIN;

		//更新数据
		strField =strprintf("grab_time=%d,confirm_height = %d,lucky_fortune = 1,lucky_amount=%lf" ,transcion.confirmedtime ,transcion.confirmedHeight,money ) ;
		strCond =strprintf(" grab_hash='%s' ", transcion.txhash.c_str());
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_red_packets_grab") ,strField,strCond )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash.c_str() );
			LogPrint("INFO","AcceptRePacketCommtRecord 更新失败:%s",transcion.txhash.c_str());
		}
	}
}
void CDacrsUIApp::SendRePacketCommtRecord(vector<unsigned char> sendRedPacket,uistruct::REVTRANSACTION_t transcion){
	if(sendRedPacket.size()==0)
		return;

	string strCond,strField;
	strCond = strprintf(" send_hash='%s' ", transcion.txhash.c_str());
	strField +=strprintf("send_time=%d,confirm_height = %d" ,transcion.confirmedtime ,transcion.confirmedHeight ) ;

	/// 查找数据库中是否存在此记录
	int item = m_SqliteDeal.GetTableCountItem(_T("t_red_packets_send"),strCond);
	if (item != 0)
	{
		//更新数据
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_red_packets_send") ,strField,strCond )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash.c_str() );
			LogPrint("INFO","SendRePacketCommtRecord 更新失败:%s",transcion.txhash.c_str());
		}
	}
}
INT64 CDacrsUIApp::GetRedPacketAmount(CString acceptregid,uistruct::RED_DATA redPacket)
{

	for (int i =0;i <redPacket.dbdata.takennum;i++)
	{
		uistruct::USER_INFO userinfo = redPacket.userinfo[i];
		std::vector<unsigned char> vSendid;
		vSendid.assign(userinfo.regid,userinfo.regid+sizeof(userinfo.regid));
		string regid = CSoyPayHelp::getInstance()->HexStr(vSendid);
		if (strcmp(regid.c_str(),acceptregid) == 0)
		{
			return userinfo.amount;
		}
	}

	return 0;
}
bool CDacrsUIApp::IsLuckyRedPacket(CString acceptregid,uistruct::RED_DATA redPacket)
{
	uistruct::USER_INFO userinfo = redPacket.userinfo[0];
	for (int i =1;i <redPacket.dbdata.number;i++)
	{
		if (userinfo.amount < redPacket.userinfo[i].amount)
		{
			userinfo = redPacket.userinfo[i];
		}
	}
	std::vector<unsigned char> vSendid;
	vSendid.assign(userinfo.regid,userinfo.regid+sizeof(userinfo.regid));
	string regid = CSoyPayHelp::getInstance()->HexStr(vSendid);
	if (strcmp(regid.c_str(),acceptregid) == 0)
	{
		return true;
	}
	return false;
}
void CDacrsUIApp::AcceptRePacketSpecailRecord(vector<unsigned char> acceptRedPacket,uistruct::REVTRANSACTION_t transcion){
	if(acceptRedPacket.size()==0)
		return;

	string strCond,strField;
	strCond = strprintf(" grab_hash='%s' ", transcion.txhash.c_str());

	/// 查找数据库中是否存在此记录
	int item = m_SqliteDeal.GetTableCountItem(_T("t_red_packets_grab"),strCond);
	if (item != 0)
	{
		ACCEPT_RED_PACKET acceptPacket;
		memset(&acceptPacket , 0 , sizeof(ACCEPT_RED_PACKET));
		memcpy(&acceptPacket, &acceptRedPacket[0], sizeof(ACCEPT_RED_PACKET));

		vector<unsigned char>vHash;
		vHash.assign(acceptPacket.redhash,acceptPacket.redhash+sizeof(acceptPacket.redhash));
		reverse(vHash.begin(),vHash.end());
		string SendHash = CSoyPayHelp::getInstance()->HexStr(vHash);

		string strCommand,strShowData;
		strCommand = strprintf("%s %s","gettxdetail",SendHash.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == "")
		{
			return;
		}
		Json::Reader reader; 
		Json::Value root;
		if (!reader.parse(strShowData, root)) 
			return;
		int npos = strShowData.find("confirmHeight");
		int confirHeight = 1440;
		if ( npos >= 0 ) { //
			confirHeight += root["confirmHeight"].asInt() ;    //交易被确认的高度
		}

		CSoyPayHelp::getInstance()->revert((char*)&confirHeight);

		char key[36];
		memset(key,0,36);

		memcpy(key,&confirHeight,sizeof(confirHeight));
		memcpy(&key[4],acceptPacket.redhash,sizeof(acceptPacket.redhash));

		vHash.assign(key,key+sizeof(key));
		string strKeyHex = CSoyPayHelp::getInstance()->HexStr(vHash);
		strCommand = strprintf("%s %s %s","getscriptdata" ,m_redPacketScriptid,strKeyHex.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData == _T("") || strShowData.find("value") < 0)
			return;

		if (!reader.parse(strShowData, root)) 
			return;

		string nValue = root["value"].asString();
		uistruct::RED_DATA redPacket;
		memset(&redPacket , 0 , sizeof(uistruct::RED_DATA));
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
		if (vTemp.size() <=0)
		{
			return;
		}
		memcpy(&redPacket, &vTemp[0], sizeof(uistruct::RED_DATA));

		string strCond;
		strCond = strprintf(" address = '%s' ", transcion.addr.c_str());
		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		if (addrsql.address.length() == 0 )
		{
			return;
		}
		string regid = CSoyPayHelp::getInstance()->GetFullRegID(addrsql.RegID);
		INT64 GetAmout = GetRedPacketAmount(regid.c_str(),redPacket);
		double money = (GetAmout*1.0)/COIN;
		if (IsLuckyRedPacket(regid.c_str(),redPacket))
		{
			strCond = strprintf(" grab_hash='%s' ", transcion.txhash.c_str());
			strField = strprintf("grab_time=%d,confirm_height = %d,lucky_fortune = 2,lucky_amount=%lf" ,transcion.confirmedtime ,transcion.confirmedHeight, money) ;
			if ( !m_SqliteDeal.UpdateTableItem(_T("t_red_packets_grab") ,strField,strCond )) {
				TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash.c_str() );
				LogPrint("INFO","AcceptRePacketSpecailRecord 更新失败:%s",transcion.txhash.c_str());
			}
		}
		else{
			//更新数据
			strField = strprintf("grab_time=%d,confirm_height = %d,lucky_fortune = 1,lucky_amount=%lf" ,transcion.confirmedtime ,transcion.confirmedHeight,money ) ;
			strCond = strprintf(" grab_hash='%s' ", transcion.txhash.c_str());
			if ( !m_SqliteDeal.UpdateTableItem(_T("t_red_packets_grab") ,strField,strCond )) {
				TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash.c_str() );
					LogPrint("INFO","AcceptRePacketSpecailRecord 更新失败:%s",transcion.txhash.c_str());
			}
		}
	}
}
void CDacrsUIApp::SendRePacketSpecailRecord(vector<unsigned char> sendRedPacket,uistruct::REVTRANSACTION_t transcion){
	if(sendRedPacket.size()==0)
		return;

	string strCond,strField;
	strCond = strprintf(" send_hash='%s' ", transcion.txhash.c_str());
	strField = strprintf("send_time=%d,confirm_height = %d" ,transcion.confirmedtime ,transcion.confirmedHeight ) ;

	/// 查找数据库中是否存在此记录
	int item = m_SqliteDeal.GetTableCountItem(_T("t_red_packets_send"),strCond);
	if (item != 0)
	{
		//更新数据
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_red_packets_send") ,strField,strCond )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash.c_str() );
				LogPrint("INFO","SendRePacketSpecailRecord 更新失败:%s",transcion.txhash.c_str());
		}
	}
}