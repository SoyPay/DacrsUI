#include "StdAfx.h"
#include "PostMsg.h"


CPostMsg::CPostMsg(int uitype,int datatype)
{
	 UI_TYPE = uitype;
	 DATA_TYPE= datatype;
	 strType = _T("");
	 strData = _T("");
}
CPostMsg::CPostMsg(void){
	UI_TYPE = 0;
	DATA_TYPE= 0;
	strType = _T("");
	strData = _T("");
}

CPostMsg::~CPostMsg(void)
{
}
void CPostMsg::SetStrType(CString const &strTpye){
	strType = strTpye;
}
bool CPostMsg::operator==(CPostMsg const&meg)const{
	if (meg.UI_TYPE != UI_TYPE && meg.DATA_TYPE !=DATA_TYPE&&meg.strType != strType)
	{
		return false;
	}
	if( meg.strData != strData)
	{
	 return false;
	}
	
	return true;
}
int CPostMsg::GetUItype() const{
	return UI_TYPE;
}
int CPostMsg::GetDatatype() const{
	return DATA_TYPE;
}
CString CPostMsg::GetStrType() const{
	return strType;
}
void CPostMsg::SetType(int uitype,int datatype){
	UI_TYPE = uitype;
	DATA_TYPE= datatype;
}