
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
		int requiredCount = 100;
		int index = 1;
		while(TRUE)
		{
			string strCommand;
			strCommand = strprintf("%s %s %d %d",_T("getscriptvalidedata"),theApp.m_betScritptid,requiredCount,index);
			index++;


			if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
			{
				TRACE("UpdateQuizPoolData rpccmd getscriptvalidedata error");
				return;
			}
			string str = root.toStyledString();
			int size = root.size();
			//LogPrint("INFO","Size:%d\r\n",size);
			for ( int index =0; index < size; ++index )
			{
				string txhash = root[index]["key"].asString();
				//LogPrint("INFO","index:%d,%s\r\n",index,txhash);
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

				if (DBbet.betstate == 0x00 || DBbet.betstate == 0x01)
				{
					std::vector<unsigned char> vSendid;
					vSendid.assign(DBbet.sendbetid,DBbet.sendbetid+sizeof(DBbet.sendbetid));
					string regid = CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);

					string acceptid = "";
					string accepthash = "";
					if (DBbet.betstate == 0x01)
					{
						vSendid.assign(DBbet.acceptbetid,DBbet.acceptbetid+sizeof(DBbet.acceptbetid));
						acceptid = CSoyPayHelp::getInstance()->GetNotFullRegID(vSendid);

						std::vector<unsigned char> vaccept;
						vaccept.assign(DBbet.accepthash,DBbet.accepthash+sizeof(DBbet.accepthash));
						reverse(vaccept.begin(),vaccept.end());
						accepthash = CSoyPayHelp::getInstance()->HexStr(vaccept);

					}
					

					string strSourceData;
					strSourceData=strprintf("'%s' , '%s',%ld,%ld,%d,'%s','%s',%d,%d" , strTemp.c_str(), regid.c_str(),DBbet.money,DBbet.accept_money,DBbet.hight,acceptid,accepthash,DBbet.acceptebetdata,DBbet.betstate);
					m_SqliteDeal.InsertTableItem(_T("t_quiz_pool") ,strSourceData);
				}
			}

			if ((int)root.size() < requiredCount ||(int)root.size()>requiredCount )
			{
				break;
			}
		}
	}

	////////////////////通知更新赌约数据界面
	m_readQuizPool = TRUE;
	//::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_REFRESHP2PUI,0,0);

}
void CDacrsUIApp::UpdateAddressData(){
	string strCommand;
	strCommand =strprintf("%s","listaddr");

	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateAddressData rpccmd listaddr error");
		return;
	}

	
	for(unsigned int i = 0; i < root.size(); ++i){
		//memset(&listaddr , 0 , sizeof(uistruct::LISTADDR_t));
		uistruct::LISTADDR_t listaddr;
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
				SendUIMsg((int)WM_UP_ADDRESS,Temp);
				//if (listaddr.bSign != addrsql.bSign)
				//{
				//	/// p2pdlg 激活的地址要插入
				//	SendP2pMsg((int)WM_UP_NEWADDRESS,Temp.c_str());
				//}
			}
		}
	}
}

