
// DacrsUI.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "DacrsUI.h"
#include "DacrsUIDlg.h"


#include <afxsock.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CDacrsUIApp::UpdataBetPoolData()
{
	Json::Reader reader;  
	Json::Value root; 
	Json::Value root1;

	if(theApp.m_betScritptid != _T(""))
	{
		CString strCommand;
		strCommand.Format(_T("%s %s %s %s"),_T("getscriptdata"),theApp.m_betScritptid.GetBuffer(),_T("100"),_T("1"));
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
			CString nValue = root[index]["value"].asCString();
			uistruct::DBBET_DATA DBbet;
			memset(&DBbet , 0 , sizeof(uistruct::DBBET_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue.GetString());

			memcpy(&DBbet, &vTemp[0], sizeof(DBbet));

			std::vector<unsigned char> vnTemp = CSoyPayHelp::getInstance()->ParseHex(txhash);
			reverse(vnTemp.begin(),vnTemp.end());
			string strTemp = CSoyPayHelp::getInstance()->HexStr(vnTemp);

			/// 查找数据库中此赌约是否正在接赌
			uistruct::P2P_BET_RECORD_t  betrecord;
			CString findhash;
			findhash.Format(_T("%s"),strTemp.c_str());
//			theApp.cs_SqlData.Lock();
			int nItem =  theApp.m_SqliteDeal.FindDB(_T("p2p_bet_record") , findhash ,_T("tx_hash"),&betrecord ) ;
//			theApp.cs_SqlData.Unlock();
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
			if(curheight >=(confirheight+DBbet.hight+500))
				continue;

			if (DBbet.betstate == 0x00)
			{
				CString strSourceData;
				strSourceData.Format(_T("'%s' , '%s'") ,txhash,nValue) ;
//				theApp.cs_SqlData.Lock();
				m_SqliteDeal.InsertData(_T("p2ppool") ,strSourceData ) ;
//				theApp.cs_SqlData.Unlock();

			}
		}
	}
}
void CDacrsUIApp::UpdataAddressData(){
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

		uistruct::LISTADDR_t addrsql;
		int item = m_SqliteDeal.FindDB(_T("MYWALLET") ,strSourceData, feild,&addrsql) ;

		if (item == 0 )
		{
			strSourceData.Format(_T("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'") , listaddr.address ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Lebel ) ;
			if (!m_SqliteDeal.InsertData(_T("MYWALLET") ,strSourceData ) )
			{
				TRACE(_T("MYWALLET数据插入失败!") );
				TRACE("insert:%d\r\n",i);
			}
			
			string Temp = listaddr.ToJson();
			SendRecvieUiMes((int)WM_UP_NEWADDRESS,Temp.c_str());
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("regid = '%s', money = %.8f ,coldig =%d,sign =%d") ,listaddr.RegID ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere.Format(_T("address = '%s'") , listaddr.address  ) ;
				if ( !m_SqliteDeal.Updatabase(_T("MYWALLET") , strSourceData , strWhere ) ){
					TRACE(_T("MYWALLET数据更新失败!") );
				}

				string Temp = listaddr.ToJson();
				SendRecvieUiMes((int)WM_UP_ADDRESS,Temp.c_str());
			}
		}
	}
}

void CDacrsUIApp::UpdatarevtransactionData(string hash){

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
			strSourceData.Format(_T("confirmHeight = %d , confirmedtime = '%d' ,blockhash ='%s'") ,transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str() ) ;
			strWhere.Format(_T("hash = '%s'") , hash.c_str() ) ;
			if ( !m_SqliteDeal.Updatabase(_T("revtransaction") , strSourceData , strWhere ) ){
				TRACE(_T("revtransaction数据更新失败!") );
			}
		}
	}
}

void CDacrsUIApp::UpdatarevAppRecord(string txdetail){
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(txdetail))
	{
		CString strCommand,strShowData;
		strCommand.Format(_T("%s %s"),_T("getaccountinfo") ,theApp.m_betScritptid );
		CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

		if (strShowData  =_T("")){		
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
			OPEN_DATA Openbet;
			memset(&Openbet , 0 , sizeof(OPEN_DATA));
			std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);
			if(vTemp.size()>0)
			memcpy(&Openbet, &vTemp[0], sizeof(OPEN_DATA));
			if (Openbet.type != TX_OPENBET)
			{
				return;
			}
			vector<unsigned char>vHash;
			vHash.assign(Openbet.txhash,Openbet.txhash+sizeof(Openbet.txhash));
			reverse(vHash.begin(),vHash.end());
			string hexHash = CSoyPayHelp::getInstance()->HexStr(vHash);
			int nItem =  theApp.m_SqliteDeal.FindDB(_T("p2p_bet_record") , hexHash.c_str() ,_T("tx_hash")) ;
			if (nItem != 0)
			{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("recvtime = %d ,guessnum ='%s'") ,transcion.confirmedtime,(int)Openbet.dhash[32] ) ;
				strWhere.Format(_T("tx_hash = '%s'") , hexHash.c_str() ) ;
				if ( !m_SqliteDeal.Updatabase(_T("p2p_bet_record") , strSourceData , strWhere ) ){
					TRACE(_T("p2p_bet_record数据更新失败!") );
				}

			}
		}
	}
}
void CDacrsUIApp::InsertarevtransactionData(string hash){

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
		int nItem =  theApp.m_SqliteDeal.FindDB(_T("MYWALLET") , transcion.addr.c_str() ,_T("address")) ;
		int nItem1 =  theApp.m_SqliteDeal.FindDB(_T("MYWALLET") , transcion.desaddr.c_str() ,_T("address")) ;
		if (nItem != 0)
		{
			transcion.state = 1;        //// 扣钱
		}else if (nItem1 != 0)
		{
			transcion.state = 2;        ////价钱
		}
		CString strSourceData;
		strSourceData.Format(_T("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d") , transcion.txhash ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		m_SqliteDeal.InsertData(_T("revtransaction") ,strSourceData ) ;
	}
}
void  CDacrsUIApp::InsertAddbook(uistruct::ADDRBOOK_t addr)
{
	CString strSourceData;
	strSourceData.Format(_T("'%s' , '%s' ") , addr.lebel,addr.address ) ;
	m_SqliteDeal.InsertData(_T("addrbook") ,strSourceData ) ;
}
void  CDacrsUIApp::UpdataAddbook(uistruct::ADDRBOOK_t addr)
{
	CString strSourceData,strWhere;
	strSourceData.Format(_T("Lebel = '%s'") ,addr.lebel ) ;
	strWhere.Format(_T("address = '%s'") ,addr.address ) ;
	if ( !m_SqliteDeal.Updatabase(_T("addrbook") , strSourceData , strWhere ) ){
		TRACE(_T("addrbook数据更新失败!") );
	}
}
