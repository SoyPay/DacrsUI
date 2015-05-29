
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
		CString strCommand;
		strCommand.Format(_T("%s %s %s %s"),_T("getscriptvalidedata"),theApp.m_betScritptid.GetBuffer(),_T("100"),_T("1"));
		CStringA strShowData;
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		//m_AppID
		if (strShowData.Find("key") < 0)
		{
			return;
		}
		if (!reader.parse(strShowData.GetString(), root)) 
			return;
		int size = root.size();
		for ( int index =0; index < size; ++index )
		{
			CString txhash = root[index]["key"].asCString();
			txhash = txhash.Right(64);
			CString nValue = root[index]["value"].asCString();
			uistruct::DBBET_DATA DBbet;
			memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue.GetString());

			memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

			std::vector<unsigned char> vnTemp = CSoyPayHelp::getInstance()->ParseHex(txhash);
			reverse(vnTemp.begin(),vnTemp.end());
			string strTemp = CSoyPayHelp::getInstance()->HexStr(vnTemp);

			/// 查找数据库中此赌约是否正在接赌
			uistruct::P2P_QUIZ_RECORD_t  betrecord;
			CString strCond;
			strCond.Format(_T(" tx_hash = '%s' "), strTemp);

			int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(strCond, &betrecord );

			if (nItem != 0 && betrecord.state == 4) ////此赌约正在接赌，只是在block中没有确认
			{
				continue;
			}

			strCommand.Format(_T("%s %s"),_T("gettxdetail"),strTemp.c_str());
			CStringA strShowData;
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (!reader.parse(strShowData.GetString(), root1)) 
				return;
			int confirheight =root1["confirmHeight"].asInt();

			strCommand.Format(_T("%s"),_T("getinfo"));
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (!reader.parse(strShowData.GetString(), root1)) 
				return;
			int curheight =root1["blocks"].asInt();
			if(curheight >=(confirheight+500))
				continue;

			if (DBbet.betstate == 0x00)
			{
				std::vector<unsigned char> txTemp = CSoyPayHelp::getInstance()->ParseHex(txhash.GetString());
				reverse(txTemp.begin(),txTemp.end());
				string newTxhash =  CSoyPayHelp::getInstance()->HexStr(txTemp);
				CString strSourceData;
				strSourceData.Format(_T("'%s' , '%s'") , newTxhash.c_str(), nValue);
				m_SqliteDeal.InsertTableItem(_T("t_quiz_pool") ,strSourceData);
			}
		}
	}
}
void CDacrsUIApp::UpdateAddressData(){
	CString strCommand;
	strCommand.Format(_T("%s"),_T("listaddr"));
	CStringA strShowData ;

	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) 
		return  ;

	uistruct::LISTADDR_t listaddr;
	for(int i = 0; i < root.size(); ++i){
		memset(&listaddr , 0 , sizeof(uistruct::LISTADDR_t));
		//address
		CString strData;
		strData.Format( _T("%s") , root[i]["addr"].asCString() ) ;
		strncpy(listaddr.address  , strData , strlen(strData) > sizeof(listaddr.address) ? sizeof(listaddr.address):strlen(strData));
		//RegID
		strData.Format( _T("%s") , root[i]["regid"].asCString() ) ;
		strncpy(listaddr.RegID  , strData , strlen(strData) > sizeof(listaddr.RegID) ? sizeof(listaddr.RegID):strlen(strData));
		//金额
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//是否支持冷挖矿
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//是否注册GetLength();
		CString strSign ;
		strSign.Format(_T("%s") ,listaddr.RegID ) ;
		if ( 3 <= strSign.GetLength() ) {
			listaddr.bSign    = 1 ;
		}else{
			listaddr.bSign   = 0 ;
		}
		CString strSourceData,feild;
		feild.Format(_T("address"));
		strSourceData.Format(_T("%s"),listaddr.address);

		CString strCond;
		strCond.Format(_T(" address = '%s' "), listaddr.address);
		uistruct::LISTADDR_t addrsql;
		int item = m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		if (strlen(addrsql.address) == 0 )
		{
			strSourceData.Format(_T("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'") , listaddr.address ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Label ) ;
			if (!m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strSourceData ) )
			{
				TRACE("Insert t_wallet_address error!\n");
			}
			
			string Temp = listaddr.ToJson();
			SendUIMsg((int)WM_UP_NEWADDRESS,Temp.c_str());
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("reg_id = '%s', money = %.8f ,cold_dig =%d,sign =%d") ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere.Format(_T("address = '%s'") , listaddr.address  ) ;
				if ( !m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE(_T("Update t_wallet_address failed!") );
				}
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

	CString strCommand,strShowData;
	strCommand.Format(_T("%s %s"),_T("gettxdetail") ,hash.c_str() );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData.Find("hash") < 0){		
		return;
	}

	//解析Json数据
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) {		
		return;
	}

	if (strShowData.Find("blockhash") >= 0)
	{
		uistruct::REVTRANSACTION_t transcion;
		if (transcion.JsonToStruct(root.toStyledString()))
		{
			CString strSourceData,strWhere;
			strSourceData.Format(_T("send_time =%d,confirm_height = %d , confirmed_time = '%d' ,block_hash ='%s'") ,transcion.confirmedtime,transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str() ) ;
			strWhere.Format(_T("hash = '%s'") , hash.c_str() ) ;
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
	CString strCond;
	strCond.Format(_T(" tx_hash='%s' "), hexHash.c_str());
	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_p2p_quiz"), strCond) ;
	if (nItem != 0)
	{
		CString strSourceData,strWhere;
		strSourceData.Format(_T("comfirmed = %d ,height = %d,content = '%s',state=2") ,transcion.confirmedtime,transcion.confirmedHeight,Openbet.dhash ) ;
		strWhere.Format(_T("tx_hash = '%s'") , hexHash.c_str() ) ;
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") , strSourceData , strWhere)){
			TRACE(_T("t_p2p_quiz数据更新失败!") );
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
	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

	CString strCommand,strShowData;
	strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,transcion.addr.c_str() );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData  ==_T("")){		
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) {		
		return;
	}
	CString desaddr = root["RegID"].asCString();
	CString strCond,strField;
	strCond.Format(_T(" tx_hash='%s' "), SendTxhash.c_str());
	
	strField.AppendFormat(" right_addr = '%s' ,",desaddr );
	strField.AppendFormat(_T("recv_time = '%s' ,height = %d ,state = %d ,relate_hash = '%s' ,guess_num = %d ") ,strTime ,transcion.confirmedHeight ,1 ,transcion.txhash ,(int)acceptcbet.data) ;

	//更新数据
	if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
		TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , SendTxhash );
	}

}
void CDacrsUIApp::SendBetRecord(vector<unsigned char> sendbet,uistruct::REVTRANSACTION_t transcion)
{
	if(sendbet.size()==0)
		return;

	SYSTEMTIME curTime =UiFun::Time_tToSystemTime(transcion.confirmedtime);
	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d",curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);

	CString strCond,strField;
	strCond.Format(_T(" tx_hash='%s' "), transcion.txhash);
	strField.AppendFormat(_T("send_time='%s',height = %d") ,strTime ,transcion.confirmedHeight ) ;

	//更新数据
	if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") ,strField,strCond )) {
		TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , transcion.txhash );
	}
}
void CDacrsUIApp::UpdateAppRecord(string txdetail){
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(txdetail))
	{
		CString strCommand,strShowData;
		strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,theApp.m_betScritptid );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData  ==_T("")){		
			return;
		}
		Json::Reader reader;  
		Json::Value root; 
		if (!reader.parse(strShowData.GetString(), root)) {		
			return;
		}
		CString DesAddr = root["Address"].asCString();
		if (!strcmp(transcion.desaddr.c_str(),DesAddr))
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
		}
	}
}
void CDacrsUIApp::InsertTransaction(string hash){
	CString strCommand,strShowData;
	strCommand.Format(_T("%s %s"),_T("gettxdetail") ,hash.c_str() );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData.Find("hash") < 0){		
		return;
	}

	//解析Json数据
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) {		
		return;
	}

	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(root.toStyledString()))
	{

		if (!strcmp("COMMON_TX",transcion.txtype.c_str()))
		{
			CString conditon;
			conditon.Format(_T("address = '%s'"),transcion.addr.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			int nItem1 =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			if (nItem1 !=0&nItem != 0)
			{
				transcion.state = 1; 
			}else if (nItem != 0)
			{
				transcion.state = 1;        //// 扣钱
			}else if (nItem1 != 0)
			{
				transcion.state = 2;        ////加钱
			}
		}



		CString strSourceData;
		strSourceData.Format(_T("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d") , transcion.txhash ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		m_SqliteDeal.InsertTableItem(_T("t_transaction") ,strSourceData ) ;

		//// 数据插入了，更新到交易详情界面
		CPostMsg Postmsg(MSG_USER_TRANSRECORD_UI,WM_INSERT);
		string temp =root.toStyledString();
		Postmsg.SetData(temp.c_str());	
		m_UiTxDetailQueue.push(Postmsg);
		DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_TRANSRECORD_UI ,WM_INSERT,0);
	}
}
void  CDacrsUIApp::InsertAddbook(uistruct::ADDRBOOK_t addr)
{
	CString strSourceData;
	strSourceData.Format(_T("'%s' , '%s' ") , addr.label, addr.address ) ;
	m_SqliteDeal.InsertTableItem(_T("t_address_book") ,strSourceData ) ;
}
void  CDacrsUIApp::UpdateAddbook(uistruct::ADDRBOOK_t addr)
{
	CString strSourceData,strWhere;
	strSourceData.Format(_T("Label = '%s'") ,addr.label ) ;
	strWhere.Format(_T("address = '%s'") ,addr.address ) ;
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
			CString conditon;
			conditon.Format(_T("address = '%s'"),transcion.addr.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			int nItem1 =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_wallet_address"),conditon) ;
			if (nItem1 !=0&nItem != 0)
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

		CString strSourceData;
		strSourceData.Format(_T("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d") , transcion.txhash ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		m_SqliteDeal.InsertTableItem(_T("t_transaction") ,strSourceData );
	}
}
void CDacrsUIApp::ClearTransaction()
{
	m_SqliteDeal.ClearTableData(_T("t_transaction"));
}
BOOL  CDacrsUIApp::UpdateP2pBetRecord()
{
	uistruct::P2PBETRECORDLIST pTransaction;
	m_SqliteDeal.GetP2PQuizRecordList(_T(" 1=1 "), &pTransaction);

	if (0 == pTransaction.size() ) return FALSE ;

	vector<string> vSignHash;
	std::vector<uistruct::P2P_QUIZ_RECORD_t>::const_iterator const_it;
	for (const_it = pTransaction.begin() ; const_it != pTransaction.end() ; const_it++ ) {
		vSignHash.push_back(const_it->tx_hash);
	}
	if (vSignHash.size() == 0)
	{
		return true;
	}
	Json::Reader reader; 
	Json::Value root;
	root["method"] = "getappkeyvalue";

	Json::Value item;
	for ( size_t i = 0;i< vSignHash.size(); i++ )
	{
		item.append(vSignHash.at(i));
	}
	root["params"].append(theApp.m_betScritptid.GetBuffer());
	root["params"].append(item);

	CString strData ,strShowData ;

	strData.Format(_T("%s") , root.toStyledString().c_str() ) ;

	CSoyPayHelp::getInstance()->SendContacrRpc(strData , strShowData);
	if (strShowData.Find("key") <0)
	{
		return false;
	}
	if (!reader.parse(strShowData.GetString(), root)) 
		return FALSE;
	int size = root.size();
	for ( int index =0; index < size; ++index )
	{
		CString txhash = root[index]["key"].asCString();
		CString nValue = root[index]["value"].asCString();
		INT64   nTime  = root[index]["confirmedtime"].asInt64() ;
		int     nConfirmHeight = root[index]["confirmHeight"].asInt() ;
		uistruct::DBBET_DATA DBbet;
		memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
		std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue.GetString());
		uistruct::P2P_QUIZ_RECORD_t  betrecord;
		//theApp.cs_SqlData.Lock();
		//int nItem =  theApp.m_SqliteDeal.FindDB(_T("t_p2p_quiz") , txhash ,_T("tx_hash"),&betrecord ) ;
		//theApp.cs_SqlData.Unlock();
		CString strCond;
		strCond.Format(_T(" tx_hash='%s' "), txhash);
		int nItem =m_SqliteDeal.GetP2PQuizRecordItem(strCond, &betrecord ) ;
		if (vTemp.size() == 0)  /// 此条数据在应用数据库中被删除了,如果被接赌了,说明已经揭赌了
		{
			if (strlen(betrecord.left_addr) != 0 && (betrecord.state == 1 || betrecord.state == 4 || betrecord.state == 5))
			{
				CString strField,strWhere;
				strField.Format(_T("state = %d ") , 2) ;
				strWhere.Format(_T("tx_hash = '%s'") , txhash );

				//更新数据
				if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") , strField , strWhere )) {
					TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , txhash );
				}
			}
			continue;
		}
		memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

		if (DBbet.betstate == 0x00 && betrecord.state ==4)   //正在接赌
		{
			continue;
		}

		if (DBbet.betstate == 0x01 && betrecord.state ==5)   //正在揭赌
		{
			continue;
		}
		int nComfirmed = 0 ;
		if ( 0 != nTime ) {
			nComfirmed = 1 ;
		}

		int state = DBbet.betstate;
		string txaccepthash;
		if ( state == 1)
		{
			std::vector<unsigned char> vTemp;// = CSoyPayHelp::getInstance()->ParseHex(string(DBbet.accepthash,DBbet.accepthash+sizeof(DBbet.accepthash)));
			vTemp.assign(DBbet.accepthash,DBbet.accepthash+sizeof(DBbet.accepthash));
			reverse(vTemp.begin(),vTemp.end());
			txaccepthash = CSoyPayHelp::getInstance()->HexStr(vTemp);
			CString strCommand;
			strCommand.Format(_T("%s %s"),_T("gettxdetail") ,txaccepthash.c_str() );
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

			Json::Value root1;
			if (!reader.parse(strShowData.GetString(), root1)) 
				continue;
			int npos = strShowData.Find("confirmHeight");
			int confirh = 0;
			if ( npos >= 0 ) { //
				confirh = root1["confirmHeight"].asInt() ;    //交易被确认的高度
			}
			strCommand.Format(_T("%s"),_T("getinfo"));
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
			if (!reader.parse(strShowData.GetString(), root1)) 
				continue;
			int curheight =root1["blocks"].asInt();
			if (curheight >=(confirh +DBbet.hight ))
			{
				state = 3;                              ///说明超时了
			}
		}
		//更新数据库  update t_p2p_quiz set timeout=15 where actor=0; 
		CString strSql , strField  , strWhere;
		vTemp.clear();
		vTemp.assign(DBbet.acceptbetid,DBbet.acceptbetid+sizeof(DBbet.acceptbetid));
		string acceptaddr = CSoyPayHelp::getInstance()->HexStr(vTemp);
		CString strCommand;
		strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,acceptaddr.c_str() );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);
		Json::Value rootinfo;
		if (!reader.parse(strShowData.GetString(), rootinfo)) 
			continue;
		CString acceptbase;
		if (strShowData.Find("Address") >= 0)
		{
			acceptbase =  rootinfo["Address"].asCString();
		}
		strField.Format(_T("send_time = %ld , ") , nTime) ;
		strField.AppendFormat(" right_addr = '%s' ,",acceptbase );
		strField.AppendFormat(_T("comfirmed = %d ,height = %d ,state = %d ,relate_hash = '%s' ,guess_num = %d ") ,nComfirmed ,nConfirmHeight ,state ,txaccepthash.c_str() ,(int)DBbet.acceptebetdata ) ;
		strWhere.Format(_T("tx_hash = '%s'") , txhash );

		//更新数据
		if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") , strField , strWhere )) {
			TRACE(_T("t_p2p_quiz:更新数据失败!  Hash: %s") , txhash );
		}
	}
	return true;
}