void CDacrsUIApp::UpdateTransaction(string hash){

	string strCommand;
	strCommand = strprintf("%s %s",_T("gettxdetail") ,hash.c_str());
	Json::Value root;
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("UpdateTransaction rpccmd gettxdetail error");
		return;
	}

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
	PopupCommBalloonTip(hash);
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
			LogPrint("INFO","OpenBetRecord 更新失败:%s\r\n",hexHash.c_str() );
		}
		LogPrint("OPENBET","发赌约hash:%s  开奖hash:%s\r\n",hexHash.c_str(),transcion.txhash);
		PopupContactBalloonTip(transcion,0,1);
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
		return;
	}
	if (pPoolItem.actor != 0 && pPoolItem.actor != 2)
	{
		return;
	}
	if (pPoolItem.tx_hash.length()== 0)
	{
		return;
	}

	if (!theApp.IsSyncBlock && Flag)
	{
		return;
	}


	string strShowData = _T(""),txaccepthash;
	txaccepthash = strprintf("%s",pPoolItem.relate_hash.c_str());
	string sendhash = CSoyPayHelp::getInstance()->GetReverseHash(txhash.GetBuffer());
	txhash.ReleaseBuffer(txhash.GetLength());
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
		LogPrint("INFO","发赌约hash:%s  开奖失败:%s\r\n",txhash,strShowData);
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
			DatabaseInfo.strTabName = _T("t_p2p_quiz");
			CPostMsg postmsg(MSG_USER_UPDATA_DATA,0);
			string strtemp = DatabaseInfo.ToJson();

			postmsg.SetData(strtemp);
			theApp.m_MsgQueue.push(postmsg);


		}
	}
	
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
	strField +=strprintf(" ,accept_amount = %lf ",(acceptcbet.money*1.0)/COIN );

	int item = m_SqliteDeal.GetTableCountItem(_T("t_p2p_quiz"),strCond);
	if (item != 0)
	{
		//更新数据
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , SendTxhash.c_str() );
			LogPrint("INFO","AcceptBetRecord 更新失败:%s\r\n",SendTxhash.c_str());
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
			 Json::Value root;
			 if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
			 {
				 TRACE("AcceptBetRecord rpccmd gettxdetail error");
				 return;
			 }
			 strShowData = root.toStyledString();
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

			 strSourceData +=strprintf(",'%s' ,'%d','%d','%d','%d','%s','%d','0',","" ,1 ,0 ,transcion.confirmedHeight ,1 ,\
				 transcion.txhash ,(int)acceptcbet.data ) ;
			 if ( !m_SqliteDeal.InsertTableItem(_T("t_p2p_quiz") ,strSourceData)) {
				 TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , SendTxhash.c_str() );
				 LogPrint("INFO","AcceptBetRecord 插入失败:%s\r\n",SendTxhash.c_str());
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
			LogPrint("INFO","SendBetRecord 更新失败:%s\r\n",transcion.txhash.c_str() );
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
			}else{
				string strCond;
				strCond = strprintf(" address='%s' ", transcion.addr);
				int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"), strCond) ;
				if (nItem != 0)
				{
					PopupContactBalloonTip(transcion,0,5);
				}
				
			}
		}else  if (!strcmp(transcion.desregid.c_str(),theApp.m_redPacketScriptid.c_str())){ //// 接龙红包	
			string nValue = transcion.Contract;
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
			if (vTemp[0] == TX_COMM_SENDREDPACKET)
			{
				SendRePacketCommtRecord(vTemp,transcion);
				PopupContactBalloonTip(transcion,1,1);

			}else if (vTemp[0] == TX_COMM_ACCEPTREDPACKET)
			{
				AcceptRePacketCommtRecord(vTemp,transcion);
				PopupContactBalloonTip(transcion,1,2);
			}else if (vTemp[0] == TX_SPECIAL_SENDREDPACKET)
			{
				SendRePacketSpecailRecord(vTemp,transcion);
				PopupContactBalloonTip(transcion,1,3);
			}else if (vTemp[0] == TX_SPECIAL_ACCEPTREDPACKET)
			{
				AcceptRePacketSpecailRecord(vTemp,transcion);
				PopupContactBalloonTip(transcion,1,4);
			}else{
				PopupContactBalloonTip(transcion,1,5);
			}
		}else if (!strcmp(transcion.desregid.c_str(),theApp.m_ipoScritptid.c_str()))//// ipo领币
		{
			PopupContactBalloonTip(transcion,2,5);
		}
	}
}
void CDacrsUIApp::InsertTransaction(string hash){
	string strCommand,strShowData;
	strCommand = strprintf("%s %s",_T("gettxdetail") ,hash.c_str() );
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("InsertTransaction rpccmd gettxdetail error");
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
				transcion.state = 3;      /// 平
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
		PopupCommBalloonTip(transcion.txhash);
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

		PopupCommBalloonTip(transcion.txhash);
	}
}
void CDacrsUIApp::DeleteItemTransaction(int height)
{
	string strCondition =strprintf("confirm_height >=%d or confirm_height=0",height);
	m_SqliteDeal.DeleteTableItem(_T("t_transaction"),strCondition);
}


