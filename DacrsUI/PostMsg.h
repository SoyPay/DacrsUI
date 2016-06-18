#pragma once
using namespace std;
class CPostMsg
{
	int UI_TYPE;
    int DATA_TYPE;
	string strType;
	string strData;
public:

	CPostMsg(int uitype,int datatype);
	CPostMsg(void);
	~CPostMsg(void);
	void SetStrType(string const &strTpye);
	void SetType(int uitype,int datatype);
	int GetUItype() const;
	int GetDatatype() const;
	void SetData(string const & data){
		strData = data;
	}
	string GetData(){
		return strData;
	}
	string GetStrType() const;
public:
	 bool operator==(CPostMsg const&meg)const;
};

