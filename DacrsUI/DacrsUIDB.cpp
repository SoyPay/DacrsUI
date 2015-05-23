
// DacrsUI.cpp : ����Ӧ�ó��������Ϊ��
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

			/// �������ݿ��д˶�Լ�Ƿ����ڽӶ�
			uistruct::P2P_QUIZ_RECORD_t  betrecord;
			CString strCond;
			strCond.Format(_T(" tx_hash = '%s' "), strTemp);

			int nItem =  theApp.m_SqliteDeal.GetP2PQuizRecordItem(strCond, &betrecord );

			if (nItem != 0 && betrecord.state == 4) ////�˶�Լ���ڽӶģ�ֻ����block��û��ȷ��
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
		//���
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//�Ƿ�֧�����ڿ�
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//�Ƿ�ע��GetLength();
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

	//����Json����
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
			strSourceData.Format(_T("confirm_height = %d , confirmed_time = '%d' ,block_hash ='%s'") ,transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str() ) ;
			strWhere.Format(_T("hash = '%s'") , hash.c_str() ) ;
			if ( !m_SqliteDeal.UpdateTableItem(_T("t_transaction") , strSourceData , strWhere ) ){
				TRACE(_T("update t_transaction failed\n"));
			}
		}
	}
}

void CDacrsUIApp::UpdateAppRecord(string txdetail){
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
			CString strCond;
			strCond.Format(_T(" tx_hash='%s' "), hexHash.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_p2p_quiz"), strCond) ;
			if (nItem != 0)
			{
				CString strSourceData,strWhere;
				strSourceData.Format(_T("recvtime = %d ,guessnum ='%s'") ,transcion.confirmedtime,(int)Openbet.dhash[32] ) ;
				strWhere.Format(_T("tx_hash = '%s'") , hexHash.c_str() ) ;
				if ( !m_SqliteDeal.UpdateTableItem(_T("t_p2p_quiz") , strSourceData , strWhere)){
					TRACE(_T("t_p2p_quiz���ݸ���ʧ��!") );
				}

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

	//����Json����
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(strShowData.GetString(), root)) {		
		return;
	}

	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(root.toStyledString()))
	{
		CString strSourceData;
		strSourceData.Format(_T("'%s' , '%s' ,'%d' ,'%s' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' , '%.8f' ,'%s' ,'%d','%d','%s',%d") , transcion.txhash ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.pubkey.c_str(),transcion.miner_pubkey.c_str(),transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str(),transcion.state) ;
		m_SqliteDeal.InsertTableItem(_T("t_transaction") ,strSourceData ) ;

		//// ���ݲ����ˣ����µ������������
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
		TRACE(_T("t_address_book���ݸ���ʧ��!") );
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