void CDacrsUIApp::UpdateRedPacketPoolData()
{
	Json::Reader reader;  
	Json::Value root; 
	Json::Value root1;
	m_SqliteDeal.ClearTableData(_T("t_red_packets_pool"));
	if(theApp.m_redPacketScriptid != _T(""))
	{
		int requiredCount = 100;
		int index = 1;
		while(TRUE)
		{
			string strCommand;
			strCommand = strprintf("%s %s %s %d",_T("getscriptvalidedata"),theApp.m_redPacketScriptid,requiredCount,index);
		      index++;

			if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
			{
				TRACE("UpdateRedPacketPoolData rpccmd getscriptvalidedata error");
				return;
			}
		
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

				if (redPacket.dbdata.fover== 0x00)
				{
					std::vector<unsigned char> txTemp = CSoyPayHelp::getInstance()->ParseHex(txhash);
					reverse(txTemp.begin(),txTemp.end());
					string newTxhash =  CSoyPayHelp::getInstance()->HexStr(txTemp);

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
			if ((int)root.size() < requiredCount ||(int)root.size()>requiredCount )
			{
				break;
			}
		}
	}

	////////////////////通知更新红包池数据界面
	m_readReadCommPacketPool = TRUE;;        
	m_readReadSpecalPacketPool = TRUE;; 
	//::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_REFRESHREDPACKET,0,0);
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
		Json::Value root;
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("AcceptRePacketCommtRecord rpccmd gettxdetail error");
			return;
		}
		strShowData = root.toStyledString();
	
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
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("AcceptRePacketCommtRecord rpccmd getscriptdata error");
			return;
		}

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
			LogPrint("INFO","AcceptRePacketCommtRecord 更新失败:%s\r\n",transcion.txhash.c_str());
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
			LogPrint("INFO","SendRePacketCommtRecord 更新失败:%s\r\n",transcion.txhash.c_str());
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
		Json::Value root;
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("AcceptRePacketSpecailRecord rpccmd gettxdetail error");
			return;
		}
		strShowData = root.toStyledString();
		
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
		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("AcceptRePacketSpecailRecord rpccmd getscriptdata error");
			return;
		}


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
				LogPrint("INFO","AcceptRePacketSpecailRecord 更新失败:%s\r\n",transcion.txhash.c_str());
			}
		}
		else{
			//更新数据
			strField = strprintf("grab_time=%d,confirm_height = %d,lucky_fortune = 1,lucky_amount=%lf" ,transcion.confirmedtime ,transcion.confirmedHeight,money ) ;
			strCond = strprintf(" grab_hash='%s' ", transcion.txhash.c_str());
			if ( !m_SqliteDeal.UpdateTableItem(_T("t_red_packets_grab") ,strField,strCond )) {
				TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash.c_str() );
					LogPrint("INFO","AcceptRePacketSpecailRecord 更新失败:%s\r\n",transcion.txhash.c_str());
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
				LogPrint("INFO","SendRePacketSpecailRecord 更新失败:%s\r\n",transcion.txhash.c_str());
		}
	}
}


void CDacrsUIApp::PopupCommBalloonTip(string hash)//)
{
	string conditon;
	conditon =strprintf("hash='%s'",hash);
	uistruct::REVTRANSACTION_t commtx;
	theApp.m_SqliteDeal.GetTransactionItem(conditon,&commtx);
	if (commtx.txhash == "" || commtx.confirmedHeight ==0 || theApp.m_pMainWnd == NULL)
	{
		return;
	}
	string strShow = "";
	if (strcmp(commtx.txtype.c_str(),"REWARD_TX") == 0)
	{
		//strShow ="  流入交易\r\n";
		strShow =strprintf("  %s\r\n",UiFun::UI_LoadString("DACRSU" , "DACRSU_INPUTTX" ,theApp.gsLanguage));

		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(commtx.confirmedtime);
		strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		strShow += strprintf("%s: +%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),commtx.money);
		strShow += strprintf("%s: %s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_MINING" ,theApp.gsLanguage));
		strShow += strprintf("%s: (%s)\r\n",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage) ,commtx.addr.c_str());
	}else if(strcmp(commtx.txtype.c_str(),"COMMON_TX") == 0 && commtx.state == 1){
		//strShow ="  流出交易\r\n";
		strShow =strprintf("  %s\r\n",UiFun::UI_LoadString("RECEIVE_MODULE" , "DACRSU_OUTTX" ,theApp.gsLanguage));
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(commtx.confirmedtime);
		strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		strShow += strprintf("%s: -%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),commtx.money);
		strShow += strprintf("%s: %s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("COMM_MODULE" , "COMM_TRAN" ,theApp.gsLanguage));
		strShow += strprintf("%s: (%s)\r\n",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage),commtx.addr.c_str());
	}else if(strcmp(commtx.txtype.c_str(),"COMMON_TX") == 0 && commtx.state == 2){
		//strShow ="  流入交易\r\n";
		strShow =strprintf("  %s\r\n",UiFun::UI_LoadString("DACRSU" , "DACRSU_INPUTTX" ,theApp.gsLanguage));
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(commtx.confirmedtime);
		strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		strShow += strprintf("%s: +%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),commtx.money);
		strShow += strprintf("%s: %s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("DACRSU" , "DACRSU_STIPTYPE" ,theApp.gsLanguage));
		strShow += strprintf("%s: (%s)\r\n",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage),commtx.addr.c_str());
	}else if(strcmp(commtx.txtype.c_str(),"COMMON_TX") == 0 && commtx.state == 3){
		//strShow =" 转账交易\r\n";
		strShow =strprintf("  %s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TRANSF_TRANSA" ,theApp.gsLanguage));
		SYSTEMTIME curTime =UiFun::Time_tToSystemTime(commtx.confirmedtime);
		strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
		strShow += strprintf("%s: %.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),commtx.money);
		strShow += strprintf("%s: %s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("DACRSU" , "DACRSU_TIPTYPE" ,theApp.gsLanguage));
		strShow += strprintf("%s: %s\r\n",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage),commtx.addr.c_str());
	}
	if(strShow != "")
	::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_POPUPBAR,0,(LPARAM)strShow.c_str());
}
/// apptype 0 表示猜你妹 1 表示抢红包
//
void CDacrsUIApp::PopupContactBalloonTip(uistruct::REVTRANSACTION_t tx,int apptype,int txtype)
{
	if (theApp.m_pMainWnd == NULL)
	{
		return;
	}

	if (apptype == 1 || apptype == 2 || txtype ==5)     // 红包 ipo领币
	{
		string strCond;
		strCond = strprintf(" address='%s' ", tx.addr);
		int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"), strCond) ;
		if (nItem == 0)
		{
			return;
		}
	}
	string strShow = "";
	if (apptype ==0)
	{
		//strShow = "猜你妹交易";
		strShow =UiFun::UI_LoadString("DACRSU" , "DACRSU_TRANTYPE" ,theApp.gsLanguage);
		if (txtype == 1)  /// 表示开奖
		{
			string nValue = tx.Contract;
			std::vector<unsigned char> openbet = CSoyPayHelp::getInstance()->ParseHex(nValue);
			OPEN_DATA Openbet;
			memset(&Openbet , 0 , sizeof(OPEN_DATA));
			memcpy(&Openbet, &openbet[0], sizeof(OPEN_DATA));

			vector<unsigned char>vHash;
			vHash.assign(Openbet.txhash,Openbet.txhash+sizeof(Openbet.txhash));
			reverse(vHash.begin(),vHash.end());
			string hexHash = CSoyPayHelp::getInstance()->HexStr(vHash);
			string strCond;
			strCond = strprintf(" tx_hash='%s' ", hexHash.c_str());
			uistruct::P2P_QUIZ_RECORD_t pPoolItem;
			theApp.m_SqliteDeal.GetP2PQuizRecordItem(strCond, &pPoolItem) ;
			if (pPoolItem.tx_hash == "")
			{
				return;
			}
			{
				string amount = "";
				string acotor = "";
				string addr = "";
				if (pPoolItem.actor == 0)
				{
					if (pPoolItem.content[32] != pPoolItem.guess_num)
					{
						//strShow +="(恭喜赢了)\r\n" ;
						strShow +=strprintf("(%s)\r\n",UiFun::UI_LoadString("DACRSU" , "DACRSU_WINER" ,theApp.gsLanguage));
						amount=strprintf("%s:+%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),pPoolItem.amount);
					}else{
						//strShow +="(输了)\r\n" ;
						strShow +=strprintf("(%s)\r\n",UiFun::UI_LoadString("DACRSU" , "DACRSU_LOSER" ,theApp.gsLanguage));
						amount=strprintf("%s:-%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),pPoolItem.amount);
					}
					//acotor = "类别:发竞猜\r\n";
					acotor=strprintf("%s:%S\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("DACRSU" , "DACRSU_SENDTYPE" ,theApp.gsLanguage));
					addr= pPoolItem.left_addr;
				}else if (pPoolItem.actor == 1)
				{
					if (pPoolItem.content[32] == pPoolItem.guess_num)
					{
						strShow +=strprintf("(%s)\r\n",UiFun::UI_LoadString("DACRSU" , "DACRSU_WINER" ,theApp.gsLanguage));
						amount=strprintf("%s:+%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),pPoolItem.amount);
					}else{
						strShow +=strprintf("(%s)\r\n",UiFun::UI_LoadString("DACRSU" , "DACRSU_LOSER" ,theApp.gsLanguage));
						amount=strprintf("%s:-%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),pPoolItem.amount);
					}
					//acotor = "类别:接单\r\n";
					acotor=strprintf("%s:%s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_ORDERS" ,theApp.gsLanguage));
					addr= pPoolItem.right_addr;
				}else{
					//strShow +="(持平)\r\n" ;
					strShow +=strprintf("(%s)\r\n",UiFun::UI_LoadString("DACRSU" , "DACRSU_EQUAL" ,theApp.gsLanguage));
					amount=strprintf("%s:%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),pPoolItem.amount);
					//acotor = "类别:即是发竞猜也是接单\r\n";
					acotor=strprintf("%s:%s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("DACRSU" , "DACRSU_ALLTYPE" ,theApp.gsLanguage));
					
					addr= pPoolItem.right_addr;
				}
				SYSTEMTIME curTime =UiFun::Time_tToSystemTime(tx.confirmedtime);
				strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
				strShow += amount;
				strShow += acotor;
				strShow += strprintf("%s:%s",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage),addr);
			}
		}
	}else if (apptype == 1)
	{
		if (txtype ==1 || txtype ==3)
		{
			if(txtype ==1)
			strShow = UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_REDENVELOPE" ,theApp.gsLanguage)+"\r\n";//"发普通红包交易\r\n";
			if(txtype ==3)
			strShow =strShow = UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_SEND_JL_REDENVELOPE" ,theApp.gsLanguage)+"\r\n";// "发接龙红包交易\r\n";

			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(tx.confirmedtime);
			strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

			string nValue = tx.Contract;
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
			RED_PACKET redpacket;
			memset(&redpacket,0,sizeof(RED_PACKET));
			memcpy(&redpacket, &vTemp[0],sizeof(SEND_DATA));

			strShow += strprintf("%s:-%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),(redpacket.money*1.0)/COIN);
			strShow += strprintf("%s:%s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("DACRSU" , "DACRSU_SENDGIFT" ,theApp.gsLanguage));
			strShow += strprintf("%s:%s",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage),tx.regid);
		}else if (txtype ==2 || txtype ==4)
		{
			if(txtype ==2)
				strShow =UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_PT_REDENVELOPE" ,theApp.gsLanguage)+"\r\n";// "抢普通红包交易\r\n";
			if(txtype ==4)
				strShow =UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_GRAB_JL_REDENVELOPE" ,theApp.gsLanguage)+"\r\n";//"抢接龙红包交易\r\n";

			SYSTEMTIME curTime =UiFun::Time_tToSystemTime(tx.confirmedtime);
			strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

			string conditon;
			conditon =strprintf("grab_hash='%s'",tx.txhash);
			uistruct::REDPACKETGRAB_t redpacket;
			theApp.m_SqliteDeal.GetRedPacketGrabItem(conditon,&redpacket);
			if (redpacket.send_hash != "")
			{
				strShow += strprintf("%s:+%.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),redpacket.lucky_amount);
			
			}
			strShow += strprintf("%s:%s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("TITLEBAR_MODULE" , "TITLEBAR_MORTGAGE" ,theApp.gsLanguage));
			strShow += strprintf("%s:%s",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage),tx.regid);
		 }

	}

	if (txtype ==5)
	{
		if (apptype == 0)
		{
			strShow =UiFun::UI_LoadString("DACRSU" , "DACRSU_TRANTYPE" ,theApp.gsLanguage)+"\r\n"; //"猜你妹交易\r\n";
		}else if (apptype == 1)
		{
			strShow = UiFun::UI_LoadString("DACRSU" , "DACRSU_REDTX" ,theApp.gsLanguage)+"\r\n";
		}else if (apptype == 2)
		{
			//strShow = "IPO领币\r\n";
			strShow = UiFun::UI_LoadString("DACRSU" , "DACRSU_IPOTX" ,theApp.gsLanguage)+"\r\n";
		}
		string nValue = tx.Contract;
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
		if (vTemp[0] == 0xff)
		{

			if (vTemp[1] == 0x02)    /// 充值
			{
				SYSTEMTIME curTime =UiFun::Time_tToSystemTime(tx.confirmedtime);
				strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
				strShow+=strprintf("%s:  %.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),tx.money );
				strShow+=strprintf("%s:  %s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("MORTTARD_MODULE" , "MORTTARD_RECHARGE" ,theApp.gsLanguage) );
				strShow+=strprintf("%s:  %s\r\n",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage),tx.regid );
			}else if (vTemp[1] == 0x03 ||vTemp[1] == 0x02 )   /// 提现
			{
				SYSTEMTIME curTime =UiFun::Time_tToSystemTime(tx.confirmedtime);
				strShow += strprintf("%s: %04d-%02d-%02d %02d:%02d:%02d\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_OK_TIME" ,theApp.gsLanguage),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
				if (vTemp.size() == sizeof(APPACC))
				{
					APPACC drawtx;
					memcpy(&drawtx, &vTemp[0],sizeof(APPACC));
					strShow+=strprintf("%s:  %.8f\r\n",UiFun::UI_LoadString("COMM_MODULE" , "COMM_MONEY" ,theApp.gsLanguage),(drawtx.money*1.0)/COIN );
					strShow+=strprintf("%s:  %s\r\n",UiFun::UI_LoadString("TRAD_MODULE" , "TRAD_TYPE" ,theApp.gsLanguage),UiFun::UI_LoadString("IPO_MODULE" , "IPO_WITHDRAWALS" ,theApp.gsLanguage));
					strShow+=strprintf("%s:  %s\r\n",UiFun::UI_LoadString("RECEIVE_MODULE" , "RECEIVE_ADDRESS" ,theApp.gsLanguage),tx.regid );
				}else{
					strShow="";
				}
			}
		}
	}

	::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_POPUPBAR,0,(LPARAM)strShow.c_str());
}

void CDacrsUIApp::ScanQUIZNotAcceptBet()
{
	string strCond;
	strCond = " state =0 and height !=0 and (actor =0 or actor =2)";

	uistruct::P2PBETRECORDLIST pQuizList;
	theApp.m_SqliteDeal.GetP2PQuizRecordList(strCond,&pQuizList);
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::iterator item = pQuizList.begin();
	for (;item!= pQuizList.end();item++)
	{
		strCond = strprintf("hash ='%s' and state=1",item->tx_hash);

		uistruct::LISTP2POOL_T pPoolItem;
		theApp.m_SqliteDeal.GetP2PQuizPoolItem(strCond, &pPoolItem);
		if (pPoolItem.hash.length() != 0)
		{
			string strCommand,strShowData;
			strCommand = strprintf("%s %s",_T("gettxdetail") ,pPoolItem.accepthash );
			Json::Value root; 
			if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
			{
				TRACE("InsertTransaction rpccmd gettxdetail error");
				return;
			}

			uistruct::REVTRANSACTION_t transcion;
			if (transcion.JsonToStruct(root.toStyledString()))
			{
				if (transcion.confirmedHeight == 0)
				 return;

				string strCond,strField;
				strCond = strprintf(" tx_hash='%s' ", item->tx_hash);

				SYSTEMTIME curTime =UiFun::Time_tToSystemTime(transcion.confirmedtime);
				string strTime;
				strTime = strprintf("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

				strField +=strprintf(" right_addr = '%s' ,",transcion.regid.c_str() );
				strField+=strprintf("recv_time = '%s' ,height = %d ,state = %d ,relate_hash = '%s' ,guess_num = %d " ,strTime ,transcion.confirmedHeight ,1 ,transcion.txhash ,(int)pPoolItem.guess) ;
				
				string addr;
				addr = strprintf("address='%s'",transcion.addr.c_str());
				int itemaddr = m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),addr);
				if (itemaddr != 0)
				{
					strField +=",actor =2";
				}
				//更新数据
				if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , item->tx_hash.c_str() );
					LogPrint("INFO","AcceptBetRecord 更新失败:%s\r\n", item->tx_hash.c_str());
				}
			}
		}
		
	}

}
void CDacrsUIApp::InsertAppTransaction(string txdetail)
{
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(txdetail))
	{
		/// 插入配置的相关的id
		map<string,string>::const_iterator item = m_listappid.find(transcion.desregid);
		if (item == m_listappid.end())
		{
			return;
		}
		string strSourceData;
		strSourceData = strprintf("'%s' , '%s' ,'%d' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' ,'%s', '%.8f' ,'%s' ,'%d','%d','%s'" , transcion.txhash.c_str() ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.regid,transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.desregid.c_str(),transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str()) ;
		m_SqliteDeal.InsertTableItem(_T("t_app_transaction") ,strSourceData ) ;
	}
}